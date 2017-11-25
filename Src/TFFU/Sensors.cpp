
#include "TFFU/Sensors.h"
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "TFFU/params.h"
#include "TFFU/HW/I2C.h"
#include "TFFU/TFFUMain.h"

const int8_t WEIGHTS[14] = {-52, -44, -36, -28, -20, -12, -4, 4, 12, 20, 28, 36, 44, 52};
uint8_t dataready = 0;
uint8_t readIdx = 0;
uint16_t sensvals[14];

Sensors::Sensors(int dummy)
{
    sens_enabled = false;
    thresholds = 0;
    curOffset = 0;
    lastOffset = 0;
}

void Sensors::Start(void)
{
    uint8_t writecmd[] = {0x80, 0x01};
	uint8_t targetvalue = 1;
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(SENSORS_ADDR1), 3, 100) != HAL_OK) {}
	while (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(SENSORS_ADDR2), 3, 100) != HAL_OK) {}
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), writecmd, 2, 500); // Enable
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR1), &targetvalue, 1, 100); // Select raw values
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR2), writecmd, 2, 500); // Enable
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SENSORS_ADDR2), &targetvalue, 1, 100); // Select raw values
    sens_enabled = true;

    readIdx = 0;
    dataready = 0;
	StartRead();
}
void Sensors::Stop(void)
{
    sens_enabled = false;
}
void Sensors::Update(void)
{
	if(sens_enabled)
	{
		if(dataready != 0)
		{
			if(readIdx == 1)
			{
				UpdateOffset();
				readIdx = 0;
			}
			else
				readIdx = 1;
			dataready = 0;
			StartRead();
		}
	}
}
void Sensors::StartRead(void)
{
	uint8_t* ptr = (uint8_t*)sensvals;
	if (readIdx == 0)
		I2C_Read(SENSORS_ADDR1, 14, ptr, (uint8_t*)&dataready);
	else
		I2C_Read(SENSORS_ADDR2, 14, ptr+14, (uint8_t*)&dataready);
}
void Sensors::UpdateOffset(void)
{
	float unwsum = 0;
	float wsum = 0;
	uint16_t thres = 0;
	uint8_t count = 0;
	for( uint8_t i=0; i<14; i++ )
	{
		uint16_t invValue = SENSORS_MAXVALUE - sensvals[i];
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
		float newoffs = wsum/unwsum/20;
		if(newoffs > 1.f)
			newoffs = 1.f;
		else if(newoffs < -1.f)
			newoffs = -1.f;
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

