
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			delay.h
  Author:				Raymond
  Date:				2017.5.25
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

#ifndef __DELAY_H__
#define __DELAY_H__

#include "rtdef.h"


void systick_us_delay(rt_uint16_t us);
void systick_ms_delay(rt_uint16_t ms);
void hw_us_delay(rt_uint16_t us);
void hw_ms_delay(rt_uint16_t ms);

#endif

