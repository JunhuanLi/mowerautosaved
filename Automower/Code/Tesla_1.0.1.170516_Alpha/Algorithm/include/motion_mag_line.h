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
#ifndef __MOTION_MAG_LINE__
#define __MOTION_MAG_LINE__

#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"
#include "motion_math.h"
#include "motion_types.h"

void Motion_Run_Mag_Line(T_motion_tracker* obj);

void Motion_Set_Mag_Tracking_Param(T_motion_tracker* obj,float kp, float ki, float il);
void Motion_Set_Mag_Gotoline_Param(T_motion_tracker* obj,float kp, float ki, float il);

void Motion_Start_Mag_Line(T_motion_tracker* obj,float vel,T_motion_mag_line_dir_type dir);

#endif
