/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: wheel_up.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: wheel up  
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _WHEEL_UP_H_
#define _WHEEL_UP_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define WHEEL_UP_LEFT_PORT										GPIOH
#define	WHEEL_UP_LEFT_PIN										GPIO_Pin_14
#define	WHEEL_UP_LEFT_CLK										RCC_AHB1Periph_GPIOH

#define	WHEEL_UP_RIGHT_PORT										GPIOH
#define	WHEEL_UP_RIGHT_PIN										GPIO_Pin_15
#define	WHEEL_UP_RIGHT_CLK										RCC_AHB1Periph_GPIOH



/* funcitons ******************************************************************/
void wheel_up_init(void);
void reset_wheel_up(void);
void get_wheel_up_info(T_wheel_up *wheel_up);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
