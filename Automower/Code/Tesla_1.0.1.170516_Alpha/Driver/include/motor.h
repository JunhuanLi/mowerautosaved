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

#ifndef _MOTOR_H_
#define _MOTOR_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define MOTOR_TIMER										TIM8
#define	MOTOR_PWM_CLOCK									(u32)(180000000)		
#define	MOTOR_PWM_FREQUENCY								(u16)(10000)		
#define MOTOR_PWM_PRESCALE								(u8)(5)
#define MOTOR_PWM_PERIOD								(u16)(MOTOR_PWM_CLOCK / (MOTOR_PWM_FREQUENCY \
														* MOTOR_PWM_PRESCALE))	
																					
#define MOTOR_PWM1_LEFT_PORT							GPIOC
#define	MOTOR_PWM1_LEFT_PIN								GPIO_Pin_6
#define	MOTOR_PWM1_LEFT_CLK								RCC_AHB1Periph_GPIOC
#define	MOTOR_PWM1_LEFT_PINSOURCE						GPIO_PinSource6

#define MOTOR_PWM2_LEFT_PORT							GPIOC
#define	MOTOR_PWM2_LEFT_PIN								GPIO_Pin_7
#define	MOTOR_PWM2_LEFT_CLK								RCC_AHB1Periph_GPIOC
#define	MOTOR_PWM2_LEFT_PINSOURCE						GPIO_PinSource7

#define MOTOR_PWM1_RIGHT_PORT							GPIOC
#define	MOTOR_PWM1_RIGHT_PIN							GPIO_Pin_8
#define	MOTOR_PWM1_RIGHT_CLK							RCC_AHB1Periph_GPIOC
#define	MOTOR_PWM1_RIGHT_PINSOURCE						GPIO_PinSource8

#define MOTOR_PWM2_RIGHT_PORT							GPIOC
#define	MOTOR_PWM2_RIGHT_PIN							GPIO_Pin_9
#define	MOTOR_PWM2_RIGHT_CLK							RCC_AHB1Periph_GPIOC
#define	MOTOR_PWM2_RIGHT_PINSOURCE						GPIO_PinSource9


#define MOTOR_ENABLE_PORT								GPIOI
#define	MOTOR_ENABLE_PIN								GPIO_Pin_1
#define	MOTOR_ENABLE_CLK								RCC_AHB1Periph_GPIOI

#define MOTOR_BRAKE_PORT								GPIOI
#define	MOTOR_BRAKE_PIN									GPIO_Pin_2
#define	MOTOR_BRAKE_CLK									RCC_AHB1Periph_GPIOI

#define MOTOR_EXCEPTION_LEFT_PORT						GPIOI
#define	MOTOR_EXCEPTION_LEFT_PIN						GPIO_Pin_7
#define	MOTOR_EXCEPTION_LEFT_CLK						RCC_AHB1Periph_GPIOI

#define MOTOR_EXCEPTION_RIGHT_PORT						GPIOI
#define	MOTOR_EXCEPTION_RIGHT_PIN						GPIO_Pin_4
#define	MOTOR_EXCEPTION_RIGHT_CLK						RCC_AHB1Periph_GPIOI

typedef enum
{
	DIRECTION_CW = 0,
	DIRECTION_CCW = 1	
}T_direction;

typedef struct
{
	T_bool motor_left_enable;
	T_bool motor_right_enable;
	T_bool motor_left_exception;
	T_bool motor_right_exception;
	T_bool motor_brake_state;
	T_direction motor_left_direction;
	T_direction motor_right_direction;
	s32 motor_left_pwm;
	s32 motor_right_pwm;
	s32 motor_left_current;
	s32 motor_right_current;
}T_motor;

/* funcitons ******************************************************************/
void motor_init(void);
void enable_motor(void);
void disable_motor(void);
void motor_brake(void);
void motor_brake_release(void);

void set_motor_pwm(s32 left_pwm, s32 right_pwm);
void update_motor_current(void);
void reset_motor(void);
void get_motor_info(T_motor *motor);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
