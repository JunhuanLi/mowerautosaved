/*********************************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			drtc.c
  Author:				Raymond
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

#include "stm32f4xx.h"

#include "posix_types.h"
#include "rtthread.h"
//#include "time.h"
#include "drtc.h"
#include "rtc.h"
#include "drtc.h"

#define MINUTE  60
#define HOUR    (MINUTE * 60)
#define DAY     (HOUR * 24)
#define YEAR    (DAY * 365)

struct rtc_tag rtc_timer; 

void rt_drtc_hw_config(void);
static rt_err_t rt_drtc_t_d_init(void);

const int month[12] =
{ 
    0, 
    DAY*(31), 
    DAY*(31+29), 
    DAY*(31+29+31), 
    DAY*(31+29+31+30), 
    DAY*(31+29+31+30+31), 
    DAY*(31+29+31+30+31+30), 
    DAY*(31+29+31+30+31+30+31), 
    DAY*(31+29+31+30+31+30+31+31), 
    DAY*(31+29+31+30+31+30+31+31+30), 
    DAY*(31+29+31+30+31+30+31+31+30+31), 
    DAY*(31+29+31+30+31+30+31+31+30+31+30) 
};

static time_t rt_mktime(struct tm *tm) 
{ 
    long res; 
    int year; 
    year = tm->tm_year - 70; 
    
    res = YEAR * year + DAY * ((year + 1) / 4); 
    res += month[tm->tm_mon]; 

    if (tm->tm_mon > 1 && ((year + 2) % 4)) 
    {
        res -= DAY;
    }
    res += DAY * (tm->tm_mday - 1); 
    res += HOUR * tm->tm_hour; 
    res += MINUTE * tm->tm_min; 
    res += tm->tm_sec; 
    return res; 
} 


/**
 ******************************
 * author   : Raymond 
 * input    : NO
 * output   : NO
 * return   : RT_EOK / RT_ERROR
 * function : rtc init
 ********************************
 */
static rt_err_t rt_drtc_init(rt_device_t device)
{
    time_t time;

    rt_uint32_t temp;

    temp = RTC_ReadBackupRegister(RTC_BKP_DRX);

    #ifdef CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n  rtc backe data : 0x%4x..",temp);
    #endif

    
    if(RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
    {
        /* 初次上电要重新设定 */
        rt_drtc_hw_config();
        
        //device.control(device,RT_DEVICE_CTRL_RTC_SET_TIME,(void *)&time);
        rt_drtc_t_d_init();
    }
    else
    {
        if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n!!! power reset...");
            #endif
        }
        else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n!!! external reset...");
            #endif
        }
            #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n!!! no need to reset RTC...");
            #endif
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,  ENABLE);
        PWR_BackupAccessCmd(ENABLE); 

        RTC_WaitForSynchro();/* 等待 RTC APB同步*/
        
    }
    return RT_EOK;
}



/**
 ******************************
 * author   : Raymond 
 * input    : NO
 * output   : NO
 * return   : RT_EOK / RT_ERROR
 * function : rtc device open
 ********************************
 */
static rt_err_t rt_drtc_open(rt_device_t device, rt_uint16_t oflag)
{
    return RT_EOK;
}


/**
 ******************************
 * author   : Raymond 
 * input    : NO
 * output   : NO
 * return   : RT_EOK / RT_ERROR
 * function : rtc device close
 ********************************
 */
static rt_err_t rt_drtc_close(rt_device_t device)
{
    return RT_EOK;
}


/**
 ************************************************************
 * author   :           Raymond 
 * input    :           NO
 * output   :           NO
 * return   :           RT_EOK / RT_ERROR
 * function :           rtc device setting 
 *                      1、set time  
 *                      2、get time 
 *                      3、set alarm  
 *                      4、get alarm     
 ************************************************************
 */
