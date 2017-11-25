
#include "TFFU/HW/I2C.h"
//#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "dma.h"
#include "TFFU/TFFUMain.h"

volatile bool i2cbusy = false;
static uint8_t* targetFlag = NULL;
//static uint8_t dma_rx;
//static uint8_t rxBuff[I2C_RX_BUFFER_SIZE];
static volatile uint8_t rxBuffHead, rxBuffTail;


void I2C_Init(void)
{
	rxBuffHead = 0;
	rxBuffTail = 0;
	//HAL_UART_Receive_IT(&huart1, &dma_rx, 1);
}

void I2C_Update(void)
{
}

void I2C_Send(uint8_t* pdata, uint8_t amount)
{
}

bool I2C_Available(void)
{
	return 0;//rxBuffHead != rxBuffTail;
}
void I2C_Read(uint8_t devAddr, uint8_t amount, uint8_t* pBuff, uint8_t* flag)
{
	targetFlag = flag;
	i2cbusy = true;
	HAL_I2C_Master_Receive_DMA(&hi2c1, (uint16_t)(devAddr), pBuff, amount);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	*targetFlag = 1;
	i2cbusy = false;
}
/*void HAL_I2C_MasterTxCpltCallback(UART_HandleTypeDef *huart)
{
	  huart1_finished_tx = 1;
}*/
