
#include "TFFU/Motor.h"

Motor::Motor(TIM_TypeDef* encTimer, TIM_TypeDef* pwmTimer, int pwmChannel,
		GPIO_TypeDef* dirPort, int dirPin,
		TIM_HandleTypeDef* encHTimer, TIM_HandleTypeDef* pwmHTimer)
{
	encPos = 0;
	lastEncVal = 0;
	realSpeed = 0;
	targetSpeed = 1;
	encTim = encTimer;
	pwmTim = pwmTimer;
	pwmChan = pwmChannel;
	dirPin_port = dirPort;
	dirPin_mask = dirPin;
	HAL_TIM_Encoder_Start(encHTimer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(pwmHTimer, pwmChannel*4);
}

void Motor::update()
{
	uint16_t newEncVal = encTim->CNT;
	int16_t encdelta = newEncVal-lastEncVal;
	encPos += (int32_t)encdelta;
	realSpeed = encdelta;
	lastEncVal = newEncVal;

	//float err = realSpeed/120.f-;
	int16_t motspd = targetSpeed+(targetSpeed-realSpeed/120.f);
	uint16_t pwm;
	if (motspd >= 0)
	{
		pwm = MOT_PWM_MAX-motspd;
		dirPin_port->BSRR = dirPin_mask; // High
	}
	else
	{
		pwm = -motspd;
		dirPin_port->BRR  = dirPin_mask; // Low
	}
	*(&(pwmTim->CCR1)+pwmChan) = pwm;
}
void Motor::setSpeed(int32_t newspeed)
{
	if(newspeed > MOT_PWM_MAX)
		newspeed = MOT_PWM_MAX;
	else if(newspeed < -MOT_PWM_MAX)
		newspeed = -MOT_PWM_MAX;
	targetSpeed = newspeed;

}

void Motor::resetPos()
{
	encPos = 0;
}
