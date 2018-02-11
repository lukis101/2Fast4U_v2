
#include <TFFU/Buzzer.h>

const uint8_t melodies[2][6] = {
	{4,20,40,60,80, 0},
	{4,20,40,60,80, 0}
};

Buzzer::Buzzer()
{
	// TODO Auto-generated constructor stub
}

Buzzer::~Buzzer()
{
	// TODO Auto-generated destructor stub
}

void Buzzer::Update(uint32_t systime)
{
	if (systime >= nextUpdate)
	{
		curNote++;
		if (curNote >= melodies[curMelody][0])
		{
			//pwm =0

			curNote = 0;
			curMelody++;
		}
	}
}

void Buzzer::PlayMelody(Melody_t melody)
{

}
