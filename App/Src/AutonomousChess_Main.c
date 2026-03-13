/**
 * @file AutonomousChess_Main.c
 * @brief Autonomous chessboard firmware entry point.
 *
 * This application controls an STM32F411-based XY gantry that moves chess
 * pieces using two stepper axes, endstops, and a servo-actuated electromagnet.
 * Motion commands are received over USART2 using a lightweight protocol.
 */

#include <stdint.h>
#include <stm32f4xx.h>

#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
#include "SysTickDriver.h"
#include "PwmDriver.h"

#define SERVO_UP 		0
#define SERVO_DOWN 		1
#define SERVO_STEP_COUNT			40
#define STEPS_PER_SQUARE 		500
#define NORMAL_CAPTURE		0
#define REVERSE_CAPTURE	1

GPIO_Handler_t handlerOnBoardLed 			={0};
GPIO_Handler_t handlerSignalM1				={0};
GPIO_Handler_t handlerSignalM2				={0};
GPIO_Handler_t handlerDirectionM1				={0};
GPIO_Handler_t handlerDirectionM2				={0};
GPIO_Handler_t handlerEnableM1				={0};
GPIO_Handler_t handlerEnableM2				={0};
GPIO_Handler_t handlerServoSignal			={0};
GPIO_Handler_t handlerEndStopX				={0};
GPIO_Handler_t handlerEndStopY				={0};
GPIO_Handler_t handlerPinTX					={0};
GPIO_Handler_t handlerPinRX					={0};

BasicTimer_Handler_t handlerTimerBlinky 	={0};

PWM_Handler_t handlerPwmM1					={0};
PWM_Handler_t handlerPwmM2					={0};
PWM_Handler_t handlerPwmServo				={0};

USART_Handler_t handlerUSART2				={0};

EXTI_Config_t handlerEXTIEndStopX			={0};
EXTI_Config_t handlerEXTIEndStopY			={0};

char uartRxByte 			=0;
char uartTxBuffer[100] 			={0};
char moveMessage[10]			={0};

uint16_t motorStepCounter	=0;
uint16_t servoStepCounter		=0;
int16_t xySteps[2] 			={0};
uint8_t upperCaptureXCount		=0;
uint8_t lowerCaptureXCount			=0;

uint8_t endStopXFlag			=1;
uint8_t endStopYFlag			=1;
uint8_t gameStarted			=0;
uint8_t diagonalMoveDetected				=0;
uint8_t knightMoveDetected				=0;
uint8_t castlingDetected 				=0;

void InitHardware (void);
void Home(void);
void BasicMove (void);
void SetServoPosition(uint8_t servoPosition);
void MoveX (int16_t stepCount);
void MoveY (int16_t stepCount);
void MoveDiagonal(int16_t xStepCount,int16_t yStepCount);
void MoveKnight(int16_t xStepCount,int16_t yStepCount);
void HandleCastling(int16_t kingXStepCount,char *message);
void HandleCapture (char *message, uint8_t captureType);
uint16_t StepsFromBoardCoordinate(char boardCoordinate);
void CalculateSteps (char *moveString,uint8_t moveStage);
void ReceiveInstruction(void);

