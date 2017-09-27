#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PB0和PB1为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<1;//使能PORTB时钟 
	GPIO_Set(GPIOB,PIN0|PIN1,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PB0,PB1设置
	LED0=1;//LED0关闭
	LED1=1;//LED1关闭
}






