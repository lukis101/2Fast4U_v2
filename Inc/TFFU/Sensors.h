/*
 * Sensors.h
 *
 *  Created on: 2017-11-20
 *      Author: lukis
 */

#ifndef TFFU_SENSORS_H_
#define TFFU_SENSORS_H_

#include <stdint.h>

class Sensors
{
public:
	const uint8_t SENSORS_BUFFSIZE = 10;
	//static int16_t offsets[ SENSORS_BUFFSIZE ]; // History of readings
	static int16_t offsetSum; // Summed readings // TODO: 16/32
	static int16_t latestOffset;
	static int8_t latestNum;
	static int8_t sampleCount;
	bool sens_enabled = false;

	volatile uint16_t sens_values; // Latest single reading
	int16_t sens_lastOffset = 0;   // Averaged
	int16_t sens_curOffset  = 0;   // Averaged
	int16_t sens_curNum  = 0;
	int16_t sens_lastNum  = 0;

public:
	Sensors();

	void Init(void);
	void Update(void);
	void BlockingUpdate(void);
	void UpdateOffset(void);
	void StartRead(void);
	void Enable(void);
	void Disable(void);
	void Calibrate(void);
};

#endif /* TFFU_SENSORS_H_ */
