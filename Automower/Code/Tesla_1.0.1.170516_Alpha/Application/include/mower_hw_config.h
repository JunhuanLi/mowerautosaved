/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			mower_hw_config.h
  Author:				Raymond
  Date:				2017.6.13
  Version:        
  Description:		存放一些公共的函数
  						
  						
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef __MOWER_HW_CONFIG_H__
#define __MOWER_HW_CONFIG_H__

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

// AHB1
#define  AHB1_A         RCC_AHB1Periph_GPIOA  
#define  AHB1_B         RCC_AHB1Periph_GPIOB 
#define  AHB1_C         RCC_AHB1Periph_GPIOC 
#define  AHB1_D         RCC_AHB1Periph_GPIOD 
#define  AHB1_E         RCC_AHB1Periph_GPIOE 
#define  AHB1_F         RCC_AHB1Periph_GPIOF 
#define  AHB1_G         RCC_AHB1Periph_GPIOG 
#define  AHB1_H         RCC_AHB1Periph_GPIOH 
#define  AHB1_I         RCC_AHB1Periph_GPIOI 
#define  AHB1_J         RCC_AHB1Periph_GPIOJ 
#define  AHB1_K         RCC_AHB1Periph_GPIOK
#define  AHB1_CRC       RCC_AHB1Periph_CRC 
#define  AHB1_DMA1      RCC_AHB1Periph_DMA1 
#define  AHB1_DMA2      RCC_AHB1Periph_DMA2 
#define  AHB1_DMA2D     RCC_AHB1Periph_DMA2D 
#define  AHB1_ETH_MAC   RCC_AHB1Periph_ETH_MAC
#define  AHB1_OTG_HS    RCC_AHB1Periph_OTG_HS 
#define  AHB1_RNG       RCC_AHB1Periph_RNG 

// AHB2

// AHB3

// APB1
#define APB1_TIM6       RCC_APB1Periph_TIM6
// APB2


















void peripheral_hw_config(void);
void peripheral_hw_init(void);

#endif

