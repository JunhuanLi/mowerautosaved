/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: 2.4g_spi.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: 2.4g spi driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _2_4_G_SPI_H_
#define _2_4_G_SPI_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define _2_4G_SPI			SPI2
#define _2_4G_SPI_CLK		RCC_APB1Periph_SPI2

#define _2_4G_CS_PORT		GPIOB		
#define _2_4G_CS_PIN		GPIO_Pin_12
#define _2_4G_CS_SOURCE		GPIO_PinSource12
#define _2_4G_CS_CLK		RCC_AHB1Periph_GPIOB

#define _2_4G_MOSI_PORT		GPIOB		
#define _2_4G_MOSI_PIN		GPIO_Pin_15
#define _2_4G_MOSI_SOURCE	GPIO_PinSource15
#define _2_4G_MOSI_CLK		RCC_AHB1Periph_GPIOB

#define _2_4G_MISO_PORT		GPIOB		
#define _2_4G_MISO_PIN		GPIO_Pin_14
#define _2_4G_MISO_SOURCE	GPIO_PinSource14
#define _2_4G_MISO_CLK		RCC_AHB1Periph_GPIOB

#define _2_4G_CLK_PORT		GPIOB		
#define _2_4G_CLK_PIN		GPIO_Pin_13
#define _2_4G_CLK_SOURCE	GPIO_PinSource13
#define _2_4G_CLK_CLK		RCC_AHB1Periph_GPIOB

#define _2_4G_SPI_AF		GPIO_AF_SPI2

#define _2_4G_SET_CS()		GPIO_SetBits(_2_4G_CS_PORT, _2_4G_CS_PIN)		
#define _2_4G_RESET_CS()	GPIO_ResetBits(_2_4G_CS_PORT, _2_4G_CS_PIN)

/* funcitons ******************************************************************/
T_bool _2_4g_init(void);
void get_2_4g_info(void);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
