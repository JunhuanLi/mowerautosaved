/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: filter.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: filter
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _FILTER_H_
#define _FILTER_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
typedef struct
{
	s32 tap;
	s32 len;
	s64 sum;
	s32 *filter_arrary;
}T_moving_average_filter;

typedef struct
{
	s32 tap;
	s32 len;
	s32 *filter_arrary;
}T_median_filter;

typedef struct
{
	s32 len;
	s32 *filter_array;
	s32 *factor_array;
}T_fir_filter;

typedef struct
{
	s32 tap;
	s32 len;
	s32 threshold;
	s32 *data_array;
}T_step_seeker;


/* funcitons ******************************************************************/
s32 get_moving_average_filter_value(T_moving_average_filter *filter, s32 data);
s32 get_median_filter_value(T_median_filter *filter, s32 data);
s32 get_fir_filter_value(T_fir_filter *filter, s32 data);
T_bool is_step_signal(T_step_seeker *seeker, s32 data);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
