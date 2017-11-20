
// ------ PROTOCOL ------ //
// DATATYPE - 3 bits
// COMMAND  - 5 bits
// Command:
//     [START] [XXX|COMMAND] [END]
// 1 variable, data size in bytes(1-4) indicated by DATATYPE
//     [START] [DATATYPE|COMMAND] [DATA1]..[DATAn] [END]
// Variable array, INDEX indicates starting index, COUNT - number of vars (COUNT*(1-4) bytes)
//     [START] [DATATYPE|COMMAND] [COUNT] [DATA11]..[DATA1n]....[DATAx1]..[DATAxn] [END]
// Text message:
//     [START] [XXX|COMMAND] [LENGTH] [CHAR1]..[CHARn] [END]

#include "TFFU/SerComm.h"
#include "TFFU/Params.h"
#include <stdint.h>

static const uint8_t PROTOCOL_START = '#';
static const uint8_t PROTOCOL_END = 0xFF;

static const uint8_t SERBUFF_TICK[3] = { PROTOCOL_START, CMD_TICK, PROTOCOL_END };
static const uint8_t SERBUFF_VARCOUNT[4] = { PROTOCOL_START, (VARTYPE_UBYTE<<5)|CMD_PARAM_LIST, PARAMCOUNT, PROTOCOL_END };
static uint8_t SERBUFF_STRING[3] = { PROTOCOL_START, CMD_PRINT_STRING, 0 };
static uint8_t SERBUFF_VAR[8] = { PROTOCOL_START, 0 };

enum SERIAL_STATE
{
    SERSTATE_IDLE,
    SERSTATE_START,
    SERSTATE_INDEX,
    SERSTATE_DATA,
    SERSTATE_END,
};
unsigned long ser_lastUpdate = 0; // for timeout
uint8_t ser_state = SERSTATE_IDLE;
uint8_t ser_vartype = 0;
uint8_t ser_command = 0;
uint8_t ser_varid = 0; // sent var index
uint8_t ser_buff[4] = {};
uint8_t ser_buff_i = 0;

inline void SendAck()
{
    Serial.write( SERBUFF_TICK, 3 );
}
void SendVar( uint8_t index, uint8_t vartype, void* ptr, uint8_t command )
{
    uint8_t varsize = VARTYPE_SIZES[ vartype & ~VARTYPE_SIGNED ];
    SERBUFF_VAR[ 1 ] = (vartype<<5) | command;
    SERBUFF_VAR[ 2 ] = index;
    memcpy( (void*)(&SERBUFF_VAR[3]), ptr, varsize );
    SERBUFF_VAR[ 3+varsize ] = PROTOCOL_END;
    Serial.write( SERBUFF_VAR, 4+varsize );
}
void SendParam( uint8_t index )
{
    SendVar( index, PARAMDATA[ index*2 ], PARAM_PTRS[ index ], CMD_PARAM_READ );
}
inline void SendMonVar( uint8_t index, uint8_t vartype, void* ptr )
{
    SendVar( index, vartype, ptr, CMD_MONITOR_READ );
}

void PrintVar( uint8_t vartype, void* ptr, uint8_t command )
{
    uint8_t varsize = VARTYPE_SIZES[ vartype & ~VARTYPE_SIGNED ];
    SERBUFF_VAR[ 1 ] = (vartype<<5) | command;
    memcpy( (void*)(&SERBUFF_VAR[2]), ptr, varsize );
    SERBUFF_VAR[ 2+varsize ] = PROTOCOL_END;
    Serial.write( SERBUFF_VAR, 3+varsize );
}
void PrintVarDec( uint8_t vartype, void* ptr )
{
    PrintVar( vartype, ptr, CMD_PRINT_DEC );
}
void PrintVarHex( uint8_t vartype, void* ptr )
{
    PrintVar( vartype, ptr, CMD_PRINT_HEX );
}
void PrintString( const char* str, uint8_t len )
{
    //if( len == 0 ) return;
    SERBUFF_STRING[ 2 ] = len;
    Serial.write( SERBUFF_STRING, 3 );
    Serial.write( str, len );
    Serial.write( PROTOCOL_END );
}

