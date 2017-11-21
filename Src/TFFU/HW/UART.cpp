
#include "TFFU/HW/UART.h"
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "usart.h"
#include "dma.h"

uint8_t dma_rx;
uint8_t rxBuff[UART_RX_BUFFER_SIZE];
volatile uint8_t rxBuffHead, rxBuffTail;
volatile int huart1_finished_tx = 1;


void UART_Init(void)
{
	rxBuffHead = 0;
	rxBuffTail = 0;
	HAL_UART_Receive_IT(&huart1, &dma_rx, 1);
}

void UART_Update(void)
{
}

void UART_Send(uint8_t* pdata, uint8_t amount)
{
	// Sets transmit lock
	huart1_finished_tx = 0;
	// Transmit
	while (HAL_UART_Transmit_DMA(&huart1, (uint8_t*)pdata, amount) == HAL_BUSY);
	// Checks and waits if previous tx is not finished
	//while (!huart1_finished_tx);
}

bool UART_Available(void)
{
	return rxBuffHead != rxBuffTail;
}
uint8_t UART_Read(void)
{
	return rxBuff[rxBuffTail++];
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t inbyte = dma_rx;
	HAL_UART_Receive_IT(&huart1, &dma_rx, 1);

	rxBuff[rxBuffHead] = inbyte;
	rxBuffHead++;
	if(rxBuffHead >= UART_RX_BUFFER_SIZE)
		rxBuffHead = 0;
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	  huart1_finished_tx = 1;
}
