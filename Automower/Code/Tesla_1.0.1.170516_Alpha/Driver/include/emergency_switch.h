/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: emergency_switch.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: emergency switch
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _EMERGENCY_SWITCH_H_
#define _EMERGENCY_SWITCH_H_


/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define EMERGENCY_SWITCH_PORT										GPIOF
#define	EMERGENCY_SWITCH_PIN										GPIO_Pin_4
#define	EMERGENCY_SWITCH_CLK										RCC_AHB1Periph_GPIOF

/* funcitons ******************************************************************/
void emergency_switch_init(void);
void reset_emergency_switch(void);
void get_emergency_switch_info(T_bool *emergency_switch);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
