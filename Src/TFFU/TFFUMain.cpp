
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
#include "TFFU/Compass.h"
#include "TFFU/PID.h"


volatile uint32_t last;

char aTxBuffer[100];


// VARS
uint8_t raceActive = false;
uint8_t standby = true;
bool startFlag;
bool stopFlag;
int32_t totalpos;

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
void JumpToBootloader();

Motor motorL = Motor(TIM1, TIM3, 0, MOT1_DIR_GPIO_Port, MOT1_DIR_Pin);
Motor motorR = Motor(TIM2, TIM3, 1, MOT2_DIR_GPIO_Port, MOT2_DIR_Pin);
Sensors sensors = Sensors();
Compass compass = Compass();
PID PID_Angle(&AllParams.PID_Angle);

void TFFUMain(void)
{
	/* Setup */
	//SWO_Setup();
	UART_Init();
	Params_SetDefaults();
	HAL_TIM_Base_Start(&htim3);
	motorL.Enable(&htim1, &htim3);
	motorR.Enable(&htim2, &htim3);
	//HAL_TIM_Base_Start(&htim4);
	//HAL_TIM_PWM_Start(&htim4, 0);

	//HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, (GPIO_PinState)(1));
	SetDriveMode((uint8_t)DRIVEMODE_STOP);

	sensors.Start();
	compass.Start();

	/* Main loop */
	while(1)
	{
		ParseSerial();

		if (sysTickflag== true)
		{
			sysTickflag = false;
			uint32_t systicks = HAL_GetTick();
			uint32_t millis = systicks % 1000;

			SetPin(LED_L_GPIO_Port, LED_L_Pin); // Red ON
			SerialTick();
			sensors.Update();
			motorL.Update();
			motorR.Update();
			totalpos = (motorL.encPos+motorR.encPos)/2;

            //PID_Update( sensors.curOffset );

			if (millis == 0) // heartbeat
				SetPin(LED_R_GPIO_Port, LED_R_Pin); // Blue ON
			else if (millis == 100) // heartbeat
				ClearPin(LED_R_GPIO_Port, LED_R_Pin); // Blue OFF

			float speed_l, speed_r;
			float drivethrottle, driveangle;
			switch (AllParams.DriveMode)
			{
			case DRIVEMODE_STOP:
				if (HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin) == GPIO_PIN_RESET)
					SetDriveMode(DRIVEMODE_SIMPLE);
				//if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_RESET)
				//	AllParams.RaceMode = RACEMODE_SIMPLE;
				//SetPin(LED_F_GPIO_Port, LED_F_Pin); // Red ON
				break;
			case DRIVEMODE_MANUAL:
				drivethrottle = AllParams.ManualThrottle/255.0*MOT_PWM_MAX;
				driveangle = (AllParams.ManualAngle/128.f)*drivethrottle;
			    speed_l = driveangle+drivethrottle;
			    speed_r = -driveangle+drivethrottle;

				motorL.SetSpeed((int32_t)speed_l);
				motorR.SetSpeed((int32_t)speed_r);
				break;
			case DRIVEMODE_SIMPLE:
				//if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_RESET)
					//SetDriveMode(DRIVEMODE_STOP);
				float pidang = -PID_Angle.Update(sensors.curOffset);
				drivethrottle = (float)AllParams.Speed;
				driveangle = pidang*(MOT_PWM_MAX);
			    speed_l = -driveangle+drivethrottle;
			    speed_r = driveangle+drivethrottle;

				motorL.SetSpeed((int32_t)speed_l);
				motorR.SetSpeed((int32_t)speed_r);
				break;
			}
			ClearPin(LED_L_GPIO_Port, LED_L_Pin); // Red OFF

			/// MONITORING / DEBUG
			if( AllParams.MonitoringEnable != 0 )
			{
				if (systicks >= nextTick_monitor)
				{
					SetPin(LED_F_GPIO_Port, LED_F_Pin); // Green ON

					//SendMonVar(MONVAR_OFFSET, VARTYPE_FLOAT, &sensors.curOffset);
					SendMonVar(MONVAR_OFFSET, VARTYPE_FLOAT, &sensors.values[6]);
					SendMonVar(MONVAR_SENSORS, VARTYPE_UWORD, &(sensors.thresholds));
					SendMonVar(MONVAR_1, VARTYPE_FLOAT, &PID_Angle.values.p);
					//SendMonVar(MONVAR_2, VARTYPE_FLOAT, &PID_Angle.unstability);
					SendMonVar(MONVAR_3, VARTYPE_FLOAT, &PID_Angle.values.d);
					//SendMonVar(MONVAR_2, VARTYPE_SDWORD, &motorL.realSpeed);
					//SendMonVar(MONVAR_3, VARTYPE_SDWORD, &motorR.realSpeed);
					//SendMonVar(MONVAR_3, VARTYPE_FLOAT, &sensa);

					//SendMonVar(MONVAR_2, VARTYPE_DWORD, &totalpos);
					//while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(MPU9250_ADDRESS), 3, 100) != HAL_OK) {}
					//PrintVarHex(VARTYPE_UDWORD, &totalpos);
					//HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), writecmd, 2, 500); // Enable
					//HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), &targetreg, 1, 100); // Select raw values

					nextTick_monitor = systicks + AllParams.MonitoringInterval;
					ClearPin(LED_F_GPIO_Port, LED_F_Pin); // Green OFF
				}
			}
		}
	}
}
void SetDriveMode(uint8_t newMode)
{
	switch (newMode)
	{
	case DRIVEMODE_STOP:
		HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, GPIO_PIN_RESET);
		motorL.SetSpeed(0);
		motorR.SetSpeed(0);
		motorL.Disable(&htim1, &htim3);
		motorR.Disable(&htim2, &htim3);
		break;
	case DRIVEMODE_MANUAL:
		motorL.Enable(&htim1, &htim3);
		motorR.Enable(&htim2, &htim3);
		motorL.SetSpeed(0);
		motorR.SetSpeed(0);
		HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, GPIO_PIN_RESET);
		break;
	case DRIVEMODE_SIMPLE:
	    PID_Angle.Init();
		motorL.Enable(&htim1, &htim3);
		motorR.Enable(&htim2, &htim3);
		motorL.SetSpeed(0);
		motorR.SetSpeed(0);
		//HAL_GPIO_WritePin(Vmot_EN_GPIO_Port, Vmot_EN_Pin, GPIO_PIN_SET);
		break;
	default:
		SetDriveMode(DRIVEMODE_STOP);
		return;
	}
	AllParams.DriveMode = newMode;
}

