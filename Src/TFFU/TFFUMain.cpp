
#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "main.h"
//#include "SWO.h"

#include "usart.h"
#include "i2c.h"
#include "tim.h"

#include "TFFU/TFFUMain.h"
#include "TFFU/HW/UART.h"
#include "TFFU/SerComm.h"
#include "TFFU/Params.h"


volatile uint32_t last;

char aTxBuffer[100];


// VARS
uint8_t raceActive = false;
uint8_t standby = true;
bool startFlag;
bool stopFlag;


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

bool sysTick = false;

void TFFUMain(void)
{
	/* Setup */
	//SWO_Setup();
	UART_Init();
	Params_SetDefaults();

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

	/* Main loop */
	while(1)
	{
		uint32_t systicks = HAL_GetTick();
		ParseSerial(systicks);

		/*if(param_manual_enable != 0)
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(1));
		else
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(0));*/

		if (sysTick)
		{
			sysTick = false;
			uint32_t millis = systicks % 1000;

			if (millis == 0)
			{
				//HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(1));
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(1));
				//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			}
			else if (millis == 100)
			{
				//HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(0));
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(0));
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			}
			if (millis % 50 == 0)
			{
				HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(1));
				//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x34), 3, 100) != HAL_OK) {}
				//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x36), 3, 100) != HAL_OK) {}

				int outstrlen = 0;

				char formatenc[] = "%05d %05d";
				uint16_t tim1val = TIM1->CNT;
				uint16_t tim2val = TIM2->CNT;
				//outstrlen += sprintf(aTxBuffer+outstrlen, formatenc, tim1val, tim2val);

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
				//UART_Send(aTxBuffer, outstrlen);

				//HAL_Delay(1);
				HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(0));
			}

			/// MONITORING / DEBUG
			/*if( AllParams.MonitoringInterval > 0 )
			{
			}*/
		}
	}
}
