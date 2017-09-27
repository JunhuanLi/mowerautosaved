/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: typedef.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: type def  
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"

/*****Macros**********/
#define PI             3.141592653589793
#define mag_cali_len   400
#define TRAP_TIME_TRESH     300           //¨º¨¹¨¤¡ì¨º¡À???D?¦Ì 300s
#define TRAP_POS_TRESH      3             //¨º¨¹¨¤¡ì?????D?¦Ì 3m
/******Structs*********/
typedef unsigned long long   u64;
typedef signed long long 	 s64;

#define GYRO_FACTOR		 	131
#define ACC_FACTOR          16384
#define GYRO_FACTOR_20602	16.4
#define ACC_FACTOR_20602    (16384/2)


typedef enum
{
	FALSE = 0,
	TRUE	= !FALSE
}T_bool;

/*************defined struct***************/
typedef struct{
	u16  year;
	u8   month;
	u8   day;
	u8   hour;
	u8   min;
	u8   sec;
}local_time;

typedef struct
{
	s16 left;
	s16 right;
}T_sonar;

typedef struct
{
	s16 mag_x;
	s16 mag_y;
	s16 mag_z;
	s16 temperature;
}T_mag;

typedef struct
{
	s32 gx;
	s32 gy;
	s32 gz;
	s32 ax;
	s32 ay;
	s32 az;
	s32 temp;
}T_mpu;

typedef struct
{
	int16_t gx;
	int16_t gy;
	int16_t gz;
	int16_t ax;
	int16_t ay;
	int16_t az;
	//s32 temp;
}T_mpu_20602;

typedef struct 
{
	u8 left;
	u8 right;
	u8 state;
}T_wheel_up;

typedef struct 
{
	u8 left;
	u8 right;
	u8 bumper_state;
}T_frontBumper;

typedef struct
{
	T_mag 			mag1,mag2;
	T_mpu			mpu;
	T_wheel_up	    wheel_up;
	T_frontBumper	bumper;
	T_sonar			sonar;
}T_sensor;

typedef struct{
	float gyro_rps[3];
	float acc_m_s2[3];
	float gyro_rps_20602[3];
	float acc_m_s2_20602[3];
	float imu_temp;
	float mag1[3];
	float mag2[3];
}imu_scaled_body;         //scale sensor info in body frame

typedef struct {
  float a;
  float b;
  float phi;
  float X0;
  float Y0;
  float X0_in;
  float Y0_in;
  float long_axis;
  float short_axis;
  float status;
}ellipse;   //magnetometer calibration ellipse



#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
