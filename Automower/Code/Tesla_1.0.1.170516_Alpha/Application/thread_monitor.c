
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			thread_monitor.c
  Author:				Raymond
  Date:				2017.7.7
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
#include <rtthread.h>
//#include <string.h>
#include <stdio.h>
#include "mower_common.h"
#include "exce.h"



static void exception_handle(rt_uint32_t exe)
{
    if(!(exe & ( EVENT_EXC_LOWER_POWER|EVENT_EXC_OUTSIDE_AREA\
                |EVENT_EXC_NO_LOOP_SINGNAL|EVENT_EXC_LIFT\
                |EVENT_EXC_TRAPPED|EVENT_EXC_TOO_STEEP\
                |EVENT_EXC_BM_OVERCURRENT|EVENT_EXC_BM_OVERLOAD\
                |EVENT_EXC_WM_OVERCURRENT|EVENT_EXC_WM_OVERLOAD)))
        return;


    if(exe & EVENT_EXC_LOWER_POWER)
    {

    }
    if(exe & EVENT_EXC_OUTSIDE_AREA)
    {

    }
    if(exe & EVENT_EXC_NO_LOOP_SINGNAL)
    {

    }
    if(exe & EVENT_EXC_LIFT)
    {

    }
    if(exe & EVENT_EXC_TRAPPED)
    {

    }
    if(exe & EVENT_EXC_TOO_STEEP)
    {

    }
    if(exe & EVENT_EXC_BM_OVERCURRENT)
    {

    }
    if(exe & EVENT_EXC_BM_OVERLOAD)
    {

    }
    if(exe & EVENT_EXC_WM_OVERCURRENT)
    {

    }
    if(exe & EVENT_EXC_WM_OVERLOAD)
    {

    }




}



/* allocate space */
ALIGN(RT_ALIGN_SIZE)
char thread_monitor_stack[1024];
struct rt_thread thread_monitor;
void mower_monitor_thread(void* parameter)
{
	rt_uint32_t receive_event;
	rt_err_t err;
    
    while (1)
    {
#if 1//def CUSTOMER_SERIAL_DEBUG

        /* real time */
        if(sys_power_state) // 开机状态下
        {
            err = rt_event_recv(&exception_event,
                EVENT_EXC_LOWER_POWER|EVENT_EXC_OUTSIDE_AREA\
                |EVENT_EXC_NO_LOOP_SINGNAL|EVENT_EXC_LIFT\
                |EVENT_EXC_TRAPPED|EVENT_EXC_TOO_STEEP\
                |EVENT_EXC_BM_OVERCURRENT|EVENT_EXC_BM_OVERLOAD\
                |EVENT_EXC_WM_OVERCURRENT|EVENT_EXC_WM_OVERLOAD,
                RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                30,
                &receive_event);
            
           #if 0//def CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n...monitor:%s...",receive_event);
		   #endif
            // 故障处理    
            exception_handle(receive_event);
           
            // 切换状态           
            err = rt_event_recv(&sys_event,SYS_EVN_PATTERN_REQ,RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,0,&receive_event);
            if(receive_event == SYS_EVN_PATTERN_REQ)
            {
				receive_event = SYS_EVN_MSAK;
            }
        } 
#endif      
        //rt_kprintf("\r\n>>>monitor_thread");
		rt_thread_delay(10);


		#ifdef CUSTOMER_SERIAL_DEBUG
		//rt_kprintf("\r\n>>> p = %d...ptr=%d...",p,ptr);
		#endif
    }
}