static rt_err_t rt_drtc_control(rt_device_t device, rt_uint8_t cmd, void * args)
{
    rt_uint8_t ctrl_cmd;
    time_t *time;

    struct tm ti,*to;

    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct; 
    
    //rt_err_t eflag;
    //eflag = RT_EOK;

    //time = (time_t *)args;
    
    ctrl_cmd = cmd; /* get rtc time cmd*/

    switch(ctrl_cmd)
    {
        case RT_DEVICE_CTRL_RTC_GET_TIME:/**< get time */
            time = (time_t *)args;
            // 获取时间
            RTC_GetTime(RTC_FORMAT_BIN_BCD,&RTC_TimeStruct);
            
            ti.tm_sec = RTC_TimeStruct.RTC_Seconds;
            ti.tm_min = RTC_TimeStruct.RTC_Minutes;
            ti.tm_hour = RTC_TimeStruct.RTC_Hours;

            RTC_GetDate(RTC_FORMAT_BIN_BCD,&RTC_DateStruct);
            ti.tm_wday = RTC_DateStruct.RTC_WeekDay;
            ti.tm_mon = RTC_DateStruct.RTC_Month;
            ti.tm_mday = RTC_DateStruct.RTC_Date;
            ti.tm_year = RTC_DateStruct.RTC_Year;
            
           // *time = rt_mktime(&ti);

            #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n sec:%2d..min:%2d..hour:%2d..weekday:%2d..day:%2d..moth:%2d..year:%4d..",ti.tm_sec,ti.tm_min,ti.tm_hour,ti.tm_wday,ti.tm_mday,ti.tm_mon,ti.tm_year);
            #endif
                
            break;
        case RT_DEVICE_CTRL_RTC_SET_TIME:/**< set time */

            time = (time_t *)args;

           // to = localtime(time);
            // set time
            RTC_TimeStruct.RTC_H12 = RTC_H12_AM_PM;
            RTC_TimeStruct.RTC_Hours = to->tm_hour;
            RTC_TimeStruct.RTC_Minutes = to->tm_min;
            RTC_TimeStruct.RTC_Seconds = to->tm_sec;
            // set data
            RTC_DateStruct.RTC_WeekDay = to->tm_wday;
            RTC_DateStruct.RTC_Date = to->tm_mday;
            RTC_DateStruct.RTC_Month = to->tm_mon;
            RTC_DateStruct.RTC_Year = to->tm_year;
            // write time & data
            RTC_SetTime(RTC_FORMAT_BIN_BCD,&RTC_TimeStruct);
            RTC_SetDate(RTC_FORMAT_BIN_BCD,&RTC_DateStruct);
            // write backup register
            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
            break;
        case RT_DEVICE_CTRL_RTC_GET_ALARM:/**< get alarm */

            break;
        case RT_DEVICE_CTRL_RTC_SET_ALARM:/**< set alarm */
            
            break;
    }
    return RT_EOK;
}


/**
 ******************************
 * author   : Raymond 
 * input    : NO
 * output   : NO
 * return   : RT_EOK / RT_ERROR
 * function : rtc device register
 ********************************
 */
rt_err_t rt_hw_rtc_register(rt_rtc_t device_rtc, const char * name, rt_uint32_t flags, void * data)
{
    rt_device_t dev;

    if(flags != RT_DEVICE_FLAG_RDWR)return RT_ERROR;

    dev =&(device_rtc->rtc);

    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->type = RT_Device_Class_RTC;
    dev->init = rt_drtc_init;
    dev->open = rt_drtc_open;
    dev->close = rt_drtc_close;
    dev->write = RT_NULL;
    dev->read = RT_NULL;
    dev->control = rt_drtc_control;
    dev->user_data = data;

    return rt_device_register(dev, name, flags);

}

/**
 ************************************************************
 * author   :               Raymond 
 * input    :               NO
 * output   :               NO
 * return   :               RT_EOK / RT_ERROR
 * function :               config rtc (BSP)
 ************************************************************
 */
void rt_drtc_hw_config(void)
{
   RTC_InitTypeDef RTC_InitStructure;

   RCC_AHB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/* enable the pwr clock */
    
   PWR_BackupAccessCmd(ENABLE);
   
   RCC_LSEConfig(RCC_LSE_ON);/* enable LSE */
   
   while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
   {
        ;/* NOTHING */ 
   }
   
   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

   RCC_RTCCLKCmd(ENABLE);

   RTC_WaitForSynchro();/* 等待 RTC APB同步*/

   RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
   RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;
   RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

   if(RTC_Init(&RTC_InitStructure)==ERROR)
   {
        #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n  !!! RTC init FALSE...");
        #endif
   }
}


/**
 **********************************************
 * author   :               Raymond 
 * input    :               NO
 * output   :               NO
 * return   :               RT_EOK / RT_ERROR
 * function :               rtc init
 **********************************************
 */
void rt_drtc_config(void)
{
    rt_device_t device;
    
    rt_hw_rtc_register(&rtc_timer,"rtc",RT_DEVICE_FLAG_RDWR,RT_NULL);

    device = rt_device_find("rtc");

    if(device != NULL)
    {
        device->init(device);
    }
}


/**
 **********************************************
 * author   :               Raymond 
 * input    :               NO
 * output   :               NO
 * return   :               RT_EOK / RT_ERROR
 * function :               rtc init
 **********************************************
 */
static rt_err_t rt_drtc_t_d_init(void)
{
    RTC_TimeTypeDef RTC_TimeStructer;
    RTC_DateTypeDef RTC_DateStructer;

    RTC_TimeStructer.RTC_H12 = RTC_H12_AM_PM;
    RTC_TimeStructer.RTC_Hours = RTC_HOURS; 
    RTC_TimeStructer.RTC_Minutes = RTC_MIN;
    RTC_TimeStructer.RTC_Seconds = RTC_SEC;
    RTC_SetTime(RTC_FORMAT_BIN_BCD, &RTC_TimeStructer);
    RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);

    RTC_DateStructer.RTC_WeekDay = RTC_WEEKDAY; 
    RTC_DateStructer.RTC_Year = RTC_YEAR;
    RTC_DateStructer.RTC_Month = RTC_MON;
    RTC_DateStructer.RTC_Date = RTC_DATE;
    //RTC_SetTime(RTC_FORMAT_BIN_BCD, &RTC_TimeStructer);
    RTC_SetDate(RTC_FORMAT_BIN_BCD, &RTC_DateStructer);
    RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);

    return RT_EOK;
}





/**************************end**************************************/
