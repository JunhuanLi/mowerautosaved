/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: ekf.h
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
#ifndef __IMU_H__
#define __IMU_H__

/* Include Files */
#include <stddef.h>    
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "global.h"
#include "math.h"
#include "normK.h"
#include "imu_processing.h"


/*********struct************/

/*functions*/
void  quat2eulf(float q[4], float eul[3]);
float hampel(float x[11]);
float att_mu_acc(float P[16],  imu_scaled_body *imu, float stateVec[4], float R[9]);
void  att_tu(float P[16], float quatl[4], imu_scaled_body *imu,float dt, float Q[9]);
//float att_mu_acc_20602(float P[16],imu_scaled_body *imu, float quatl[4],float R[9]);
void  att_tu_20602(float P[16], float quatl[4], imu_scaled_body *imu,float dt, float Q[9]);
float att_mu_mag(float *P, float yaw_mag_prev, float yaw_mag, float R, float Q);
void  send_data_imu(float g_timediff_s,imu_scaled_body *imu);

//double magHeading2Geog(double points[3]);


#endif
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

