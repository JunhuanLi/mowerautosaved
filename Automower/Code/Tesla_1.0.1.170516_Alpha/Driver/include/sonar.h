/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	sonar.h
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/13
Description		:	sonar 
*******************************************************************************/

#ifndef _SONAR_H_
#define _SONAR_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define LEFT_SONAR_TX_PORT		GPIOF
#define LEFT_SONAR_TX_PIN		GPIO_Pin_2
#define LEFT_SONAR_TX_CLK		RCC_AHB1Periph_GPIOF
	
#define RIGHT_SONAR_TX_PORT		GPIOF
#define RIGHT_SONAR_TX_PIN		GPIO_Pin_3
#define RIGHT_SONAR_TX_CLK  	RCC_AHB1Periph_GPIOF

#define LEFT_SONAR_RX_PORT		GPIOF
#define LEFT_SONAR_RX_PIN		GPIO_Pin_8
#define LEFT_SONAR_RX_CLK		RCC_AHB1Periph_GPIOF
#define LEFT_SONAR_RX_PINSOURCE		EXTI_PinSource8
#define LEFT_SONAR_RX_PORTSOURCE	EXTI_PortSourceGPIOF

#define RIGHT_SONAR_RX_PORT			GPIOF
#define RIGHT_SONAR_RX_PIN			GPIO_Pin_9
#define RIGHT_SONAR_RX_CLK			RCC_AHB1Periph_GPIOF
#define RIGHT_SONAR_RX_PINSOURCE	EXTI_PinSource9
#define RIGHT_SONAR_RX_PORTSOURCE	EXTI_PortSourceGPIOF


	
#define LEFT_SONAR_TX_OPEN()      GPIO_SetBits(LEFT_SONAR_TX_PORT, LEFT_SONAR_TX_PIN);
#define LEFT_SONAR_TX_CLOSE()     GPIO_ResetBits(LEFT_SONAR_TX_PORT, LEFT_SONAR_TX_PIN);

#define RIGHT_SONAR_TX_OPEN()	  GPIO_SetBits(RIGHT_SONAR_TX_PORT, RIGHT_SONAR_TX_PIN);
#define RIGHT_SONAR_TX_CLOSE() 	  GPIO_ResetBits(RIGHT_SONAR_TX_PORT, RIGHT_SONAR_TX_PIN);


#define SONAR_ERROR				32767
#define MIN_SONAR_RANGE				20
#define MAX_SONAR_RANGE				4500
#define VELOVITY_OF_SPEED			340	
/* funcitons ******************************************************************/
void sonar_init(void);
void sonar_launch(void);
void get_sonar_info(T_sonar *sonar);
void process_left_sonar(void);
void process_right_sonar(void);
#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
