
#ifndef TFFU_SERCOMM_H_
#define TFFU_SERCOMM_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

const uint16_t SERCOM_TIMEOUT = 1000;

const uint8_t PROTOCOL_START = '#';
const uint8_t PROTOCOL_END = 0xFF;

// COMMANDS: 5 bits (32)
const uint8_t CMD_MASK = 0x1F;
const uint8_t CMD_TICK = 0;
const uint8_t CMD_CALIBRATE = 1;
const uint8_t CMD_RACE_START = 2;
const uint8_t CMD_RACE_STOP = 3;
const uint8_t CMD_EEPROM_READ = 4;
const uint8_t CMD_EEPROM_WRITE = 5;
const uint8_t CMD_PARAM_LIST = 6;
const uint8_t CMD_PARAM_READ = 7;
const uint8_t CMD_PARAM_WRITE = 8;
const uint8_t CMD_MONITOR_LIST = 9;
const uint8_t CMD_MONITOR_READ = 10;
const uint8_t CMD_PRINT_STRING = 11;
const uint8_t CMD_PRINT_DEC = 12;
const uint8_t CMD_PRINT_HEX = 13;

// VARTYPES: 3 bits, most significant is sign
const uint8_t VARTYPE_MASK = 0x07; // VARTYPE: 3 bits (8)
#define VARTYPE_SIGNED    0x04
#define VARTYPE_UNSIGNED  0x00
#define VARTYPE_BYTE   1
#define VARTYPE_WORD   2
#define VARTYPE_DWORD  3
#define VARTYPE_UBYTE  (VARTYPE_UNSIGNED | VARTYPE_BYTE)
#define VARTYPE_SBYTE  (VARTYPE_SIGNED   | VARTYPE_BYTE)
#define VARTYPE_UWORD  (VARTYPE_UNSIGNED | VARTYPE_WORD)
#define VARTYPE_SWORD  (VARTYPE_SIGNED   | VARTYPE_WORD)
#define VARTYPE_UDWORD (VARTYPE_UNSIGNED | VARTYPE_DWORD)
#define VARTYPE_SDWORD (VARTYPE_SIGNED   | VARTYPE_DWORD)
#define VARTYPE_FLOAT  (VARTYPE_SIGNED)
const uint8_t VARTYPE_SIZES[] = { 4, 1, 2, 4 };

enum SERIAL_STATE
{
    SERSTATE_IDLE,
    SERSTATE_START,
    SERSTATE_INDEX,
    SERSTATE_DATA,
    SERSTATE_END,
};

void ParseSerial();
void SerialTick();
void SendMonVar( uint8_t ID, uint8_t vartype, void* ptr );
void PrintVar( uint8_t vartype, void* ptr, uint8_t command );
void PrintVarDec( uint8_t vartype, void* ptr );
void PrintVarHex( uint8_t vartype, void* ptr );
void PrintString(const char* str, uint8_t len );

#ifdef __cplusplus
}
#endif
#endif /* TFFU_SERCOMM_H_ */