int main(void) {

	InitHardware();

	
	SetServoPosition(SERVO_DOWN);

	
	while (1) {

		
		if(!gameStarted){

			
			BasicMove();
			
			GPIO_WritePin(&handlerEnableM1, 0);
			GPIO_WritePin(&handlerEnableM2, 0);

			
			while(uartRxByte != ' '){
				__NOP();
			}

			gameStarted = 1;

			Home();
		}

		
		ReceiveInstruction();

		
		if (moveMessage[4]=='#' || moveMessage[5]=='#' || moveMessage[6]=='#'){
			
			gameStarted = 0;

			
			continue;
		}

		
		
		HandleCapture(moveMessage, NORMAL_CAPTURE);

		

		
		CalculateSteps(moveMessage, 1);

		
		BasicMove();

		

			
			MoveX(xySteps[0]);

			
			MoveY(xySteps[1]);

		
		SetServoPosition(SERVO_UP);

		
		
		CalculateSteps(moveMessage, 2);

		
		HandleCastling(xySteps[0], moveMessage);

		
		
		if(!(castlingDetected)){

			
			MoveKnight(xySteps[0], xySteps[1]);

			
			
			if(!(knightMoveDetected)){
				MoveDiagonal(xySteps[0], xySteps[1]);

				
				
				if(!(diagonalMoveDetected)){
					
					MoveX(xySteps[0]);

					
					MoveY(xySteps[1]);
				}
			}
		}

		
		SetServoPosition(SERVO_DOWN);

		
		Home();

		

		HandleCapture(moveMessage, REVERSE_CAPTURE);

		sprintf(uartTxBuffer,"\nMove completed\n");
		writeMsg(&handlerUSART2, uartTxBuffer);

		writeChar(&handlerUSART2, '*');

	}
}

