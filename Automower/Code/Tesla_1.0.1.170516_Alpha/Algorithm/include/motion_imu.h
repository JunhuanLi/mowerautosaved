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

#ifndef __MOTION_IMU_H__
#define __MOTION_IMU_H__


/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"
#include "motion_math.h"
#include "motion_types.h"
#include "global_planner.h"
/* macros *********************************************************************/

/* static variables ***********************************************************/

//void Tracking_Init(T_motion_tracker* obj,uint8_t en, T_motion_tracking_type tracking);

void Motion_Run_Tracker(T_motion_tracker* obj);
void Motion_Run_2D_Angle(T_motion_tracker* obj);
void Motion_Run_2D_Line(T_motion_tracker* obj);
void Motion_Run_3D_Angle(T_motion_tracker* obj);
void Motion_Run_3D_Line(T_motion_tracker* obj);

void Motion_Targert_Convert_3D(T_motion_tracker* obj);
extern __inline uint8_t Motion_find_sector_2D(float x, float y){return (x<0)?((y<0)?3:4):((y>0)?1:2);}

void Motion_Set_Path_Param(T_motion_tracker* obj,float advanc);
void Motion_Set_cornerAngle_Param(T_motion_tracker* obj,float kp, float ki, float il);
void Motion_Set_lineAngle_Param(T_motion_tracker* obj,float kp, float ki, float il);

void Motion_Update_2D_Angle(T_motion_tracker* obj,float dir_x,float dir_y,float vel);
void Motion_Update_2D_Line(T_motion_tracker* obj,float point_x, float point_y, float dir_x, float dir_y, float vel);
void Motion_Update_2D_Arc(T_motion_tracker* obj,float point_x, float point_y, float center_x, float center_y,float vel);
void Motion_Update_2D_Point(T_motion_tracker* obj,float point_x, float point_y,float vel);


float Motion_Vect2Angle(float x, float y);
void rotateAngle(T_motion_tracker* obj, float angle, T_side rot_dir);
void rotateVector(T_motion_tracker* obj, float vec_x, float vec_y, T_side rot_dir);

void trackPoint(T_motion_tracker* obj, float target_x, float target_y);
void trackVector(T_motion_tracker* obj, float vec_x, float vec_y);
void setVelocity(T_motion_tracker* obj, float linear_vel, float angular_vel);
#endif
