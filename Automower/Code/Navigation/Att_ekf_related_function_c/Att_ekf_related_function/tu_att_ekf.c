/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Function Definitions */
/*
 * 1st-order approximation
 * Arguments    : double P[16]
 *                const double stateVec[4]
 *                const double angular_imu_rad_s[3]
 *                double dt
 *                const double Q[9]
 *                double P_updated[16]
 *                double stateVec_updated[4]
 * Return Type  : void
 */
void tu_att_ekf(double P[16], double stateVec[4], double
                angular_imu_rad_s[3], double dt, double Q[9], double
                P_updated[16], double stateVec_updated[4])
{
  signed char I[16];
  int k;
  double F_discrete[16];
  double b_F_discrete[16];
  int i0;
  double a;
  double b_stateVec[12];
  double G[12];
  double c_F_discrete[16];
  double b_G[16];
  int i1;
  int i;

  /* 'tu_att_ekf:5' phi = [0 -angular_imu_rad_s(1)*dt -angular_imu_rad_s(2)*dt    -angular_imu_rad_s(3)*dt;... */
  /* 'tu_att_ekf:6'        angular_imu_rad_s(1)*dt 0 angular_imu_rad_s(3)*dt      -angular_imu_rad_s(2)*dt;... */
  /* 'tu_att_ekf:7'        angular_imu_rad_s(2)*dt   -angular_imu_rad_s(3)*dt  0   angular_imu_rad_s(1)*dt;... */
  /* 'tu_att_ekf:8'        angular_imu_rad_s(3)*dt   angular_imu_rad_s(2)*dt      -angular_imu_rad_s(1)*dt 0]; */
  /* discrete transition matrix */
  /* 'tu_att_ekf:10' F_discrete = (0.5 * phi + eye(length(stateVec))); */
  for (k = 0; k < 16; k++) {
    I[k] = 0;
  }

  for (k = 0; k < 4; k++) {
    I[k + (k << 2)] = 1;
  }

  F_discrete[0] = 0.0;
  F_discrete[4] = -angular_imu_rad_s[0] * dt;
  F_discrete[8] = -angular_imu_rad_s[1] * dt;
  F_discrete[12] = -angular_imu_rad_s[2] * dt;
  F_discrete[1] = angular_imu_rad_s[0] * dt;
  F_discrete[5] = 0.0;
  F_discrete[9] = angular_imu_rad_s[2] * dt;
  F_discrete[13] = -angular_imu_rad_s[1] * dt;
  F_discrete[2] = angular_imu_rad_s[1] * dt;
  F_discrete[6] = -angular_imu_rad_s[2] * dt;
  F_discrete[10] = 0.0;
  F_discrete[14] = angular_imu_rad_s[0] * dt;
  F_discrete[3] = angular_imu_rad_s[2] * dt;
  F_discrete[7] = angular_imu_rad_s[1] * dt;
  F_discrete[11] = -angular_imu_rad_s[0] * dt;
  F_discrete[15] = 0.0;
  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      b_F_discrete[i0 + (k << 2)] = 0.5 * F_discrete[i0 + (k << 2)] + (double)
        I[i0 + (k << 2)];
    }
  }

  /* noise transition matrix */
  /* 'tu_att_ekf:12' G = 0.5*dt*[-stateVec(2) -stateVec(3) -stateVec(4);... */
  /* 'tu_att_ekf:13'              stateVec(1) -stateVec(4) stateVec(3);... */
  /* 'tu_att_ekf:14'             -stateVec(4) stateVec(1) -stateVec(2);... */
  /* 'tu_att_ekf:15'             -stateVec(3) stateVec(2) stateVec(1)]; */
  a = 0.5 * dt;
  b_stateVec[0] = -stateVec[1];
  b_stateVec[4] = -stateVec[2];
  b_stateVec[8] = -stateVec[3];
  b_stateVec[1] = stateVec[0];
  b_stateVec[5] = -stateVec[3];
  b_stateVec[9] = stateVec[2];
  b_stateVec[2] = -stateVec[3];
  b_stateVec[6] = stateVec[0];
  b_stateVec[10] = -stateVec[1];
  b_stateVec[3] = -stateVec[2];
  b_stateVec[7] = stateVec[1];
  b_stateVec[11] = stateVec[0];
  for (k = 0; k < 3; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      G[i0 + (k << 2)] = a * b_stateVec[i0 + (k << 2)];
    }
  }

  /* discrete system noise variance    */
  /* 'tu_att_ekf:17' Q_discrete = G * Q * G'; */
  /* time update(attitude propagation) */
  /* 'tu_att_ekf:19' stateVec_updated = F_discrete * stateVec; */
  /* P matrix calculation */
  /* 'tu_att_ekf:21' P = F_discrete * P *(F_discrete') + Q_discrete; */
  for (k = 0; k < 4; k++) {
    stateVec_updated[k] = 0.0;
    for (i0 = 0; i0 < 4; i0++) {
      F_discrete[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        F_discrete[k + (i0 << 2)] += b_F_discrete[k + (i1 << 2)] * P[i1 + (i0 <<
          2)];
      }

      stateVec_updated[k] += b_F_discrete[k + (i0 << 2)] * stateVec[i0];
    }

    for (i0 = 0; i0 < 3; i0++) {
      b_stateVec[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        b_stateVec[k + (i0 << 2)] += G[k + (i1 << 2)] * Q[i1 + 3 * i0];
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      c_F_discrete[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        c_F_discrete[k + (i0 << 2)] += F_discrete[k + (i1 << 2)] *
          b_F_discrete[i0 + (i1 << 2)];
      }

      b_G[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        b_G[k + (i0 << 2)] += b_stateVec[k + (i1 << 2)] * G[i0 + (i1 << 2)];
      }
    }
  }

  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      P[i0 + (k << 2)] = c_F_discrete[i0 + (k << 2)] + b_G[i0 + (k << 2)];
    }
  }

  /* guarantee P ,prevent filter diverse due to illness of P matrix */
  /* 'tu_att_ekf:23' P_updated = 0.5*(P + transpose(P)); */
  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      P_updated[i0 + (k << 2)] = 0.5 * (P[i0 + (k << 2)] + P[k + (i0 << 2)]);
    }
  }

  /* guarantee main diagonal elements positive */
  /* 'tu_att_ekf:25' for i=1:length(P_updated) */
  for (i = 0; i < 4; i++) {
    /* 'tu_att_ekf:26' if P_updated(i,i) < 0 */
    if (P_updated[i + (i << 2)] < 0.0) {
      /* 'tu_att_ekf:27' P_updated(i,i) = 0; */
      P_updated[i + (i << 2)] = 0.0;
    }
  }
}



/*
 * File trailer for tu_att_ekf.c
 *
 * [EOF]
 */
