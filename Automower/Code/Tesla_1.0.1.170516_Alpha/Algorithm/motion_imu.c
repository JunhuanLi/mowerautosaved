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
#include "motion_imu.h"
#include "PI.h"
#include "math.h"
#include "global.h"
#include "usart_driver.h"

/* macros *********************************************************************/
/* static variables ***********************************************************/
static float Tracking_2D_Angle(T_motion_tracker* obj);
static void Tracking_2D_Line(T_motion_tracker* obj);
/* funcitons ******************************************************************/
/*
void Tracking_Init(T_motion_tracker* obj,uint8_t en, T_motion_tracker_state_type state)
{
	obj->enable = en;
	obj->state = state;
	obj->error = TRACKING_ERROR_NONE;
	//obj->sensor.mag_polarity = 1;
	Tracking_Set_Path_Param(obj,0,0,0);
	Tracking_Set_Angle_Param(obj,0,0,0);
	Tracking_Set_Mag_Tracking_Param(obj,0,0,0);
	Tracking_Set_Mag_Gotoline_Param(obj,0,0,0);
	Tracking_Update_2D_Angle(obj,0,0,0);
}
*/

void Motion_Set_Path_Param(T_motion_tracker* obj,float advanc)
{
	obj->path_imu.advancing = advanc;
}

void Motion_Set_Angle_Param(T_motion_tracker* obj,float kp, float ki, float il)
{
	obj->path_imu.direction_pi.kp = kp;
	obj->path_imu.direction_pi.ki = ki;
	obj->path_imu.direction_pi.il = il;
	obj->path_imu.rotationFinished = FALSE;
	obj->path_imu.preDirStored = FALSE;
}



void Motion_Update_2D_Angle(T_motion_tracker* obj,float dir_x,float dir_y,float vel)
{
	Motion_Norm_2D(&dir_x,&dir_y);
	obj->tracking														= MOTION_TRACKING_2D_ANGLE;
	obj->path_imu.dir_x 										= dir_x;
	obj->path_imu.dir_y 										= dir_y;	
	obj->path_imu.dir_x_adj									= dir_x;
	obj->path_imu.dir_y_adj									= dir_y;
	obj->command_vel 												= vel;
	obj->target_vel													= vel;
	obj->path_imu.direction_pi.integral			= 0;
}

void Motion_Update_2D_Line(T_motion_tracker* obj,float point_x, float point_y, float dir_x, float dir_y, float vel)
{
	Motion_Norm_2D(&dir_x,&dir_y);
	obj->tracking  													= MOTION_TRACKING_2D_LINE;
	obj->path_imu.point_x 									= point_x;
	obj->path_imu.point_y 									= point_y;
	obj->path_imu.dir_x 										= dir_x;
	obj->path_imu.dir_y 										= dir_y;
	obj->path_imu.dir_x_adj									= dir_x;
	obj->path_imu.dir_y_adj									= dir_y;
	obj->command_vel												= vel;
	obj->target_vel 												= vel;//vel;bug 170914
	obj->path_imu.position_pi.integral			= 0;
	obj->path_imu.direction_pi.integral			= 0;
}

void Motion_Update_2D_Arc(T_motion_tracker* obj,float point_x, float point_y, float center_x, float center_y,float vel)
{
	obj->tracking 													= MOTION_TRACKING_2D_ARC;
	obj->path_imu.point_x 									= point_x;
	obj->path_imu.point_y 									= point_y;
	obj->path_imu.center_x 									= center_x;
	obj->path_imu.center_y	 								= center_y;
	obj->command_vel												= vel;
	obj->target_vel 												= vel;
	obj->path_imu.position_pi.integral			= 0;
	obj->path_imu.direction_pi.integral			= 0;
}

void Motion_Update_2D_Point(T_motion_tracker* obj,float point_x, float point_y,float vel)
{
	obj->tracking 													= MOTION_TRACKING_2D_POINT;
	obj->path_imu.point_x 									= point_x;
	obj->path_imu.point_y 									= point_y;
	obj->command_vel												= vel;
	obj->target_vel													= vel;
	obj->path_imu.position_pi.integral			= 0;
	obj->path_imu.direction_pi.integral			= 0;
}


void Tracking_Convert_3D(T_motion_tracker* obj)
{
	
}


void Motion_Run_Tracker(T_motion_tracker* obj)
{
	if(obj->tracking == MOTION_TRACKING_2D_ANGLE)
	{
		Motion_Run_2D_Angle(obj);
	}
	else if(obj->tracking == MOTION_TRACKING_2D_LINE)
	{
		Motion_Run_2D_Line(obj);
	}
}



