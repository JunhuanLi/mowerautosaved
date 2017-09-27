
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			delay.c
  Author:				Raymond
  Date:				2017.5.25
  Version:        
  Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                  // �ơ�˳�򡢶����������ȹ�ϵ
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
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
   ��ȡϵͳ�δ�ʱ�ӼĴ�������ʱ1usϵͳʱ�䡣
input: 
   ms ��ֵ>=1

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
 	��ȡϵͳ�δ�ʱ�ӼĴ�������ʱ1msϵͳʱ�䡣
 input: 
 	ms ��ֵ>=1 С���߳�����ʱ��tick
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
 	�ⲿ����25Mhz,��Ƶ��180Mhzʱ����ʱ1us
 input: 
 	ms ��ֵ>=1
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
 	�ⲿ����25Mhz,��Ƶ��180Mhzʱ����ʱ1ms
 input: 
 	ms ��ֵ>=1
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

