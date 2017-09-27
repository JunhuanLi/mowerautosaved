/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: motor.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: motor driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _ENCODER_MOWER_H_
#define _ENCODER_MOWER_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"


/* macros *********************************************************************/
/* PH9 TIM12 CH2 */
#define ENCODER_MOWER_PORT						GPIOA
#define	ENCODER_MOWER_PIN						GPIO_Pin_1
#define	ENCODER_MOWER_CLK						RCC_AHB1Periph_GPIOA
#define	ENCODER_MOWER_PINSOURCE				    GPIO_PinSource1
#define	ENCODER_MOWER_AF						GPIO_AF_TIM2

#define ENCODER_MOWER_TIM_CLK					RCC_APB1Periph_TIM2			
#define ENCODER_MOWER_TIMER						TIM2


#define ENCODER_MOWER_PPR					(s32)4
#define ENCODER_MOWER_UPDATE_FREQUENCY		(s32)250

typedef struct
{
	T_bool first_measure_flag;
	s32	 mower_speed;
	s32	 mower_encoder;
	s32  last_mower_encoder;
	s32  encoder_mower_overflow;
	s32  encoder_mower_delta;
	s32  encoder_mower_sum;
}T_encoder_mower;

/* funcitons ******************************************************************/
void encoder_mower_init(void);
s32 get_encoder_mower_speed(void);
s64 get_encoder_mower_encoder(void);
void reset_encoder_mower(void);
void get_encoder_mower_info(T_encoder_mower *encoder_mower);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
