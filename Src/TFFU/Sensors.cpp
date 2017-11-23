
#include "TFFU/Sensors.h"
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "i2c.h"

const int8_t WEIGHTS[14] = {-52, -44, -36, -28, -20, -12, -4, 4, 12, 20, 28, 36, 44, 52};

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
}
void Sensors::Stop(void)
{
    sens_enabled = false;
}
void Sensors::Update(void)
{
	if(sens_enabled)
	{
		uint8_t sensvals[14*2];
		uint16_t* rawvalues = (uint16_t*)sensvals;
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x34), sensvals,    14, 10);
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(0x36), sensvals+14, 14, 10);

		float unwsum = 0;
		float wsum = 0;
		uint16_t thres = 0;
		uint8_t count = 0;
        for( uint8_t i=0; i<14; i++ )
        {
        	uint16_t invValue = SENSORS_MAXVALUE - rawvalues[i];
        	float absval = 0;
            if( invValue > SENSORS_THRESHOLD )
            {
            	thres |= (1<<i);
                count++;
                absval =  1;
                //absval = ((float)invValue)/(SENSORS_MAXVALUE*0.8);
            }
            unwsum += absval;
            wsum += absval * WEIGHTS[ i ];
        }
        if(count > 0 || 0)
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
}

