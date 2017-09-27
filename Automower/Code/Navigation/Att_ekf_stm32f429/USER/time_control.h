/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: time_control.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: time control  
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _TIME_CONTROL_H_
#define _TIME_CONTROL_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"
#include "sys.h"

/* macros *********************************************************************/
#define TIME_CONTROL_CLOCK_TIMER				TIM6
#define TIME_CONTROL_CLOCK_TIMER_CLK			RCC_APB1Periph_TIM6

#define	TIME_CONTROL_CLOCK_TIMER_FREQUENCY		(u32)(90000000) // 
#define	TIME_CONTROL_CLOCK_FREQUENCY			(u32)(100000)
#define TIME_CONTROL_CLOCK_PRESCALE				(u16)(1)
#define TIME_CONTROL_CLOCK_PERIOD				(u16)(TIME_CONTROL_CLOCK_TIMER_FREQUENCY / \
												(TIME_CONTROL_CLOCK_PRESCALE * TIME_CONTROL_CLOCK_FREQUENCY))	

//#define	TIME_CONTROL_CLOCK_TIMER_FREQUENCY		(u32)(90000000) // 
//#define	TIME_CONTROL_CLOCK_FREQUENCY			(u32)(1000000)
//#define TIME_CONTROL_CLOCK_PRESCALE				(u16)(1)
//#define TIME_CONTROL_CLOCK_PERIOD				(u16)(TIME_CONTROL_CLOCK_TIMER_FREQUENCY / \
//												(TIME_CONTROL_CLOCK_PRESCALE * TIME_CONTROL_CLOCK_FREQUENCY))	
/* funcitons ******************************************************************/
void time_control_clock_init(void);
u64 get_time_stamp(void);
void time_delay_10us(u32 delay_time);
void time_delay_ms(u32 delay_time);
u64 get_time_diff(void);
#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
