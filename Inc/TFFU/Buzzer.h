
#ifndef TFFU_BUZZER_H_
#define TFFU_BUZZER_H_

#include <stdint.h>

typedef enum
{
	MELODY_NONE = 0,
	MELODY_BOOT,
	MELODY_ERROR,
	MELODY_CONNECT,
	MELODY_START,
	MELODY_TURN_LEFT,
	MELODY_TURN_RIGHT,
	MELODY_CROSSROAD
} Melody_t;

class Buzzer
{
public:
	uint32_t nextUpdate;
	Melody_t curMelody;
	uint8_t curNote;
	void Update(uint32_t systime);
	void PlayMelody(Melody_t melody);
	Buzzer();
	virtual ~Buzzer();
};

#endif /* TFFU_BUZZER_H_ */
