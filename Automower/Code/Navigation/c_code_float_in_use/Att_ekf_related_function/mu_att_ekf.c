

/* Function Definitions */
/*
 * Arguments    : float P[16]
 *                float acc_imu_g[3]
 *                float stateVec[4]
 *                float R[9]
 *                float P_updated[16]
 *                float stateVec_updated[4]
 * Return Type  : void
 */
void mu_att_ekf(float P[16], float acc_imu_g[3], float stateVec[4],
                float R[9], float P_updated[16], float stateVec_updated[4])
{
  float b_stateVec[12];
  float H[12];
  int rtemp;
  int k;
  float B[9];
  float y[12];
  int r1;
  int r2;
  int r3;
  float maxval;
  float a21;
  float K[12];
  float b_y;
  float t;
  float b_acc_imu_g[3];
  signed char I[16];
  float b_I[16];
  float c_I[16];
  int i;

  /* 'mu_att_ekf:4' H = -2*[-stateVec(3) stateVec(4) -stateVec(1) stateVec(2);... */
  /* 'mu_att_ekf:5'        stateVec(2) stateVec(1) stateVec(4) stateVec(3);... */
  /* 'mu_att_ekf:6'        stateVec(1) -stateVec(2) -stateVec(3) stateVec(4)]; */
  b_stateVec[0] = -stateVec[2];
  b_stateVec[3] = stateVec[3];
  b_stateVec[6] = -stateVec[0];
  b_stateVec[9] = stateVec[1];
  b_stateVec[1] = stateVec[1];
  b_stateVec[4] = stateVec[0];
  b_stateVec[7] = stateVec[3];
  b_stateVec[10] = stateVec[2];
  b_stateVec[2] = stateVec[0];
  b_stateVec[5] = -stateVec[1];
  b_stateVec[8] = -stateVec[2];
  b_stateVec[11] = stateVec[3];
  for (rtemp = 0; rtemp < 4; rtemp++) {
    for (k = 0; k < 3; k++) {
      H[k + 3 * rtemp] = -2.0 * b_stateVec[k + 3 * rtemp];
    }
  }

  /* kalman filter */
  /* 'mu_att_ekf:8' K = P * H'/((H * P * H' + R )); */
  for (rtemp = 0; rtemp < 4; rtemp++) {
    for (k = 0; k < 3; k++) {
      y[rtemp + (k << 2)] = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        y[rtemp + (k << 2)] += P[rtemp + (r1 << 2)] * H[k + 3 * r1];
      }
    }
  }

  for (rtemp = 0; rtemp < 3; rtemp++) {
    for (k = 0; k < 4; k++) {
      b_stateVec[rtemp + 3 * k] = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        b_stateVec[rtemp + 3 * k] += H[rtemp + 3 * r1] * P[r1 + (k << 2)];
      }
    }

    for (k = 0; k < 3; k++) {
      maxval = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        maxval += b_stateVec[rtemp + 3 * r1] * H[k + 3 * r1];
      }

      B[rtemp + 3 * k] = maxval + R[rtemp + 3 * k];
    }
  }

  r1 = 0;
  r2 = 1;
  r3 = 2;
  maxval = fabs(B[0]);
  a21 = fabs(B[1]);
  if (a21 > maxval) {
    maxval = a21;
    r1 = 1;
    r2 = 0;
  }

  if (fabs(B[2]) > maxval) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }

  B[r2] /= B[r1];
  B[r3] /= B[r1];
  B[3 + r2] -= B[r2] * B[3 + r1];
  B[3 + r3] -= B[r3] * B[3 + r1];
  B[6 + r2] -= B[r2] * B[6 + r1];
  B[6 + r3] -= B[r3] * B[6 + r1];
  if (fabs(B[3 + r3]) > fabs(B[3 + r2])) {
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }

  B[3 + r3] /= B[3 + r2];
  B[6 + r3] -= B[3 + r3] * B[6 + r2];
  for (k = 0; k < 4; k++) {
    K[k + (r1 << 2)] = y[k] / B[r1];
    K[k + (r2 << 2)] = y[4 + k] - K[k + (r1 << 2)] * B[3 + r1];
    K[k + (r3 << 2)] = y[8 + k] - K[k + (r1 << 2)] * B[6 + r1];
    K[k + (r2 << 2)] /= B[3 + r2];
    K[k + (r3 << 2)] -= K[k + (r2 << 2)] * B[6 + r2];
    K[k + (r3 << 2)] /= B[6 + r3];
    K[k + (r2 << 2)] -= K[k + (r3 << 2)] * B[3 + r3];
    K[k + (r1 << 2)] -= K[k + (r3 << 2)] * B[r3];
    K[k + (r1 << 2)] -= K[k + (r2 << 2)] * B[r2];
  }

  /* measurement */
  /* 'mu_att_ekf:10' measurement_acc = acc_imu_g/norm(acc_imu_g); */
  b_y = 0.0;
  maxval = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    a21 = fabs(acc_imu_g[k]);
    if (a21 > maxval) {
      t = maxval / a21;
      b_y = 1.0 + b_y * t * t;
      maxval = a21;
    } else {
      t = a21 / maxval;
      b_y += t * t;
    }
  }

  b_y = maxval * sqrt(b_y);

  /* state vector correction */
  /* 'mu_att_ekf:12' stateVec_updated = stateVec + K * (measurement_acc - H * stateVec); */
  for (rtemp = 0; rtemp < 3; rtemp++) {
    maxval = 0.0;
    for (k = 0; k < 4; k++) {
      maxval += H[rtemp + 3 * k] * stateVec[k];
    }

    b_acc_imu_g[rtemp] = acc_imu_g[rtemp] / b_y - maxval;
  }

  for (rtemp = 0; rtemp < 4; rtemp++) {
    maxval = 0.0;
    for (k = 0; k < 3; k++) {
      maxval += K[rtemp + (k << 2)] * b_acc_imu_g[k];
    }

    stateVec_updated[rtemp] = stateVec[rtemp] + maxval;
  }

  /* P matrix correction */
  /* 'mu_att_ekf:14' P = (eye(length(P)) - K * H)* P; */
  for (rtemp = 0; rtemp < 16; rtemp++) {
    I[rtemp] = 0;
  }

  for (k = 0; k < 4; k++) {
    I[k + (k << 2)] = 1;
  }

  for (rtemp = 0; rtemp < 4; rtemp++) {
    for (k = 0; k < 4; k++) {
      maxval = 0.0;
      for (r1 = 0; r1 < 3; r1++) {
        maxval += K[rtemp + (r1 << 2)] * H[r1 + 3 * k];
      }

      b_I[rtemp + (k << 2)] = (float)I[rtemp + (k << 2)] - maxval;
    }

    for (k = 0; k < 4; k++) {
      c_I[rtemp + (k << 2)] = 0.0;
      for (r1 = 0; r1 < 4; r1++) {
        c_I[rtemp + (k << 2)] += b_I[rtemp + (r1 << 2)] * P[r1 + (k << 2)];
      }
    }
  }

  for (rtemp = 0; rtemp < 4; rtemp++) {
    for (k = 0; k < 4; k++) {
      P[k + (rtemp << 2)] = c_I[k + (rtemp << 2)];
    }
  }

  /* guarantee P ,prevent filter diverse due to illness of P matrix */
  /* 'mu_att_ekf:16' P_updated = 0.5*(P + transpose(P)); */
  for (rtemp = 0; rtemp < 4; rtemp++) {
    for (k = 0; k < 4; k++) {
      P_updated[k + (rtemp << 2)] = 0.5 * (P[k + (rtemp << 2)] + P[rtemp + (k <<
        2)]);
    }
  }

  /* guarantee main diagonal elements positive */
  /* 'mu_att_ekf:18' for i=1:length(P) */
  for (i = 0; i < 4; i++) {
    /* 'mu_att_ekf:19' if P_updated(i,i) < 0 */
    if (P_updated[i + (i << 2)] < 0.0) {
      /* 'mu_att_ekf:20' P_updated(i,i) = 0; */
      P_updated[i + (i << 2)] = 0.0;
    }
  }
}


/*
 * File trailer for mu_att_ekf.c
 *
 * [EOF]
 */
