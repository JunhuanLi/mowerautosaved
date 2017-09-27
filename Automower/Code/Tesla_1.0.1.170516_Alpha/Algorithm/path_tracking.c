/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: path_tracking.c
  Author		: George 	
  Version		: V1.0.0	  
  Date			: 2017/07/21
  Description	: path_tracking 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

/* includes *******************************************************************/
#include "path_tracking.h"
//#include "adc.h"
//#include "math_custom.h"
#include "math.h"
#include "PI.h"
#include "global.h"
#include "motor_control.h"
#include "usart_driver.h"


/* macros *********************************************************************/
#define MAG_LINE_POLARITY
#define MAG_LINE_MAX						52000.0f
#define MAG_LINE_MIN						1000.0f
#define MAG_LINE_FACING_MARGIN	0.1f

/* static variables ***********************************************************/
static float Tracking_2D_Angle(T_tracking* obj);
static float Tracking_2D_Line(T_tracking* obj);
static float Tracking_Mag_Line(T_tracking* obj);

static __inline void Tracking_Norm_3D(float* x, float* y, float* z)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y) +(*z)*(*z));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
		*z = *z / length;
	}
}

static __inline void Tracking_Norm_2D(float* x, float* y)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
	}
}

/* funcitons ******************************************************************/
void Tracking_Init(T_tracking* obj,uint8_t en, T_tracking_state_type state)
{
	obj->enable = en;
	obj->state = state;
	obj->error = TRACKING_ERROR_NONE;
	//obj->sensor.mag_polarity = 1;
	Tracking_Set_Path_Param(obj,0,0,0);
	Tracking_Set_Angle_Param(obj,0,0,0);
	Tracking_Set_Mag_Tracking_Param(obj,0,0,0);
	Tracking_Set_Mag_Gotoline_Param(obj,0,0,0);
	Tracking_Start_2D_Angle(obj,0,0,0);
}

void Tracking_Set_Path_Param(T_tracking* obj,float kp, float ki, float il)
{
	obj->path_pi.kp = kp;
	obj->path_pi.ki = ki;
	obj->path_pi.il = il;
}

void Tracking_Set_Angle_Param(T_tracking* obj,float kp, float ki, float il)
{
	obj->angle_pi.kp = kp;
	obj->angle_pi.ki = ki;
	obj->angle_pi.il = il;
}

void Tracking_Set_Mag_Tracking_Param(T_tracking* obj,float kp, float ki, float il)
{
	obj->mag_tracking_pi.kp = kp;
	obj->mag_tracking_pi.ki = ki;
	obj->mag_tracking_pi.il = il;
}
void Tracking_Set_Mag_Gotoline_Param(T_tracking* obj,float kp, float ki, float il)
{
	obj->mag_gotoline_pi.kp = kp;
	obj->mag_gotoline_pi.ki = ki;
	obj->mag_gotoline_pi.il = il;
}

void Tracking_Start_2D_Angle(T_tracking* obj,float dir_x,float dir_y,float vel)
{
	Tracking_Norm_2D(&dir_x,&dir_y);
	obj->state  					= TRACKING_STATE_2D_ANGLE;
	obj->path_2D.dir_x 		= dir_x;
	obj->path_2D.dir_y 		= dir_y;
	obj->target_vel 			= vel;
	obj->angle_pi.integral			= 0;
}

void Tracking_Start_2D_Line(T_tracking* obj,float point_x, float point_y, float dir_x, float dir_y, float vel)
{
	Tracking_Norm_2D(&dir_x,&dir_y);
	obj->state  					= TRACKING_STATE_2D_LINE;
	obj->path_2D.point_x 	= point_x;
	obj->path_2D.point_y 	= point_y;
	obj->path_2D.dir_x 		= dir_x;
	obj->path_2D.dir_y 		= dir_y;
	obj->target_vel 			= vel;
	obj->path_pi.integral				= 0;
	obj->angle_pi.integral			= 0;
}

void Tracking_Start_2D_Arc(T_tracking* obj,float point_x, float point_y, float center_x, float center_y,float vel)
{
	obj->state 						= TRACKING_STATE_2D_ARC;
	obj->path_2D.point_x 	= point_x;
	obj->path_2D.point_y 	= point_y;
	obj->path_2D.center_x = center_x;
	obj->path_2D.center_y = center_y;
	obj->target_vel 			= vel;
	obj->path_pi.integral				= 0;
	obj->angle_pi.integral			= 0;
}

