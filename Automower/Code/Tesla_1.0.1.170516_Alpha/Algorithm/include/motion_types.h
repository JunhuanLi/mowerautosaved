/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: path_tracking.h
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

#ifndef __MOTION_TYPES_H__
#define __MOTION_TYPES_H__

#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"

#define MAG_LINE_POLARITY
#define MAG_LINE_MAX						52000.0f
#define MAG_LINE_MIN						50.0f
#define MAG_LINE_FACING_MARGIN	0.2f
#define MOTION_ACC							0.18  //3000 /s


typedef enum
{
	MOTION_TRACKING_2D_ANGLE = 0,
	MOTION_TRACKING_2D_LINE,
	MOTION_TRACKING_2D_ARC,
	MOTION_TRACKING_2D_POINT,
	MOTION_TRACKING_3D_ANGLE,
	MOTION_TRACKING_3D_LINE,
	MOTION_TRACKING_3D_ARC,
	MOTION_TRACKING_3D_POINT,
	MOTION_TRACKING_MAG_LINE
}T_motion_tracking_type;

typedef enum
{
	MOTION_ERROR_NONE = 0,
	MOTION_ERROR_NO_MAG_LINE,				//The Mag Line is not found
}T_motion_err_type;

typedef enum
{
	MOTION_TURN_NONE = 0,
	MOTION_TURN_CLOCKWISE,
	MOTION_TURN_COUNTERCLOCKWISE
}T_motion_turn_type;

typedef enum
{
	MOTION_MAG_LINE_MISSING = 0,
	MOTION_MAG_LINE_INSIDE,
	MOTION_MAG_LINE_OUTSIDE
}T_motion_mag_line_side_type;

typedef enum
{
	MOTION_MAG_LINE_DIRECT = 0,			//counter clockwise
	MOTION_MAG_LINE_REVERSE					//clockwise
}T_motion_mag_line_dir_type;

typedef enum
{
	MOTION_MAG_LINE_STATE_IDLE = 0,
	MOTION_MAG_LINE_STATE_START,
	MOTION_MAG_LINE_STATE_FINDDIR,   //If the robot direction is not correct
	MOTION_MAG_LINE_STATE_GOTOLINE,
	MOTION_MAG_LINE_STATE_ALIGNING,
	MOTION_MAG_LINE_STATE_TRACELINE,
	MOTION_MAG_LINE_STATE_DONE
}T_motion_mag_line_state_type;

typedef enum
{
	MOTION_MAG_LINE_ORI_MISSING = 0,
	MOTION_MAG_LINE_ORI_IN_LHRL,			//Left High Right Low inside 
	MOTION_MAG_LINE_ORI_IN_LLRH,			//Left Low Right High inside
	MOTION_MAG_LINE_ORI_IN_NORMAL,		//´¹Ö±                inside 
	MOTION_MAG_LINE_ORI_OUT_LHRL,			//Left High Right Low outside
	MOTION_MAG_LINE_ORI_OUT_LLRH,			//Left Low Right High outside
	MOTION_MAG_LINE_ORI_OUT_NORMAL,		//´¹Ö±	              outside
	MOTION_MAG_LINE_ORI_REVERSE, 
	MOTION_MAG_LINE_ORI_ALIGNED,
	
	WIRE_MISSING,
	NOT_ON_WIRE,
	ON_WIRE
}T_motion_mag_line_orientation_type;

typedef struct			//for path tracking all position is in unit of meter 
{
	float 					point_x;		//used for line, arc and point
	float 					point_y;		//used for line, arc and point
	float 					point_z;		//used for line, arc and point
	float 					dir_x;			//used for line
	float 					dir_y;			//used for line
	float 					dir_z;			//used for line
	float 					center_x;		//used for arc
	float 					center_y;		//used for arc
	float 					center_z;		//used for arc
	
	float						advancing;	//Advancing (in meters) for path tracking
	float						dir_x_adj;	//The adjusted direction(after position control)
	float						dir_y_adj;
	float 					dir_z_adj;
	
	T_pi						cornerAngle_pi;
	T_pi						lineAngle_pi;
	
	float 					pre_dir_x;
	float 					pre_dir_y;
	T_bool 					preDirStored;
	T_bool					rotationFinished;
	T_bool					pointReached;
}T_motion_path_imu;

typedef struct
{
	T_motion_mag_line_dir_type			wire_dir;								// direct = counter-clockwise reverse = clockwise
	T_motion_mag_line_state_type		state;
	T_motion_turn_type							turn_dir;
	T_motion_mag_line_side_type     pre_wheelside;
	float                           pre_magvalue;
	
	T_pi														mag_gotoline_pi;
	T_pi														mag_tracking_pi;
}T_motion_path_mag_line;

typedef struct
{
	//Sensed robot position
	float pos_x;
	float pos_y;
	float pos_z;
	
	//Sensed robot orientation
	float dir_x;
	float dir_y;
	float dir_z;
	
	//Sensed robot velocity
	float vel;
	
	//Mag line sensor
	T_motion_mag_line_side_type			side_l;							//1: inside    -1:outside   0:too far
	T_motion_mag_line_side_type			side_r;
	float 													value_l;						// the larger the closer
	float 													value_r;
	
	//Ultrasonic
	float 													sonar_l;
	float 													sonar_r;
	
	//Bump
	uint8_t 												bump_l;
	uint8_t 												bump_r;
	
	//Lift
	uint8_t													lift_l;
	uint8_t													lift_r;
}T_motion_sense;

typedef struct
{
	T_motion_err_type							error;
	T_motion_tracking_type 				tracking;
	T_motion_path_imu							path_imu;
	T_motion_path_mag_line				path_mag_line;
	T_motion_sense								sense;		
	
	float													acc;							//Acceleration (a parameter) for the wheel
	float													omega_leftWheel;						//The current velocity for wheel l
	float													omega_rightWheel;						//The current velocity for wheel r
	float													command_vel;			//Velocity Command
	float													target_vel;				//target velocity 
	float													line_vel;					//The velocity(linear) considering acceleration and obstacle
	float													angular_vel;
}T_motion_tracker;

#endif
