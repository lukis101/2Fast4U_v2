
#include "main.h"
#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "TFFU/TFFUMain.h"

#include "i2c.h"
#include "usart.h"
#include "tim.h"

#define DF 1

volatile uint32_t sysTime;

char aTxBuffer[100];


// VARS
uint8_t raceActive = false;
uint8_t standby = true;

// INTERNALS
const uint8_t NUMEVENTS = 5;
uint8_t events[NUMEVENTS] = {};
uint8_t lastevent = 0;

const uint16_t INTERVAL_VOLTAGE = 1000;
const uint16_t INTERVAL_SENSORS = 3;
const uint16_t INTERVAL_MOTORS = 4;
const uint16_t INTERVAL_PID = 3;
uint32_t lastTick_voltage;
uint32_t lastTick_monitor;
uint32_t nextTick_monitor;
uint32_t lastTick_motors;
uint32_t lastTick_sensors;
uint32_t nextTick_sensors;

void TFFUMain(void)
{
	/* Setup */
	//int hver = HAL_GetHalVersion();
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&htim3);
	//HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, (GPIO_PinState)(1));

    uint8_t writecmd[] = {0x80, 0x01};
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x34), 3, 100) != HAL_OK) {}
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x36), 3, 100) != HAL_OK) {}
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x34), writecmd, 2, 1000);
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x36), writecmd, 2, 1000);

	uint32_t lastread = 0;

	/* Main loop */
	while(1)
	{
		uint32_t systicks = HAL_GetTick();
		uint32_t millis = systicks % 1000;
		if (millis == 0)
		{
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(1));
			HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(1));
			//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		}
		else if (millis == 100)
		{
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(0));
			HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(0));
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
		}
		if ((millis % 50 == 0) &&  (lastread != millis))
		{
			lastread = millis;
			HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(1));
			//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x34), 3, 100) != HAL_OK) {}
			//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x36), 3, 100) != HAL_OK) {}

			int outstrlen = 0;

			char formatenc[] = "%05d %05d";
			uint16_t tim1val = TIM1->CNT;
			uint16_t tim2val = TIM2->CNT;
			outstrlen += sprintf(aTxBuffer+outstrlen, formatenc, tim1val, tim2val);

			/*uint8_t vals[14*2];

			uint8_t targetvalue = 1;
			HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x34), &targetvalue, 1, 100);
			HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x34), vals, 7*2, 100);
			HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x36), &targetvalue, 1, 100);
			HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x36), vals+7*2, 7*2, 100);
			char format16[] = "%04d ";
			for(int i=0; i<14; i++)
				outstrlen += sprintf(aTxBuffer+outstrlen, format16, ((uint16_t*)vals)[i]);
			aTxBuffer[outstrlen++] = '\n';*/

			/*targetvalue = 2;
			//HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x34), &targetvalue, 1, 1000);
			//HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x34), vals, 7*1, 1000);
			char format8[] = "%02X ";
			for(int i=0; i<7; i++)
				outstrlen += sprintf(aTxBuffer+outstrlen, format8, vals[i*1]);
			aTxBuffer[outstrlen++] = '\n';

			targetvalue = 3;
			//HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x34), &targetvalue, 1, 1000);
			//HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x34), vals, 1, 1000);
			outstrlen += sprintf(aTxBuffer+outstrlen, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(vals[0]));
			aTxBuffer[outstrlen++] = '\n';
			aTxBuffer[outstrlen++] = '\n';*/

			aTxBuffer[outstrlen++] = '\n';
			HAL_UART_Transmit(&huart1, (uint8_t*)aTxBuffer, outstrlen, 1000);

			//HAL_Delay(1);
			HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(0));
		}
		/*
		HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);

		uint16_t timval = TIM1->CNT;
		HAL_GPIO_WritePin(LD4_GPIO_Port,LD3_Pin, (GPIO_PinState)((timval&1)!=0));
		HAL_GPIO_WritePin(LD4_GPIO_Port,LD4_Pin, (GPIO_PinState)((timval&2)!=0));
		HAL_GPIO_WritePin(LD4_GPIO_Port,LD5_Pin, (GPIO_PinState)((timval&4)!=0));
		HAL_GPIO_WritePin(LD4_GPIO_Port,LD6_Pin, (GPIO_PinState)((timval&8)!=0));

		uint32_t systicks = HAL_GetTick();
		if (systicks % 100 == 0)
		{
			//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			//HAL_Delay(50);
			//HAL_GPIO_WritePin(LD3_GPIO_Port,LD3_Pin,GPIO_PIN_RESET);
			char fmt[] = "%d\n";
			int nums = sprintf(aTxBuffer, fmt, timval);
			HAL_UART_Transmit(&huart3, (uint8_t*)aTxBuffer, nums, 1000);

			HAL_Delay(1);
		}*/
	}
}
