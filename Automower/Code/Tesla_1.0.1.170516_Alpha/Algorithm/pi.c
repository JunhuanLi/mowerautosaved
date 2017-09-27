/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: pi.h
  Author		: George 	
  Version		: V1.0.0	  
  Date			: 2017/07/21
  Description	: pi 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

/* includes *******************************************************************/
#include "pi.h"
/* macros *********************************************************************/

/* static variables ***********************************************************/

/* funcitons ******************************************************************/
void PI_Init(T_pi* pi,float kp, float ki,float il)
{
	pi->kp = kp;
	pi->ki = ki;
	pi->il = il;
	pi->fb = 0.0f;
	pi->ref = 0.0f;
	pi->err = 0.0f;
	pi->out = 0.0f;
	pi->integral = 0.0f;
}

