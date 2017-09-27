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

/* macros *********************************************************************/
#define TIME_CONTROL_CLOCK_TIMER				TIM6
#define TIME_CONTROL_CLOCK_TIMER_CLK			RCC_APB1Periph_TIM6

#define	TIME_CONTROL_CLOCK_TIMER_FREQUENCY		(u32)(90000000) // 
#define	TIME_CONTROL_CLOCK_FREQUENCY			(u32)(100000)
#define TIME_CONTROL_CLOCK_PRESCALE				(u16)(1)
#define TIME_CONTROL_CLOCK_PERIOD				(u16)(TIME_CONTROL_CLOCK_TIMER_FREQUENCY / \
												(TIME_CONTROL_CLOCK_PRESCALE * TIME_CONTROL_CLOCK_FREQUENCY))	

/* variables ***********************************************/
extern struct rt_timer timer_10ms;
extern struct rt_timer timer_100ms;
												
/* funcitons ******************************************************************/
void time_control_clock_init(void);
u64 get_time_stamp(void);
void time_delay_10us(u32 delay_time);
void time_delay_ms(u32 delay_time);

void delay_systick(unsigned char my_ticks);// 90 ticks is 1us
void delay_us(u32 nus);
void delay_1us(void);
void delay_ms(u16 nms);

void soft_timeout_10ms(void * parameter);
void soft_timeout_100ms(void * parameter);
void soft_timeout_1000ms(void * parameter);

u64 get_time_diff1(void);
u64 get_time_diff2(void);
u64 get_time_diff_trapping(void);



#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
