
#ifndef TFFU_HW_UART_H_
#define TFFU_HW_UART_H_

#include <stdint.h>
#include <stdbool.h>

#define UART_RX_BUFFER_SIZE  256
extern uint8_t uart_RxBuff[UART_RX_BUFFER_SIZE];
#define DMA_RX_BUFFER_SIZE          64
extern uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];


void UART_Init(void);
bool UART_Available(void);
uint8_t UART_Read(void);
void UART_Send(uint8_t* pdata, uint8_t amount);

#endif /* TFFU_HW_UART_H_ */
