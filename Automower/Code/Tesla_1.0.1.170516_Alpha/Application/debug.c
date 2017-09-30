/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			debug.C
  Author:				Raymond
  Date:				2017.9.21
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
  static char rt_log_buf[200];
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
        rt_uint16_t old_flag = debug_device->open_flag;
        debug_device->open_flag &= (~RT_DEVICE_FLAG_STREAM);
        rt_device_write(debug_device, 0, rt_log_buf, length);
        debug_device->open_flag = old_flag;
    }
#else
    rt_hw_console_output(rt_log_buf);
#endif
}
RTM_EXPORT(rt_debug);
#endif


/***********************end*******************************/