void Tracking_Start_2D_Point(T_tracking* obj,float point_x, float point_y,float vel)
{
	obj->state 						= TRACKING_STATE_2D_POINT;
	obj->path_2D.point_x 	= point_x;
	obj->path_2D.point_y 	= point_y;
	obj->target_vel				= vel;
	obj->path_pi.integral				= 0;
	obj->angle_pi.integral			= 0;
}

void Tracking_Start_Mag_Line(T_tracking* obj,float vel,T_tracking_mag_line_dir dir)
{
	obj->state											= TRACKING_STATE_SENSOR_MID;
	obj->sensor.mag_line.state 			= TRACKING_MAG_LINE_STATE_START;
	obj->target_vel 								= vel;
	obj->sensor.mag_line.dir 				= dir;
	obj->mag_gotoline_pi.integral		= 0;
	obj->mag_tracking_pi.integral		= 0;
}

void Tracking_Convert_3D(T_tracking* obj)
{
	
}

void Tracking_Run(T_tracking* obj)
{
	if(obj->state == TRACKING_STATE_2D_ANGLE)
	{
		Tracking_Run_2D_Angle(obj);
	}
	else if(obj->state == TRACKING_STATE_2D_LINE)
	{
		Tracking_Run_2D_Line(obj);
	}
	else if(obj->state == TRACKING_STATE_SENSOR_MID)
	{
		Tracking_Run_Sensor_Mid(obj);
	}
}

static float Tracking_2D_Angle(T_tracking* obj)
{
	T_tracking_rotate_type 			rot_dir;
	float												err;
	volatile float							cross_product;
	volatile float							dot_product;
	float												pi_out;
	float												angular_vel;
	
	/************************The Angle Part*****************************/
	cross_product = (obj->robot.dir_x*obj->path_2D.dir_y)-(obj->path_2D.dir_x*obj->robot.dir_y);
	dot_product = (obj->robot.dir_x*obj->path_2D.dir_x)+(obj->robot.dir_y*obj->path_2D.dir_y);
	
	/************ Anticlockwise positive; Clockwise negative ********************/
	rot_dir = (cross_product < 0)?TRACKING_ROTATE_CLOCKWISE:TRACKING_ROTATE_COUNTERCLOCKWISE;
	//err_angle = acosf(dot_product/sqrtf(obj->robot.dir_x*obj->robot.dir_x+obj->robot.dir_y*obj->robot.dir_y)/
	//															sqrtf(obj->path_2D.dir_x*obj->path_2D.dir_x+obj->path_2D.dir_y*obj->path_2D.dir_y));
	err = acosf(dot_product);  //acosf 0 ~ pi (?)
	
	if(err != 0.0f)
	{	
		pi_out = PI_Run(&(obj->angle_pi),0,err);
	}
	else
	{
		rot_dir = TRACKING_ROTATE_NONE;
		pi_out = 0;
	}
	
	if(rot_dir == TRACKING_ROTATE_COUNTERCLOCKWISE)
		angular_vel = -pi_out;
	else if(rot_dir == TRACKING_ROTATE_CLOCKWISE)
		angular_vel = pi_out;
	else
		angular_vel = 0;
	
	return angular_vel;
}

