/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			rainbow_led.c
  Author:				Raymond
  Date:				2017.6.9
  Version:        
  Description:    // 七色灯控制，在函数thread_rainbow_led.c中的mower_rainbowled_thread()函数中调用
                  // 
                  // 
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "rainbow_led.h"

/* 七彩灯硬件配置函数 */
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

/* 本文件用到变量初始化 */
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