void InitHardware (void){
	
	handlerOnBoardLed.pGPIOx 								= GPIOB;
	handlerOnBoardLed.GPIO_PinConfig.GPIO_PinNumber			= PIN_10;
	handlerOnBoardLed.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerOnBoardLed.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerOnBoardLed.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerOnBoardLed.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;

	handlerTimerBlinky.ptrTIMx								=TIM2;
	handlerTimerBlinky.TIMx_Config.TIMx_mode				=BTIMER_MODE_UP;
	handlerTimerBlinky.TIMx_Config.TIMx_speed				=BTIMER_SPEED_1ms;
	handlerTimerBlinky.TIMx_Config.TIMx_period				=250;
	handlerTimerBlinky.TIMx_Config.TIMx_interruptEnable		=BTIMER_INTERRUPT_ENABLE;

	GPIO_Config(&handlerOnBoardLed);
	BasicTimer_Config(&handlerTimerBlinky);

	
	handlerSignalM1.pGPIOx 								= GPIOB;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinNumber		= PIN_4;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_ALTFN;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerSignalM1.GPIO_PinConfig.GPIO_PinAltFunMode	= AF2;

	handlerSignalM2.pGPIOx 								= GPIOA;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinNumber		= PIN_7;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_ALTFN;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerSignalM2.GPIO_PinConfig.GPIO_PinAltFunMode	= AF2;

	GPIO_Config(&handlerSignalM1);
	GPIO_Config(&handlerSignalM2);

	
	GPIO_WritePin(&handlerSignalM1, 1);
	GPIO_WritePin(&handlerSignalM2, 1);

	
	handlerPwmM1.ptrTIMx				=TIM3;
	handlerPwmM1.config.channel 		=PWM_CHANNEL_1;
	handlerPwmM1.config.polarity 		=PWM_POLARITY_ACTIVE_LOW;
	handlerPwmM1.config.prescaler 		=PWM_PRESCALER_100us;
	handlerPwmM1.config.period 		=10; 
	handlerPwmM1.config.pulseWidth 		=5; 
	handlerPwmM1.config.interruption	=PWM_PERIOD_INTERRUPT_ENABLE;

	handlerPwmM2.ptrTIMx				=TIM3;
	handlerPwmM2.config.channel 		=PWM_CHANNEL_2;
	handlerPwmM2.config.polarity 		=PWM_POLARITY_ACTIVE_LOW;
	handlerPwmM2.config.prescaler 		=PWM_PRESCALER_100us;
	handlerPwmM2.config.period 		=10; 
	handlerPwmM2.config.pulseWidth 		=5; 
	handlerPwmM2.config.interruption	=PWM_PERIOD_INTERRUPT_ENABLE;

	
	pwm_Config(&handlerPwmM1);
	pwm_Config(&handlerPwmM2);

	
	
	startPwmSignal(&handlerPwmM1);

	
	disableOutput(&handlerPwmM1);
	disableOutput(&handlerPwmM2);

	
	handlerDirectionM1.pGPIOx 								= GPIOB;
	handlerDirectionM1.GPIO_PinConfig.GPIO_PinNumber		= PIN_5;
	handlerDirectionM1.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerDirectionM1.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerDirectionM1.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerDirectionM1.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;

	handlerDirectionM2.pGPIOx 								= GPIOA;
	handlerDirectionM2.GPIO_PinConfig.GPIO_PinNumber		= PIN_12;
	handlerDirectionM2.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerDirectionM2.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerDirectionM2.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerDirectionM2.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;

	GPIO_Config(&handlerDirectionM1);
	GPIO_Config(&handlerDirectionM2);

	
	handlerEnableM1.pGPIOx 								= GPIOC;
	handlerEnableM1.GPIO_PinConfig.GPIO_PinNumber		= PIN_4;
	handlerEnableM1.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerEnableM1.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerEnableM1.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerEnableM1.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;

	handlerEnableM2.pGPIOx 								= GPIOA;
	handlerEnableM2.GPIO_PinConfig.GPIO_PinNumber		= PIN_8;
	handlerEnableM2.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerEnableM2.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerEnableM2.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerEnableM2.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;

	GPIO_Config(&handlerEnableM1);
	GPIO_Config(&handlerEnableM2);

	
	GPIO_WritePin(&handlerEnableM1, 0); 
	GPIO_WritePin(&handlerEnableM2, 0);

	
	handlerServoSignal.pGPIOx 								= GPIOB;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinNumber			= PIN_9;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_ALTFN;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerServoSignal.GPIO_PinConfig.GPIO_PinAltFunMode		= AF2;

	GPIO_Config(&handlerServoSignal);

	
	handlerPwmServo.ptrTIMx					=TIM4;
	handlerPwmServo.config.channel 			=PWM_CHANNEL_4;
	handlerPwmServo.config.polarity 		=PWM_POLARITY_ACTIVE_LOW;
	handlerPwmServo.config.prescaler 		=PWM_PRESCALER_100us;
	handlerPwmServo.config.period 			=200; 
	handlerPwmServo.config.pulseWidth 		=15;
	handlerPwmServo.config.interruption		=PWM_PERIOD_INTERRUPT_ENABLE;

	
	pwm_Config(&handlerPwmServo);

	
	startPwmSignal(&handlerPwmServo);

	
	disableOutput(&handlerPwmServo);

	
	
	handlerPinTX.pGPIOx 								= GPIOA;
	handlerPinTX.GPIO_PinConfig.GPIO_PinNumber 			= PIN_2;
	handlerPinTX.GPIO_PinConfig.GPIO_PinMode 			= GPIO_MODE_ALTFN;
	handlerPinTX.GPIO_PinConfig.GPIO_PinAltFunMode 		= AF7;

	handlerPinRX.pGPIOx 								= GPIOA;
	handlerPinRX.GPIO_PinConfig.GPIO_PinNumber 			= PIN_3;
	handlerPinRX.GPIO_PinConfig.GPIO_PinMode 			= GPIO_MODE_ALTFN;
	handlerPinRX.GPIO_PinConfig.GPIO_PinAltFunMode 		= AF7;

	
	GPIO_Config(&handlerPinTX);
	GPIO_Config(&handlerPinRX);

	
	handlerUSART2.ptrUSARTx 						= USART2;
	handlerUSART2.USART_Config.USART_baudrate 		= USART_BAUDRATE_115200;
	handlerUSART2.USART_Config.USART_datasize 		= USART_DATASIZE_8BIT;
	handlerUSART2.USART_Config.USART_mode			= USART_MODE_RXTX;
	handlerUSART2.USART_Config.USART_parity 		= USART_PARITY_NONE;
	handlerUSART2.USART_Config.USART_stopbits 		= USART_STOPBIT_1;
	handlerUSART2.USART_Config.USART_enableIntRX 	= USART_RX_INTERRUP_ENABLE;
	handlerUSART2.USART_Config.USART_enableIntTX 	= USART_TX_INTERRUP_DISABLE;

	USART_Config(&handlerUSART2);

	

	
	handlerEndStopX.pGPIOx								= GPIOA;
	handlerEndStopX.GPIO_PinConfig.GPIO_PinNumber		= PIN_1;
	handlerEndStopX.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_IN;
	handlerEndStopX.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;

	handlerEndStopY.pGPIOx								= GPIOA;
	handlerEndStopY.GPIO_PinConfig.GPIO_PinNumber		= PIN_4;
	handlerEndStopY.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_IN;
	handlerEndStopY.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;

	
	handlerEXTIEndStopX.pGPIOHandler	=&handlerEndStopX;
	handlerEXTIEndStopX.edgeType		=EXTERNAL_INTERRUPT_RISING_EDGE;

	handlerEXTIEndStopY.pGPIOHandler	=&handlerEndStopY;
	handlerEXTIEndStopY.edgeType		=EXTERNAL_INTERRUPT_RISING_EDGE;

	
	ExtInt_Config(&handlerEXTIEndStopX);
	ExtInt_Config(&handlerEXTIEndStopY);

	
	config_SysTick_ms(HSI_CLOCK_CONFIGURED);
}

