/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: mpu6500_spi.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: mpu6500 spi driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/


#ifndef _MATH_CUSTOM_H_
#define _MATH_CUSTOM_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"



/* macros *********************************************************************/

/* funcitons ******************************************************************/
s32 get_limited_data(s32 data, s32 threshold1, s32 threshold2);
s32 get_bound_data(s32 data, s32 threshold);
s32 custom_abs(s32 data);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
