/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: usart_driver.h
  Author		: klaus 	
  Version		: V1.0.0	  
  Date			: 2017/05/26
  Description	: usart driver
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"


/* macros *********************************************************************/

/******************************************************************************
*******************		USART1		USART_BRODER	(5.0V) 		
*******************		USART2		USART_DEBUG				
*******************		USART3		USART_COM		(5.0v)
*******************		USART4		USART_BMS
*******************		USART8		USART_GPS	
*******************************************************************************/

/*---------------------------- USART BMS -------------------------------------*/
#define USART_BMS_BAUDRATE		    9600
#define USART_BMS				    UART4
#define USART_BMS_CLK				RCC_APB1Periph_UART4
#define USART_BMS_IRQ				UART4_IRQn


#define USART_BMS_TX_PORT		    GPIOC
#define USART_BMS_TX_PIN		    GPIO_Pin_10
#define USART_BMS_TX_CLK		    RCC_AHB1Periph_GPIOC
#define USART_BMS_TX_AF		    	GPIO_AF_UART4
#define USART_BMS_TX_SOURCE		    GPIO_PinSource10

#define USART_BMS_RX_PORT		    GPIOC
#define USART_BMS_RX_PIN		    GPIO_Pin_11
#define USART_BMS_RX_CLK		    RCC_AHB1Periph_GPIOC
#define USART_BMS_RX_AF		   		GPIO_AF_UART4
#define USART_BMS_RX_SOURCE		   	GPIO_PinSource11

#define USART_BMS_HANDLER		   	UART4_IRQHandler

/*---------------------------- USART DEBUG -----------------------------------*/
#define USART_DEBUG_BAUDRATE		115200
#define USART_DEBUG				    USART2
#define USART_DEBUG_CLK				RCC_APB1Periph_USART2
#define USART_DEBUG_IRQ				USART2_IRQn


#define USART_DEBUG_TX_PORT		    GPIOA
#define USART_DEBUG_TX_PIN		    GPIO_Pin_2
#define USART_DEBUG_TX_CLK		    RCC_AHB1Periph_GPIOA
#define USART_DEBUG_TX_AF		   	GPIO_AF_USART2
#define USART_DEBUG_TX_SOURCE		GPIO_PinSource2

#define USART_DEBUG_RX_PORT		    GPIOA
#define USART_DEBUG_RX_PIN		    GPIO_Pin_3
#define USART_DEBUG_RX_CLK		    RCC_AHB1Periph_GPIOA
#define USART_DEBUG_RX_AF		    GPIO_AF_USART2
#define USART_DEBUG_RX_SOURCE		GPIO_PinSource3

#define USART_DEBUG_HANDLER		   	USART2_IRQHandler

/*---------------------------- USART GPS -----------------------------------*/
#define USART_GPS_BAUDRATE		  115200
#define USART_GPS				    		USART3
#define USART_GPS_CLK						RCC_APB1Periph_USART3
#define USART_GPS_IRQ						USART3_IRQn


#define USART_GPS_TX_PORT		    GPIOB
#define USART_GPS_TX_PIN		    GPIO_Pin_10
#define USART_GPS_TX_CLK		    RCC_AHB1Periph_GPIOB
#define USART_GPS_TX_AF		    	GPIO_AF_USART3
#define USART_GPS_TX_SOURCE		  GPIO_PinSource10

#define USART_GPS_RX_PORT		    GPIOB
#define USART_GPS_RX_PIN		    GPIO_Pin_11
#define USART_GPS_RX_CLK		    RCC_AHB1Periph_GPIOB
#define USART_GPS_RX_AF		   		GPIO_AF_USART3
#define USART_GPS_RX_SOURCE		  GPIO_PinSource11

#define USART_GPS_EN_PORT				GPIOB
#define USART_GPS_EN_PIN				GPIO_Pin_0

#define USART_GPS_HANDLER		   	USART3_IRQHandler

/*---------------------------- USART COM -----------------------------------*/