void Home(void){

	
	disableOutput(&handlerPwmM1);
	disableOutput(&handlerPwmM2);

	
	GPIO_WritePin(&handlerEnableM1, 1);
	GPIO_WritePin(&handlerEnableM2, 1);

	

	endStopXFlag =0;

	
	GPIO_WritePin(&handlerDirectionM1, 1); 
	GPIO_WritePin(&handlerDirectionM2, 1); 

	enableOutput(&handlerPwmM1);
	enableOutput(&handlerPwmM2);

	
	while(!endStopXFlag){
		__NOP();
	}

	

	endStopYFlag =0;

	
	GPIO_WritePin(&handlerDirectionM1, 1); 
	GPIO_WritePin(&handlerDirectionM2, 0); 

	
	while(!endStopYFlag){
		__NOP();
	}

	
	disableOutput(&handlerPwmM1);
	disableOutput(&handlerPwmM2);

	
	GPIO_WritePin(&handlerEnableM1, 0);
	GPIO_WritePin(&handlerEnableM2, 0);

}

void BasicMove (void) {

	

	
	GPIO_WritePin(&handlerEnableM1, 1);
	GPIO_WritePin(&handlerEnableM2, 1);

	
	GPIO_WritePin(&handlerDirectionM1, 0); 
	GPIO_WritePin(&handlerDirectionM2, 1); 

	
	enableOutput(&handlerPwmM1);
	enableOutput(&handlerPwmM2);

	
	motorStepCounter =0;

	

	while(!(motorStepCounter > 385)){
		__NOP();
	}

	

	
	disableOutput(&handlerPwmM2);

	
	GPIO_WritePin(&handlerDirectionM1, 0); 

	
	

	
	motorStepCounter =0;

	while(!(motorStepCounter > STEPS_PER_SQUARE)){
		__NOP();
	}

	
	disableOutput(&handlerPwmM1);
	disableOutput(&handlerPwmM2);

}

