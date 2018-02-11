
#ifndef TFFU_SENSORS_H_
#define TFFU_SENSORS_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define SENSORS_ADDR1 0x34
#define SENSORS_ADDR2 0x36

#define SENSORS_THRESHOLD 200
#define SENSORS_MAXVALUE  (0x3FF)
#define SENSORS_AMOUNT    14
extern uint16_t sensvals[SENSORS_AMOUNT];

class Sensors
{
private:
	uint8_t dataReady;
	uint8_t groupIdx;
	const uint8_t SENSORS_BUFFSIZE = 10;
	//static int16_t offsets[ SENSORS_BUFFSIZE ]; // History of readings
	static int16_t offsetSum; // Summed readings // TODO: 16/32
	static int16_t latestOffset;
	static int8_t latestNum;
	static int8_t sampleCount;
	bool enabled = false;

public:
	//volatile uint16_t sens_values; // Latest single reading
	int16_t sens_lastOffset = 0;   // Averaged
	int16_t sens_curOffset  = 0;   // Averaged
	int16_t sens_curNum  = 0;
	int16_t sens_lastNum  = 0;

	uint16_t thresholds; // Latest single reading
	uint16_t rawValues[SENSORS_AMOUNT];
	float values[SENSORS_AMOUNT];
	float lastOffset;
	float curOffset;

public:
	Sensors();
	void Update(void);
	void BlockingUpdate(void);
	void UpdateOffset(void);
	void Start(void);
	void StartRead(void);
	void Stop(void);
	void Calibrate(void);
};

#ifdef __cplusplus
}
#endif
#endif /* TFFU_SENSORS_H_ */
