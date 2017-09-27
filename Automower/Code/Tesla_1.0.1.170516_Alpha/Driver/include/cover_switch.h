/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: cover_switch.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: cover switch 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _COVER_SWITCH_H_
#define _COVER_SWITCH_H_


/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define COVER_SWITCH_PORT										GPIOD
#define	COVER_SWITCH_PIN										GPIO_Pin_6
#define	COVER_SWITCH_CLK										RCC_AHB1Periph_GPIOD

/* funcitons ******************************************************************/
void cover_switch_init(void);
void reset_cover_switch(void);
void get_cover_switch_info(T_bool *cover_switch);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