void MoveX (int16_t stepCount){

	if(stepCount >= 0){

		
		
		GPIO_WritePin(&handlerDirectionM1, 0); 
		GPIO_WritePin(&handlerDirectionM2, 0); 

		
		enableOutput(&handlerPwmM1);
		enableOutput(&handlerPwmM2);

		
		motorStepCounter =0;

		
		
		while(!(motorStepCounter >= stepCount)){
			__NOP();
		}

		
		disableOutput(&handlerPwmM1);
		disableOutput(&handlerPwmM2);

	}
	else{

		
		
		stepCount = -stepCount;

		
		GPIO_WritePin(&handlerDirectionM1, 1); 
		GPIO_WritePin(&handlerDirectionM2, 1); 

		
		enableOutput(&handlerPwmM1);
		enableOutput(&handlerPwmM2);

		
		motorStepCounter =0;

		
		
		while(!(motorStepCounter >= stepCount)){
			__NOP();
		}

		
		disableOutput(&handlerPwmM1);
		disableOutput(&handlerPwmM2);

	}
}

void MoveY(int16_t stepCount){

	if(stepCount >= 0){

		
		
		GPIO_WritePin(&handlerDirectionM1, 0); 
		GPIO_WritePin(&handlerDirectionM2, 1); 

		
		enableOutput(&handlerPwmM1);
		enableOutput(&handlerPwmM2);

		
		motorStepCounter =0;

		
		
		while(!(motorStepCounter >= stepCount)){
			__NOP();
		}

		
		disableOutput(&handlerPwmM1);
		disableOutput(&handlerPwmM2);

	}
	else{

		
		
		stepCount = -stepCount;

		
		GPIO_WritePin(&handlerDirectionM1, 1); 
		GPIO_WritePin(&handlerDirectionM2, 0); 

		
		enableOutput(&handlerPwmM1);
		enableOutput(&handlerPwmM2);

		
		motorStepCounter =0;

		
		
		while(!(motorStepCounter >= stepCount)){
			__NOP();
		}

		
		disableOutput(&handlerPwmM1);
		disableOutput(&handlerPwmM2);

	}
}

void MoveDiagonal(int16_t xStepCount,int16_t yStepCount){

	

	uint16_t diagonalStepSpan =0;

	if(xStepCount/yStepCount == 1 || xStepCount/yStepCount == -1 ){

		if(xStepCount >0 && yStepCount >0){

			
			
			GPIO_WritePin(&handlerDirectionM1, 0); 

			
			
			
			enableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);

			
			motorStepCounter =0;

			
			
			diagonalStepSpan = (2*xStepCount) + STEPS_PER_SQUARE;
			while(!(motorStepCounter >= diagonalStepSpan )){
				__NOP();
			}

			
			disableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);
		}

		else if(xStepCount <0 && yStepCount >0){

			
			
			GPIO_WritePin(&handlerDirectionM2, 1); 

			
			
			
			disableOutput(&handlerPwmM1);
			enableOutput(&handlerPwmM2);

			
			motorStepCounter =0;

			
			
			diagonalStepSpan = (2*yStepCount);
			while(!(motorStepCounter >= diagonalStepSpan)){
				__NOP();
			}

			
			disableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);
		}

		else if(xStepCount >0 && yStepCount <0){

			
			
			GPIO_WritePin(&handlerDirectionM2, 0); 

			
			
			
			disableOutput(&handlerPwmM1);
			enableOutput(&handlerPwmM2);

			
			motorStepCounter =0;

			
			
			diagonalStepSpan = (2*xStepCount);
			while(!(motorStepCounter >= diagonalStepSpan)){
				__NOP();
			}

			
			disableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);
		}

		else if(xStepCount <0 && yStepCount <0){

			
			
			GPIO_WritePin(&handlerDirectionM1, 1); 

			
			
			
			enableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);

			
			motorStepCounter =0;

			
			
			diagonalStepSpan = -(2*xStepCount) + STEPS_PER_SQUARE;
			while(!(motorStepCounter >= diagonalStepSpan)){
				__NOP();
			}

			
			disableOutput(&handlerPwmM1);
			disableOutput(&handlerPwmM2);
		}

		diagonalMoveDetected =1;
	}
	else{
		diagonalMoveDetected =0;
	}

}

