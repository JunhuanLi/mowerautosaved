/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: MOW_MOTOR.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: MOW_MOTOR driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _MOW_MOTOR_H_
#define _MOW_MOTOR_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define MOW_MOTOR_TIMER										TIM5
#define	MOW_MOTOR_PWM_CLOCK									(u32)(180000000)		
#define	MOW_MOTOR_PWM_FREQUENCY								(u16)(10000)		
#define MOW_MOTOR_PWM_PRESCALE								(u8)(5)
#define MOW_MOTOR_PWM_PERIOD								(u16)(MOW_MOTOR_PWM_CLOCK / (MOW_MOTOR_PWM_FREQUENCY \
															* MOW_MOTOR_PWM_PRESCALE))	
																					
#define MOW_MOTOR_PWM1_PORT								GPIOH
#define	MOW_MOTOR_PWM1_PIN								GPIO_Pin_12
#define	MOW_MOTOR_PWM1_CLK								RCC_AHB1Periph_GPIOH
#define	MOW_MOTOR_PWM1_PINSOURCE						GPIO_PinSource12

#define MOW_MOTOR_PWM2_PORT								GPIOI
#define	MOW_MOTOR_PWM2_PIN								GPIO_Pin_0
#define	MOW_MOTOR_PWM2_CLK								RCC_AHB1Periph_GPIOI
#define	MOW_MOTOR_PWM2_PINSOURCE						GPIO_PinSource0


#define MOW_MOTOR_ENABLE_PORT							GPIOI
#define	MOW_MOTOR_ENABLE_PIN							GPIO_Pin_9
#define	MOW_MOTOR_ENABLE_CLK							RCC_AHB1Periph_GPIOI

#define MOW_MOTOR_BRAKE_PORT							GPIOI
#define	MOW_MOTOR_BRAKE_PIN								GPIO_Pin_10
#define	MOW_MOTOR_BRAKE_CLK								RCC_AHB1Periph_GPIOI

#define MOW_MOTOR_EXCEPTION_PORT						GPIOI
#define	MOW_MOTOR_EXCEPTION_PIN							GPIO_Pin_11
#define	MOW_MOTOR_EXCEPTION_CLK							RCC_AHB1Periph_GPIOI



typedef enum
{
	DIRECTION_CW = 0,
	DIRECTION_CCW	
}T_mower_direction;

typedef struct
{
	T_bool mow_motor_enable;
	T_bool mow_motor_exception;
	T_bool mow_motor_brake_state;
	T_mower_direction mow_motor_direction;
	s32 mow_motor_pwm;
	s32 mow_motor_current;
}T_mow_motor;

/* funcitons ******************************************************************/
void mow_motor_init(void);
void enable_mow_motor(void);
void disable_mow_motor(void);
void mow_motor_brake(void);
void mow_motor_brake_release(void);

void set_mow_motor_pwm(s32 mower_pwm);
void update_mow_motor_current(void);
void reset_mow_motor(void);
void get_mow_motor_info(T_mow_motor *mow_motor);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
