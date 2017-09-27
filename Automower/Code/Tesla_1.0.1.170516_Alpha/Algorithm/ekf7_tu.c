/* Include Files */
#include "imu.h"
#include "hardware.h"

/* Function Definitions */

/*
 * 地球常数
 *  axis_a = 6378137;             %赤道处地球半径[m]
 * Arguments    : const float acc[3]
 *                float pos[3]
 *                float vel[3]
 *                const float quat[4]
 *                float dt
 *                float P[49]
 *                const float Q[9]
 *                float stateVec[7]
 * Return Type  : void
 */

void ekf7_tu(float acc[3], float pos_prev[3], float vel_prev[3],float quatl[4], float dt, float P[49], float Q[9], 
			 float pos[3], float vel[3], float stateVec[7])
{
  float qm;
  float scale;
  int k;
  float q[4];
  float absxk;
  float t;
  float dcm[9];
  float Cnb[9];
  int i0;
  float a_n[3];
  volatile float b_a_n;
  static const float fv1[3] = { 0.0F, 0.0F, 9.8005F };

  float Z[49];
  float FDisc[49];
  static const signed char iv0[49] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1 };

  float G[21];
  float b_FDisc[49];
  float b_G[21];
  float c_FDisc[49];
  float c_G[49];
  int i1;
  float z[7];

  /*  重力及科里奥利力补偿 */
  /* 'ekf7_tu:6' g_n  = [0,0,9.8005]'; */
  /*  位置与速度解算 */
  /* 'ekf7_tu:9' Cnb = quat2dcmf(quat)'; */
  /* 'quat2dcmf:3' qin = quatnormalize( q ); */
  qm = 0.0F;
  scale = 1.17549435E-38F;
  for (k = 0; k < 4; k++) {
    q[k] = quatl[k];
    absxk = (float)fabs(quatl[k]);
    if (absxk > scale) {
      t = scale / absxk;
      qm = 1.0F + qm * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      qm += t * t;
    }
  }

  qm = scale * (float)sqrt(qm);
  for (k = 0; k < 4; k++) {
    q[k] /= qm;
  }

  /* 'quat2dcmf:5' dcm = zeros(3,3,size(qin,1)); */
  /* 'quat2dcmf:7' dcm(1,1,:) = qin(:,1).^2 + qin(:,2).^2 - qin(:,3).^2 - qin(:,4).^2; */
  dcm[0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];

  /* 'quat2dcmf:8' dcm(1,2,:) = 2.*(qin(:,2).*qin(:,3) + qin(:,1).*qin(:,4)); */
  dcm[3] = 2.0F * (q[1] * q[2] + q[0] * q[3]);

  /* 'quat2dcmf:9' dcm(1,3,:) = 2.*(qin(:,2).*qin(:,4) - qin(:,1).*qin(:,3)); */
  dcm[6] = 2.0F * (q[1] * q[3] - q[0] * q[2]);

  /* 'quat2dcmf:10' dcm(2,1,:) = 2.*(qin(:,2).*qin(:,3) - qin(:,1).*qin(:,4)); */
  dcm[1] = 2.0F * (q[1] * q[2] - q[0] * q[3]);

  /* 'quat2dcmf:11' dcm(2,2,:) = qin(:,1).^2 - qin(:,2).^2 + qin(:,3).^2 - qin(:,4).^2; */
  dcm[4] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];

  /* 'quat2dcmf:12' dcm(2,3,:) = 2.*(qin(:,3).*qin(:,4) + qin(:,1).*qin(:,2)); */
  dcm[7] = 2.0F * (q[2] * q[3] + q[0] * q[1]);

  /* 'quat2dcmf:13' dcm(3,1,:) = 2.*(qin(:,2).*qin(:,4) + qin(:,1).*qin(:,3)); */
  dcm[2] = 2.0F * (q[1] * q[3] + q[0] * q[2]);

  /* 'quat2dcmf:14' dcm(3,2,:) = 2.*(qin(:,3).*qin(:,4) - qin(:,1).*qin(:,2)); */
  dcm[5] = 2.0F * (q[2] * q[3] - q[0] * q[1]);

  /* 'quat2dcmf:15' dcm(3,3,:) = qin(:,1).^2 - qin(:,2).^2 - qin(:,3).^2 + qin(:,4).^2; */
  dcm[8] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  for (i0 = 0; i0 < 3; i0++) {
    for (k = 0; k < 3; k++) {
      Cnb[k + 3 * i0] = dcm[i0 + 3 * k];
    }
  }

  /* 'ekf7_tu:10' a_n = Cnb*acc + g_n  ; */
  /* 'ekf7_tu:11' vel = vel_prev + a_n*dt; */
  for (k = 0; k < 3; k++) {
    qm = 0.0F;
    for (i0 = 0; i0 < 3; i0++) {
      qm += Cnb[k + 3 * i0] * acc[i0];
    }

    b_a_n = qm + fv1[k];
    vel[k] = vel_prev[k] + b_a_n * dt;
    a_n[k] = b_a_n;
  }

  /* 梯形公式计算位置 */
  /* 'ekf7_tu:14' pos = zeros(1,3); */
  /* 'ekf7_tu:15' pos(1) = pos_prev(1) + dt*0.5*(vel(1) + vel_prev(1)); */
  pos[0] = pos_prev[0] + dt * 0.5F * (vel[0] + vel_prev[0]);

  /* 'ekf7_tu:16' pos(2) = pos_prev(2) + dt*0.5*(vel(2) + vel_prev(2)); */
  pos[1] = pos_prev[1] + dt * 0.5F * (vel[1] + vel_prev[1]);

  /* 'ekf7_tu:17' pos(3) = pos_prev(3) + dt*0.5*(vel(3) + vel_prev(3)); */
  pos[2] = pos_prev[2] + dt * 0.5F * (vel[2] + vel_prev[2]);

  /*  Kalman filter time update */
  
  /* 'ekf7_tu:52' Z = zeros(length(P)); */
  memset(&Z[0], 0, 49U * sizeof(float));

  /* 'ekf7_tu:53' Z(4,4) = dt; */
  Z[21] = dt;

  /* 'ekf7_tu:54' Z(5,5) = dt; */
  Z[29] = dt;

  /* 'ekf7_tu:55' Z(6,6) = dt; */
  Z[37] = 0;

  /* 'ekf7_tu:56' FDisc = eye(length(P)) + Z; */
  for (i0 = 0; i0 < 49; i0++) {
    FDisc[i0] = (float)iv0[i0] + Z[i0];
  }
	
  FDisc[48] = 0.0F;
	
	  /* 'ekf7_tu:33' if((pos_prev(1)~=0)&&(pos_prev(2)~=0)) */
