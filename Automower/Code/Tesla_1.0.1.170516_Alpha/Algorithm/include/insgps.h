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
#ifndef __INSGPS_H__
#define __INSGPS_H__

/* Include Files */
#include <math.h>
#include <stddef.h>    
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "global.h"


/*********struct************/
typedef struct
{
  float latitude_rad;				//纬度 [rad]
	float longitude_rad;			//经度 [rad]
	float hMSL_m;             //height above mean sea level [mm]
	float height_m;           //height above ellipsoid [mm]
	float hAcc_m;             //水平精度估计 [mm]
	float vAcc_m;             //垂直精度估计 [mm]
	float sAcc_m_s;           //速度精度估计 [mm/s]
	float velN_m_s;           //north velocity [mm/s]
	float velE_m_s;           //north velocity [mm/s]
	float velD_m_s;           //north velocity [mm/s]
	float heading_deg;        //heading of motion:[deg]
	float headAcc_deg;        //heading accuracy estimate [deg]
	float tAcc_ns;            //time accuracy of estimate UTC[ns]
	u16  year;
	u8   month;
	u8   day;
	u8   hour;
	u8   min;
	u8   sec;
	u8   ns;                 //fraction of seconde, -1e9-1e9 [ns]
	u8   valid;              //valid = 7可用
	u8   numSV;
}gpsx;

/*functions*/

void ekf7_tu(float acc[3], float pos_prev[3], float vel_prev[3],float quatl[4], float dt, float P[49], float Q[9], float pos[3], float vel[3], float stateVec[7]);
void ekf7_odom_mu(float pos_prev[3], float vel_prev[3], float *yaw,float stateVec[7], float delta_odo_r, float delta_odo_l,
                   float P[49], float R[9],float R_odom[9], float D, float pos[3], float vel[3]);
					   void ekf7_gps6_mu(float pos[3], float vel[3], float *yaw, float stateVec[7],float P[49], double pos_lla_ref[3], gpsx *gps);

//double magHeading2Geog(double points[3]);
u8 get_gps_data(u8 *init_flag);
void odom_preprocessing(float *odom_valid,float *delta_odo_l,float *delta_odo_r);
void send_data(float g_timediff_s,float delta_odo_l,float delta_odo_r,gpsx gps_data);
extern float delta_odo_l,delta_odo_r;

#endif
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