void MoveKnight(int16_t xStepCount,int16_t yStepCount){
	

	
	if(xStepCount/yStepCount == 2 || xStepCount/yStepCount == -2 ){
		

		
		
		MoveY(yStepCount/2);

		
		MoveX(xStepCount);

		
		MoveY(yStepCount/2);

		
		knightMoveDetected =1;

	}
	else if(yStepCount/xStepCount == 2 || yStepCount/xStepCount == -2 ){
		

		
		
		MoveX(xStepCount/2);

		
		MoveY(yStepCount);

		
		MoveX(xStepCount/2);

		
		knightMoveDetected =1;

	}
	else{
		knightMoveDetected =0;
	}
}

void HandleCastling(int16_t kingXStepCount,char *message){

	

	if(message[5] != '-' && message[5] != '#'){
		

		if (message[4] == '&'){
			

			

			
			
			MoveX(kingXStepCount);

			
			SetServoPosition(SERVO_DOWN);

			
			
			MoveX(STEPS_PER_SQUARE);

			
			SetServoPosition(SERVO_UP);

			
			MoveY(-STEPS_PER_SQUARE/2);

			
			MoveX(-2*STEPS_PER_SQUARE);

			
			MoveY(STEPS_PER_SQUARE/2);

			
			castlingDetected =1;

		}
		else if(message[4] == '@'){
			

			

			
			
			MoveX(kingXStepCount);

			
			SetServoPosition(SERVO_DOWN);

			
			
			MoveX(-2*STEPS_PER_SQUARE);

			
			SetServoPosition(SERVO_UP);

			
			MoveY(-STEPS_PER_SQUARE/2);

			
			MoveX(3*STEPS_PER_SQUARE);

			
			MoveY(STEPS_PER_SQUARE/2);

			
			castlingDetected =1;

		}
		else{
			castlingDetected =0;
		}
	}
	else if (message[5] == '-'){
		

		if (message[4] == '&'){
			

			
			
			MoveY(STEPS_PER_SQUARE/2);

			
			
			MoveX(kingXStepCount);

			
			MoveY(-STEPS_PER_SQUARE/2);

			
			SetServoPosition(SERVO_DOWN);

			
			
			MoveX(STEPS_PER_SQUARE);

			
			SetServoPosition(SERVO_UP);

			
			MoveX(2*STEPS_PER_SQUARE);

			
			castlingDetected =1;

		}
		else if(message[4] == '@'){
			

			
			
			MoveY(STEPS_PER_SQUARE/2);

			
			
			MoveX(kingXStepCount);

			
			MoveY(-STEPS_PER_SQUARE/2);

			
			SetServoPosition(SERVO_DOWN);

			
			
			MoveX(-STEPS_PER_SQUARE);

			
			SetServoPosition(SERVO_UP);

			
			MoveX(-3*STEPS_PER_SQUARE);

			
			castlingDetected =1;

		}
		else{
			castlingDetected =0;
		}
	}
}

