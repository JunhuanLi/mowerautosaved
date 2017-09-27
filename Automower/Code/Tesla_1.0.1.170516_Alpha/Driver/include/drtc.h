/*********************************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		drtc.h
  Author:			Raymond
  Date:				2017.9.13
  Version:        
  Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                  // �ơ�˳�򡢶����������ȹ�ϵ

                        ʵʱʱ������
                        
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
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

/* ʱ����ʾ��ʽ */
#define RTC_H12_AM_PM   RTC_H12_AM

/* ʱ�����ݸ�ʽ */
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