static float Tracking_2D_Line(T_tracking* obj)
{
	T_tracking_rotate_type 			rot_dir;
	float												err;
	float												distance;
	float												pi_out;
	float												angular_vel;
	
	/***************************The Line Part*************************************/
	distance = (obj->path_2D.point_x-obj->robot.pos_x)*(-obj->path_2D.dir_y)+(obj->path_2D.point_y-obj->robot.pos_y)*(obj->path_2D.dir_x);
	
	if(distance == 0)
	{
		rot_dir = TRACKING_ROTATE_NONE;
		distance = 0;
	}
	else if(distance < 0 && obj->robot.vel >= 0)
	{
		rot_dir = TRACKING_ROTATE_CLOCKWISE;
		distance = -distance; 
	}
	else if(distance > 0 && obj->robot.vel >= 0)
	{
		rot_dir = TRACKING_ROTATE_COUNTERCLOCKWISE;
		distance = distance; 
	}
	else if(distance < 0 && obj->robot.vel < 0)
	{
		rot_dir = TRACKING_ROTATE_COUNTERCLOCKWISE;
		distance = -distance; 
	}
	else if(distance > 0 && obj->robot.vel < 0)
	{
		rot_dir = TRACKING_ROTATE_CLOCKWISE;
		distance = distance; 
	}
	
	if(obj->robot.vel != 0)
	{
		err = atan2f(distance,obj->robot.vel);  //may try just using distance as error
	}
	else
	{
		err = 0;
	}
	
	pi_out = PI_Run(&(obj->path_pi),0,err);

	/************ Anticlockwise positive; Clockwise negative ********************/
	if(rot_dir == TRACKING_ROTATE_CLOCKWISE)
	{
		angular_vel = pi_out;
	}
	else if(rot_dir == TRACKING_ROTATE_COUNTERCLOCKWISE)
	{
		angular_vel = -pi_out;
	}
	else
	{
		angular_vel = 0;
	}
	return angular_vel;
}
static T_tracking_mag_line_orientation_type Tracking_Mag_Line_Get_Orientation(T_tracking* obj)
{		
	if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_MISSING 
		|| obj->sensor.mag_line.side_r == TRACKING_MAG_LINE_MISSING)
	{
		return TRACKING_MAG_LINE_ORIENTATION_NONE;
	}
	
	if(obj->sensor.mag_line.side_l == obj->sensor.mag_line.side_r)
	{
		if(obj->sensor.mag_line.value_l < obj->sensor.mag_line.value_r * (1.0f + MAG_LINE_FACING_MARGIN)
			&& obj->sensor.mag_line.value_r < obj->sensor.mag_line.value_l * (1.0f + MAG_LINE_FACING_MARGIN))
		{
			return TRACKING_MAG_LINE_ORIENTATION_NORMAL;
		}
		else
		{
			if(fabsf(obj->sensor.mag_line.value_l) > fabsf(obj->sensor.mag_line.value_r))
			{
				return TRACKING_MAG_LINE_ORIENTATION_LHRL;
			}
			else
			{
				return TRACKING_MAG_LINE_ORIENTATION_LLRH;
			}
		}
	}
	else
	{
		if(obj->sensor.mag_line.dir == TRACKING_MAG_LINE_DIRECT)
		{
			if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_INSIDE)
			{
				return TRACKING_MAG_LINE_ORIENTATION_ALIGNED;
			}
			else
			{
				return TRACKING_MAG_LINE_ORIENTATION_REVERSE;
			}
		}
		else
		{
			if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_INSIDE)
			{
				return TRACKING_MAG_LINE_ORIENTATION_REVERSE;
			}
			else
			{
				return TRACKING_MAG_LINE_ORIENTATION_ALIGNED;
			}
		}
	}
	return TRACKING_MAG_LINE_ORIENTATION_NONE;
}