void HandleCapture (char *message, uint8_t captureType){

	int16_t xTravelSteps 	=0;
	int16_t yTravelSteps 	=0;
	int16_t yParkingOffset	=0;
	int16_t xParkingOffset	=0;

	if(message[4] == 'x'){
		

		if(lowerCaptureXCount < 0){
			lowerCaptureXCount =0;

		}

		if (upperCaptureXCount<0){
			upperCaptureXCount =0;
		}

		switch(captureType){

		case NORMAL_CAPTURE: {

			if(message[5] != '-' && message[5] != '#'){
				

				

				
				BasicMove();

				
				xTravelSteps = StepsFromBoardCoordinate(message[2]);

				
				yTravelSteps = StepsFromBoardCoordinate(message[3]);

				
				MoveX(xTravelSteps);

				
				MoveY(yTravelSteps);

				
				SetServoPosition(SERVO_UP);

				
				MoveX(STEPS_PER_SQUARE/2);

				
				
				if(yTravelSteps >= 4*STEPS_PER_SQUARE){
					

					
					
					yParkingOffset = 8*STEPS_PER_SQUARE - yTravelSteps - STEPS_PER_SQUARE/2;

					
					MoveY(yParkingOffset);

					
					xParkingOffset = ((upperCaptureXCount-1)*STEPS_PER_SQUARE/2) - xTravelSteps;

					
					MoveX(xParkingOffset);

					
					MoveY(STEPS_PER_SQUARE/2);

					
					SetServoPosition(SERVO_DOWN);

					
					upperCaptureXCount++;

					
					Home();

				}
				else{
					

					
					
					yParkingOffset = -STEPS_PER_SQUARE -yTravelSteps + STEPS_PER_SQUARE/2;

					
					MoveY(yParkingOffset);

					
					xParkingOffset = ((lowerCaptureXCount-1)*STEPS_PER_SQUARE/2) - xTravelSteps;

					
					MoveX(xParkingOffset);

					
					MoveY(-STEPS_PER_SQUARE/2);

					
					SetServoPosition(SERVO_DOWN);

					
					lowerCaptureXCount++;

					
					Home();

				}
				}

			break;
			}
		case REVERSE_CAPTURE:{

			if(message[5] == '-'){

				uint16_t targetSquareX =0;
				uint16_t targetSquareY =0;

				

				

				
				
				

				
				
				targetSquareX = StepsFromBoardCoordinate(message[0]);
				targetSquareY = StepsFromBoardCoordinate(message[1]);

				
				BasicMove();

				if (targetSquareY >= 4*STEPS_PER_SQUARE){
					

					
					yParkingOffset = 8*STEPS_PER_SQUARE;

					
					MoveY(yParkingOffset);

					
					xParkingOffset = (upperCaptureXCount -1)*STEPS_PER_SQUARE/2;

					
					MoveX(xParkingOffset);

					
					SetServoPosition(SERVO_UP);

					
					MoveY(-STEPS_PER_SQUARE/2);

					
					

					xTravelSteps = targetSquareX - xParkingOffset + (STEPS_PER_SQUARE/2);

					
					MoveX(xTravelSteps);

					
					yTravelSteps = targetSquareY - yParkingOffset + STEPS_PER_SQUARE/2;

					
					MoveY(yTravelSteps);

					
					MoveX(-STEPS_PER_SQUARE/2);

					
					SetServoPosition(SERVO_DOWN);

					
					upperCaptureXCount-- ;

					
					Home();

				}

				else{
					

					
					yParkingOffset = -STEPS_PER_SQUARE;

					
					MoveY(yParkingOffset);

					
					xParkingOffset = (lowerCaptureXCount -1)*STEPS_PER_SQUARE/2;

					
					MoveX(xParkingOffset);

					
					SetServoPosition(SERVO_UP);

					
					MoveY(STEPS_PER_SQUARE/2);

					
					

					xTravelSteps = targetSquareX - xParkingOffset + (STEPS_PER_SQUARE/2);

					
					MoveX(xTravelSteps);

					
					yTravelSteps = 390 + targetSquareY - (STEPS_PER_SQUARE/2);

					
					MoveY(yTravelSteps);

					
					MoveX(-STEPS_PER_SQUARE/2);

					
					SetServoPosition(SERVO_DOWN);

					
					lowerCaptureXCount--;

					
					Home();

				}

			}

			break;
		}
		default:{
			__NOP();
			break;
		}

		}

	}

}