//  if (pos_prev[0] != 0.0F && pos_prev[1] != 0.0F) {
//    /* 'ekf7_tu:34' a = pos(1); */
//    /* 'ekf7_tu:34' b = pos(2); */
//    /* 'ekf7_tu:35' FDisc(7,1) = -a/(pos_prev(1)^2+pos_prev(2)^2)*dt; */
//    FDisc[6] = -pos[0] / (pos_prev[0] * pos_prev[0] + pos_prev[1] * pos_prev[1]) * dt;

//    /* 'ekf7_tu:36' FDisc(7,2) = b/(pos_prev(1)^2+pos_prev(2)^2)*dt; */
//    FDisc[13] = pos[1] / (pos_prev[0] * pos_prev[0] + pos_prev[1] * pos_prev[1]) * dt;
//  }
  
  /* 'ekf7_tu:58' G = zeros(7,3); */
  memset(&G[0], 0, 21U * sizeof(float));

  /* 'ekf7_tu:59' G(1:3,1:3) = 0.5*dt*dt*Cnb; */
  qm = 0.5F * dt * dt;

  /* 'ekf7_tu:60' G(4:6,1:3) = dt*Cnb; */
  for (i0 = 0; i0 < 3; i0++) {
    for (k = 0; k < 3; k++) {
      G[k + 7 * i0] = qm * Cnb[k + 3 * i0];
      G[k + 7 * i0 + 3] = dt * Cnb[k + 3 * i0];
    }
  }

  /* 计算离散Q矩阵 */
  /* 'ekf7_tu:63' QDisc = G * Q * G'; */
  /* P矩阵计算 */
  /* 'ekf7_tu:66' P = FDisc * P *(FDisc') + QDisc; */
  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      b_FDisc[i0 + 7 * k] = 0.0F;
      for (i1 = 0; i1 < 7; i1++) {
        b_FDisc[i0 + 7 * k] += FDisc[i0 + 7 * i1] * P[i1 + 7 * k];
      }
    }

    for (k = 0; k < 3; k++) {
      b_G[i0 + 7 * k] = 0.0F;
      for (i1 = 0; i1 < 3; i1++) {
        b_G[i0 + 7 * k] += G[i0 + 7 * i1] * Q[i1 + 3 * k];
      }
    }

    for (k = 0; k < 7; k++) {
      c_FDisc[i0 + 7 * k] = 0.0F;
      for (i1 = 0; i1 < 7; i1++) {
        c_FDisc[i0 + 7 * k] += b_FDisc[i0 + 7 * i1] * FDisc[k + 7 * i1];
      }

      c_G[i0 + 7 * k] = 0.0F;
      for (i1 = 0; i1 < 3; i1++) {
        c_G[i0 + 7 * k] += b_G[i0 + 7 * i1] * G[k + 7 * i1];
      }
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      P[k + 7 * i0] = c_FDisc[k + 7 * i0] + c_G[k + 7 * i0];
    }
  }

  /* 保证P矩阵对称性,防止因为病态矩阵引起滤波器发散 */
  /* 'ekf7_tu:69' P = 0.5*(P + transpose(P)); */
  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      b_FDisc[k + 7 * i0] = 0.5F * (P[k + 7 * i0] + P[i0 + 7 * k]);
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (k = 0; k < 7; k++) {
      P[k + 7 * i0] = b_FDisc[k + 7 * i0];
    }
  }

  /* 保证对角线元素为正 */
  /* 'ekf7_tu:72' for i=1:length(P) */
  /* 'ekf7_tu:78' z = zeros(1,7); */
  for (k = 0; k < 7; k++) {
    /* 'ekf7_tu:73' if P(i,i) < 0 */
    if (P[k + 7 * k] < 0.0F) {
      /* 'ekf7_tu:74' P(i,i) = 0; */
      P[k + 7 * k] = 0.0F;
    }

    z[k] = 0.0F;
  }

  /* 'ekf7_tu:79' z(4:6) = a_n*dt; */
  /* 'ekf7_tu:80' z(1:3) = dt*0.5*(vel(1:3) + vel_prev(1:3)); */
  qm = dt * 0.5F;
  for (i0 = 0; i0 < 3; i0++) {
    z[3 + i0] = a_n[i0] * dt;
    z[i0] = qm * (vel[i0] + vel_prev[i0]);
  }

  /* 'ekf7_tu:81' z(7) = 0; */
  z[6] = 0.0F;

  /* 'ekf7_tu:82' stateVec = z; */
  for (i0 = 0; i0 < 7; i0++) {
    stateVec[i0] = z[i0];
  }
}
