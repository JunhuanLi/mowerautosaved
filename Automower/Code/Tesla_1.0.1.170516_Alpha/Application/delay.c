
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			delay.c
  Author:				Raymond
  Date:				2017.5.25
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/


#include "stm32f4xx.h"
#include "core_cm4.h"
#include "rtthread.h"
#include "rtdef.h"


/******************************
description:
   读取系统滴答时钟寄存器，延时1us系统时间。
input: 
   ms 的值>=1

*******************************/
void systick_us_delay(rt_uint16_t us)
{
    rt_uint32_t cnt;
	rt_uint32_t delay;
	
    cnt = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	delay=SysTick->VAL;
	
	if(delay < cnt)
	{
		while(SysTick->VAL > (SysTick->LOAD - (cnt - delay)) || SysTick->VAL < delay)
		{
			__ASM("nop");
		}	

	}
	else
	{
		while(delay - SysTick->VAL < cnt)
		{
			__ASM("nop");
		}	
	}
}


/****************************************
 description:
 	读取系统滴答时钟寄存器，延时1ms系统时间。
 input: 
 	ms 的值>=1 小于线程运行时间tick
 ****************************************/

void systick_ms_delay(rt_uint16_t ms)
{
    rt_uint32_t cnt;
	rt_uint32_t delay;
	
    cnt = ms * (SysTick->LOAD/(1000/RT_TICK_PER_SECOND));
	delay=SysTick->VAL;

	if(delay < cnt)
	{
		while(SysTick->VAL > (SysTick->LOAD - (cnt - delay)) || SysTick->VAL < delay)
		{
			__ASM("nop");
		}
	}
	else
	{
		while(delay - SysTick->VAL < cnt)
		{
			__ASM("nop");
		}
	}
}


/****************************************
 description:
 	外部晶振25Mhz,倍频至180Mhz时，延时1us
 input: 
 	ms 的值>=1
 ****************************************/
void hw_us_delay(rt_uint16_t us)
{
	rt_uint32_t i,j;
	
	for(i=0;i<us;i++)
	{
		for(j=0;j<35;j++)
		{
			__ASM("nop");
		}
	}
}


/****************************************
 description:
 	外部晶振25Mhz,倍频至180Mhz时，延时1ms
 input: 
 	ms 的值>=1
 ****************************************/
void hw_ms_delay(rt_uint16_t ms)
{
	rt_uint32_t i,j;
	
	for(i=0;i<ms;i++)
	{
		for(j=0;j<38000;j++)
		{
			__ASM("nop");
		}
	}
}