static float Tracking_Mag_Line(T_tracking* obj)
{
	float 																	left_distance, right_distance;
	static 	T_tracking_rotate_type 					rot_dir;
	static	T_tracking_mag_line_side_type		prev_side_l, prev_side_r;				//1: inside    -1:outside   0:too far
	static float prev_rot_dir = 0;
	float 																	angular_vel = 0;
	T_tracking_mag_line_orientation_type 		orientation = 0;
	
	if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_MISSING || obj->sensor.mag_line.side_r == TRACKING_MAG_LINE_MISSING)
	{
		obj->error = TRACKING_ERROR_NO_MAG_LINE;
		return 0;
	}
		
	orientation = Tracking_Mag_Line_Get_Orientation(obj);
	
	if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_IDLE)
	{
		obj->angle_vel = 0;
		obj->line_vel = 0;
		return 0;
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_START)
	{
		if(orientation == TRACKING_MAG_LINE_ORIENTATION_NORMAL)
		{
			prev_side_l = obj->sensor.mag_line.side_l;
			prev_side_r = obj->sensor.mag_line.side_r;
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_GOTOLINE; //Maybe in the wrong direction which should be checked later
		}
		else if(orientation == TRACKING_MAG_LINE_ORIENTATION_LHRL 
					|| orientation == TRACKING_MAG_LINE_ORIENTATION_LLRH)
		{
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_FINDDIR;
			prev_rot_dir = TRACKING_ROTATE_NONE;
		}
		else if(orientation == TRACKING_MAG_LINE_ORIENTATION_ALIGNED)
		{
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_TRACELINE;
		}
		else if(orientation == TRACKING_MAG_LINE_ORIENTATION_REVERSE)
		{
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_FINDLINE;//May Search for ever, Can be merged with FINDDIR state
		}
		obj->angle_vel = 0;
		obj->line_vel = 0;
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_FINDDIR)
	{
		if(orientation == TRACKING_MAG_LINE_ORIENTATION_LHRL)
		{
			rot_dir = TRACKING_ROTATE_COUNTERCLOCKWISE;
			obj->angle_vel = obj->target_vel;
			obj->line_vel = 0;
		}
		else if(orientation == TRACKING_MAG_LINE_ORIENTATION_LLRH)
		{
			rot_dir = TRACKING_ROTATE_CLOCKWISE;
			obj->angle_vel = -obj->target_vel;
			obj->line_vel = 0;
		}
		else
		{
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_START;
			obj->angle_vel = 0;
			obj->line_vel = 0;
		}
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_GOTOLINE)
	{
		if(prev_side_l != obj->sensor.mag_line.side_l || prev_side_r != obj->sensor.mag_line.side_r 
			|| obj->sensor.mag_line.side_l != obj->sensor.mag_line.side_r)
		{
			if(orientation == TRACKING_MAG_LINE_ORIENTATION_ALIGNED)
			{
				obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_TRACELINE;
			}
			else
			{
				obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_FINDLINE;
			}
		}
		if(obj->sensor.mag_line.value_l > obj->sensor.mag_line.value_r)
		{
			obj->angle_vel = 0.1f * obj->target_vel;
		}
		else if(obj->sensor.mag_line.value_l < obj->sensor.mag_line.value_r)
		{
			obj->angle_vel = -0.1f * obj->target_vel;
		}
		obj->line_vel = obj->target_vel;
		
		prev_side_l = obj->sensor.mag_line.side_l;
		prev_side_r = obj->sensor.mag_line.side_r;		
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_FINDLINE)
	{	
		if(orientation == TRACKING_MAG_LINE_ORIENTATION_LHRL)
		{
			rot_dir = TRACKING_ROTATE_COUNTERCLOCKWISE;
			obj->angle_vel = obj->target_vel;
			obj->line_vel = 0;
		}
		else if(orientation == TRACKING_MAG_LINE_ORIENTATION_LLRH)
		{
			rot_dir = TRACKING_ROTATE_CLOCKWISE;
			obj->angle_vel = -obj->target_vel;
			obj->line_vel = 0;
		}
		else
		{
			obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_START;
			obj->angle_vel = 0;
			obj->line_vel = 0;
		}
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_TRACELINE)
	{
		float												err;
		float												pi_out;
		
		if(obj->sensor.mag_line.side_l == obj->sensor.mag_line.side_r)
		{
			if(obj->sensor.mag_line.dir == TRACKING_MAG_LINE_DIRECT)
			{
				if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_INSIDE)
				{
					obj->angle_vel = -obj->target_vel;
					obj->line_vel = obj->target_vel;
				}
				else if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_OUTSIDE)
				{
					obj->angle_vel = obj->target_vel;
					obj->line_vel = obj->target_vel;
				}
			}
			else
			{
				if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_INSIDE)
				{
					obj->angle_vel = obj->target_vel;
					obj->line_vel = obj->target_vel;					
				}
				else if(obj->sensor.mag_line.side_l == TRACKING_MAG_LINE_OUTSIDE)
				{
					obj->angle_vel = -obj->target_vel;
					obj->line_vel = obj->target_vel;
				}
			}
		}
		else
		{
			left_distance = sqrtf(MAG_LINE_MAX - obj->sensor.mag_line.value_l);
			right_distance = sqrtf(MAG_LINE_MAX - obj->sensor.mag_line.value_r);
			if(left_distance > right_distance)
			{
				rot_dir = TRACKING_ROTATE_CLOCKWISE;
				err = left_distance - right_distance;
			}
			else if(left_distance < right_distance)
			{
				rot_dir = TRACKING_ROTATE_COUNTERCLOCKWISE;
				err = right_distance - left_distance;
			}
			else
			{
				rot_dir = TRACKING_ROTATE_NONE;
				err = 0;
			}
			
			if(err != 0)
				pi_out = PI_Run(&(obj->mag_tracking_pi),0,err);
			
			if(rot_dir == TRACKING_ROTATE_CLOCKWISE)
			{
				//angular_vel = -pi_out;
				angular_vel = -obj->target_vel * 0.2f;
			}
			else if(rot_dir == TRACKING_ROTATE_COUNTERCLOCKWISE)
			{
				//angular_vel = pi_out;
				angular_vel = obj->target_vel * 0.2f;
			}
			else
			{
				angular_vel = 0;
			}
			
			if(angular_vel > obj->target_vel)
			{
				angular_vel = obj->target_vel;
			}
			else if(angular_vel < -obj->target_vel)
			{
				angular_vel = -obj->target_vel;
			}
			
			obj->angle_vel = angular_vel;
			obj->line_vel = obj->target_vel;
		}
	}
	else if(obj->sensor.mag_line.state == TRACKING_MAG_LINE_STATE_DONE)		
	{
		obj->sensor.mag_line.state = TRACKING_MAG_LINE_STATE_IDLE;
		obj->angle_vel = 0;
		obj->line_vel = 0;
	}

	return angular_vel;
}

