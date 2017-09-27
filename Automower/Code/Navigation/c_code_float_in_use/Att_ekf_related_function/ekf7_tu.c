/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Function Definitions */
/*
 * 地球常数
 *  axis_a = 6378137;             %赤道处地球半径[m]
 * Arguments    : const double acc_m_s2[3]
 *                const double pos_prev[3]
 *                const double vel_prev[3]
 *                double yaw_prev
 *                const float quat[4]
 *                float dt
 *                const float P[49]
 *                const float Q[9]
 *                float pos[3]
 *                float vel[3]
 *                float *yaw
 *                float stateVec_updated[7]
 *                float P_updated[49]
 * Return Type  : void
 */
void ekf7_tu(float acc_m_s2[3], float pos_prev[3], float vel_prev[3], float yaw_prev, float quat[4],float dt, 
					float P[49], float Q[9], float pos[3], float vel[3], float *yaw, float stateVec_updated[7],float P_updated[49])
{
  float x;
  float qin[4];
  int i0;
  float dcm[9];
  float Cnb[9];
  int k;
  float a_n[3];
  float f0;
  float b_a_n;
  static const float fv0[3] = { 0.0F, 0.0F, 9.8005F };

  float Z[49];
  signed char I[49];
  float FDisc[49];
  float G[21];
  float b_FDisc[49];
  float b_G[21];
  float c_FDisc[49];
  float c_G[49];
  int i1;

  /*  重力及科里奥利力补偿 */
  /* 'ekf7_tu:7' g_n  = [0,0,9.8005]'; */
  /*  位置与速度解算 */
  /* 'ekf7_tu:10' Cnb = quat2dcmf(quat)'; */
  /* 'quat2dcmf:3' qin = q/sqrt(q(1)*q(1)+q(2)*q(2)+q(3)*q(3)+q(4)*q(4)); */
  x = sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] *
           quat[3]);
  for (i0 = 0; i0 < 4; i0++) {
    qin[i0] = quat[i0] / x;
  }

  /* 'quat2dcmf:4' dcm = zeros(3,3); */
  /* 'quat2dcmf:6' dcm(1,1) = qin(:,1).^2 + qin(:,2).^2 - qin(:,3).^2 - qin(:,4).^2; */
  dcm[0] = qin[0] * qin[0] + qin[1] * qin[1] - qin[2] * qin[2] - qin[3] * qin[3];

  /* 'quat2dcmf:7' dcm(1,2) = 2.*(qin(:,2).*qin(:,3) + qin(:,1).*qin(:,4)); */
  dcm[3] = 2.0 * (qin[1] * qin[2] + qin[0] * qin[3]);

  /* 'quat2dcmf:8' dcm(1,3) = 2.*(qin(:,2).*qin(:,4) - qin(:,1).*qin(:,3)); */
  dcm[6] = 2.0 * (qin[1] * qin[3] - qin[0] * qin[2]);

  /* 'quat2dcmf:9' dcm(2,1) = 2.*(qin(:,2).*qin(:,3) - qin(:,1).*qin(:,4)); */
  dcm[1] = 2.0 * (qin[1] * qin[2] - qin[0] * qin[3]);

  /* 'quat2dcmf:10' dcm(2,2) = qin(:,1).^2 - qin(:,2).^2 + qin(:,3).^2 - qin(:,4).^2; */
  dcm[4] = qin[0] * qin[0] - qin[1] * qin[1] + qin[2] * qin[2] - qin[3] * qin[3];

  /* 'quat2dcmf:11' dcm(2,3) = 2.*(qin(:,3).*qin(:,4) + qin(:,1).*qin(:,2)); */
  dcm[7] = 2.0 * (qin[2] * qin[3] + qin[0] * qin[1]);

  /* 'quat2dcmf:12' dcm(3,1) = 2.*(qin(:,2).*qin(:,4) + qin(:,1).*qin(:,3)); */
  dcm[2] = 2.0 * (qin[1] * qin[3] + qin[0] * qin[2]);

  /* 'quat2dcmf:13' dcm(3,2) = 2.*(qin(:,3).*qin(:,4) - qin(:,1).*qin(:,2)); */
  dcm[5] = 2.0 * (qin[2] * qin[3] - qin[0] * qin[1]);

  /* 'quat2dcmf:14' dcm(3,3) = qin(:,1).^2 - qin(:,2).^2 - qin(:,3).^2 + qin(:,4).^2; */
  dcm[8] = qin[0] * qin[0] - qin[1] * qin[1] - qin[2] * qin[2] + qin[3] * qin[3];

  /* 'quat2dcmf:15' dcm = single(dcm); */
  for (i0 = 0; i0 < 3; i0++) {
    for (k = 0; k < 3; k++) {
      Cnb[k + 3 * i0] = dcm[i0 + 3 * k];
    }
  }

  /* 'ekf7_tu:11' a_n = Cnb*acc_m_s2 + g_n; */
  /* 'ekf7_tu:12' vel = vel_prev + a_n*dt; */
  for (k = 0; k < 3; k++) {
    f0 = 0.0F;
    for (i0 = 0; i0 < 3; i0++) {
      f0 += Cnb[k + 3 * i0] * acc_m_s2[i0];
    }

    b_a_n = f0 + fv0[k];
    vel[k] = vel_prev[k] + b_a_n * dt;
    a_n[k] = b_a_n;
  }

  /* 梯形公式计算位置 */
  /* 'ekf7_tu:15' pos = zeros(3,1); */
  /* 'ekf7_tu:16' pos(1) = pos_prev(1) + dt*0.5*(vel(1) + vel_prev(1)); */
  pos[0] = pos_prev[0] + (dt * 0.5) * (vel[0] + vel_prev[0]);

  /* 'ekf7_tu:17' pos(2) = pos_prev(2) + dt*0.5*(vel(2) + vel_prev(2)); */
  pos[1] = pos_prev[1] + (dt * 0.5) * (vel[1] + vel_prev[1]);

  /* 'ekf7_tu:18' pos(3) = pos_prev(3) + dt*0.5*(vel(3) + vel_prev(3)); */
  pos[2] = pos_prev[2] + (dt * 0.5) * (vel[2] + vel_prev[2]);

  /*  Kalman filter time update */
  /* 'ekf7_tu:21' Z = zeros(length(P)); */
  /* 'ekf7_tu:22' Z(4,4) = dt; */
  memset(&Z[0], 0, 49U * sizeof(float));
  for (i0 = 0; i0 < 49; i0++) {
    I[i0] = 0;
  }

  Z[24] = dt;

  /* 'ekf7_tu:23' Z(5,5) = dt; */
  Z[32] = dt;

  /* 'ekf7_tu:24' Z(6,6) = dt; */
  Z[40] = dt;

  /* 'ekf7_tu:25' FDisc = eye(length(P)) + Z; */
  for (k = 0; k < 7; k++) {
    I[k + 7 * k] = 1;
  }

  for (i0 = 0; i0 < 49; i0++) {
    FDisc[i0] = I[i0] + Z[i0];
  }

  /* 'ekf7_tu:27' G = zeros(7,3); */
  memset(&G[0], 0, 21U * sizeof(float));

  /* 'ekf7_tu:28' G(1:3,1:3) = 0.5*dt*dt*Cnb; */
  x = 0.5 * dt * dt;

  /* 'ekf7_tu:29' G(4:6,1:3) = dt*Cnb; */
  for (i0 = 0; i0 < 3; i0++) {
    for (k = 0; k < 3; k++) {
      G[k + 7 * i0] = x * Cnb[k + 3 * i0];
      G[k + 7 * i0 + 3] = dt * Cnb[k + 3 * i0];
    }
  }

  /* 计算离散Q矩阵 */
  /* 'ekf7_tu:32' QDisc = G * Q * G'; */
  /* P矩阵计算 */
  /* 'ekf7_tu:35' P_updated = FDisc * P *(FDisc') + QDisc; */
  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      b_FDisc[i0 + 7 * k] = 0.0;
      for (i1 = 0; i1 < 7; i1++) {
        b_FDisc[i0 + 7 * k] += FDisc[i0 + 7 * i1] * P[i1 + 7 * k];
      }
    }

    for (k = 0; k < 3; k++) {
      b_G[i0 + 7 * k] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        b_G[i0 + 7 * k] += G[i0 + 7 * i1] * Q[i1 + 3 * k];
      }
    }

    for (k = 0; k < 7; k++) {
      c_FDisc[i0 + 7 * k] = 0.0;
      for (i1 = 0; i1 < 7; i1++) {
        c_FDisc[i0 + 7 * k] += b_FDisc[i0 + 7 * i1] * FDisc[k + 7 * i1];
      }

      c_G[i0 + 7 * k] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        c_G[i0 + 7 * k] += b_G[i0 + 7 * i1] * G[k + 7 * i1];
      }
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      P_updated[k + 7 * i0] = c_FDisc[k + 7 * i0] + c_G[k + 7 * i0];
    }
  }

  /* 保证P矩阵对称性,防止因为病态矩阵引起滤波器发散 */
  /* 'ekf7_tu:38' P_updated= 0.5*(P_updated + transpose(P_updated)); */
  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      b_FDisc[k + 7 * i0] = 0.5 * (P_updated[k + 7 * i0] + P_updated[i0 + 7 * k]);
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      P_updated[k + 7 * i0] = b_FDisc[k + 7 * i0];
    }
  }

  /* 保证对角线元素为正 */
  /* 'ekf7_tu:41' for i=1:length(P_updated) */
  /* 'ekf7_tu:47' stateVec_updated = zeros(7,1); */
  for (k = 0; k < 7; k++) {
    /* 'ekf7_tu:42' if P_updated(i,i) < 0 */
    if (P_updated[k + 7 * k] < 0.0) {
      /* 'ekf7_tu:43' P_updated(i,i) = 0; */
      P_updated[k + 7 * k] = 0.0;
    }

    stateVec_updated[k] = 0.0;
  }

  /* 'ekf7_tu:48' stateVec_updated(4:6) = a_n*dt; */
  /* 'ekf7_tu:49' stateVec_updated(1:3) = dt*0.5*(vel(1:3) + vel_prev(1:3)); */
  x = dt * 0.5;
  for (k = 0; k < 3; k++) {
    stateVec_updated[k + 3] = a_n[k] * dt;
    stateVec_updated[k] = x * (vel[k] + vel_prev[k]);
  }

  /*  yaw = yaw_att;  */
  /*  z(7) = yaw - yaw_prev; */
  /* 'ekf7_tu:53' yaw = yaw_prev; */
  *yaw = yaw_prev;

  /* 'ekf7_tu:54' stateVec_updated(7) = 0; */
  stateVec_updated[6] = 0.0;
}
