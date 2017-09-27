/* Include Files */
#include "insgps.h"


/* Function Definitions */

/*
 * yaw clockwise
 * Arguments    : const float pos_prev[3]
 *                const float vel_prev[3]
 *                float *yaw
 *                float stateVec[7]
 *                float delta_odo_r
 *                float delta_odo_l
 *                float P[49]
 *                const float R[9]
 *                float D
 *                float pos[3]
 *                float vel[3]
 * Return Type  : void
 */

void ekf7_odom_mu(float pos_prev[3], float vel_prev[3], float *yaw,float stateVec[7], float delta_odo_r, float delta_odo_l,
                   float P[49], float R[9],float R_odom[9], float D, float pos[3], float vel[3])
{
  float delta_x;
  float delta_y;
  float delta_theta;
  int rtemp;
  float a[21];
  int k;
  float c[9];
  float b_c[21];
  int r1;
  int r2;
  static const signed char b[21] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1 };
  float dx,dy;
  int r3;
  float maxval;
  float a21;
  static const signed char b_a[21] = { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1 };

  float K[21];
  float c_a[3];
  float b_delta_x[3];
  float c_delta_x[3];
  float fv1[49];
  float fv2[49];
  static const signed char iv0[49] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1 };

  int i;

  memcpy(R,R_odom,sizeof(float)*9);
