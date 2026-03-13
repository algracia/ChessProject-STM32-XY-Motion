#include "PwmDriver.h"

void pwm_Config(PWM_Handler_t *ptrPwmHandler){

	
	if(ptrPwmHandler->ptrTIMx == TIM2){
		
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM3){
		
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM4){
			
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		}
	else if(ptrPwmHandler->ptrTIMx == TIM5){
				
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
			}
	else{
		__NOP();
	}

	
	setPeriod(ptrPwmHandler);

	
	setPulseWidth(ptrPwmHandler);

	
	
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

	
	ptrPwmHandler->ptrTIMx->CNT = 0;

	

	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC1S_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC1M_Msk; 

		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1M_1; 
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1M_2; 
		

		
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1PE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1FE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC1CE_Msk;

		break;
	}

	case PWM_CHANNEL_2:{
		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC2S_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC2M_Msk; 

		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2M_1; 
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2M_2; 
		

		
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2PE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2FE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC2CE_Msk;

		break;
	}

	
	

	case PWM_CHANNEL_3:{
		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_CC1S_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_OC1M_Msk; 

		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC1M_1; 
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC1M_2; 
		

		
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC1PE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC1FE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_OC1CE_Msk;

		break;
	}

	case PWM_CHANNEL_4:{
		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_CC2S_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_OC2M_Msk; 

		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC2M_1; 
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC2M_2; 
		

		
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC2PE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR1_OC2FE_Msk;

		
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR1_OC2CE_Msk;

		break;
	}

	default:{
		break;
	}
	}

	
	enableOutput(ptrPwmHandler);

	
	setPolarity(ptrPwmHandler);

	
	
	__disable_irq();

	
	ptrPwmHandler->ptrTIMx->DIER &= 0;

	
	
	switch(ptrPwmHandler->config.interruption){

	
	case PWM_PERIOD_INTERRUPT_ENABLE:{
		ptrPwmHandler->ptrTIMx->DIER |= TIM_DIER_UIE;

		break;
	}

	
	case PWM_ALL_INTERRUPT_DISABLE: {
		ptrPwmHandler->ptrTIMx->DIER &= ~TIM_DIER_UIE;

		break;
	}

	default: {
		ptrPwmHandler->ptrTIMx->DIER &= ~TIM_DIER_UIE;
		break;
	}
	}

	
	if(ptrPwmHandler->ptrTIMx == TIM2){
		
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrPwmHandler->ptrTIMx == TIM3){
		
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrPwmHandler->ptrTIMx == TIM4){
		
		NVIC_EnableIRQ(TIM4_IRQn);
			}
	else if(ptrPwmHandler->ptrTIMx == TIM5){
		
		NVIC_EnableIRQ(TIM5_IRQn);
			}
	else{
		__NOP();
	}

	
	__enable_irq();

}

void startPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	
	ptrPwmHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;

}

void stopPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_CEN;
	disableOutput(ptrPwmHandler);
}

void enableOutput(PWM_Handler_t *ptrPwmHandler) {

	switch (ptrPwmHandler->config.channel) {

	case PWM_CHANNEL_1: {
		
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC1E_Msk;

		break;
	}

	case PWM_CHANNEL_2: {
		
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC2E_Msk;

		break;
	}

	case PWM_CHANNEL_3: {
		
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC3E_Msk;

		break;
	}

	case PWM_CHANNEL_4: {
		
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC4E_Msk;

		break;
	}

	default: {
		break;
	}
	}
}

void disableOutput(PWM_Handler_t *ptrPwmHandler) {

	switch (ptrPwmHandler->config.channel) {

	case PWM_CHANNEL_1: {
		
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC1E_Msk;

		break;
	}

	case PWM_CHANNEL_2: {
		
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC2E_Msk;

		break;
	}

	case PWM_CHANNEL_3: {
		
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC3E_Msk;

		break;
	}

	case PWM_CHANNEL_4: {
		
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC4E_Msk;

		break;
	}

	default: {
		break;
	}
	}
}

void setPeriod(PWM_Handler_t *ptrPwmHandler){

	
	
	ptrPwmHandler->ptrTIMx->PSC = ptrPwmHandler->config.prescaler;

	
	
	ptrPwmHandler->ptrTIMx->ARR = ptrPwmHandler->config.period - 1;

	
	ptrPwmHandler->ptrTIMx->CR1 |= TIM_CR1_ARPE_Msk;
}

void updatePeriod(PWM_Handler_t *ptrPwmHandler, uint16_t newPeriod){
	
    ptrPwmHandler->config.period = newPeriod;

	
	setPeriod(ptrPwmHandler);
}

void setPulseWidth(PWM_Handler_t *ptrPwmHandler){

	
	switch(ptrPwmHandler->config.channel){

	case PWM_CHANNEL_1:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.pulseWidth;

		break;
	}

	case PWM_CHANNEL_2:{
		ptrPwmHandler->ptrTIMx->CCR2 = ptrPwmHandler->config.pulseWidth;

		break;
	}

	case PWM_CHANNEL_3:{
		ptrPwmHandler->ptrTIMx->CCR3 = ptrPwmHandler->config.pulseWidth;

		break;
	}

	case PWM_CHANNEL_4:{
		ptrPwmHandler->ptrTIMx->CCR4 = ptrPwmHandler->config.pulseWidth;

		break;
	}

	default:{
		break;
	}

	}

}

void updatePulseWidth(PWM_Handler_t *ptrPwmHandler, uint16_t newPW){
	
	ptrPwmHandler->config.pulseWidth = newPW;

	
	setPulseWidth(ptrPwmHandler);
}

void setPolarity(PWM_Handler_t *ptrPwmHandler){

	if(ptrPwmHandler->config.polarity == PWM_POLARITY_ACTIVE_LOW){
		
		

		switch (ptrPwmHandler->config.channel) {

			case PWM_CHANNEL_1: {
				
				ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC1P_Msk;

				break;
			}

			case PWM_CHANNEL_2: {
				
				ptrPwmHandler->ptrTIMx->CCER  |= TIM_CCER_CC2P_Msk;

				break;
			}

			case PWM_CHANNEL_3: {
				
				ptrPwmHandler->ptrTIMx->CCER  |= TIM_CCER_CC3P_Msk;

				break;
			}

			case PWM_CHANNEL_4: {
				
				ptrPwmHandler->ptrTIMx->CCER  |= TIM_CCER_CC4P_Msk;

				break;
			}

			default: {
				break;
			}
		}
	}

	else {
		
		

		switch (ptrPwmHandler->config.channel) {

			case PWM_CHANNEL_1: {
				
				ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC1P_Msk;

				break;
			}

			case PWM_CHANNEL_2: {
				
				ptrPwmHandler->ptrTIMx->CCER  &= ~TIM_CCER_CC2P_Msk;

				break;
			}

			case PWM_CHANNEL_3: {
				
				ptrPwmHandler->ptrTIMx->CCER  &= ~TIM_CCER_CC3P_Msk;

				break;
			}

			case PWM_CHANNEL_4: {
				
				ptrPwmHandler->ptrTIMx->CCER  &= ~TIM_CCER_CC4P_Msk;

				break;
			}

			default: {
				break;
			}
		}
	}

}
