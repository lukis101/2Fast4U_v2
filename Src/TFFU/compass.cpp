
#include "TFFU/Compass.h"
#include "TFFU/Params.h"

Compass::Compass(void)
{
	direction = 0;
	dataReady = 0;
}

void Compass::Start(void)
{
}

void Compass::Update(void)
{
	if (dataReady != 0)
	{
		dataReady = 0;
	}
}

void Compass::Zero(void)
{
}