//  dx = movvarf(delta_odo_l);
//  dy = movvarf(delta_odo_r);			
//  if((dx!=0)&&(dy!=0)){
//		R[0]=dx;
//		R[4]=dy;
//  }
			
  /* 'ekf7_odom3_mu:4' theta = yaw; */
  /* eul = [yaw,pitch,roll] in the order of ZYX */
  /* 'ekf7_odom3_mu:5' r = delta_odo_r; */
  /* 'ekf7_odom3_mu:6' l = delta_odo_l; */
  /* 'ekf7_odom3_mu:7' if r~=l */
  if (delta_odo_r != delta_odo_l) {
    /*      alpha = (l-r)/D; */
    /*      delta_x = (D+2*l/alpha)*cos(theta+alpha/2)*sin(alpha/2); */
    /*      delta_y = (D+2*l/alpha)*sin(theta+alpha/2)*sin(alpha/2); */
    /*      delta_theta = alpha; */
    /* 'ekf7_odom3_mu:12' delta_x = cos(theta+(r-l)/2/D)*0.5*(r+l); */
    delta_x = (float)cos(*yaw + (delta_odo_r - delta_odo_l) / 2.0F / D) * 0.5F *
      (delta_odo_r + delta_odo_l);

    /* 'ekf7_odom3_mu:13' delta_y = sin(theta+(r-l)/2/D)*0.5*(r+l); */
    delta_y = (float)sin(*yaw + (delta_odo_r - delta_odo_l) / 2.0F / D) * 0.5F *
      (delta_odo_r + delta_odo_l);

    /* 'ekf7_odom3_mu:14' delta_theta = (l-r)/D; */
    delta_theta = (delta_odo_l - delta_odo_r) / D;
  } else {
    /* 'ekf7_odom3_mu:15' else */
    /* 'ekf7_odom3_mu:16' alpha = 0; */
    /* 'ekf7_odom3_mu:17' delta_x = cos(theta)*r; */
    delta_x = (float)cos(*yaw) * delta_odo_r;

    /* 'ekf7_odom3_mu:18' delta_y = sin(theta)*r; */
    delta_y = (float)sin(*yaw) * delta_odo_r;

    /* 'ekf7_odom3_mu:19' delta_theta = alpha; */
    delta_theta = 0.0F;
  }
	//rt_kprintf("\r\n %d,%d,%d,%d",(int)(delta_x*1000),(int)(delta_y*1000),(int)(delta_odo_r*1000),(int)(*yaw*1000));

  /* 'ekf7_odom3_mu:21' z = [delta_x;delta_y;-delta_theta]; */
  /*  delta_x = cos(theta+(r-l)/2/D)*0.5*(r+l); */
  /*  delta_y = sin(theta+(r-l)/2/D)*0.5*(r+l); */
  /*  delta_theta = (l-r)/D; */
  /*  z = [delta_x;delta_y;-delta_theta]; */
  /* 'ekf7_odom3_mu:28' H = [eye(3),zeros(3,4)]; */
  /* 'ekf7_odom3_mu:29' H(3,3) = 0; */
  /* 'ekf7_odom3_mu:30' H(3,7) = 1; */
  /* 'ekf7_odom3_mu:31' K = P * H'/((H * P * H' + R )); */
  for (rtemp = 0; rtemp < 7; rtemp++) {
    for (k = 0; k < 3; k++) {
      b_c[rtemp + 7 * k] = 0.0F;
      for (r1 = 0; r1 < 7; r1++) {
        b_c[rtemp + 7 * k] += P[rtemp + 7 * r1] * (float)b[r1 + 7 * k];
      }
    }
  }

  for (rtemp = 0; rtemp < 3; rtemp++) {
    for (k = 0; k < 7; k++) {
      a[rtemp + 3 * k] = 0.0F;
      for (r1 = 0; r1 < 7; r1++) {
        a[rtemp + 3 * k] += (float)b_a[rtemp + 3 * r1] * P[r1 + 7 * k];
      }
    }

    for (k = 0; k < 3; k++) {
      maxval = 0.0F;
      for (r1 = 0; r1 < 7; r1++) {
        maxval += a[rtemp + 3 * r1] * (float)b[r1 + 7 * k];
      }

      c[rtemp + 3 * k] = maxval + R[rtemp + 3 * k];
    }
  }

  r1 = 0;
  r2 = 1;
  r3 = 2;
  maxval = (float)fabs(c[0]);
  a21 = (float)fabs(c[1]);
  if (a21 > maxval) {
    maxval = a21;
    r1 = 1;
    r2 = 0;
  }

  if ((float)fabs(c[2]) > maxval) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }

  c[r2] /= c[r1];
  c[r3] /= c[r1];
  c[3 + r2] -= c[r2] * c[3 + r1];
  c[3 + r3] -= c[r3] * c[3 + r1];
  c[6 + r2] -= c[r2] * c[6 + r1];
  c[6 + r3] -= c[r3] * c[6 + r1];
  if ((float)fabs(c[3 + r3]) > (float)fabs(c[3 + r2])) {
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }

  c[3 + r3] /= c[3 + r2];
  c[6 + r3] -= c[3 + r3] * c[6 + r2];
  for (k = 0; k < 7; k++) {
    K[k + 7 * r1] = b_c[k] / c[r1];
    K[k + 7 * r2] = b_c[7 + k] - K[k + 7 * r1] * c[3 + r1];
    K[k + 7 * r3] = b_c[14 + k] - K[k + 7 * r1] * c[6 + r1];
    K[k + 7 * r2] /= c[3 + r2];
    K[k + 7 * r3] -= K[k + 7 * r2] * c[6 + r2];
    K[k + 7 * r3] /= c[6 + r3];
    K[k + 7 * r2] -= K[k + 7 * r3] * c[3 + r3];
    K[k + 7 * r1] -= K[k + 7 * r3] * c[r3];
    K[k + 7 * r1] -= K[k + 7 * r2] * c[r2];
  }

  /* 'ekf7_odom3_mu:32' stateVec = stateVec + K*(z - H*stateVec); */
  b_delta_x[0] = delta_x;
  b_delta_x[1] = delta_y;
  b_delta_x[2] = -delta_theta;
  for (rtemp = 0; rtemp < 3; rtemp++) {
    c_a[rtemp] = 0.0F;
    for (k = 0; k < 7; k++) {
      c_a[rtemp] += (float)b_a[rtemp + 3 * k] * stateVec[k];
    }

    c_delta_x[rtemp] = b_delta_x[rtemp] - c_a[rtemp];
  }

  /*  P矩阵更新 */
  /* 'ekf7_odom3_mu:34' P = (eye(length(P)) - K * H)* P; */
  for (rtemp = 0; rtemp < 7; rtemp++) {
    maxval = 0.0F;
    for (k = 0; k < 3; k++) {
      maxval += K[rtemp + 7 * k] * c_delta_x[k];
    }

    stateVec[rtemp] += maxval;
    for (k = 0; k < 7; k++) {
      maxval = 0.0F;
      for (r1 = 0; r1 < 3; r1++) {
        maxval += K[rtemp + 7 * r1] * (float)b_a[r1 + 3 * k];
      }

      fv1[rtemp + 7 * k] = (float)iv0[rtemp + 7 * k] - maxval;
    }

    for (k = 0; k < 7; k++) {
      fv2[rtemp + 7 * k] = 0.0F;
      for (r1 = 0; r1 < 7; r1++) {
        fv2[rtemp + 7 * k] += fv1[rtemp + 7 * r1] * P[r1 + 7 * k];
      }
    }
  }

  for (rtemp = 0; rtemp < 7; rtemp++) {
    for (k = 0; k < 7; k++) {
      P[k + 7 * rtemp] = fv2[k + 7 * rtemp];
    }
  }

  /*  保证P阵为对称阵，防止由于矩阵病态性引起滤波器发散 */
  /* 'ekf7_odom3_mu:37' P = 0.5*(P + transpose(P)); */
  for (rtemp = 0; rtemp < 7; rtemp++) {
    for (k = 0; k < 7; k++) {
      fv1[k + 7 * rtemp] = 0.5F * (P[k + 7 * rtemp] + P[rtemp + 7 * k]);
    }
  }

  for (rtemp = 0; rtemp < 7; rtemp++) {
    for (k = 0; k < 7; k++) {
      P[k + 7 * rtemp] = fv1[k + 7 * rtemp];
    }
  }

  /*  % % 保证P阵对角线元素为正 */
  /* 'ekf7_odom3_mu:41' for i=1:length(P) */
  for (i = 0; i < 7; i++) {
    /* 'ekf7_odom3_mu:42' if P(i,i) < 0 */
    if (P[i + 7 * i] < 0.0F) {
      /* 'ekf7_odom3_mu:43' P(i,i) = 0; */
      P[i + 7 * i] = 0.0F;
    }
  }

  /* 状态量更新     */
  /* 'ekf7_odom3_mu:48' pos = pos_prev + stateVec(1:3); */
  /* 'ekf7_odom3_mu:49' vel = vel_prev + stateVec(4:6); */
  for (i = 0; i < 3; i++) {
    pos[i] = pos_prev[i] + stateVec[i];
    vel[i] = vel_prev[i] + stateVec[i + 3];
  }

  /* 'ekf7_odom3_mu:50' yaw = yaw + stateVec(7); */
  *yaw += stateVec[6];

  /* 'ekf7_odom3_mu:51' yaw = mod(yaw,2*pi); */
//  maxval = *yaw / 6.28318548F;
//  maxval = (maxval - (float)floor(maxval)) * 6.28318548F;
//  *yaw = maxval;
}
