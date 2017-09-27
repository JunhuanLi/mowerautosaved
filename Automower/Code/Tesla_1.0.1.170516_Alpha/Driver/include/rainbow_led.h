/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			rainbow_led.h
  Author:				Raymond
  Date:				2017.6.9
  Version:        
  Description:    // ��ɫ�ƿ��ƣ��ں���thread_rainbow_led.c�е�mower_rainbowled_thread()�����е���
                  // 
                  // 
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef __RAINBOW_LED_H__
#define __RAINBOW_LED_H__

#include "stm32f4xx.h"

#define LED_G_ON		{PB_OUT(0) = 1;}
#define LED_G_OFF		{PB_OUT(0) = 0;}

#define LED_R_ON	{PB_OUT(1) = 1;}
#define LED_R_OFF	{PB_OUT(1) = 0;}

#define LED_B_ON	{PB_OUT(12) = 1;}
#define LED_B_OFF	{PB_OUT(12) = 0;}
	
/* */
typedef struct rainbowled
{
	u8 num;
	u8 colour;
	u8 mode;
}T_rainbowled;
		
void rainbow_led_hw_config(void);
void rainbow_led_variable_init(void);



#endif

