/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	broder_sensor.h
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/13
Description		:	broder_sensor header 
*******************************************************************************/

#ifndef _RAIN_SENSOR_H_
#define _RAIN_SENSOR_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/

/* funcitons ******************************************************************/
void broder_sensor_Init(void);
void reset_rain_sensor(void);
void get_rain_sensor_info(T_bool *rain_sensor);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
