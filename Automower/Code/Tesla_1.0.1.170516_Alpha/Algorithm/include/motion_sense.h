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
#ifndef __MOTION_SENSE_H__
#define __MOTION_SENSE_H__


#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"
#include "motion_math.h"
#include "motion_types.h"

void Motion_Get_Sensor(T_motion_sense* obj);
void Motion_Get_Position_2D(T_motion_sense* obj);
void Motion_Get_Position_3D(T_motion_sense* obj);

#endif
