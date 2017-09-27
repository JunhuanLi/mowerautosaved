 /*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: time_control.c
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

/* includes *******************************************************************/
#include "time_control.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static u64 g_time_control_clock_overflow = 0;
static u64 g_time_delay = 0;

/* funcitons ******************************************************************/
static void time_decrease(void);

/*------------------------------------------------------------------------------
Function		:	time_control_clock_init
Description		: 	time_control_clock_init
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_control_clock_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(TIME_CONTROL_CLOCK_TIMER_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIME_CONTROL_CLOCK_PERIOD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIME_CONTROL_CLOCK_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseInit(TIME_CONTROL_CLOCK_TIMER, &TIM_TimeBaseStructure);
//	TIM_UpdateDisableConfig(TIME_CONTROL_CLOCK_TIMER, DISABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIME_CONTROL_CLOCK_TIMER->CNT = 0;
	
	TIM_ITConfig(TIME_CONTROL_CLOCK_TIMER, TIM_IT_Update, ENABLE);	
	TIM_ClearFlag(TIME_CONTROL_CLOCK_TIMER, TIM_FLAG_Update);
	TIM_Cmd(TIME_CONTROL_CLOCK_TIMER, ENABLE);
}

/*------------------------------------------------------------------------------
Function		:	time_delay_us
Description		: 	time delay us
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_delay_10us(u32 delay_time)
{
	g_time_delay = delay_time;
	while(g_time_delay != 0)
	{
	}
}

/*------------------------------------------------------------------------------
Function		:	time_delay_ms
Description		: 	time_delay_ms
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_delay_ms(u32 delay_time)
{
	g_time_delay = delay_time * TIME_CONTROL_CLOCK_FREQUENCY;
	while(g_time_delay != 0)
	{
	}
}

/*------------------------------------------------------------------------------
Function		:	TIM6_IRQHandler
Description		: 	TIM6_IRQHandler
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
static void time_decrease(void)
{
	if(g_time_delay != 0)
	{
		g_time_delay--;
	}
}

/*------------------------------------------------------------------------------
Function		:	TIM6_IRQHandler
Description		: 	TIM6_IRQHandler
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
u8 cnt = 0;
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus( TIME_CONTROL_CLOCK_TIMER, TIM_IT_Update) != RESET)
	{
		if(++cnt >= 50)
		{
			cnt = 0;
//			update_encoder_speed();
		}
		g_time_control_clock_overflow++;
		time_decrease();
		TIM_ClearITPendingBit(TIME_CONTROL_CLOCK_TIMER , TIM_IT_Update);
	}
}

/*------------------------------------------------------------------------------
Function		:	get_time_stamp
Description		: 	get_time_stamp
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/

u64 get_time_stamp(void)
{
//	u64 time_stamp = ((u64)(g_time_control_clock_overflow * (u64)TIME_CONTROL_CLOCK_PERIOD)\
//	+ (u64)TIM_GetCounter(TIME_CONTROL_CLOCK_TIMER));

		u64 time_stamp = ((u64)(g_time_control_clock_overflow * (u64)10)\
	+ (u64)TIM_GetCounter(TIME_CONTROL_CLOCK_TIMER) / 90);
	return time_stamp;		// us
}

/*------------------------------------------------------------------------------
Function		:	get_time_diff
Description		: 	get_time_diff
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
u64 get_time_diff(void)
{
	static u64 current_cnt = 0;
	static u64 last_cnt = 0;
	u64 cnt_diff = 0;
	current_cnt = get_time_stamp();
	
	cnt_diff = current_cnt - last_cnt;
	last_cnt = current_cnt;
	return cnt_diff;
}
/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
