#include "ekf.h"

/* Function Definitions */
/*
 *  axis_a = 6378137;             %???????[m]
 * Arguments    : const float acc_m_s2[3]
 *                float pos[3]
 *                float vel[3]
 *                const float quat[4]
 *                float dt
 *                float P[49]
 *                const float Q[9]
 *                float stateVec[7]
 * Return Type  : void
 */

void ekf7_tu(float acc_m_s2[3], float pos[3], float vel[3], float
             quat[4], float dt, float P[49],  float Q[9], float stateVec[7])
{
	float FDisc[49];
  float G[21];
  float b_FDisc[49];
  float b_G[21];
  float c_FDisc[49];
  float c_G[49];
  float x;
  float qin[4];
  int i0;
  float dcm[9];
  float Cnb[9];
  int a;
  float a_n[3];
  int b;
  int i1;
  static const float fv1[3] = { 0.0F, 0.0F, 9.8005F };
  int i2,i;


  /*  ?????????? */
  /* 'ekf7_tu:7' g_n  = [0,0,9.8005]'; */
  /*  ??????? */
  /* 'ekf7_tu:10' Cnb = trans33(quat2dcmf(quat)); */
  /* 'quat2dcmf:3' qin = q/sqrt(q(1)*q(1)+q(2)*q(2)+q(3)*q(3)+q(4)*q(4)); */
  x = (float)sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] +
                  quat[3] * quat[3]);
  for (i0 = 0; i0 < 4; i0++) {
    qin[i0] = quat[i0] / x;
  }

  /* 'quat2dcmf:4' dcm = zeros(3,3); */
  /* 'quat2dcmf:6' dcm(1,1) = qin(:,1).^2 + qin(:,2).^2 - qin(:,3).^2 - qin(:,4).^2; */
  dcm[0] = qin[0] * qin[0] + qin[1] * qin[1] - qin[2] * qin[2] - qin[3] * qin[3];

  /* 'quat2dcmf:7' dcm(1,2) = 2.*(qin(:,2).*qin(:,3) + qin(:,1).*qin(:,4)); */
  dcm[3] = 2.0F * (qin[1] * qin[2] + qin[0] * qin[3]);

  /* 'quat2dcmf:8' dcm(1,3) = 2.*(qin(:,2).*qin(:,4) - qin(:,1).*qin(:,3)); */
  dcm[6] = 2.0F * (qin[1] * qin[3] - qin[0] * qin[2]);

  /* 'quat2dcmf:9' dcm(2,1) = 2.*(qin(:,2).*qin(:,3) - qin(:,1).*qin(:,4)); */
  dcm[1] = 2.0F * (qin[1] * qin[2] - qin[0] * qin[3]);

  /* 'quat2dcmf:10' dcm(2,2) = qin(:,1).^2 - qin(:,2).^2 + qin(:,3).^2 - qin(:,4).^2; */
  dcm[4] = qin[0] * qin[0] - qin[1] * qin[1] + qin[2] * qin[2] - qin[3] * qin[3];

  /* 'quat2dcmf:11' dcm(2,3) = 2.*(qin(:,3).*qin(:,4) + qin(:,1).*qin(:,2)); */
  dcm[7] = 2.0F * (qin[2] * qin[3] + qin[0] * qin[1]);

  /* 'quat2dcmf:12' dcm(3,1) = 2.*(qin(:,2).*qin(:,4) + qin(:,1).*qin(:,3)); */
  dcm[2] = 2.0F * (qin[1] * qin[3] + qin[0] * qin[2]);

  /* 'quat2dcmf:13' dcm(3,2) = 2.*(qin(:,3).*qin(:,4) - qin(:,1).*qin(:,2)); */
  dcm[5] = 2.0F * (qin[2] * qin[3] - qin[0] * qin[1]);

  /* 'quat2dcmf:14' dcm(3,3) = qin(:,1).^2 - qin(:,2).^2 - qin(:,3).^2 + qin(:,4).^2; */
  dcm[8] = qin[0] * qin[0] - qin[1] * qin[1] - qin[2] * qin[2] + qin[3] * qin[3];

  /* 'quat2dcmf:15' dcm = single(dcm); */
  /* 'trans33:3' y = zeros(3,3); */
  /* 'trans33:4' for a = 1:3 */
  for (a = 0; a < 3; a++) {
    /* 'trans33:5' for b = 1:3 */
    for (b = 0; b < 3; b++) {
      /* 'trans33:6' y(b,a) = x(a,b); */
      Cnb[b + 3 * a] = dcm[a + 3 * b];
    }
  }

  /* 'ekf7_tu:11' a_n = Cnb*acc_m_s2 + g_n; */
  for (i0 = 0; i0 < 3; i0++) {
    x = 0.0F;
    for (i1 = 0; i1 < 3; i1++) {
      x += Cnb[i0 + 3 * i1] * acc_m_s2[i1];
    }

    a_n[i0] = x + fv1[i0];
  }

  /* ???????? */
  /* 'ekf7_tu:14' pos(1) = pos(1) + dt*0.5*(2*vel(1)+a_n(1)*dt); */
  pos[0] += dt * 0.5F * (2.0F * vel[0] + a_n[0] * dt);

  /* 'ekf7_tu:15' pos(2) = pos(2) + dt*0.5*(2*vel(1)+a_n(1)*dt); */
  pos[1] += dt * 0.5F * (2.0F * vel[0] + a_n[0] * dt);

  /* 'ekf7_tu:16' pos(3) = pos(3) + dt*0.5*(2*vel(1)+a_n(1)*dt); */
  pos[2] += dt * 0.5F * (2.0F * vel[0] + a_n[0] * dt);

  /*  Kalman filter time update */
  /* 'ekf7_tu:19' FDisc = zeros(length(P)); */
  memset(&FDisc[0], 0, 49U * sizeof(float));

  /* 'ekf7_tu:20' FDisc(4,4) = dt + 1 ; */
  FDisc[24] = dt + 1.0F;
  /* 'ekf7_tu:21' FDisc(5,5) = dt + 1 ; */
  FDisc[32] = dt + 1.0F;
  /* 'ekf7_tu:22' FDisc(6,6) = dt + 1 ; */
  FDisc[40] = dt + 1.0F;

  /* 'ekf7_tu:24' G = zeros(7,3); */
  memset(&G[0], 0, 21U * sizeof(float));

  /* 'ekf7_tu:25' G(1:3,1:3) = 0.5*dt*dt*Cnb; */
  x = 0.5F * dt * dt;

  /* 'ekf7_tu:26' G(4:6,1:3) = dt*Cnb; */
  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      G[i1 + 7 * i0] = x * Cnb[i1 + 3 * i0];
      G[i1 + 7 * i0 + 3] = dt * Cnb[i1 + 3 * i0];
    }
  }

  /* ????Q?? */
  /* 'ekf7_tu:29' QDisc = G * Q * G'; */
  /* P???? */
  /* 'ekf7_tu:32' P = FDisc * P * FDisc' + QDisc; */
  for (i0 = 0; i0 < 7; i0++) {
    for (i1 = 0; i1 < 7; i1++) {
      b_FDisc[i0 + 7 * i1] = 0.0F;
      for (i2 = 0; i2 < 7; i2++) {
        b_FDisc[i0 + 7 * i1] += FDisc[i0 + 7 * i2] * P[i2 + 7 * i1];
      }
    }

    for (i1 = 0; i1 < 3; i1++) {
      b_G[i0 + 7 * i1] = 0.0F;
      for (i2 = 0; i2 < 3; i2++) {
        b_G[i0 + 7 * i1] += G[i0 + 7 * i2] * Q[i2 + 3 * i1];
      }
    }

    for (i1 = 0; i1 < 7; i1++) {
      c_FDisc[i0 + 7 * i1] = 0.0F;
      for (i2 = 0; i2 < 7; i2++) {
        c_FDisc[i0 + 7 * i1] += b_FDisc[i0 + 7 * i2] * FDisc[i1 + 7 * i2];
      }

      c_G[i0 + 7 * i1] = 0.0F;
      for (i2 = 0; i2 < 3; i2++) {
        c_G[i0 + 7 * i1] += b_G[i0 + 7 * i2] * G[i1 + 7 * i2];
      }
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (i1 = 0; i1 < 7; i1++) {
      P[i1 + 7 * i0] = c_FDisc[i1 + 7 * i0] + c_G[i1 + 7 * i0];
    }
  }

  /* ??P?????,??????????????? */
  /* 'ekf7_tu:35' P= 0.5*(P + P'); */
  for (i0 = 0; i0 < 7; i0++) {
    for (i1 = 0; i1 < 7; i1++) {
      b_FDisc[i1 + 7 * i0] = 0.5F * (P[i1 + 7 * i0] + P[i0 + 7 * i1]);
    }
  }

  for (i0 = 0; i0 < 7; i0++) {
    for (i1 = 0; i1 < 7; i1++) {
      P[i1 + 7 * i0] = b_FDisc[i1 + 7 * i0];
    }
  }

  /* ????????? */
  /* 'ekf7_tu:38' for i=1:length(P) */
  /* 'ekf7_tu:44' stateVec = zeros(7,1); */
  for (i = 0; i < 7; i++) {
    /* 'ekf7_tu:39' if P(i,i) < 0 */
    if (P[i + 7 * i] < 0.0F) {
      /* 'ekf7_tu:40' P(i,i) = 0; */
      P[i + 7 * i] = 0.0F;
    }

    stateVec[i] = 0.0F;
  }

  /* 'ekf7_tu:45' stateVec(4:6) = a_n*dt; */
  /* 'ekf7_tu:46' stateVec(1:3) = dt*0.5*(2*vel+a_n*dt); */
  x = dt * 0.5F;

  /*  yaw = yaw_att;  */
  /*  z(7) = yaw - yaw_prev; */
  /* 'ekf7_tu:50' stateVec(7) = 0; */
  stateVec[6] = 0.0F;

  /* 'ekf7_tu:51' vel = vel + a_n*dt; */
  for (i = 0; i < 3; i++) {
    stateVec[i + 3] = a_n[i] * dt;
    stateVec[i] = x * (2.0F * vel[i] + a_n[i] * dt);
    vel[i] += a_n[i] * dt;
  }
}