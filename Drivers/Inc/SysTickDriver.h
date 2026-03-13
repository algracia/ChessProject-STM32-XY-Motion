#ifndef SYSTICKDRIVER_H_
#define SYSTICKDRIVER_H_

#include <stm32f4xx.h>

#define HSI_CLOCK_CONFIGURED	0
#define HSE_CLOCK_CONFIGURED	1
#define PLL_CLOCK_CONFIGURED	2

#define SYSTICK_LOAD_VALUE_16MHz_1ms	16000	
#define SYSTICK_LOAD_VALUE_100MHz_1ms	100000	

void config_SysTick_ms(uint8_t systemClock);
uint64_t getTicks_ms(void);
void delay_ms(uint32_t wait_time_ms);

#endif
