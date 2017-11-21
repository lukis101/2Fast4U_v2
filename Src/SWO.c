
#include "stm32f3xx.h"
#include "core_cm4.h"

/*********************************************************************
*
*       Defines for Cortex-M debug unit
*/
#define ITM_STIM_U32(n) (*(volatile unsigned int*) (0xE0000000+4*n))  // Stimulus Port n Register word access
#define ITM_STIM_U8(n)  (*(volatile unsigned char*)(0xE0000000+4*n))  // Stimulus Port n Register byte access


void SWO_Setup(void) {
  unsigned int SWOSpeed = 256000;
  unsigned int SWOPrescaler = (72000000 / SWOSpeed) - 1;  // SWOSpeed in Hz
  *((volatile unsigned *)0xE000EDFC) = 0x01000000;   // "Debug Exception and Monitor Control Register (DEMCR)"
  *((volatile unsigned *)0xE0042004) = 0x00000027;
  *((volatile unsigned *)0xE00400F0) = 0x00000002;   // "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO)

  *((volatile unsigned *)0xE0040010) = SWOPrescaler; // "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output
  ITM->LAR = 0xC5ACCE55;   // ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC
  ITM->TCR = 0x0001000D;   // ITM Trace Control Register
  ITM->TPR = 0x0000000F;   // ITM Trace Privilege Register
  ITM->TER = 0x00000001;   // ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port.
  *((volatile unsigned *)0xE0001000) = 0x400003FE;   // DWT_CTRL
  *((volatile unsigned *)0xE0040304) = 0x00000100;   // Formatter and Flush Control Register
}

/*********************************************************************
*
*       SWO_PrintChar()
*
* Function description
*   Checks if SWO is set up. If it is not, return,
*    to avoid program hangs if no debugger is connected.
*   If it is set up, print a character to the ITM_STIM register
*    in order to provide data for SWO.
* Parameters
*   c:    The Chacracter to be printed.
* Notes
*   Additional checks for device specific registers can be added.
*/
void SWO_PrintChar(char c) {
	ITM_SendChar(c);
}

/*********************************************************************
*
*       SWO_PrintString()
*
* Function description
*   Print a string via SWO.
*
*/
void SWO_PrintString(const char *s) {
  //
  // Print out character per character
  //
  while (*s){
    SWO_PrintChar(*s++);
  }
}
