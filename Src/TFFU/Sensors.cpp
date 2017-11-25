
#include "TFFU/Sensors.h"
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "TFFU/params.h"
#include "TFFU/HW/I2C.h"
#include "TFFU/TFFUMain.h"

#define WEIGTH_MAX (7.0f)
const int8_t WEIGHTS[14] = {-7, -6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 9, 7};

static volatile uint16_t rawValueBuff[SENSORS_AMOUNT];

Sensors::Sensors()
{
    enabled = false;
    thresholds = 0;
    curOffset = 0;
    lastOffset = 0;
    groupIdx = 0;
    dataReady = 0;
}

void Sensors::Start(void)
{
    uint8_t writecmd[] = {0x80, 0x01};
	uint8_t targetreg = 1;
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(SENSORS_ADDR1), 3, 100) != HAL_OK) {}
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), writecmd, 2, 500); // Enable
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), &targetreg, 1, 100); // Select raw values
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(SENSORS_ADDR2), 3, 100) != HAL_OK) {}
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR2), writecmd, 2, 500); // Enable
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR2), &targetreg, 1, 100); // Select raw values
    enabled = true;

    groupIdx = 0;
    dataReady = 0;
	StartRead();
}
void Sensors::Stop(void)
{
    enabled = false;
}
void Sensors::Update(void)
{
	if(enabled)
	{
		if(dataReady != 0)
		{
			if(groupIdx == 1)
			{
				UpdateOffset();
				groupIdx = 0;
			}
			else
				groupIdx = 1;
			dataReady = 0;
			StartRead();
		}
	}
}
void Sensors::StartRead(void)
{
	uint8_t* ptr = (uint8_t*)rawValueBuff;
	if (groupIdx == 0)
		I2C_Read(SENSORS_ADDR1, 14, ptr, &dataReady);
	else
		I2C_Read(SENSORS_ADDR2, 14, ptr+14, &dataReady);
}
void Sensors::UpdateOffset(void)
{
	float unwsum = 0;
	float wsum = 0;
	uint16_t thres = 0;
	uint8_t count = 0;
	for( uint8_t i=0; i<14; i++ )
	{
		uint16_t invValue = SENSORS_MAXVALUE - rawValueBuff[i];
		float absval = 0;
		if( invValue > AllParams.SensThreshold )
		{
			thres |= (1<<i);
			count++;
			absval = 1;
			//absval = ((float)invValue)/(SENSORS_MAXVALUE*0.8);
		}
		unwsum += absval;
		wsum += absval * WEIGHTS[ i ];
	}
	if(count > 0 )
	{
		float newoffs = wsum/unwsum/WEIGTH_MAX;
		/*if(newoffs > 1.f)
			newoffs = 1.f;
		else if(newoffs < -1.f)
			newoffs = -1.f;*/
		curOffset = newoffs;
		//offsetSum += weightedSum;
	}
	else
	{
		curOffset += sens_lastOffset;
	}
	thresholds = thres;
	//curOffset = wsum	;
}

