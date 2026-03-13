#include "ExtiDriver.h"
#include "GPIOxDriver.h"

void ExtInt_Config(EXTI_Config_t *extiConfig){

	
	GPIO_Config(extiConfig->pGPIOHandler);

	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	

	switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {
	
	case 0: {
		
		
		SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI0_Pos);

		
		
		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PH);

		} else {
			__NOP();
		}

		break;
	}

	
	case 1: {
		
		
		SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI1_Msk);

		
        if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PA);

        }else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PB);

        }else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PC);

        }else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PD);

        }else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PE);

        }else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
        	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PH);
        }else{
        	__NOP();
        }
        break;
	}

	
	case 2: {
		
        
		SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI2_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 3: {
		
        
		SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI3_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 4: {
		
        
		SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI4_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 5: {
		
        
		SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI5_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 6: {
		
        
		SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI6_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 7: {
		
        
		SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI7_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 8: {
		
        
		SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI8_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 9: {
		
        
		SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI9_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 10: {
		
        
		SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI10_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 11: {
		
        
		SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI11_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 12: {
		
        
		SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI12_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 13: {
		
        
		SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 14: {
		
        
		SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI14_Msk);

		
		if(extiConfig->pGPIOHandler->pGPIOx == GPIOA){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PA);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOB){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PB);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOC){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PC);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOD){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PD);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOE){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PE);

		}else if(extiConfig->pGPIOHandler->pGPIOx == GPIOH){
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PH);

		}else{
			__NOP();
		}
		break;
	}

	
	case 15: {
		
		
		SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI15_Pos);

		
		
		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PH);

		} else {
			__NOP();
		}
		break;
	}

	default: {
		__NOP();
		break;
	}

	}

	
	if(extiConfig->edgeType == EXTERNAL_INTERRUPT_FALLING_EDGE){
		
		EXTI->RTSR &= ~(SET << extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber); 	
        EXTI->FTSR |= (SET << extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);		
	}
	else{
		
		EXTI->FTSR &= ~(SET << extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber); 	
        EXTI->RTSR |= (SET << extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);		

	}

	
    __disable_irq();

	
	
	EXTI->IMR |= (SET << extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	

	switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {
	case 0: {
		__NVIC_EnableIRQ(EXTI0_IRQn);
		break;
	}

	case 1: {
		__NVIC_EnableIRQ(EXTI1_IRQn);
		break;
	}

	case 2: {
		__NVIC_EnableIRQ(EXTI2_IRQn);
		break;
	}

	case 3: {
		__NVIC_EnableIRQ(EXTI3_IRQn);
		break;
	}

	case 4: {
		__NVIC_EnableIRQ(EXTI4_IRQn);
		break;
	}

	case 5: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 6: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 7: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 8: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 9: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 10: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	case 11: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	case 12: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	case 13: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	case 14: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	case 15: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	default: {
		break;
	}

	}

	
	__enable_irq();

}

__attribute__ ((weak)) void callback_extInt0(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt1(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt2(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt3(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt4(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt5(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt6(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt7(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt8(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt9(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt10(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt11(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt12(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt13(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt14(void){
	__NOP();
}

__attribute__ ((weak)) void callback_extInt15(void){
	__NOP();
}

void EXTI0_IRQHandler(void){
	
	if(EXTI->PR & EXTI_PR_PR0){
		
		EXTI->PR |= EXTI_PR_PR0;

		
		callback_extInt0();
	}
}

void EXTI1_IRQHandler(void){
	
	if(EXTI->PR & EXTI_PR_PR1){
		
		EXTI->PR |= EXTI_PR_PR1;

		
		callback_extInt1();
	}
}

void EXTI2_IRQHandler(void){
	
	if(EXTI->PR & EXTI_PR_PR2){
		
		EXTI->PR |= EXTI_PR_PR2;

		
		callback_extInt2();
	}
}

void EXTI3_IRQHandler(void){
	
	if(EXTI->PR & EXTI_PR_PR3){
		
		EXTI->PR |= EXTI_PR_PR3;

		
		callback_extInt3();
	}
}

void EXTI4_IRQHandler(void){
	
	if(EXTI->PR & EXTI_PR_PR4){
		
		EXTI->PR |= EXTI_PR_PR4;

		
		callback_extInt4();
	}
}

void EXTI9_5_IRQHandler(void){
	if(EXTI->PR & EXTI_PR_PR5){
		
		EXTI->PR |= EXTI_PR_PR5;

		
		callback_extInt5();

	}else if(EXTI->PR & EXTI_PR_PR6){
		
		EXTI->PR |= EXTI_PR_PR6;

		
		callback_extInt6();

	}else if(EXTI->PR & EXTI_PR_PR7){
		
		EXTI->PR |= EXTI_PR_PR7;

		
		callback_extInt7();

	}else if(EXTI->PR & EXTI_PR_PR8){
		
		EXTI->PR |= EXTI_PR_PR8;

		
		callback_extInt8();

	}else if(EXTI->PR & EXTI_PR_PR9){
		
		EXTI->PR |= EXTI_PR_PR9;

		
		callback_extInt9();
	}
	else{
		__NOP();
	}
}

void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & EXTI_PR_PR10){
		
		EXTI->PR |= EXTI_PR_PR10;

		
		callback_extInt10();

	}else if(EXTI->PR & EXTI_PR_PR11){
		
		EXTI->PR |= EXTI_PR_PR11;

		
		callback_extInt11();

	}else if(EXTI->PR & EXTI_PR_PR12){
		
		EXTI->PR |= EXTI_PR_PR12;

		
		callback_extInt12();

	}else if(EXTI->PR & EXTI_PR_PR13){
		
		EXTI->PR |= EXTI_PR_PR13;

		
		callback_extInt13();

	}else if(EXTI->PR & EXTI_PR_PR14){
		
		EXTI->PR |= EXTI_PR_PR14;

		
		callback_extInt14();

	}else if(EXTI->PR & EXTI_PR_PR15){
		
		EXTI->PR |= EXTI_PR_PR15;

		
		callback_extInt15();
	}
	else{
		__NOP();
	}
}
