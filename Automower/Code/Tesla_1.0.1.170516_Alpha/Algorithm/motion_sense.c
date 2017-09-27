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
#include "motion_sense.h"
#include "global.h"
#include "usart_driver.h"
#include "motion_math.h"
#include "motion_types.h"

/* macros *********************************************************************/
/* static variables ***********************************************************/
static __inline void Tracking_Norm_2D(float* x, float* y)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
	}
}

void Motion_Get_Position_2D(T_motion_sense* obj)
{
	//extern float rot_vec[3];
	float x = rot_vec[0],y = rot_vec[1];
	Motion_Norm_2D(&x,&y);
	obj->dir_x = x;
	obj->dir_y = y;
	
	x = pos_ned_m[0];
	y = pos_ned_m[1];
	obj->pos_x = x;
	obj->pos_y = y;
}

void Motion_Get_Sensor(T_motion_sense* obj)
{
	int32_t left 	= leftsensor_data;
	int32_t right = rightsensor_data;
	
	
	if(left < 0)
	{
		if(left > -MAG_LINE_MIN)
		{
			obj->side_l = MOTION_MAG_LINE_MISSING;
			obj->value_l = 0;
		}
		else
		{
			obj->side_l = MOTION_MAG_LINE_OUTSIDE;
			obj->value_l = -left;
		}
	}
	else
	{
		if(left < MAG_LINE_MIN)
		{
			obj->side_l = MOTION_MAG_LINE_MISSING;
			obj->value_l = 0;
		}
		else
		{
			obj->side_l = MOTION_MAG_LINE_INSIDE;
			obj->value_l = left;
		}
	}
	
	if(right <0)
	{
		if(right > -MAG_LINE_MIN)
		{
			obj->side_r = MOTION_MAG_LINE_MISSING;
			obj->value_r = 0;
		}
		else
		{
			obj->side_r = MOTION_MAG_LINE_OUTSIDE;
			obj->value_r = -right;
		}
	}
	else
	{
		if(right < MAG_LINE_MIN)
		{
			obj->side_r = MOTION_MAG_LINE_MISSING;
			obj->value_r = 0;
		}
		else
		{
			obj->side_r = MOTION_MAG_LINE_INSIDE;
			obj->value_r = right;
		}
	}
}
