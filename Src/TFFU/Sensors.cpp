
#include "TFFU/Sensors.h"
#include <stdint.h>

const int8_t WEIGHTS[16] = {-52, -44, -36, -28, -20, -12, -4, 4, 12, 20, 28, 36, 44, 52};

Sensors::Sensors(void)
{
    sens_enabled = false;
}

void Sensors::Init(void)
{
    sens_enabled = false;
}
void Sensors::Update(void)
{
    sens_enabled = false;
}

