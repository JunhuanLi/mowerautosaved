/* Include Files */
#include "imu.h"

/* Function Definitions */

/*
 * Arguments    : float *P, float yaw_mag_prev, float yaw_mag, float R, float Q
 * Return Type  : float
 */
float att_mu_mag(float *P, float yaw_mag_prev, float yaw_mag, float R, float Q)
{
  float yaw_mag_updated;
  float K;

  /* 'mu_att_mag_ekf:3' H = 1; */
  /* 'mu_att_mag_ekf:5' K = P * H'/((H * P * H' + R )); */
  K = *P / (*P + R);

  /* state vector update */
  /* 'mu_att_mag_ekf:7' measurement = yaw_mag_def; */
  /* 'mu_att_mag_ekf:8' yaw_mag_def_updated = yaw_mag_def_prev + K * (measurement - H * yaw_mag_def_prev); */
  yaw_mag_updated = yaw_mag_prev + K * (yaw_mag - yaw_mag_prev);

  /* P matrix update */
  /* 'mu_att_mag_ekf:10' P = (1-K*H)*P + Q; */
  *P = (1.0F - K) * *P + Q;
  return yaw_mag_updated;
}