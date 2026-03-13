#include "GPIOxDriver.h"

void GPIO_Config (GPIO_Handler_t *pGPIOHandler){

	
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	
	
	if (pGPIOHandler -> pGPIOx == GPIOA){
		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	
	else if(pGPIOHandler->pGPIOx == GPIOB){
		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	
		else if(pGPIOHandler->pGPIOx == GPIOC){
			
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	
		else if(pGPIOHandler->pGPIOx == GPIOD){
			
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
	
			else if(pGPIOHandler->pGPIOx == GPIOE){
				
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
		}
	
			else if(pGPIOHandler->pGPIOx == GPIOH){
				
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
		}
			else{
				__NOP();
			}

	

	

	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	

	pGPIOHandler->pGPIOx->MODER &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->MODER |= auxConfig;

	

	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->OTYPER |= auxConfig;

	
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->OSPEEDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->OSPEEDR |= auxConfig;

	
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->PUPDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	pGPIOHandler->pGPIOx->PUPDR |= auxConfig;

	
	if(pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		
		if(pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber < 8){
			
			auxPosition = 4*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

			
			pGPIOHandler->pGPIOx->AFR[0] &= ~(0b1111 << auxPosition);

			
			pGPIOHandler->pGPIOx->AFR[0] |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode<< auxPosition);
		}
		else{
			
			auxPosition = 4*(pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber-8);

			
			pGPIOHandler->pGPIOx->AFR[1] &= ~(0b1111 << auxPosition);

			
			pGPIOHandler->pGPIOx->AFR[1] |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}
	}
}

void GPIO_WritePin(GPIO_Handler_t*pPinHandler, uint8_t newState){
	
	pPinHandler->pGPIOx->ODR &= ~(SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	if(newState == SET){
		
		pPinHandler->pGPIOx->BSRR |= (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	}
	else{
		
		pPinHandler->pGPIOx->BSRR |= (SET << (pPinHandler->GPIO_PinConfig.GPIO_PinNumber+16));
	}
}

uint8_t GPIO_ReadPin(GPIO_Handler_t*pPinHandler){

	
	
	uint32_t pinValue =0;

	
	 pinValue= pPinHandler->pGPIOx->IDR & (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);

	
	 pinValue >>= pPinHandler->GPIO_PinConfig.GPIO_PinNumber;

	return (uint8_t) pinValue;
}

 void GPIOxTooglePin(GPIO_Handler_t*pPinHandler){
	 pPinHandler->pGPIOx->ODR ^= (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
 }
