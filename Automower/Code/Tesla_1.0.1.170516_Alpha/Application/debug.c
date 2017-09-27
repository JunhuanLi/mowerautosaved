/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			debug.C
  Author:				Raymond
  Date:				2017.9.21
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

#include <rtthread.h>
#include <rthw.h>
#include "stm32f4xx.h"
#include "board.h"


static rt_device_t debug_device = RT_NULL;

#ifdef RT_USING_CONSOLE
/**
 * This function will print a formatted string on system console
 *
 * @param fmt the format
 */
void rt_debug(const char *buf,rt_size_t length)
{
    static char rt_log_buf[100];
	int i = 0;

	for(i = 0; i < length; i++)
	{
		rt_log_buf[i] = buf[i];
	}

#ifdef RT_USING_DEVICE
    debug_device = rt_device_find(FINSH_DEVICE_NAME);

    if (debug_device == RT_NULL)
    {
        rt_hw_console_output(rt_log_buf);
    }
    else
    {
        rt_uint16_t old_flag = debug_device->flag;

        debug_device->flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(debug_device, 0, rt_log_buf, length);
        debug_device->flag = old_flag;
    }
#else
    rt_hw_console_output(rt_log_buf);
#endif
}
RTM_EXPORT(rt_debug);
#endif


/***********************end*******************************/
