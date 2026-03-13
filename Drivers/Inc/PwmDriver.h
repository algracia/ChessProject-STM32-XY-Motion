#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include "stm32f4xx.h"

#define PWM_CHANNEL_1	0
#define PWM_CHANNEL_2	1
#define PWM_CHANNEL_3	2
#define PWM_CHANNEL_4	3

#define PWM_PRESCALER_1us			16
#define PWM_PRESCALER_10us			160
#define PWM_PRESCALER_100us			1600
#define PWM_PRESCALER_1ms			16000

#define PWM_80MHz_PRESCALER_1us		80
#define PWM_80MHz_PRESCALER_10us	800
#define PWM_80MHz_PRESCALER_100us	8000

#define PWM_100MHz_PRESCALER_1us	100
#define PWM_100MHz_PRESCALER_10us	1000
#define PWM_100MHz_PRESCALER_100us	10000

#define PWM_DUTTY_0_PERCENT		0
#define PWM_DUTTY_100_PERCENT	100

#define PWM_POLARITY_ACTIVE_HIGH	0
#define PWM_POLARITY_ACTIVE_LOW		1

#define PWM_ALL_INTERRUPT_DISABLE		0
#define PWM_PERIOD_INTERRUPT_ENABLE		1

typedef struct
{
	uint8_t		channel; 		
	uint32_t	prescaler;		
	uint16_t	period;		
	uint16_t	pulseWidth;		
	uint8_t 	polarity;		
	uint8_t 	interruption;	

}PWM_Config_t;

typedef struct
{
	TIM_TypeDef		*ptrTIMx;	
	PWM_Config_t	config;	
}PWM_Handler_t;

void pwm_Config(PWM_Handler_t *ptrPwmHandler);
void setPeriod(PWM_Handler_t *ptrPwmHandler);
void updatePeriod(PWM_Handler_t *ptrPwmHandler, uint16_t newPeriod);
void setPulseWidth(PWM_Handler_t *ptrPwmHandler);
void updatePulseWidth(PWM_Handler_t *ptrPwmHandler, uint16_t newPW);
void enableOutput(PWM_Handler_t *ptrPwmHandler);
void disableOutput(PWM_Handler_t *ptrPwmHandler);
void startPwmSignal(PWM_Handler_t *ptrPwmHandler);
void stopPwmSignal(PWM_Handler_t *ptrPwmHandler);
void setPolarity(PWM_Handler_t *ptrPwmHandler);

#endif
