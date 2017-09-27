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

#ifndef _ENCODER_H_
#define _ENCODER_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"


/* macros *********************************************************************/
/* PH9 TIM12 CH2 */
#define ENCODER_LEFT_A_PORT						GPIOA
#define	ENCODER_LEFT_A_PIN						GPIO_Pin_1
#define	ENCODER_LEFT_A_CLK						RCC_AHB1Periph_GPIOA
#define	ENCODER_LEFT_A_PINSOURCE				GPIO_PinSource1
#define	ENCODER_LEFT_A_AF						GPIO_AF_TIM2

#define ENCODER_LEFT_TIM_CLK					RCC_APB1Periph_TIM2
#define ENCODER_LEFT_TIMER						TIM2

#define ENCODER_LEFT_PINSOURCE					EXTI_PinSource1
#define ENCODER_LEFT_PORTSOURCE				EXTI_PortSourceGPIOA

/* PE5 TIM9 CH1*/
#define ENCODER_RIGHT_A_PORT					GPIOA				
#define	ENCODER_RIGHT_A_PIN						GPIO_Pin_6				
#define	ENCODER_RIGHT_A_CLK						RCC_AHB1Periph_GPIOA	
#define	ENCODER_RIGHT_A_PINSOURCE				GPIO_PinSource6		
#define	ENCODER_RIGHT_A_AF						GPIO_AF_TIM3

#define	ENCODER_RIGHT_TIM_CLK					RCC_APB1Periph_TIM3 	
#define	ENCODER_RIGHT_TIMER						TIM3			

#define ENCODER_RIGHT_PINSOURCE					EXTI_PinSource6
#define ENCODER_RIGHT_PORTSOURCE				EXTI_PortSourceGPIOA

#define M_PI				3.14159265
#define WHEEL_RADIUS	  (0.177/2) 
#define WHEEL_BASE			0.35 //0.38   

#define GERA_RATIO			45
#define PLUSE_PPR	  		12 
#define PLUSE_PER_CIRCLE	(GERA_RATIO * PLUSE_PPR)

#define CIRCLE_PPS			50

#define ENCODER_PPR					(s32)4
#define ENCODER_UPDATE_FREQUENCY	(s32)250



typedef struct
{
	T_bool left_first_measure_flag;
	T_bool right_first_measure_flag;
	s32	 left_speed;
	s32	 right_speed;
	s32	 left_encoder;
	s32	 right_encoder;
	s32  last_left_encoder;
	s32  last_right_encoder;
	s32  left_encoder_overflow;
	s32  right_encoder_overflow;
	s32  left_encoder_delta;
	s32  right_encoder_delta;
	s32  left_encoder_sum;
	s32  right_encoder_sum;
}T_encoder;

/* funcitons ******************************************************************/
void encoder_init(void);
void update_left_encoder(void);
void update_right_encoder(void);
s32 get_left_motor_speed(void);
s32 get_right_motor_speed(void);

float get_left_encoder(void);
float get_right_encoder(void);
void reset_encoder(void);
void get_encoder_info(T_encoder *encoder);


#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
