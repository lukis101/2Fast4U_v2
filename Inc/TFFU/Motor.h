
#ifndef TFFU_MOTOR_H_
#define TFFU_MOTOR_H_
#ifdef __cplusplus
 extern "C" {
#endif

#define MOT_PWM_MAX 100

#include <stdint.h>
#include "stm32f3xx_hal.h"

class Motor
{
private:
	uint16_t lastEncVal;
	TIM_TypeDef* encTim;
	TIM_TypeDef* pwmTim;
	GPIO_TypeDef* dirPin_port;
	int dirPin_mask;
	int pwmChan;
	//TIM_HandleTypeDef* encHTim;
	//TIM_HandleTypeDef* pwmHTim;
public:
	int32_t encPos;
	int32_t realSpeed;
	int32_t targetSpeed;
public:
	Motor(TIM_TypeDef* encTimer, TIM_TypeDef* pwmTimer, int pwmChannel,
			GPIO_TypeDef* dirPort, int dirPin,
			TIM_HandleTypeDef* encHTimer, TIM_HandleTypeDef* pwmHTimer);
	void update(void);
	void setSpeed(int32_t newspeed);
	void resetPos(void);
};

#ifdef __cplusplus
}
#endif
#endif /* TFFU_MOTOR_H_ */
