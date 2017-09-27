
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			delay.h
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

#ifndef __DELAY_H__
#define __DELAY_H__

#include "rtdef.h"


void systick_us_delay(rt_uint16_t us);
void systick_ms_delay(rt_uint16_t ms);
void hw_us_delay(rt_uint16_t us);
void hw_ms_delay(rt_uint16_t ms);

#endif