static float Tracking_2D_Angle(T_motion_tracker* obj)
{
	T_motion_turn_type 									rot_dir;
	float																err;
	volatile float											cross_product;
	volatile float											dot_product;
	float																pi_out;
	float																angular_vel;
	
	/************************The Angle Part*****************************/
	cross_product = (obj->sense.dir_x*obj->path_imu.dir_y_adj)-(obj->path_imu.dir_x_adj*obj->sense.dir_y);
	dot_product = (obj->sense.dir_x*obj->path_imu.dir_x_adj)+(obj->sense.dir_y*obj->path_imu.dir_y_adj);
	
	/************ Anticlockwise positive; Clockwise negative ********************/
	rot_dir = (cross_product < 0)?MOTION_TURN_CLOCKWISE:MOTION_TURN_COUNTERCLOCKWISE;
	//err_angle = acosf(dot_product/sqrtf(obj->robot.dir_x*obj->robot.dir_x+obj->robot.dir_y*obj->robot.dir_y)/
	//															sqrtf(obj->path_imu.dir_x*obj->path_imu.dir_x+obj->path_imu.dir_y*obj->path_imu.dir_y));

	if(dot_product > 1.0f)		
		dot_product = 1.0f;

	err = acosf(dot_product);  //acosf 0 ~ pi (?)
	
	
	if(rot_dir == MOTION_TURN_COUNTERCLOCKWISE)
		err = -err;
	
	if(err != 0.0f)
	{	
		pi_out = PI_Run(&(obj->path_imu.direction_pi),0,err);
	}
	else
	{
		rot_dir = MOTION_TURN_NONE;
		pi_out = 0;
	}
	
	angular_vel = pi_out;
	
	return angular_vel;
}

static void Tracking_2D_Line(T_motion_tracker* obj)
{
	float												err_x,err_y;
	float												adv_x,adv_y;
	float												dir_x,dir_y;
	float												distance;
	
	/***************************The Line Part*************************************/
	distance = (obj->path_imu.point_x-obj->sense.pos_x)*(-obj->path_imu.dir_y)+(obj->path_imu.point_y-obj->sense.pos_y)*(obj->path_imu.dir_x);

	err_x = -distance * obj->path_imu.dir_y;
	err_y = distance * obj->path_imu.dir_x;
	adv_x = obj->path_imu.advancing * obj->path_imu.dir_x;
	adv_y = obj->path_imu.advancing * obj->path_imu.dir_y;
	
	dir_x = err_x + adv_x;
	dir_y = err_y + adv_y;
	
	Motion_Norm_2D(&dir_x,&dir_y);
		
	obj->path_imu.dir_x_adj = dir_x;
	obj->path_imu.dir_y_adj = dir_y;
}

void Motion_Run_2D_Angle(T_motion_tracker* obj)
{
	//Tracking_Get_Position_2D(obj);
	obj->angular_vel = Tracking_2D_Angle(obj);
	obj->line_vel = obj->target_vel;
}

void Motion_Run_2D_Line(T_motion_tracker* obj)
{
	//Tracking_Get_Position_2D(obj);
	Tracking_2D_Line(obj);
	obj->angular_vel = Tracking_2D_Angle(obj);
	
	obj->angular_vel = 0;
	obj->line_vel = obj->target_vel;
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

	float											pi_out=0;
	float 										angleRotated=0;
	float											dot_product=0;
	float											err=0;
void rotateAngle(T_motion_tracker* obj, float angle, T_motion_turn_type rot_dir) //angle is in degree
{

	if(obj->path_imu.preDirStored == FALSE)
	{
		obj->path_imu.pre_dir_x = obj->sense.dir_x;
		obj->path_imu.pre_dir_y = obj->sense.dir_y;
		obj->path_imu.preDirStored = TRUE;
	}
	
	dot_product = (obj->sense.dir_x*obj->path_imu.pre_dir_x)+(obj->sense.dir_y*obj->path_imu.pre_dir_y);
	
	if(dot_product > 1.0)
		dot_product = 1.0;
	
	angleRotated = acosf(dot_product)*57.3;
	err = angle - fabs(angleRotated);
	if(fabsf(err)<=1) //in degree
	{
		obj->path_imu.preDirStored = FALSE;
		obj->path_imu.direction_pi.integral = 0;
		obj->path_imu.rotationFinished = TRUE;
	}
	else
	{
		pi_out = PI_Run2(&(obj->path_imu.direction_pi),-err);
		obj->line_vel = 0;
		if(rot_dir == MOTION_TURN_CLOCKWISE)
		{
				obj->angular_vel = pi_out;
		}	
		else
		{
				obj->angular_vel = -pi_out;
		}	
		obj->path_imu.rotationFinished = FALSE;
	}
}