void TFFUSysTick(void)
{
	sysTickflag = true;
}

// 0x1FFFF796
void JumpToBootloader(void) {
	void (*SysMemBootJump)(void);

	/**
	 * Step: Set system memory address.
	 *
	 *       For STM32F429, system memory is on 0x1FFF 0000
	 *       For other families, check AN2606 document table 110 with descriptions of memory addresses
	 */
	volatile uint32_t addr = 0x1FFFF796;

	/**
	 * Step: Disable RCC, set it to default (after reset) settings
	 *       Internal clock, no PLL, etc.
	 */
#if defined(USE_HAL_DRIVER)
	HAL_RCC_DeInit();
#endif /* defined(USE_HAL_DRIVER) */
#if defined(USE_STDPERIPH_DRIVER)
	RCC_DeInit();
#endif /* defined(USE_STDPERIPH_DRIVER) */

	/**
	 * Step: Disable systick timer and reset it to default values
	 */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	/**
	 * Step: Disable all interrupts
	 */
	__disable_irq();

	/**
	 * Step: Remap system memory to address 0x0000 0000 in address space
	 *       For each family registers may be different.
	 *       Check reference manual for each family.
	 *
	 *       For STM32F4xx, MEMRMP register in SYSCFG is used (bits[1:0])
	 *       For STM32F0xx, CFGR1 register in SYSCFG is used (bits[1:0])
	 *       For others, check family reference manual
	 */
	//Remap by hand... {
#if defined(STM32F4)
	SYSCFG->MEMRMP = 0x01;
#endif
#if defined(STM32F0)
	SYSCFG->CFGR1 = 0x01;
#endif
	//} ...or if you use HAL drivers
	__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();	//Call HAL macro to do this for you

	/**
	 * Step: Set jump memory location for system memory
	 *       Use address with 4 bytes offset which specifies jump location where program starts
	 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));

	/**
	 * Step: Set main stack pointer.
	 *       This step must be done last otherwise local variables in this function
	 *       don't have proper value since stack pointer is located on different position
	 *
	 *       Set direct address location which specifies stack pointer in SRAM location
	 */
	__set_MSP(*(uint32_t *)addr);

	/**
	 * Step: Actually call our function to jump to set location
	 *       This will start system memory execution
	 */
	SysMemBootJump();

	/**
	 * Step: Connect USB<->UART converter to dedicated USART pins and test
	 *       and test with bootloader works with STM32 Flash Loader Demonstrator software
	 */
}
