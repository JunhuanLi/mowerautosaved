/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: bumper_front.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: bumper front   
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _BUMPER_FRONT_H_
#define _BUMPER_FRONT_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"

/* macros *********************************************************************/
#define FRONT_BUMPER_LEFT_PORT										GPIOH
#define	FRONT_BUMPER_LEFT_PIN										GPIO_Pin_8
#define	FRONT_BUMPER_LEFT_CLK										RCC_AHB1Periph_GPIOH

#define FRONT_BUMPER_RIGHT_PORT										GPIOH
#define	FRONT_BUMPER_RIGHT_PIN										GPIO_Pin_13
#define	FRONT_BUMPER_RIGHT_CLK										RCC_AHB1Periph_GPIOH


/* funcitons ******************************************************************/
void front_bumper_init(void);
void reset_front_bumper(void);
void get_front_bumper_info(T_frontBumper *front_bumper);


#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
