
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		thread_updata.c
  Author:			Raymond
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
#include <mower_common.h>
#include <stdio.h>
#include "hardware.h"

/*在分配堆栈空间时，必须要对其*/
ALIGN(RT_ALIGN_SIZE)
char thread_updata_stack[1024];
struct rt_thread thread_updata;
void mower_updata_thread(void* parameter)
{
    rt_uint32_t event_rec;
    while (1)
    {
    /* updata once per 20ms*/
		rt_event_recv(&sys_event,SYS_EVN_SENSOR_UPDATA,RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&event_rec);
		sonar_launch();		
		update_all_sensors();
		rt_event_send(&sys_event, SYS_EVN_IMU);
        //rt_thread_delay(20);// 20ms
    }
}