uint16_t StepsFromBoardCoordinate (char boardCoordinate){
	

	uint16_t steps=0;

	switch(boardCoordinate){

	
	case 'a':{
		steps = 0;
		break;
	}
	case 'b':{
		steps = STEPS_PER_SQUARE;
		break;
	}
	case 'c':{
		steps = 2*STEPS_PER_SQUARE;
		break;
	}
	case 'd':{
		steps = 3*STEPS_PER_SQUARE;
		break;
	}
	case 'e':{
		steps = 4*STEPS_PER_SQUARE;
		break;
	}
	case 'f':{
		steps = 5*STEPS_PER_SQUARE;
		break;
	}
	case 'g':{
		steps = 6*STEPS_PER_SQUARE;
		break;
	}
	case 'h':{
		steps = 7*STEPS_PER_SQUARE;
		break;
	}

	
	case '1':{
		steps = 0;
		break;
	}
	case '2':{
		steps = STEPS_PER_SQUARE;
		break;
	}
	case '3':{
		steps = 2*STEPS_PER_SQUARE;
		break;
	}
	case '4':{
		steps = 3*STEPS_PER_SQUARE;
		break;
	}
	case '5':{
		steps = 4*STEPS_PER_SQUARE;
		break;
	}
	case '6':{
		steps = 5*STEPS_PER_SQUARE;
		break;
	}
	case '7':{
		steps = 6*STEPS_PER_SQUARE;
		break;
	}
	case '8':{
		steps = 7*STEPS_PER_SQUARE;
		break;
	}
	default:{
		steps =0;

		sprintf(uartTxBuffer,"An invalid board coordinate was received");
		writeMsg(&handlerUSART2, uartTxBuffer);

		sprintf(uartTxBuffer,"\nPlease try again");
		writeMsg(&handlerUSART2, uartTxBuffer);
		break;
	}
	}
	return steps;
}

void CalculateSteps (char *moveString,uint8_t moveStage){
	

	

	switch(moveStage){

	case 1:{
		
		
		xySteps[0] = StepsFromBoardCoordinate(moveString[0]);

		
		xySteps[1] = StepsFromBoardCoordinate(moveString[1]);

		break;
	}
	case 2:{
		
		
		xySteps[0] =  StepsFromBoardCoordinate(moveString[2]) - StepsFromBoardCoordinate(moveString[0]);

		
		xySteps[1] = StepsFromBoardCoordinate(moveString[3]) - StepsFromBoardCoordinate(moveString[1]);

		break;
	}
	default:{
		xySteps[0] =0;
		xySteps[1]=0;
		break;
	}
	}

}

void SetServoPosition(uint8_t servoPosition){

	switch(servoPosition){

	case SERVO_UP:{
		
		updatePulseWidth(&handlerPwmServo, 16);

		
		enableOutput(&handlerPwmServo);

		
		servoStepCounter =0;

		
		
		while(!(servoStepCounter > SERVO_STEP_COUNT )){
			__NOP();
		}

		
		disableOutput(&handlerPwmServo);

		break;
	}

	case SERVO_DOWN:{
		
		updatePulseWidth(&handlerPwmServo, 14);

		
		enableOutput(&handlerPwmServo);

		
		servoStepCounter =0;

		
		
		while(!(servoStepCounter > SERVO_STEP_COUNT )){
			__NOP();
		}

		
		disableOutput(&handlerPwmServo);

		break;
	}

	default:{
		__NOP();
	}
	}

}

void ReceiveInstruction(void){

	uint8_t counter =0;
	uartRxByte = '\0';

	while(uartRxByte != '$'){

		if(uartRxByte != '\0' && uartRxByte != '$'){
			moveMessage[counter] =uartRxByte;
			counter++;
			uartRxByte ='\0';

		}
	}

	moveMessage[counter] = '\0';
	sprintf(uartTxBuffer, "\nReceived move: ");
	writeMsg(&handlerUSART2, uartTxBuffer);
	writeMsg(&handlerUSART2, moveMessage);
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerOnBoardLed);

}

void BasicTimer3_Callback(void){
	motorStepCounter++;

}

void BasicTimer4_Callback(void){
	servoStepCounter++;

}

void usart2Rx_Callback(void){
	
	uartRxByte =getRxData();
}

void callback_extInt1(void){
	endStopXFlag =1;
}

void callback_extInt4(void){
	endStopYFlag =1;
}
