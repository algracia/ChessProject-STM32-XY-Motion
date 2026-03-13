#include <stm32f4xx.h>
#include "USARTxDriver.h"

char sendingData =0;
uint8_t busy =0;

uint8_t auxRxData = '\0';
uint8_t iter =0;

void USART_Config(USART_Handler_t *ptrUsartHandler){
	
	

    
	if(ptrUsartHandler->ptrUSARTx == USART1){
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}

    
	else if(ptrUsartHandler->ptrUSARTx == USART2){
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}

    
	else if(ptrUsartHandler->ptrUSARTx == USART6){
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	}

	
	
	
	
	

	
	ptrUsartHandler->ptrUSARTx->CR1 = 0;
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	
	
    
	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE){
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PCE;

		
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN){
			
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PS;

		}else{
			
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PS;
		}
	}else{
		
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PCE;
	}

	
	if(ptrUsartHandler->USART_Config.USART_datasize == USART_DATASIZE_8BIT){
		
		ptrUsartHandler->ptrUSARTx->CR1 &=~USART_CR1_M;

	}else if(ptrUsartHandler->USART_Config.USART_datasize == USART_DATASIZE_9BIT){
		
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_M;
	}

	
	switch(ptrUsartHandler->USART_Config.USART_stopbits){
	case USART_STOPBIT_1: {
		
		ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP;
		break;
	}
	case USART_STOPBIT_0_5: {
		
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_0;
		break;
	}
	case USART_STOPBIT_2: {
		
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_1_5: {
		
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP;
		break;
	}
	default: {
		
		ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP;
		break;
	}
	}

	
	

	
	ptrUsartHandler->ptrUSARTx->BRR = 0;

	if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600){
		
		
		
		
		ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
	}

	else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200) {
		
		
		
		
		ptrUsartHandler->ptrUSARTx->BRR = 0x0341;
	}

	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
		
		
		
		
		ptrUsartHandler->ptrUSARTx->BRR = 0x008B;
	}

	
	switch(ptrUsartHandler->USART_Config.USART_mode){
	case USART_MODE_TX:
	{
		
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;
		break;
	}
	case USART_MODE_RX:
	{
		
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;
		break;
	}
	case USART_MODE_RXTX:
	{
		
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;
		break;
	}
	case USART_MODE_DISABLE:
	{
		
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		break;
	}

	default:
	{
		
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		break;
	}
	}

	

    __disable_irq();

	
	
	if(ptrUsartHandler->USART_Config.USART_enableIntRX == USART_RX_INTERRUP_ENABLE){
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;

	}else{
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
	}

	
	if(ptrUsartHandler->ptrUSARTx == USART1){
		
		__NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(ptrUsartHandler->ptrUSARTx == USART2){
		
		__NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(ptrUsartHandler->ptrUSARTx == USART6){
		
		__NVIC_EnableIRQ(USART6_IRQn);
	}
	else{
		__NOP();
	}

	
	__enable_irq();

	
	
	if(ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE){
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_UE;
	}

}

char writeChar(USART_Handler_t *ptrUsartHandler, char dataToSend ){

	

	sendingData = dataToSend;

	
	ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TXEIE;

	

	

	ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TXEIE;

	return sendingData;
}

void writeMsg(USART_Handler_t *ptrUsartHandler, char *MsgToSend ){

	
	
	iter = 0;

	while(MsgToSend[iter] != '\0'){
		writeChar(ptrUsartHandler, MsgToSend[iter]);
	}

	busy =0;
}

uint8_t getRxData(void){
	return auxRxData;
}

void USART1_IRQHandler (void){
	
	if(USART1->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART1->DR;
		usart1Rx_Callback();
	}
	
	else if (USART1->SR & USART_SR_TXE){

		USART1->DR = sendingData;
		

		iter++;

	}
}

void USART2_IRQHandler (void){
	
	if(USART2->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART2->DR;
		usart2Rx_Callback();
	}
	
	else if (USART2->SR & USART_SR_TXE){

		USART2->DR = sendingData;
		

		iter++;

	}
}

void USART6_IRQHandler (void){
	
	if(USART6->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART6->DR;
		usart6Rx_Callback();
	}
	
	else if (USART6->SR & USART_SR_TXE){

		USART6->DR = sendingData;
		

		iter++;

	}
}

__attribute__((weak)) void usart1Rx_Callback(void){
	  

	__NOP();
}

__attribute__((weak)) void usart2Rx_Callback(void){
	  

	__NOP();
}

__attribute__((weak)) void usart6Rx_Callback(void){
	  

	__NOP();
}
