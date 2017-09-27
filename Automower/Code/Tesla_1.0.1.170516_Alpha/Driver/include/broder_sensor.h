/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	broder_sensor.h
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/13
Description		:	broder_sensor header 
*******************************************************************************/

#ifndef _BRODER_SENSOR_H_
#define _BRODER_SENSOR_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"


/* macros *********************************************************************/
#define BRODER_SELECT_LEFT_PORT		    GPIOC
#define BRODER_SELECT_LEFT_PIN		    GPIO_Pin_4
#define BRODER_SELECT_LEFT_CLK		    RCC_AHB1Periph_GPIOC

#define BRODER_SELECT_RIGHT_PORT		GPIOC
#define BRODER_SELECT_RIGHT_PIN		    GPIO_Pin_5
#define BRODER_SELECT_RIGHT_CLK		    RCC_AHB1Periph_GPIOC

#define BRODER_SELECT_ENABLE_PORT		GPIOC
#define BRODER_SELECT_ENABLE_PIN		GPIO_Pin_13
#define BRODER_SELECT_ENABLE_CLK		RCC_AHB1Periph_GPIOC

typedef struct
{
	u8 left;
	u8 right;
	u8 state;
}T_broder_sensor;

typedef struct
{
	u16 left;
	u16 right;
}T_broder_value;


/* funcitons ******************************************************************/
void broder_sensor_Init(void);
void reset_broder_sensor(void);
void get_broder_sensor_info(T_broder_sensor *broder_sensor);


#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
