/*********************************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		drtc.h
  Author:			Raymond
  Date:				2017.9.13
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系

                        实时时钟驱动
                        
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
**********************************************************************/

#ifndef __DRTC_H__
#define __DRTC_H__

//#include "stm32f4xx.h"
#include "rtdef.h"

#define RTC_BKP_DRX      RTC_BKP_DR0
#define RTC_BKP_DATA     0x32f4

#define ASYNCHPREDIV    0x7f
#define SYNCHPREDIV     0xff

#define RTC_HOURS       1
#define RTC_MIN         1
#define RTC_SEC         1

#define RTC_WEEKDAY     1
#define RTC_YEAR        1
#define RTC_MON         1
#define RTC_DATE        1

/* 时间显示格式 */
#define RTC_H12_AM_PM   RTC_H12_AM

/* 时间数据格式 */
#define RTC_FORMAT_BIN_BCD  RTC_Format_BIN








/* rtc struct */
struct rtc_tag
{
    struct rt_device  rtc;
    void * data;
    rt_uint8_t  cnt;
};


typedef struct rtc_tag * rt_rtc_t;

void rt_drtc_config(void);

#endif
/**************************end**************************************/