void Tracking_Run_2D_Angle(T_tracking* obj)
{
	Tracking_Get_Position_2D(obj);
	obj->angle_vel = Tracking_2D_Angle(obj);
	obj->line_vel = obj->target_vel;
}

void Tracking_Run_2D_Line(T_tracking* obj)
{
	Tracking_Get_Position_2D(obj);
	obj->angle_vel = Tracking_2D_Angle(obj) + Tracking_2D_Line(obj);
	obj->line_vel = obj->target_vel;
}

void Tracking_Run_Sensor_Mid(T_tracking* obj)
{
	Tracking_Mag_Line(obj);  //angular and line vel will be auto assigned
}

void Tracking_Get_Sensor(T_tracking* obj)
{
	int32_t left 	= -leftsensor_data;
	int32_t right = -rightsensor_data;
	
	
	if(left < 0)
	{
		if(left > -MAG_LINE_MIN)
		{
			obj->sensor.mag_line.side_l = TRACKING_MAG_LINE_MISSING;
			obj->sensor.mag_line.value_l = 0;
		}
		else
		{
			obj->sensor.mag_line.side_l = TRACKING_MAG_LINE_OUTSIDE;
			obj->sensor.mag_line.value_l = -left;
		}
	}
	else
	{
		if(left < MAG_LINE_MIN)
		{
			obj->sensor.mag_line.side_l = TRACKING_MAG_LINE_MISSING;
			obj->sensor.mag_line.value_l = 0;
		}
		else
		{
			obj->sensor.mag_line.side_l = TRACKING_MAG_LINE_INSIDE;
			obj->sensor.mag_line.value_l = left;
		}
	}
	
	if(right <0)
	{
		if(right > -MAG_LINE_MIN)
		{
			obj->sensor.mag_line.side_r = TRACKING_MAG_LINE_MISSING;
			obj->sensor.mag_line.value_r = 0;
		}
		else
		{
			obj->sensor.mag_line.side_r = TRACKING_MAG_LINE_OUTSIDE;
			obj->sensor.mag_line.value_r = -right;
		}
	}
	else
	{
		if(right < MAG_LINE_MIN)
		{
			obj->sensor.mag_line.side_r = TRACKING_MAG_LINE_MISSING;
			obj->sensor.mag_line.value_r = 0;
		}
		else
		{
			obj->sensor.mag_line.side_r = TRACKING_MAG_LINE_INSIDE;
			obj->sensor.mag_line.value_r = right;
		}
	}
}

void Tracking_Get_Position_2D(T_tracking* obj)
{
	//extern float rot_vec[3];
	float x = rot_vec[0],y = rot_vec[1];
	Tracking_Norm_2D(&x,&y);
	obj->robot.dir_x = x;
	obj->robot.dir_y = y;
}

void Tracking_Update_Motor_Cmd(T_tracking*obj)
{
	//obj->line_vel = 200;
	//obj->angle_vel = 200;
	set_motor_control_speed_s32((int)(obj->line_vel+obj->angle_vel),(int)(obj->line_vel-obj->angle_vel));
	//set_motor_control_speed_s32(-3000,-3000);
}

float Tracking_Vect2Angle(float x, float y)
{
	float pi = 3.14159265;
	volatile float angle;
	if(x != 0)
	{
		angle = atan2f(y,x)/pi*180.0;
	}
	else
	{
		angle = 0;
	}
	return angle;
}
