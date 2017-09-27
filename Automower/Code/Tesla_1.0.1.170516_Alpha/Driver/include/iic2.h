/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	ist8310.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	ist8310 driiver
*******************************************************************************/
#ifndef _IIC2_H_
#define _IIC2_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"

/* macros *********************************************************************/
#define I2C2_SCL_PORT		GPIOH
#define I2C2_SCL_PIN		GPIO_Pin_4
#define I2C2_SCL_CLK		RCC_AHB1Periph_GPIOH

#define I2C2_SDA_PORT		GPIOH
#define I2C2_SDA_PIN		GPIO_Pin_5
#define I2C2_SDA_CLK		RCC_AHB1Periph_GPIOH

/* funcitons ******************************************************************/

void iic2_init(void);           			 
void iic2_start(void);				
void iic2_stop(void);	  			
void iic2_send_byte(u8 txd);			
u8 iic2_read_byte(u8 ack);
u8 iic2_wait_ack(void); 				
void iic2_ack(void);				
void iic2_nack(void);				

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

