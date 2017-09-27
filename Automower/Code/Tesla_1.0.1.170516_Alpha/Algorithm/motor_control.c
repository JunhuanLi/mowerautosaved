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


/* includes *******************************************************************/
#include "motor_control.h"
#include "math_custom.h"
#include "filter.h"

/* macros *********************************************************************/
#define MAX_SPEED_ERROR_SUM		10000
#define	MAX_LINER_ERROR_SUM		10000
#define	MAX_ANGULAR_ERROR_SUM	10000
#define	MOVING_FILTER_SIZE		4

/* static variables ***********************************************************/
static T_motor_control_params	g_controller;

static T_moving_average_filter 	g_feedback_liner_filter;
static T_moving_average_filter	g_feedback_angular_filter;

static s32 g_feedback_liner_array[MOVING_FILTER_SIZE] = {0};
static s32 g_angular_filter_array[MOVING_FILTER_SIZE] = {0};

static void process_motor_control_free(T_encoder *encoder);
static void process_motor_control_speed(T_encoder * encoder);
static void process_motor_control_velocity(T_encoder *encoder);

/*******************************************************************************
  Function		: motor_control_init      
  Description	: motor control init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void motor_control_init(void)
{
	g_controller.type = MOTOR_CONTROL_TYPE_FREE;
	
//	g_controller.speed_controller.left_feedback_speed = 0;
//	g_controller.speed_controller.left_reference_speed = 0;
//	g_controller.speed_controller.pid_params.kp = 100;
//	g_controller.speed_controller.pid_params.ki = 100;
//	g_controller.speed_controller.pid_params.kd = 30;
//	g_controller.speed_controller.right_feedback_speed = 0;
//	g_controller.speed_controller.right_reference_speed = 0;
//	g_controller.speed_controller.left_error = 0;
//	g_controller.speed_controller.left_error_sum = 0;
//	g_controller.speed_controller.right_error = 0;
//	g_controller.speed_controller.right_error_sum = 0;

//	g_controller.velocity_controller.feedback_angular_velocity = 0;
//	g_controller.velocity_controller.feedback_liner_velocity= 0;
//	g_controller.velocity_controller.liner_pid_params.kp = 200;
//	g_controller.velocity_controller.liner_pid_params.ki = 100;
//	g_controller.velocity_controller.liner_pid_params.kd = 30;
//	g_controller.velocity_controller.angular_pid_params.kp = 50;
//	g_controller.velocity_controller.angular_pid_params.ki = 100;
//	g_controller.velocity_controller.angular_pid_params.kd = 30;

//	
//	g_controller.velocity_controller.reference_angular_velocity = 0;
//	g_controller.velocity_controller.reference_liner_velocity = 0;
//	g_controller.velocity_controller.angular_velocity_error = 0;
//	g_controller.velocity_controller.angular_velocity_error_sum = 0;
//	g_controller.velocity_controller.liner_velocity_error = 0;
//	g_controller.velocity_controller.liner_velocity_error_sum = 0;


//	g_feedback_angular_filter.len =MOVING_FILTER_SIZE;
//	g_feedback_angular_filter.filter_arrary = g_angular_filter_array;

//	g_feedback_liner_filter.len = MOVING_FILTER_SIZE;
//	g_feedback_liner_filter.filter_arrary = g_feedback_liner_array;
}

/*******************************************************************************
  Function		: set_motor_control_type      
  Description	: set motor control type
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_control_type(T_motor_control_type type)
{
	g_controller.type = type;
}
/*******************************************************************************
  Function		: set_motor_control_speed_s32      
  Description	: set motor control speed s32
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_control_speed_s32(s32 left_speed, s32 right_speed)
{
	g_controller.speed_controller.left_reference_speed = get_bound_data(left_speed, MOTOR_PWM_PERIOD);
	g_controller.speed_controller.right_reference_speed = get_bound_data(right_speed, MOTOR_PWM_PERIOD);
}
/*******************************************************************************
  Function		: set_motor_control_velocity_s32      
  Description	: set motor control velocity s32
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_control_velocity_s32(s32 liner_velocity, s32 angular_velocity)
{
	g_controller.velocity_controller.reference_liner_velocity = get_bound_data(liner_velocity, MAX_LINER_VELOCITY);
	g_controller.velocity_controller.reference_angular_velocity = get_bound_data(angular_velocity, MAX_ANGULAR_VELOCITY);
}
/*******************************************************************************
  Function		: set_motor_control_speed_float      
  Description	: set motor control speed float
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_control_speed_float(float left_speed, float right_speed)
{
	s32 left_speed_pulse = (s32)((left_speed * GERA_RATIO * MOTOR_PWM_PERIOD) / (2 * M_PI * WHEEL_RADIUS * CIRCLE_PPS));
	s32 right_speed_pulse	= (s32)((right_speed * GERA_RATIO * MOTOR_PWM_PERIOD) / (2 * M_PI * WHEEL_RADIUS * CIRCLE_PPS));

	g_controller.speed_controller.left_reference_speed = get_bound_data(left_speed_pulse, MOTOR_PWM_PERIOD);
	g_controller.speed_controller.right_reference_speed = get_bound_data(right_speed_pulse, MOTOR_PWM_PERIOD);
}
/*******************************************************************************
  Function		: set_motor_control_velocity_float      
  Description	: set motor control velocity float
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_control_velocity_float(float liner_velocity, float angular_velocity)
{
	s32 liner_pulse = (s32)((liner_velocity * GERA_RATIO * MOTOR_PWM_PERIOD) / (2 * M_PI * WHEEL_RADIUS * CIRCLE_PPS));
	s32 angular_pulse = (s32)((angular_velocity * GERA_RATIO * MOTOR_PWM_PERIOD * WHEEL_BASE) / (4 * M_PI * WHEEL_RADIUS * CIRCLE_PPS));

	
	g_controller.velocity_controller.reference_liner_velocity = liner_pulse;
	
	g_controller.velocity_controller.reference_angular_velocity = angular_pulse;
}

/*******************************************************************************
  Function		: process_motor_control_free      
  Description	: process motor control free
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_motor_control_free(T_encoder * encoder)
{
	reset_motor();
	reset_motor_control();
		
//	//enable_motor();
//	disable_motor();
//	//motor_brake();
//	motor_brake_release();
}

/*******************************************************************************
  Function		: process_motor_control_speed      
  Description	: proces motor contro speed
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void process_motor_control_speed(T_encoder * encoder)
{
//	s32 left_error = g_controller.speed_controller.left_reference_speed - encoder->left_speed;
//	s32 right_error = g_controller.speed_controller.right_reference_speed - encoder->right_speed;

//	s32 left_error_sum = get_bound_data(g_controller.speed_controller.left_error_sum + left_error, MAX_SPEED_ERROR_SUM);
//	s32 right_error_sum	= get_bound_data(g_controller.speed_controller.right_error_sum + right_error, MAX_SPEED_ERROR_SUM);

//	s32 left_voltage = get_bound_data((g_controller.speed_controller.pid_params.kp * left_error + 
//									g_controller.speed_controller.pid_params.ki * left_error_sum)/ 100, MOTOR_PWM_PERIOD);

//	s32 right_voltage = get_bound_data((g_controller.speed_controller.pid_params.kp * right_error + 
//									g_controller.speed_controller.pid_params.ki * right_error_sum)/ 100, MOTOR_PWM_PERIOD);


	set_motor_pwm(g_controller.speed_controller.left_reference_speed,  
				  g_controller.speed_controller.right_reference_speed);

//	g_controller.speed_controller.left_feedback_speed = encoder->left_speed;
//	g_controller.speed_controller.right_feedback_speed = encoder->right_speed;
//	g_controller.speed_controller.left_error = left_error;
//	g_controller.speed_controller.left_error_sum = left_error_sum;
//	g_controller.speed_controller.right_error =right_error;
//	g_controller.speed_controller.right_error_sum = right_error_sum;
}
/*******************************************************************************
  Function		: process_motor_control_velocity      
  Description	: process motor control velocity
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void process_motor_control_velocity(T_encoder * encoder)
{
//	s32 feedback_liner = (encoder->left_speed + encoder->right_speed) / 2;
//	s32 feedback_angular = (encoder->right_speed - encoder->left_speed) / 2;

//	s32 filter_feedback_liner = get_moving_average_filter_value(&g_feedback_liner_filter, feedback_liner);
//	s32 filter_feedback_angular = get_moving_average_filter_value(&g_feedback_angular_filter, feedback_angular);

//	s32 liner_error = g_controller.velocity_controller.reference_liner_velocity - feedback_liner;
//	s32 angular_error = g_controller.velocity_controller.reference_angular_velocity - feedback_angular;

//	s32 liner_error_sum = get_bound_data(g_controller.velocity_controller.liner_velocity_error_sum + liner_error, MAX_LINER_ERROR_SUM);
//	s32 angular_error_sum = get_bound_data(g_controller.velocity_controller.angular_velocity_error_sum + angular_error, MAX_ANGULAR_ERROR_SUM);

//	s32 liner_voltage = (g_controller.velocity_controller.liner_pid_params.kp * liner_error + 
//						 g_controller.velocity_controller.liner_pid_params.ki * liner_error_sum) / 100;

//	s32 angular_voltage = (g_controller.velocity_controller.angular_pid_params.kp * angular_error + 
//						   g_controller.velocity_controller.angular_pid_params.ki * angular_error_sum) / 100;

//	s32 left_voltage = get_bound_data(liner_voltage - angular_voltage, MOTOR_PWM_PERIOD);
//	s32 right_voltage = get_bound_data(liner_voltage + angular_voltage, MOTOR_PWM_PERIOD);

	s32 left_voltage = g_controller.velocity_controller.reference_liner_velocity - 
					   g_controller.velocity_controller.reference_angular_velocity ;
					   
	s32 right_voltage = g_controller.velocity_controller.reference_liner_velocity + 
						g_controller.velocity_controller.reference_angular_velocity ;
						
	set_motor_pwm(left_voltage, right_voltage);

//	g_controller.velocity_controller.feedback_liner_velocity = filter_feedback_liner;
//	g_controller.velocity_controller.feedback_angular_velocity = filter_feedback_angular;
//	g_controller.velocity_controller.liner_velocity_error = liner_error;
//	g_controller.velocity_controller.angular_velocity_error = angular_error;
//	g_controller.velocity_controller.liner_velocity_error_sum = liner_error_sum;
//	g_controller.velocity_controller.angular_velocity_error_sum = angular_error_sum;
}

/*******************************************************************************
  Function		: update_motor_control      
  Description	: update motor control
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_motor_control(void)
{
	get_encoder_info(&g_controller.encoder);
	get_motor_info(&g_controller.motor);

	switch(g_controller.type)
	{
		case MOTOR_CONTROL_TYPE_FREE:
		{
			process_motor_control_free(&g_controller.encoder);
			break;
		}
		case MOTOR_CONTROL_TYPE_SPEED:
		{
			process_motor_control_speed(&g_controller.encoder);
			break;
		}
		case MOTOR_CONTROL_TYPE_VELOCITY:
		{
			process_motor_control_velocity(&g_controller.encoder);
		}
		default:
		{
			break;
		}
	}
}
/*******************************************************************************
  Function		: reset_motor_control      
  Description	: reset motor control
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_motor_control(void)
{
	u8 i = 0;

	g_controller.type = MOTOR_CONTROL_TYPE_FREE;

	g_controller.speed_controller.left_feedback_speed = 0;
	g_controller.speed_controller.left_reference_speed = 0;
	g_controller.speed_controller.pid_params.kp = 100;
	g_controller.speed_controller.pid_params.ki = 100;
	g_controller.speed_controller.pid_params.kd = 30;
	g_controller.speed_controller.right_feedback_speed = 0;
	g_controller.speed_controller.right_reference_speed = 0;
	g_controller.speed_controller.left_error = 0;
	g_controller.speed_controller.left_error_sum = 0;
	g_controller.speed_controller.right_error = 0;
	g_controller.speed_controller.right_error_sum = 0;

	g_controller.velocity_controller.feedback_angular_velocity = 0;
	g_controller.velocity_controller.feedback_liner_velocity= 0;
	g_controller.velocity_controller.liner_pid_params.kp = 200;
	g_controller.velocity_controller.liner_pid_params.ki = 100;
	g_controller.velocity_controller.liner_pid_params.kd = 30;
	g_controller.velocity_controller.angular_pid_params.kp = 50;
	g_controller.velocity_controller.angular_pid_params.ki = 100;
	g_controller.velocity_controller.angular_pid_params.kd = 30;
	g_controller.velocity_controller.reference_angular_velocity = 0;
	g_controller.velocity_controller.reference_liner_velocity = 0;
	g_controller.velocity_controller.angular_velocity_error = 0;
	g_controller.velocity_controller.angular_velocity_error_sum = 0;
	g_controller.velocity_controller.liner_velocity_error = 0;
	g_controller.velocity_controller.liner_velocity_error_sum = 0;


	for(i = 0; i < MOVING_FILTER_SIZE; i++)
	{
		g_feedback_liner_array[i] = 0;
		g_angular_filter_array[i] = 0;
	}

	g_feedback_liner_filter.sum = 0;
	g_feedback_angular_filter.sum = 0;

	g_feedback_liner_filter.tap = 0;
	g_feedback_angular_filter.tap = 0;
}
/*******************************************************************************
  Function		: get_motor_control_infor      
  Description	: get motor control infor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_motor_control_infor(T_motor_control_params *controller)
{
	controller->type = g_controller.type;

	//*controller = g_controller;

	controller->motor.motor_left_current = g_controller.motor.motor_left_current;
	controller->motor.motor_left_direction = g_controller.motor.motor_left_direction;
	controller->motor.motor_left_enable = g_controller.motor.motor_left_enable;
	controller->motor.motor_left_pwm = g_controller.motor.motor_left_pwm;
	
	controller->motor.motor_right_current = g_controller.motor.motor_right_current;
	controller->motor.motor_right_direction = g_controller.motor.motor_right_direction;
	controller->motor.motor_right_enable = g_controller.motor.motor_right_enable;
	controller->motor.motor_right_pwm = g_controller.motor.motor_right_pwm;
		
	controller->encoder.last_left_encoder = g_controller.encoder.last_left_encoder;
	controller->encoder.last_right_encoder = g_controller.encoder.last_right_encoder;
	controller->encoder.left_encoder = g_controller.encoder.left_encoder;
	controller->encoder.left_encoder_delta = g_controller.encoder.left_encoder_delta;
	controller->encoder.left_encoder_overflow = g_controller.encoder.left_encoder_overflow;
	controller->encoder.left_encoder_sum = g_controller.encoder.left_encoder_sum;
	controller->encoder.left_first_measure_flag= g_controller.encoder.left_first_measure_flag;
	controller->encoder.left_speed = g_controller.encoder.left_speed;
	controller->encoder.right_encoder = g_controller.encoder.right_encoder;
	controller->encoder.right_encoder_delta = g_controller.encoder.right_encoder_delta;
	controller->encoder.right_encoder_overflow = g_controller.encoder.right_encoder_overflow;
	controller->encoder.right_encoder_sum = g_controller.encoder.right_encoder_sum;
	controller->encoder.right_first_measure_flag = g_controller.encoder.right_first_measure_flag;
	controller->encoder.right_speed = g_controller.encoder.right_speed;
		
	controller->speed_controller.left_error = g_controller.speed_controller.left_error;
	controller->speed_controller.left_error_sum = g_controller.speed_controller.left_error_sum;
	controller->speed_controller.left_feedback_speed = g_controller.speed_controller.left_feedback_speed;
	controller->speed_controller.left_reference_speed = g_controller.speed_controller.left_reference_speed;
	controller->speed_controller.pid_params = g_controller.speed_controller.pid_params;
	controller->speed_controller.right_error = g_controller.speed_controller.right_error;
	controller->speed_controller.right_error_sum = g_controller.speed_controller.right_error_sum;
	controller->speed_controller.right_feedback_speed = g_controller.speed_controller.right_feedback_speed;
	controller->speed_controller.right_reference_speed = g_controller.speed_controller.right_reference_speed;

	controller->velocity_controller.angular_pid_params = g_controller.velocity_controller.angular_pid_params;
	controller->velocity_controller.angular_velocity_error = g_controller.velocity_controller.angular_velocity_error;
	controller->velocity_controller.angular_velocity_error_sum = g_controller.velocity_controller.angular_velocity_error_sum;
	controller->velocity_controller.feedback_angular_velocity = g_controller.velocity_controller.feedback_angular_velocity;
	controller->velocity_controller.feedback_liner_velocity = g_controller.velocity_controller.feedback_liner_velocity;
	controller->velocity_controller.liner_pid_params = g_controller.velocity_controller.liner_pid_params;
	controller->velocity_controller.liner_velocity_error = g_controller.velocity_controller.liner_velocity_error;
	controller->velocity_controller.liner_velocity_error_sum = g_controller.velocity_controller.liner_velocity_error_sum;
	controller->velocity_controller.reference_angular_velocity = g_controller.velocity_controller.reference_angular_velocity;
	controller->velocity_controller.reference_liner_velocity = g_controller.velocity_controller.reference_liner_velocity;
}

/*******************************************************************************
  Function		: get_motor_control_infor      
  Description	: get motor control infor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 g_leftpwm = 0;
s32 g_rightpwm = 0;
u8 g_enable = 1;
u8 g_brake = 0;

void motor_test(void)
{
		if(g_enable == 2)
		{
			set_motor_control_type(MOTOR_CONTROL_TYPE_SPEED);
			set_motor_control_speed_s32(200, -200);
		}
		else if(g_enable == 3)
		{	
			set_motor_control_type(MOTOR_CONTROL_TYPE_SPEED);
			set_motor_control_speed_float(-0.2, 0.1);
		}
		else if(g_enable == 4)
		{	
			
			set_motor_control_type(MOTOR_CONTROL_TYPE_VELOCITY);
			set_motor_control_velocity_s32(500, -10);
		}
		else if(g_enable == 5)
		{	
			set_motor_control_type(MOTOR_CONTROL_TYPE_VELOCITY);
			set_motor_control_velocity_float(-0.2, 0.1);
		}
		else
		{
			set_motor_control_type(MOTOR_CONTROL_TYPE_FREE);
		}
}
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
