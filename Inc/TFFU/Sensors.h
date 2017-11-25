
#ifndef TFFU_SENSORS_H_
#define TFFU_SENSORS_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define SENSORS_ADDR1 0x34
#define SENSORS_ADDR2 0x36

#define SENSORS_THRESHOLD 200
#define SENSORS_MAXVALUE  0x3FF

extern uint16_t sensvals[14];

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

	uint16_t thresholds; // Latest single reading
	float lastOffset;
	float curOffset;

public:
	Sensors(int dummy);
	//void Init(void);
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
