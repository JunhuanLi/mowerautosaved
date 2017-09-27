/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	ist8310.h
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/13
Description		:	ist8310 header 
*******************************************************************************/

#ifndef _IST_8310_H_
#define _IST_8310_H_

/* includes *******************************************************************/
//#include "stm32f4xx.h"
#include "typedef.h"
#include "ist8310.h"

/* funcitons ******************************************************************/
T_bool mag_init(void);
void get_mag_info1(T_mag *mag);


#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
