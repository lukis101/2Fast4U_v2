
#include "TFFU/Motor.h"
#include "TFFU/TFFUMain.h"

Motor::Motor(TIM_TypeDef* encTimer, TIM_TypeDef* pwmTimer, int pwmChannel,
		GPIO_TypeDef* dirPort, int dirPin)
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
}

void Motor::Enable(TIM_HandleTypeDef* encHTimer, TIM_HandleTypeDef* pwmHTimer)
{
	HAL_TIM_Encoder_Start(encHTimer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(pwmHTimer, pwmChan*4);
}
void Motor::Disable(TIM_HandleTypeDef* encHTimer, TIM_HandleTypeDef* pwmHTimer)
{
	//HAL_TIM_Encoder_Stop(encHTimer, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(pwmHTimer, pwmChan*4);
}
void Motor::Update()
{
	uint16_t newEncVal = -encTim->CNT;
	int16_t encdelta = newEncVal-lastEncVal;
	encPos += (int32_t)encdelta;
	realSpeed = encdelta;
	lastEncVal = newEncVal;

	//float err = realSpeed/120.f-;
	int16_t motspd = targetSpeed;//+(targetSpeed-realSpeed/120.f);
	uint16_t pwm;
	if (motspd >= 0)
	{
		pwm = MOT_PWM_MAX-motspd;
		SetPin(dirPin_port, dirPin_mask); // High
	}
	else
	{
		pwm = -motspd;
		ClearPin(dirPin_port, dirPin_mask); // Low
	}
	*(&(pwmTim->CCR1)+pwmChan) = pwm;
}
void Motor::SetSpeed(int32_t newspeed)
{
	if(newspeed > MOT_PWM_MAX)
		newspeed = MOT_PWM_MAX;
	else if(newspeed < -MOT_PWM_MAX)
		newspeed = -MOT_PWM_MAX;
	targetSpeed = newspeed;

}

void Motor::ResetPos()
{
	encPos = 0;
}
