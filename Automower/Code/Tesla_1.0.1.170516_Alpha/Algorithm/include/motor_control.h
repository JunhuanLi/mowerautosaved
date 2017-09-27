/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: motor_control.h
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

#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"
#include "motor.h"
#include "encoder.h"


/* macros *********************************************************************/


#define MAX_SPEED				5200 	// pulse/s  60*5200/48 = 6500RPM/min
#define MAX_LINER_VELOCITY		3600	// 0.5m/s
#define MAX_ANGULAR_VELOCITY	3600	// PI RAD/S


typedef enum
{
	MOTOR_CONTROL_TYPE_FREE = 0,
	MOTOR_CONTROL_TYPE_SPEED,
	MOTOR_CONTROL_TYPE_VELOCITY
}T_motor_control_type;

typedef struct
{
	s32 kp;
	s32 kd;
	s32 ki;
}T_pid_params;

typedef struct
{
	T_pid_params pid_params;
	s32 		left_reference_speed;
	s32 		right_reference_speed;
	s32			left_feedback_speed;
	s32			right_feedback_speed;
	s32			left_error;
	s32			right_error;
	s32			left_error_sum;
	s32			right_error_sum;
}T_motor_speed_control;


typedef struct
{
	T_pid_params liner_pid_params;
	T_pid_params angular_pid_params;
	
	s32			reference_liner_velocity;
	s32			reference_angular_velocity;

	s32			feedback_liner_velocity;
	s32			feedback_angular_velocity;

	s32			liner_velocity_error;
	s32			angular_velocity_error;

	s32			liner_velocity_error_sum;
	s32			angular_velocity_error_sum;
}T_motor_velocity_control;

typedef struct
{
	T_motor_control_type 		type;
	T_motor_speed_control      speed_controller;
	T_motor_velocity_control	velocity_controller;
	T_encoder				encoder;
	T_motor					motor;
}T_motor_control_params;


void motor_control_init(void);
void set_motor_control_type(T_motor_control_type type);

void set_motor_control_speed_s32(s32 left_speed, s32 right_speed);
void set_motor_control_velocity_s32(s32 liner_velocity, s32 angular_velocity);

void set_motor_control_speed_float(float left_speed, float right_speed);
void set_motor_control_velocity_float(float liner_velocity, float angular_velocity);

void update_motor_control(void);

void reset_motor_control(void);
void get_motor_control_infor(T_motor_control_params *controller);

void motor_test(void);
#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
