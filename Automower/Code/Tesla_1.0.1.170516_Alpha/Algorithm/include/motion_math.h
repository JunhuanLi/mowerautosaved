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

#ifndef __MOTION_CONTROL_H__
#define __MOTION_CONTROL_H__

#include "stm32f4xx.h"
#include "typedef.h"
#include "pi.h"
#include "math.h"
//#include "motion_types.h"

extern __inline void Motion_Norm_2D(float* x, float* y)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
	}
}
	
extern __inline void Motion_Norm_3D(float* x, float* y, float* z)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y) +(*z)*(*z));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
		*z = *z / length;
	}
}

extern __inline int Motion_Abs(int x){return x<0?-x:x;}

#endif
