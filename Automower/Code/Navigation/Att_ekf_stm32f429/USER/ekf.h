/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include "delay.h" 
#include "usart.h" 
#include "sdram.h"   
#include "usmart.h" 
#include "mpu9250.h"
#include "inv_mpu_dmp_motion_driver.h" 

void quat2eulf(float q[4], float eul[3]);
float hampel11(float x[11]);

void mu_att_ekf(float P[16], float acc_imu_g[3], float stateVec[4], float R[9]);
void tu_att_ekf(float P[16], float stateVec[4], float angular_imu_rad_s[3],float dt, float Q[9]);

void ekf7_tu( float acc_m_s2[3], float pos[3], float vel[3],  float quat[4], float dt, float P[49],  float Q[9], float stateVec[7]);
void ekf7_odom_mu(float pos[3], float vel[3], float *yaw, float stateVec[7], float delta_odo_r, float delta_odo_l, float P[49], 
										float R[9], float D);
void ekf7_gps6_mu(float pos[3], float vel[3], float *yaw, double gps_pos_lla[3], float gps_vel_ned[3], float stateVec[7],
                  float P[49], double pos_lla_ref[3], float gps_hAcc_m, float gps_vAcc_m, float gps_sAcc_m_s);
//float grav_acc(float lat_rad);
double magHeading2Geog(double points[3]);