inline void ParseSerial( unsigned long curtime )
{
    while( Serial.available() > 0 ) // Byte by byte input processing
    {
        uint8_t inByte = Serial.read();

        // Check for timeout
        if( ser_state != SERSTATE_IDLE )
            if( curtime-ser_lastUpdate >= SERIAL_TIMEOUT )
                ser_state = SERSTATE_IDLE;
        ser_lastUpdate = curtime;

        // State-based parsing
        switch( ser_state )
        {
        case SERSTATE_IDLE: // Wait for start token
        SERSTATE_IDLE:
            if( inByte == 0x30 ) // STK_GET_SYNC - reset for programming
                reset();
            else if( inByte == PROTOCOL_START )
                ser_state = SERSTATE_START;
            break;

        case SERSTATE_START: // Parse command
            ser_command = inByte & CMD_MASK;
            switch( ser_command )
            {
            // Commands without params
            case CMD_TICK:
            case CMD_CALIBRATE:
            case CMD_RACE_START:
            case CMD_RACE_STOP:
            case CMD_EEPROM_READ: // EEPROM->RAM
            case CMD_EEPROM_WRITE: // RAM->EEPROM
            case CMD_PARAM_LIST:
            case CMD_MONITOR_LIST:
                ser_state = SERSTATE_END;
                break;

            // Commands with index byte
            case CMD_PARAM_READ:
            case CMD_PARAM_WRITE:
                ser_vartype = (inByte >> 5) & VARTYPE_MASK;
                ser_state = SERSTATE_INDEX;
                break;

            // Unknown command, rewind
            default:
                ser_state = SERSTATE_IDLE;
                //goto SERSTATE_IDLE; // Optimisation?
            }
            break;

        case SERSTATE_INDEX: // Read variable index
            ser_varid = inByte;
            if( ser_command == CMD_PARAM_WRITE )
            {
                // Determine payload size
                byte baseType = ser_vartype & ~(VARTYPE_SIGNED);
                if( baseType == VARTYPE_BYTE )
                    ser_buff_i = 0;
                else if( baseType == VARTYPE_WORD )
                    ser_buff_i = 1;
                else if( baseType == VARTYPE_DWORD || ser_vartype == VARTYPE_FLOAT )
                    ser_buff_i = 3;
                // else error?
                ser_state = SERSTATE_DATA;
            }
            else
                ser_state = SERSTATE_END;
            break;

        case SERSTATE_DATA: // Read payload data
            ser_buff[ ser_buff_i ] = inByte;
            if( ser_buff_i == 0 )
                ser_state = SERSTATE_END;
            else
                ser_buff_i--;
            break;

        case SERSTATE_END: // Packet complete, check end token to acknowledge packet
            if( inByte == PROTOCOL_END )
            {
                // Run the command
                switch( ser_command )
                {
                case CMD_TICK:
                    SendAck(); // Acknowledge
                    break;

                case CMD_CALIBRATE:
                    Sensors_Calibrate();
                    SendAck(); // Acknowledge
                    break;
                case CMD_RACE_START:
                    startFlag = true;
                    SendAck(); // Acknowledge
                    break;
                case CMD_RACE_STOP:
                    stopFlag = true;
                    SendAck(); // Acknowledge
                    break;

                case CMD_EEPROM_READ:
                    EEPROM_ReadParams(); // EEPROM to RAM
                    SendAck(); // Acknowledge
                    break;
                case CMD_EEPROM_WRITE:
                    EEPROM_WriteParams(); // RAM to EEPROM
                    SendAck(); // Acknowledge
                    break;

                case CMD_PARAM_READ:
                    SendParam( ser_varid );
                    break;

                case CMD_PARAM_WRITE:
                    memcpy( PARAM_PTRS[ ser_varid ], ser_buff, VARTYPE_SIZES[ PARAMDATA[ser_varid*2] & ~VARTYPE_SIGNED ] );
                    //SendAck(); // Acknowledge
                    if( ser_varid==PARAM_MANUAL_ENABLE || ser_varid==PARAM_MONITOR_ENABLE )
                        Standby( !(param_monitor_enable || param_manual_enable) );
                    break;
                case CMD_PARAM_LIST:
                    Serial.write( SERBUFF_VARCOUNT, 4 ); // Send param amount
                    for( uint8_t i=0; i<PARAMCOUNT; i++ )
                        SendParam( i );
                    break;
                }
            }
            //else
                // Communication error, rewind
            ser_state = SERSTATE_IDLE;
            break;
        }
    }
}
