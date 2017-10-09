/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			userdef.h
  Author:			  	Rui.fang
  Date:			    	2017.6.8
  Version:        
  Description:		�������Զ���ĺ�  ���ļ���STM32F4XX.H����� 

  					һЩȫ�ֱ���Ҳ���Է�������ļ���������̶����Ե���
  						
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/* includes ************************************/
#include "stm32f4xx.h"

/************Global Variables***********/
extern float gyro_bias[3];
extern float acc_bias_g[3]; 
extern float gyro_bias_20602[3];
extern float acc_bias_g_20602[3]; 
extern float g_timediff_s_imu;
//extern float g_timediff_s_gps;
extern float eul_rad[3];
extern float rot_vec[3];
extern local_time ltime;
extern float quat[4];
extern u8 trapped_flag;
extern float pos_ned_m[3];
extern float vel_ned_m_s[3];
extern ellipse mag_cali_ellipse;
extern ellipse mag_cali_ellipse2;
extern float mag_declination_rad;
extern double pos_lla_init[3];        
extern float declination_rad;       
extern u8 is_imu_valid;
extern u8 is_odo_valid;
extern int is_att_valid;
extern int is_odo_ready;
/*****functions*******/
void  quat2eulf(float q[4], float eul[3]);
void  eul2quatf(float eul[3], float q[4]);
float hampel(float x[11]);
void  get_local_time(local_time *time);   //get local time
void  get_pos_ned(float pos[3]);          //get 3D positionin in ned frame
void  get_vel_ned(float vel[3]);          //get 3D velocity in ned frame
void  get_rot_vector(float vector[3]);    //get rotation vector
void  get_eul_rad(float angle[3]);        //get euler angle defined by the order of yaw-pitch-roll
void  rot_update(float quatl[4],float rotVec[3]);

#endif

