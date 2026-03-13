#ifndef INC_BASICTIMER_H_
#define INC_BASICTIMER_H_

#include "stm32f4xx.h"

#define BTIMER_MODE_UP				0
#define BTIMER_MODE_DOWN			1

#define BTIMER_SPEED_10us			160
#define BTIMER_SPEED_100us			1600
#define BTIMER_SPEED_1ms			16000

#define BTIMER_80MHz_10us			800
#define BTIMER_80MHz_100us			8000

#define BTIMER_100MHz_1us			100
#define BTIMER_100MHz_10us			1000
#define BTIMER_100MHz_100us			10000

#define BTIMER_INTERRUPT_ENABLE		0
#define BTIMER_INTERRUPT_DISABLE	1

typedef struct
{
	uint8_t		TIMx_mode; 		
	uint32_t	TIMx_speed;		
	uint32_t	TIMx_period;	
	uint8_t		TIMx_interruptEnable;	
}BasicTimer_Config_t;

typedef struct
{
	TIM_TypeDef			*ptrTIMx;
	BasicTimer_Config_t	TIMx_Config;
}BasicTimer_Handler_t;

void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler);
void BasicTimer2_Callback(void); 
void BasicTimer3_Callback(void);
void BasicTimer4_Callback(void);
void BasicTimer5_Callback(void);

#endif
