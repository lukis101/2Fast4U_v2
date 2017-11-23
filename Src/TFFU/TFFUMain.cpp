
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
#include "TFFU/Motor.h"
#include "TFFU/Sensors.h"



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

bool sysTickflag = false;

void TFFUMain(void)
{
	/* Setup */
	//SWO_Setup();
	UART_Init();
	Params_SetDefaults();
	HAL_TIM_Base_Start(&htim3);

	//HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, (GPIO_PinState)(1));
	Motor motorL(TIM1, TIM3, 0, MOT1_DIR_GPIO_Port, MOT1_DIR_Pin, &htim1, &htim3);
	Motor motorR(TIM2, TIM3, 1, MOT2_DIR_GPIO_Port, MOT2_DIR_Pin, &htim2, &htim3);
	motorL.setSpeed(0);
	motorR.setSpeed(0);

	Sensors sensors(4);
	sensors.Start();

	/* Main loop */
	while(1)
	{
		uint32_t systicks = HAL_GetTick();
		ParseSerial(systicks);

		if(AllParams.ManualThrottle > 50)
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(1));
		else
			HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(0));

		if (sysTickflag== true)
		{
			sysTickflag = false;
			uint32_t millis = systicks % 1000;

			motorL.update();
			motorR.update();
			sensors.Update();


			if (millis == 0) // heartbeat
			{
				//HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(1));
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(1));
				//SendMonVar(MONVAR_VOLTAGE, VARTYPE_UDWORD, &systicks);
			}
			else if (millis == 100) // heartbeat
			{
				//HAL_GPIO_WritePin(LED_L_GPIO_Port,LED_L_Pin, (GPIO_PinState)(0));
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, (GPIO_PinState)(0));
				//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			}
			//if (systicks % 10 == 0) // motors
			{
				float speed_l, speed_r;
				float drivethrottle = AllParams.ManualThrottle/255.0*MOT_PWM_MAX;
				float driveangle = -sensors.curOffset;//AllParams.ManualAngle/128.f;
			    if( driveangle <= 0.f ) // Left
			    {
			        speed_l = driveangle;
			        speed_r = -driveangle;
			    }
			    else // Right
			    {
			        speed_l = driveangle;
			        speed_r = -driveangle;
			    }
			    float steerspd = AllParams.MaxPWM;
			    speed_l = -driveangle*steerspd;
			    speed_r = driveangle*steerspd;

				motorL.setSpeed((int32_t)(speed_l+drivethrottle));
				motorR.setSpeed((int32_t)(speed_r+drivethrottle));
			}
			if (systicks % 200 == 0)
			{
				HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(1));
				//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x34), 3, 100) != HAL_OK) {}
				//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x36), 3, 100) != HAL_OK) {}

				SendMonVar(MONVAR_2, VARTYPE_SDWORD, &motorL.realSpeed);
				SendMonVar(MONVAR_3, VARTYPE_SDWORD, &motorR.realSpeed);
				//SendMonVar(MONVAR_1, VARTYPE_FLOAT, &sensors.curOffset);
				//SendMonVar(MONVAR_SENSORS, VARTYPE_UWORD, &sensors.thresholds);

				//int outstrlen = 0;

				//char formatenc[] = "%05d %05d";
				//outstrlen += sprintf(aTxBuffer+outstrlen, formatenc, tim1val, tim2val);

				//uint8_t vals[14*2];

				/*uint8_t targetvalue = 1;
				HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x34), &targetvalue, 1, 100);
				HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x34), vals, 7*2, 100);
				HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(0x36), &targetvalue, 1, 100);
				HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x36), vals+7*2, 7*2, 100);
				char format16[] = "%04d ";
				for(int i=0; i<14; i++)
					outstrlen += sprintf(aTxBuffer+outstrlen, format16, ((uint16_t*)vals)[i]);
				aTxBuffer[outstrlen++] = '\n';
				UART_Send((uint8_t*)aTxBuffer, outstrlen);*/

				//HAL_Delay(1);
				HAL_GPIO_WritePin(LED_F_GPIO_Port,LED_F_Pin, (GPIO_PinState)(0));
			}

			/// MONITORING / DEBUG
			if( AllParams.MonitoringInterval > 0 )
			{
			}
		}
	}
}

void TFFUSysTick(void)
{
	sysTickflag = true;
}
