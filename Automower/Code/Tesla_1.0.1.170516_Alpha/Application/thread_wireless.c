/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		thread_wireless.c
  Author:			Raymond
  Date:				2017.8.8
  Version:        
  Description:    // 
                  // 
                  // 
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/


#include <rtthread.h>
#include "stm32f4xx.h"
#include "mower_common.h"

ALIGN(RT_ALIGN_SIZE)
char thread_wireless_stack[512];
struct rt_thread thread_wireless;

void mower_wireless_thread(void* parameter)
{
    rt_uint32_t recved;
    rt_err_t err;
	while(1)
	{   
        /* 接收触发事件 */
        //err = rt_event_recv(&sys_event, SYS_EVN_WIRELESS, RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

        #if 1//def CUSTOMER_SERIAL_DEBUG
		//rt_kprintf("\r\n...wireless>>>");
		#endif
		rt_thread_delay(1000); //200ms
	}
}

