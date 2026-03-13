#include <stm32f4xx.h>
#include "SysTickDriver.h"

uint64_t ticks =0;
uint64_t ticks_start =0;
uint64_t ticks_counting =0;

void config_SysTick_ms(uint8_t systemClock){
	
	ticks=0;

	
	switch(systemClock){

	
	case 0: {
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break;
	}

	
	case 1: {
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break;
	}

	
	case 2:{
		SysTick->LOAD = SYSTICK_LOAD_VALUE_100MHz_1ms;
	}

	
	default:{
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break;
	}
	}

	
	SysTick->VAL =0;

	
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	
	__disable_irq();

	
	NVIC_EnableIRQ(SysTick_IRQn);

	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	
	__enable_irq();

}

uint64_t getTicks_ms(void){
	return ticks;
}

void delay_ms(uint32_t wait_time_ms){
	
	ticks_start = getTicks_ms();

	
	ticks_counting = getTicks_ms();

	while(ticks_counting < (ticks_start + (uint64_t)wait_time_ms)){

		
		ticks_counting = getTicks_ms();
	}
}

void SysTick_Handler(void){
	
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){

		
		SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;

		
		ticks++;
	}
}
