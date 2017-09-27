
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		thread_updata.c
  Author:			Raymond
  Date:				2017.7.7
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
#include <rtthread.h>
#include <mower_common.h>
#include <stdio.h>
#include "hardware.h"

/*�ڷ����ջ�ռ�ʱ������Ҫ����*/
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








