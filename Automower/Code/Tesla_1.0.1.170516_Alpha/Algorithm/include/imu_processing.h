/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: imu_processing.h
  Author		: 	
  Version		: V1.0.0	  
  Date			: 2017/05/27
  Description	: ekf.h 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/
#ifndef __IMU_PROCESSING_H__
#define __IMU_PROCESSING_H__


//include files
#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include <stddef.h>    
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "mower_common.h"
#include "global.h"
#include "math.h"
#include "hardware.h"
#include <stdlib.h>
#include "movvarf.h"

//constants
#define FILTER_LEN 11

//functions
float grav_acc(float lat_rad);
void quat_norm(float quatl[4]);
void  get_rot_vector(float vector[3]);               //get rotation vector
void  get_eul_rad(float angle[3]);
void mpu_preprocessing(float grav,imu_scaled_body *imu,float *yaw1_after_cali,float *yaw2_after_cali);
void frame_alignment(imu_scaled_body *imu);
void hampel_imu(imu_scaled_body *imu);
void rot_update(float quatl[4],float rotVec[3]);
void rot_ele_update(float quat_in[4],float eul_in[3],volatile float delta_yaw);
void update_yaw1_mag(float yaw,float *yaw1_mag);
void yaw_init(s8 *flag,float *yaw1_prev,float *yaw2_prev, float yaw1,float yaw2);
void filter_converge(float trace_P,float *delta_yaw,float eul_temp[3]);
u8 odo_validation(void);
u8 imu_validation(void);
#endif
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
