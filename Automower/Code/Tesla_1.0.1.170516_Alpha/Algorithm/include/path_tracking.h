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

#ifndef __PATH_TRACKING_H__
#define __PATH_TRACKING_H__

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
typedef enum
{
	TRACKING_STATE_2D_ANGLE = 0,
	TRACKING_STATE_2D_LINE,
	TRACKING_STATE_2D_ARC,
	TRACKING_STATE_2D_POINT,
	TRACKING_STATE_3D_ANGLE,
	TRACKING_STATE_3D_LINE,
	TRACKING_STATE_3D_ARC,
	TRACKING_STATE_3D_POINT,
	TRACKING_STATE_SENSOR_MID,
	TRACKING_STATE_SENSOR_LEFT,
	TRACKING_STATE_SENSOR_RIGHT
}T_tracking_state_type;

typedef enum
{
	TRACKING_ERROR_NONE = 0,
	TRACKING_ERROR_NO_MAG_LINE,				//The Mag Line is not found
}T_tracking_error_type;

typedef enum
{
	TRACKING_ROTATE_NONE = 0,
	TRACKING_ROTATE_CLOCKWISE,
	TRACKING_ROTATE_COUNTERCLOCKWISE
}T_tracking_rotate_type;

typedef enum
{
	TRACKING_MAG_LINE_MISSING = 0,
	TRACKING_MAG_LINE_INSIDE,
	TRACKING_MAG_LINE_OUTSIDE
}T_tracking_mag_line_side_type;

typedef enum
{
	TRACKING_MAG_LINE_DIRECT = 0,			//counter clockwise
	TRACKING_MAG_LINE_REVERSE					//clockwise
}T_tracking_mag_line_dir;

typedef enum
{
	TRACKING_MAG_LINE_STATE_IDLE = 0,
	TRACKING_MAG_LINE_STATE_START,
	TRACKING_MAG_LINE_STATE_FINDDIR,   //If the robot direction is not correct
	TRACKING_MAG_LINE_STATE_GOTOLINE,
	TRACKING_MAG_LINE_STATE_FINDLINE,
	TRACKING_MAG_LINE_STATE_TRACELINE,
	TRACKING_MAG_LINE_STATE_DONE
}T_tracking_mag_line_state_type;

typedef enum
{
	TRACKING_MAG_LINE_ORIENTATION_NONE = 0,
	TRACKING_MAG_LINE_ORIENTATION_LHRL,			//Left High Right Low
	TRACKING_MAG_LINE_ORIENTATION_LLRH,			//Left Low Right High
	TRACKING_MAG_LINE_ORIENTATION_NORMAL,
	TRACKING_MAG_LINE_ORIENTATION_REVERSE,
	TRACKING_MAG_LINE_ORIENTATION_ALIGNED
}T_tracking_mag_line_orientation_type;

typedef struct
{
	float pos_x;
	float pos_y;
	float pos_z;
	float dir_x;
	float dir_y;
	float dir_z;
	float vel;
}T_tracking_robot;

typedef struct
{
	//int8_t												mag_polarity;				//-1: inside positive outside negetive   1: inside negetive outside positive
	T_tracking_mag_line_side_type		side_l;							//1: inside    -1:outside   0:too far
	T_tracking_mag_line_side_type		side_r;
	float 													value_l;						// the larger the closer
	float 													value_r;
	T_tracking_mag_line_dir					dir;								// direct = counter-clockwise reverse = clockwise
	T_tracking_mag_line_state_type	state;
}T_tracking_mag_line;

typedef struct
{
	T_tracking_mag_line							mag_line;
	float 													sonar_l;
	float 													sonar_r;
	uint8_t 												bump_l;
	uint8_t 												bump_r;
	uint8_t													lift_l;
	uint8_t													lift_r;
}T_tracking_sensor;

typedef struct			//for 2D tracking and target input
{
	float point_x;		//used for line, arc and point
	float point_y;		//used for line, arc and point
	float dir_x;			//used for line
	float dir_y;			//used for line
	float center_x;		//used for arc
	float center_y;		//used for arc
	float vel;
}T_tracking_path_2D;

typedef struct			//for 3D tracking
{
	float point_x;
	float point_y;
	float point_z;
	float dir_x;
	float dir_y;
	float dir_z;
	float center_x;
	float center_y;
	float center_z;
	float vel;
}T_tracking_path_3D;

typedef struct
{
	uint8_t											enable;
	T_tracking_error_type				error;
	T_tracking_state_type 			state;
	T_tracking_path_2D					path_2D;
	T_tracking_path_3D					path_3D;
	T_tracking_robot						robot;
	T_tracking_sensor						sensor;
	T_pi												path_pi;
	T_pi												angle_pi;
	T_pi												mag_gotoline_pi;
	T_pi												mag_tracking_pi;
	
	float												target_vel;
	float												line_vel;
	float												angle_vel;
	
}T_tracking;
/* funcitons ******************************************************************/

void Tracking_Init(T_tracking* obj,uint8_t en, T_tracking_state_type state);

void Tracking_Run(T_tracking* obj);
void Tracking_Run_2D_Angle(T_tracking* obj);
void Tracking_Run_2D_Line(T_tracking* obj);
void Tracking_Run_3D_Angle(T_tracking* obj);
void Tracking_Run_3D_Line(T_tracking* obj);
void Tracking_Run_Sensor_Mid(T_tracking* obj);
void Tracking_Run_Sensor_Left(T_tracking* obj);
void Tracking_Run_Sensor_Right(T_tracking* obj);

extern __inline void Tracking_enable(T_tracking* obj){obj->enable = 1;}
extern __inline void Tracking_disable(T_tracking* obj){obj->enable = 0;}

void Tracking_Targert_Convert_3D(T_tracking* obj);
extern __inline uint8_t Tracking_find_sector_2D(float x, float y){return (x<0)?((y<0)?3:4):((y>0)?1:2);}

void Tracking_Set_Path_Param(T_tracking* obj,float kp, float ki, float il);
void Tracking_Set_Angle_Param(T_tracking* obj,float kp, float ki, float il);
void Tracking_Set_Mag_Tracking_Param(T_tracking* obj,float kp, float ki, float il);
void Tracking_Set_Mag_Gotoline_Param(T_tracking* obj,float kp, float ki, float il);

void Tracking_Start_2D_Angle(T_tracking* obj,float dir_x,float dir_y,float vel);
void Tracking_Start_2D_Line(T_tracking* obj,float point_x, float point_y, float dir_x, float dir_y, float vel);
void Tracking_Start_2D_Arc(T_tracking* obj,float point_x, float point_y, float center_x, float center_y,float vel);
void Tracking_Start_2D_Point(T_tracking* obj,float point_x, float point_y,float vel);
void Tracking_Start_Mag_Line(T_tracking* obj,float vel,T_tracking_mag_line_dir dir);


void Tracking_Get_Sensor(T_tracking* obj);
void Tracking_Get_Position_2D(T_tracking* obj);
//void Tracking_Set_Robot(T_tracking* obj,float pos_x,float pos_y,float pos_z,float dir_x,float dir_y,float dir_z,float sensor_l,float sensor_r,float vel);

void Tracking_Update_Motor_Cmd(T_tracking*obj);

float Tracking_Vect2Angle(float x, float y);
#endif /* __PATH_TRACKING_H__ */