#define USART_COM_BAUDRATE		    115200
#define USART_COM				    USART3
#define USART_COM_CLK				RCC_APB1Periph_USART3
#define USART_COM_IRQ				USART3_IRQn


#define USART_COM_TX_PORT		    GPIOB
#define USART_COM_TX_PIN		    GPIO_Pin_10
#define USART_COM_TX_CLK		    RCC_AHB1Periph_GPIOB
#define USART_COM_TX_AF		    	GPIO_AF_USART3
#define USART_COM_TX_SOURCE		    GPIO_PinSource10

#define USART_COM_RX_PORT		    GPIOB
#define USART_COM_RX_PIN		    GPIO_Pin_11
#define USART_COM_RX_CLK		    RCC_AHB1Periph_GPIOB
#define USART_COM_RX_AF		   		GPIO_AF_USART3
#define USART_COM_RX_SOURCE		  GPIO_PinSource11

//#define USART_COM_HANDLER		   	USART3_IRQHandler


/*---------------------------- USART BRODER -----------------------------------*/

#define USART_BRODER_BAUDRATE		    115200
#define USART_BRODER				    USART1
#define USART_BRODER_CLK				RCC_APB2Periph_USART1
#define USART_BRODER_IRQ				USART1_IRQn

#define USART_BRODER_TX_PORT		    GPIOA
#define USART_BRODER_TX_PIN		  	 	GPIO_Pin_9
#define USART_BRODER_TX_CLK		  		RCC_AHB1Periph_GPIOA
#define USART_BRODER_TX_AF		    	GPIO_AF_USART1
#define USART_BRODER_TX_SOURCE		    GPIO_PinSource9

#define USART_BRODER_RX_PORT		    GPIOA
#define USART_BRODER_RX_PIN		    	GPIO_Pin_10
#define USART_BRODER_RX_CLK		   		RCC_AHB1Periph_GPIOA
#define USART_BRODER_RX_AF		   		GPIO_AF_USART1
#define USART_BRODER_RX_SOURCE		   	GPIO_PinSource10

#define USART_BRODER_HANDLER		   	USART1_IRQHandler

/*---------------------------- USART EM_SENSOR -----------------------------------*/

#define USART_EM_SENSOR_BAUDRATE		  115200
#define USART_EM_SENSOR				    	 	UART5
#define USART_EM_SENSOR_CLK					 	RCC_APB1Periph_UART5
#define USART_EM_SENSOR_IRQ						UART5_IRQn

#define USART_EM_SENSOR_TX_PORT		    GPIOC
#define USART_EM_SENSOR_TX_PIN		  	GPIO_Pin_12
#define USART_EM_SENSOR_TX_CLK		  	RCC_AHB1Periph_GPIOC
#define USART_EM_SENSOR_TX_AF		    	GPIO_AF_UART5
#define USART_EM_SENSOR_TX_SOURCE		  GPIO_PinSource12

#define USART_EM_SENSOR_RX_PORT		    GPIOD
#define USART_EM_SENSOR_RX_PIN		    GPIO_Pin_2
#define USART_EM_SENSOR_RX_CLK		   	RCC_AHB1Periph_GPIOD
#define USART_EM_SENSOR_RX_AF		   		GPIO_AF_UART5
#define USART_EM_SENSOR_RX_SOURCE		  GPIO_PinSource2

#define USART_EM_SENSOR_HANDLER		   	UART5_IRQHandler

/* funcitons ******************************************************************/

/* funcitons ******************************************************************/
void usart_bms_init(void);
void usart_debug_init(void);
void usart_broder_init(void);
void usart_gps_init(void);
void usart_com_init(void);  

void usart_send_byte(USART_TypeDef * USARTx, u8 ch);
void usarts_init(void);

void uart_emsensor_init(void);

void usart_send_byte(USART_TypeDef * USARTx, u8 ch);


void USART_GPS_ENABLE(void);
void USART_GPS_DISABLE(void);

extern u8 UART_RX_BUFFER[200];
extern int32_t leftsensor_data;
extern int32_t rightsensor_data;
#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
