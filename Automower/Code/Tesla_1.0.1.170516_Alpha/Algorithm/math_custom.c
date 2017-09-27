/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: math_custom.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: math_custom driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/


/* includes *******************************************************************/
#include "math_custom.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/

/* funcitons ******************************************************************/

/*******************************************************************************
  Function		: get_limited_data      
  Description	: get limited data
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_limited_data(s32 data, s32 threshold1, s32 threshold2)
{
	if(data > threshold1)
	{
		return threshold1;
	}
	else if(data < threshold2)
	{
		return threshold2;
	}
	else
	{
		return data;
	}
}
/*******************************************************************************
  Function		: get_bound_data      
  Description	: get bound data
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_bound_data(s32 data, s32 threshold)
{
	if(data > threshold)
	{
		return threshold;
	}
	else if(data < -threshold)
	{
		return -threshold;
	}
	else
	{
		return data;
	}
}

/*******************************************************************************
  Function		: custom_abs      
  Description	: custom abs
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 custom_abs(s32 data)
{
	return (data > 0? data : -data);
}


/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
