/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			rainbow_led.c
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
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "rainbow_led.h"

/* �߲ʵ�Ӳ�����ú��� */
void rainbow_led_hw_config(void)
{
#if 0
	GPIO_InitTypeDef io_init_type;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	io_init_type.GPIO_Mode = GPIO_Mode_OUT;
	io_init_type.GPIO_OType = GPIO_OType_PP;
	io_init_type.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12;
	io_init_type.GPIO_PuPd = GPIO_PuPd_UP;
	io_init_type.GPIO_Speed =GPIO_Speed_100MHz;

	GPIO_Init(GPIOB,&io_init_type);
#endif	

}

/* ���ļ��õ�������ʼ�� */
void rainbow_led_variable_init(void)
{
	LED_R_ON;
	LED_G_OFF;
	LED_B_OFF;
	//GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	//GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}

void rainbow_led_control(u8 colour,u8 state)
{
	
}


