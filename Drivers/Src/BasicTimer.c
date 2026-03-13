#include "BasicTimer.h"

TIM_TypeDef	*ptrTimerUsed;

void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler){
	
	ptrTimerUsed = ptrBTimerHandler->ptrTIMx;

	
	__disable_irq();
	
	if(ptrBTimerHandler->ptrTIMx == TIM2){
		
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrBTimerHandler->ptrTIMx == TIM3){
		
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrBTimerHandler->ptrTIMx == TIM4){
			
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		}
	else if(ptrBTimerHandler->ptrTIMx == TIM5){
				
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
			}
	else{
		__NOP();
	}

	

	ptrBTimerHandler->ptrTIMx->PSC = ptrBTimerHandler->TIMx_Config.TIMx_speed;

	
	if(ptrBTimerHandler->TIMx_Config.TIMx_mode == BTIMER_MODE_UP){

		
		

		ptrBTimerHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

		
		ptrBTimerHandler->ptrTIMx->ARR = ptrBTimerHandler->TIMx_Config.TIMx_period - 1 ;

		
		ptrBTimerHandler->ptrTIMx->CNT = 0;

	}else{
		

		ptrBTimerHandler->ptrTIMx->CR1 |= TIM_CR1_DIR;

		

		ptrBTimerHandler->ptrTIMx->ARR = 0;

		

		ptrBTimerHandler->ptrTIMx->CNT = ptrBTimerHandler->TIMx_Config.TIMx_period - 1;
	}

	
	ptrBTimerHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;

	

	if(ptrBTimerHandler->TIMx_Config.TIMx_interruptEnable == BTIMER_INTERRUPT_ENABLE){
		ptrBTimerHandler->ptrTIMx->DIER |= TIM_DIER_UIE;
	}
	else{
		ptrBTimerHandler->ptrTIMx->DIER &= ~TIM_DIER_UIE;
	}

	
	if(ptrBTimerHandler->ptrTIMx == TIM2){
		
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrBTimerHandler->ptrTIMx == TIM3){
		
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrBTimerHandler->ptrTIMx == TIM4){
			
			NVIC_EnableIRQ(TIM4_IRQn);
		}
	else if(ptrBTimerHandler->ptrTIMx == TIM5){
			
			NVIC_EnableIRQ(TIM5_IRQn);
		}
	else{
		__NOP();
	}

	
	__enable_irq();
}

__attribute__((weak)) void BasicTimer2_Callback(void){
	  

	__NOP();
}

__attribute__((weak)) void BasicTimer3_Callback(void){
	  

	__NOP();
}

__attribute__((weak)) void BasicTimer4_Callback(void){
	  

	__NOP();
}

__attribute__((weak)) void BasicTimer5_Callback(void){
	  

	__NOP();
}

void TIM2_IRQHandler(void){
	
	TIM2->SR &= ~TIM_SR_UIF;

	
	BasicTimer2_Callback();

}

void TIM3_IRQHandler(void){
	
	TIM3->SR &= ~TIM_SR_UIF;

	
	BasicTimer3_Callback();

}

void TIM4_IRQHandler(void){
	
	TIM4->SR &= ~TIM_SR_UIF;

	
	BasicTimer4_Callback();

}

void TIM5_IRQHandler(void){
	
	TIM5->SR &= ~TIM_SR_UIF;

	
	BasicTimer5_Callback();

}
