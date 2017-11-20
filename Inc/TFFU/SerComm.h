
#ifndef TFFU_SERCOMM_H_
#define TFFU_SERCOMM_H_

#include <stdint.h>

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
const uint8_t VARTYPE_SIGNED = 0x04;
const uint8_t VARTYPE_UNSIGNED = 0x00;
const uint8_t VARTYPE_BYTE  = 1;
const uint8_t VARTYPE_WORD  = 2;
const uint8_t VARTYPE_DWORD  = 3;
const uint8_t VARTYPE_UBYTE  = VARTYPE_UNSIGNED | VARTYPE_BYTE;
const uint8_t VARTYPE_SBYTE  = VARTYPE_SIGNED   | VARTYPE_BYTE;
const uint8_t VARTYPE_UWORD  = VARTYPE_UNSIGNED | VARTYPE_WORD;
const uint8_t VARTYPE_SWORD  = VARTYPE_SIGNED   | VARTYPE_WORD;
const uint8_t VARTYPE_UDWORD = VARTYPE_UNSIGNED | VARTYPE_DWORD;
const uint8_t VARTYPE_SDWORD = VARTYPE_SIGNED   | VARTYPE_DWORD;
const uint8_t VARTYPE_FLOAT  = VARTYPE_SIGNED;
const uint8_t VARTYPE_SIZES[] = { 4, 1, 2, 4 };


#endif /* TFFU_SERCOMM_H_ */
