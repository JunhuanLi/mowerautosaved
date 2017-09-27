/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			debug.h
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
#ifndef __DEBUG_H__
#define __DEBUG_H__

//#include <rtthread.h>
//#include <rthw.h>
//#include "stm32f4xx.h"
//#include "lcd.h"

void debug_send(uint8_t* buf,uint8_t length);
void rt_debug(const char *buf,rt_size_t length);
//Kservice is changed rt_debug is added
//serial.c is changed comment line 226~line 230

#endif
/***********************end*************************/
