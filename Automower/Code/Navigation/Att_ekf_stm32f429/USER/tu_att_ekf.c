/* Include Files */
#include "ekf.h"

/* Function Definitions */
/*
 * 1st-order approximation
 * Arguments    : float P[16]
 *                float stateVec[4]
 *                const float angular_imu_rad_s[3]
 *                float dt
 *                const float Q[9]
 * Return Type  : void
 */
void tu_att_ekf(float P[16], float stateVec[4], float angular_imu_rad_s[3],float dt, float Q[9])
{
  float F_discrete[16];
  float b_F_discrete[16];
  int i0;
  int i1;
  static const signed char iv0[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 1 };

  float c;
  float b_stateVec[12];
  float G[12];
  float c_F_discrete[4];
  float d_F_discrete[16];
  float b_G[16];
  int i2;
  int i;

  /* 'tu_att_ekf:5' phi = [0 -angular_imu_rad_s(1)*dt -angular_imu_rad_s(2)*dt    -angular_imu_rad_s(3)*dt;... */
  /* 'tu_att_ekf:6'        angular_imu_rad_s(1)*dt 0 angular_imu_rad_s(3)*dt      -angular_imu_rad_s(2)*dt;... */
  /* 'tu_att_ekf:7'        angular_imu_rad_s(2)*dt   -angular_imu_rad_s(3)*dt  0   angular_imu_rad_s(1)*dt;... */
  /* 'tu_att_ekf:8'        angular_imu_rad_s(3)*dt   angular_imu_rad_s(2)*dt      -angular_imu_rad_s(1)*dt 0]; */
  /* discrete transition matrix */
  /* 'tu_att_ekf:10' F_discrete = (0.5 * phi + eye(length(stateVec))); */
  F_discrete[0] = 0.0F;
  F_discrete[4] = -angular_imu_rad_s[0] * dt;
  F_discrete[8] = -angular_imu_rad_s[1] * dt;
  F_discrete[12] = -angular_imu_rad_s[2] * dt;
  F_discrete[1] = angular_imu_rad_s[0] * dt;
  F_discrete[5] = 0.0F;
  F_discrete[9] = angular_imu_rad_s[2] * dt;
  F_discrete[13] = -angular_imu_rad_s[1] * dt;
  F_discrete[2] = angular_imu_rad_s[1] * dt;
  F_discrete[6] = -angular_imu_rad_s[2] * dt;
  F_discrete[10] = 0.0F;
  F_discrete[14] = angular_imu_rad_s[0] * dt;
  F_discrete[3] = angular_imu_rad_s[2] * dt;
  F_discrete[7] = angular_imu_rad_s[1] * dt;
  F_discrete[11] = -angular_imu_rad_s[0] * dt;
  F_discrete[15] = 0.0F;
  for (i0 = 0; i0 < 4; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      b_F_discrete[i1 + (i0 << 2)] = 0.5F * F_discrete[i1 + (i0 << 2)] + (float)
        iv0[i1 + (i0 << 2)];
    }
  }

  /* noise transition matrix */
  /* 'tu_att_ekf:12' G = 0.5*dt*[-stateVec(2) -stateVec(3) -stateVec(4);... */
  /* 'tu_att_ekf:13'              stateVec(1) -stateVec(4) stateVec(3);... */
  /* 'tu_att_ekf:14'             -stateVec(4) stateVec(1) -stateVec(2);... */
  /* 'tu_att_ekf:15'             -stateVec(3) stateVec(2) stateVec(1)]; */
  c = 0.5F * dt;
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
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      G[i1 + (i0 << 2)] = c * b_stateVec[i1 + (i0 << 2)];
    }
  }

  /* discrete system noise variance    */
  /* 'tu_att_ekf:17' Q_discrete = G * Q * G'; */
  /* time update(attitude propagation) */
  /* 'tu_att_ekf:19' stateVec = F_discrete * stateVec; */
  for (i0 = 0; i0 < 4; i0++) {
    c_F_discrete[i0] = 0.0F;
    for (i1 = 0; i1 < 4; i1++) {
      c_F_discrete[i0] += b_F_discrete[i0 + (i1 << 2)] * stateVec[i1];
    }
  }

  /* P matrix calculation */
  /* 'tu_att_ekf:21' P = F_discrete * P *(F_discrete') + Q_discrete; */
  for (i0 = 0; i0 < 4; i0++) {
    stateVec[i0] = c_F_discrete[i0];
    for (i1 = 0; i1 < 4; i1++) {
      F_discrete[i0 + (i1 << 2)] = 0.0F;
      for (i2 = 0; i2 < 4; i2++) {
        F_discrete[i0 + (i1 << 2)] += b_F_discrete[i0 + (i2 << 2)] * P[i2 + (i1 <<
          2)];
      }
    }

    for (i1 = 0; i1 < 3; i1++) {
      b_stateVec[i0 + (i1 << 2)] = 0.0F;
      for (i2 = 0; i2 < 3; i2++) {
        b_stateVec[i0 + (i1 << 2)] += G[i0 + (i2 << 2)] * Q[i2 + 3 * i1];
      }
    }

    for (i1 = 0; i1 < 4; i1++) {
      d_F_discrete[i0 + (i1 << 2)] = 0.0F;
      for (i2 = 0; i2 < 4; i2++) {
        d_F_discrete[i0 + (i1 << 2)] += F_discrete[i0 + (i2 << 2)] *
          b_F_discrete[i1 + (i2 << 2)];
      }

      b_G[i0 + (i1 << 2)] = 0.0F;
      for (i2 = 0; i2 < 3; i2++) {
        b_G[i0 + (i1 << 2)] += b_stateVec[i0 + (i2 << 2)] * G[i1 + (i2 << 2)];
      }
    }
  }

  for (i0 = 0; i0 < 4; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      P[i1 + (i0 << 2)] = d_F_discrete[i1 + (i0 << 2)] + b_G[i1 + (i0 << 2)];
    }
  }

  /* guarantee P ,prevent filter diverse due to illness of P matrix */
  /* 'tu_att_ekf:23' P = 0.5*(P + transpose(P)); */
  for (i0 = 0; i0 < 4; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      F_discrete[i1 + (i0 << 2)] = 0.5F * (P[i1 + (i0 << 2)] + P[i0 + (i1 << 2)]);
    }
  }

  for (i0 = 0; i0 < 4; i0++) {
    for (i1 = 0; i1 < 4; i1++) {
      P[i1 + (i0 << 2)] = F_discrete[i1 + (i0 << 2)];
    }
  }

  /* guarantee main diagonal elements positive */
  /* 'tu_att_ekf:25' for i=1:length(P) */
  for (i = 0; i < 4; i++) {
    /* 'tu_att_ekf:26' if P(i,i) < 0 */
    if (P[i + (i << 2)] < 0.0F) {
      /* 'tu_att_ekf:27' P(i,i) = 0; */
      P[i + (i << 2)] = 0.0F;
    }
  }
}
