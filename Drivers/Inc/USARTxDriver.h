#include <stdio.h>

#ifndef USARTXDRIVER_H_
#define USARTXDRIVER_H_

#define USART_MODE_TX		0
#define USART_MODE_RX		1
#define USART_MODE_RXTX		2
#define USART_MODE_DISABLE	3

#define USART_BAUDRATE_9600		0
#define USART_BAUDRATE_19200	1
#define USART_BAUDRATE_115200	2

#define USART_DATASIZE_8BIT		0
#define USART_DATASIZE_9BIT		1

#define USART_PARITY_NONE	0
#define USART_PARITY_ODD	1
#define USART_PARITY_EVEN	2

#define USART_STOPBIT_1		0
#define USART_STOPBIT_0_5	1
#define USART_STOPBIT_2		2
#define USART_STOPBIT_1_5	3

#define USART_RX_INTERRUP_DISABLE	0
#define USART_TX_INTERRUP_DISABLE	1
#define USART_RX_INTERRUP_ENABLE	2
#define USART_TX_INTERRUP_ENABLE	3

typedef struct
{
	uint8_t USART_mode;
	uint8_t USART_baudrate;
	uint8_t USART_datasize;
	uint8_t USART_parity;
	uint8_t USART_stopbits;
	uint8_t USART_enableIntRX;
	uint8_t USART_enableIntTX;
}USART_Config_t;

typedef struct
{
	USART_TypeDef	*ptrUSARTx;
	USART_Config_t	USART_Config;
	uint8_t			receptionBuffer[64];
	uint8_t			dataInputSize;
	uint8_t			transmisionBuffer[64];
	uint8_t			dataOutputSize;
}USART_Handler_t;

void USART_Config(USART_Handler_t *ptrUsartHandler);
char writeChar(USART_Handler_t *ptrUsartHandler, char dataToSend );
void writeMsg(USART_Handler_t *ptrUsartHandler, char *MsgToSend);

uint8_t getRxData(void);

void usart1Rx_Callback(void);
void usart2Rx_Callback(void);
void usart6Rx_Callback(void);

#endif
