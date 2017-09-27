/* Include Files */
#include "insgps.h"

/* Function Definitions */
/*
 * Arguments    : float pos[3]
 *                float vel[3]
 *                float *yaw
 *                const float gps_pos_lla[3]
 *                const float gps_vel_ned[3]
 *                float stateVec[7]
 *                float P[49]
 *                const float pos_lla_ref[3]
 *                float gps_hAcc_m
 *                float gps_vAcc_m
 *                float gps_sAcc_m_s
 * Return Type  : void
 */
void ekf7_gps6_mu(float pos[3], float vel[3], float *yaw, float stateVec[7],float P[49], double pos_lla_ref[3], gpsx *gps)
{
	//gps data
	double gps_pos_lla[3];
	float gps_vel_ned[3],gps_hAcc_m, gps_vAcc_m, gps_sAcc_m_s;
  float v[6],d[36];
  int j;
  float temp;
  float R_E,gps_pos_ecef[3],p_e_ref[3],delta_xyz[3];
  float enu[3],a[42],A[36];
  signed char ipiv[6];
  int k,kBcol,c,jp,ix,ijA,jAcol;
  float K[42],s,c_a[6],d_a[6];
	float fv1[49],fv2[49];
		
	
	static const signed char b_a[42] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
  static const signed char b[42] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 };
  static const signed char e_a[9] = { 0, 1, 0, 1, 0, 0, 0, 0, -1 };
  static const signed char iv0[49] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1 };
		
	/*get gps data */
	gps_pos_lla[0] = gps->latitude_rad;
	gps_pos_lla[1] = gps->longitude_rad;
	gps_pos_lla[2] = gps->hMSL_m;
	gps_vel_ned[0] = gps->velN_m_s;
	gps_vel_ned[1] = gps->velE_m_s;
	gps_vel_ned[2] = gps->velD_m_s;
	gps_hAcc_m = gps->hAcc_m;
	gps_vAcc_m = gps->vAcc_m;
	gps_sAcc_m_s = gps->sAcc_m_s;
		
  /* 'ekf7_gps6_mu:3' R_gps = diag([gps_hAcc_m,gps_hAcc_m,gps_vAcc_m,gps_sAcc_m_s,gps_sAcc_m_s,gps_sAcc_m_s]).^2; */
  v[0] = gps_hAcc_m * gps_hAcc_m;
  v[1] = gps_hAcc_m * gps_hAcc_m;
  v[2] = gps_vAcc_m * gps_vAcc_m;
  v[3] = gps_sAcc_m_s * gps_sAcc_m_s;
  v[4] = gps_sAcc_m_s * gps_sAcc_m_s;
  v[5] = gps_sAcc_m_s * gps_sAcc_m_s;
  memset(&d[0], 0, 36U * sizeof(float));
  for (j = 0; j < 6; j++) {
    d[j + 6 * j] = v[j];
  }

  /* 'ekf7_gps6_mu:5' gps_pos_ecef = wgslla2xyz(gps_pos_lla(1),gps_pos_lla(2),gps_pos_lla(3)); */
  /*  ellipsoid constants */
  /* 'wgslla2xyz:4' f = 1/298.257223563; */
  /*    WGS-84 Flattening. */
  /* 'wgslla2xyz:5' e = sqrt(f*(2 - f)); */
  /*    Eccentricity. */
  /* 'wgslla2xyz:6' R_0 = 6378137; */
  /*    WGS-84 equatorial radius (m).                             */
  /*    Compute East-West Radius of curvature at current position */
  /* 'wgslla2xyz:9' R_E = double(R_0/(sqrt(1 - (e*sin(lat))^2))); */
  temp = 0.0818191916F * (float)sin(gps_pos_lla[0]);
  R_E = 6.378137E+6F / (float)sqrt(1.0F - temp * temp);

  /*    Compute ECEF coordinates */
  /* 'wgslla2xyz:12' p_e = zeros(3,1); */
  /* 'wgslla2xyz:13' p_e(1) = (R_E + alt)*cos(lat)*cos(lon); */
  gps_pos_ecef[0] = (R_E + gps_pos_lla[2]) * (float)cos(gps_pos_lla[0]) * (float)
    cos(gps_pos_lla[1]);

  /* 'wgslla2xyz:14' p_e(2) = (R_E + alt)*cos(lat)*sin(lon); */
  gps_pos_ecef[1] = (R_E + gps_pos_lla[2]) * (float)cos(gps_pos_lla[0]) * (float)
    sin(gps_pos_lla[1]);

  /* 'wgslla2xyz:15' p_e(3) = ((1 - e^2)*R_E + alt)*sin(lat); */
  gps_pos_ecef[2] = (0.993305624F * R_E + gps_pos_lla[2]) * (float)sin
    (gps_pos_lla[0]);

  /* 'ekf7_gps6_mu:6' gps_pos_ned = wgsxyz2ned(gps_pos_ecef,pos_lla_ref(1),pos_lla_ref(2),pos_lla_ref(3)); */
  /*    Calculate the relative position vector */
  /* 'wgsxyz2ned:5' p_e_ref = wgslla2xyz(ref_lat, ref_lon, ref_alt); */
  /*  ellipsoid constants */
  /* 'wgslla2xyz:4' f = 1/298.257223563; */
  /*    WGS-84 Flattening. */
  /* 'wgslla2xyz:5' e = sqrt(f*(2 - f)); */
  /*    Eccentricity. */
  /* 'wgslla2xyz:6' R_0 = 6378137; */
  /*    WGS-84 equatorial radius (m).                             */
  /*    Compute East-West Radius of curvature at current position */
  /* 'wgslla2xyz:9' R_E = double(R_0/(sqrt(1 - (e*sin(lat))^2))); */
  temp = 0.0818191916F * (float)sin(pos_lla_ref[0]);
  R_E = 6.378137E+6F / (float)sqrt(1.0F - temp * temp);

  /*    Compute ECEF coordinates */
  /* 'wgslla2xyz:12' p_e = zeros(3,1); */
  /* 'wgslla2xyz:13' p_e(1) = (R_E + alt)*cos(lat)*cos(lon); */
  p_e_ref[0] = (R_E + pos_lla_ref[2]) * (float)cos(pos_lla_ref[0]) * (float)cos
    (pos_lla_ref[1]);

  /* 'wgslla2xyz:14' p_e(2) = (R_E + alt)*cos(lat)*sin(lon); */
  p_e_ref[1] = (R_E + pos_lla_ref[2]) * (float)cos(pos_lla_ref[0]) * (float)sin
    (pos_lla_ref[1]);

  /* 'wgslla2xyz:15' p_e(3) = ((1 - e^2)*R_E + alt)*sin(lat); */
  p_e_ref[2] = (0.993305624F * R_E + pos_lla_ref[2]) * (float)sin(pos_lla_ref[0]);

  /* 'wgsxyz2ned:6' delta_xyz = p_e - p_e_ref; */
  for (ijA = 0; ijA < 3; ijA++) {
    delta_xyz[ijA] = gps_pos_ecef[ijA] - p_e_ref[ijA];
  }

  /*    Calculate ENU coordinates */
  /* 'wgsxyz2ned:9' enu = zeros(3,1); */
  /* 'wgsxyz2ned:10' enu(1)= -sin(ref_lon)*delta_xyz(1) + cos(ref_lon)*delta_xyz(2); */
  enu[0] = -(float)sin(pos_lla_ref[1]) * delta_xyz[0] + (float)cos(pos_lla_ref[1])
    * delta_xyz[1];

  /* 'wgsxyz2ned:11' enu(2)= -sin(ref_lat)*cos(ref_lon)*delta_xyz(1) - ... */
  /* 'wgsxyz2ned:12'             sin(ref_lat)*sin(ref_lon)*delta_xyz(2) + ... */
  /* 'wgsxyz2ned:13'             cos(ref_lat)*delta_xyz(3); */
  enu[1] = -(float)sin(pos_lla_ref[0]) * (float)cos(pos_lla_ref[1]) * delta_xyz
    [0] - (float)sin(pos_lla_ref[0]) * (float)sin(pos_lla_ref[1]) * delta_xyz[1]
    + (float)cos(pos_lla_ref[0]) * delta_xyz[2];

  /* 'wgsxyz2ned:14' enu(3)= cos(ref_lat)*cos(ref_lon)*delta_xyz(1) + ... */
  /* 'wgsxyz2ned:15'             cos(ref_lat)*sin(ref_lon)*delta_xyz(2) + ...  */
  /* 'wgsxyz2ned:16'             sin(ref_lat)*delta_xyz(3); */
  enu[2] = (float)cos(pos_lla_ref[0]) * (float)cos(pos_lla_ref[1]) * delta_xyz[0]
    + (float)cos(pos_lla_ref[0]) * (float)sin(pos_lla_ref[1]) * delta_xyz[1] +
    (float)sin(pos_lla_ref[0]) * delta_xyz[2];

  /*    Define ENU to NED rotation matrix */
  /* 'wgsxyz2ned:19' C = [0 1 0;1 0 0;0 0 -1]; */
  /*    From NED position vector */
  /* 'wgsxyz2ned:22' ned = C*enu; */
  /* 'ekf7_gps6_mu:7' posInnov = gps_pos_ned - pos; */
  /* 'ekf7_gps6_mu:8' velInnov = gps_vel_ned - vel; */
  /* 'ekf7_gps6_mu:9' z = [posInnov; velInnov]; */
  /* 'ekf7_gps6_mu:11' H = [eye(6),zeros(6,1)]; */
  /* 'ekf7_gps6_mu:12' K = P * H'/((H * P * H' + R_gps )); */
  for (k = 0; k < 6; k++) {
    for (kBcol = 0; kBcol < 7; kBcol++) {
      a[k + 6 * kBcol] = 0.0F;
      for (jp = 0; jp < 7; jp++) {
        a[k + 6 * kBcol] += (float)b_a[k + 6 * jp] * P[jp + 7 * kBcol];
      }
    }

    for (kBcol = 0; kBcol < 6; kBcol++) {
      temp = 0.0F;
      for (jp = 0; jp < 7; jp++) {
        temp += a[k + 6 * jp] * (float)b[jp + 7 * kBcol];
      }

      A[k + 6 * kBcol] = temp + d[k + 6 * kBcol] * 0.2F;
    }

    ipiv[k] = (signed char)(1 + k);
  }

  for (j = 0; j < 5; j++) {
    c = j * 7;
    kBcol = 0;
    ix = c;
    temp = (float)fabs(A[c]);
    for (k = 2; k <= 6 - j; k++) {
      ix++;
      s = (float)fabs(A[ix]);
      if (s > temp) {
        kBcol = k - 1;
        temp = s;
      }
    }

    if (A[c + kBcol] != 0.0F) {
      if (kBcol != 0) {
        ipiv[j] = (signed char)(j + kBcol + 1);
        ix = j;
        kBcol += j;
        for (k = 0; k < 6; k++) {
          temp = A[ix];
          A[ix] = A[kBcol];
          A[kBcol] = temp;
          ix += 6;
          kBcol += 6;
        }
      }

      k = c - j + 6;
      for (ijA = c + 1; ijA + 1 <= k; ijA++) {
        A[ijA] /= A[c];
      }
    }

    jp = c;
    jAcol = c + 6;
    for (kBcol = 1; kBcol <= 5 - j; kBcol++) {
      temp = A[jAcol];
      if (A[jAcol] != 0.0F) {
        ix = c + 1;
        k = jp - j + 12;
        for (ijA = 7 + jp; ijA + 1 <= k; ijA++) {
          A[ijA] += A[ix] * -temp;
          ix++;
        }
      }

      jAcol += 6;
      jp += 6;
    }
  }

  for (k = 0; k < 7; k++) {
    for (kBcol = 0; kBcol < 6; kBcol++) {
      K[k + 7 * kBcol] = 0.0F;
      for (jp = 0; jp < 7; jp++) {
        K[k + 7 * kBcol] += P[k + 7 * jp] * (float)b[jp + 7 * kBcol];
      }
    }
  }

  for (j = 0; j < 6; j++) {
    jp = 7 * j;
    jAcol = 6 * j;
    for (k = 1; k <= j; k++) {
      kBcol = 7 * (k - 1);
      if (A[k + jAcol - 1] != 0.0F) {
        for (ijA = 0; ijA < 7; ijA++) {
          K[ijA + jp] -= A[k + jAcol - 1] * K[ijA + kBcol];
        }
      }
    }

    temp = 1.0F / A[j + jAcol];
    for (ijA = 0; ijA < 7; ijA++) {
      K[ijA + jp] *= temp;
    }
  }

  for (j = 5; j >= 0; j += -1) {
    jp = 7 * j;
    jAcol = 6 * j - 1;
    for (k = j + 2; k < 7; k++) {
      kBcol = 7 * (k - 1);
      if (A[k + jAcol] != 0.0F) {
        for (ijA = 0; ijA < 7; ijA++) {
          K[ijA + jp] -= A[k + jAcol] * K[ijA + kBcol];
        }
      }
    }
  }

  for (kBcol = 4; kBcol >= 0; kBcol += -1) {
    if (ipiv[kBcol] != kBcol + 1) {
      jp = ipiv[kBcol] - 1;
      for (jAcol = 0; jAcol < 7; jAcol++) {
        temp = K[jAcol + 7 * kBcol];
        K[jAcol + 7 * kBcol] = K[jAcol + 7 * jp];
        K[jAcol + 7 * jp] = temp;
      }
    }
  }

  /* 'ekf7_gps6_mu:13' stateVec = stateVec + K*(z - H*stateVec); */
  for (k = 0; k < 3; k++) {
    temp = 0.0F;
    for (kBcol = 0; kBcol < 3; kBcol++) {
      temp += (float)e_a[k + 3 * kBcol] * enu[kBcol];
    }

    v[k] = temp - pos[k];
    v[k + 3] = gps_vel_ned[k] - vel[k];
  }

  for (k = 0; k < 6; k++) {
    c_a[k] = 0.0F;
    for (kBcol = 0; kBcol < 7; kBcol++) {
      c_a[k] += (float)b_a[k + 6 * kBcol] * stateVec[kBcol];
    }

    d_a[k] = v[k] - c_a[k];
  }

  /*  P矩阵更新 */
  /* 'ekf7_gps6_mu:16' P = (eye(length(P)) - K * H)* P; */
  for (k = 0; k < 7; k++) {
    temp = 0.0F;
    for (kBcol = 0; kBcol < 6; kBcol++) {
      temp += K[k + 7 * kBcol] * d_a[kBcol];
    }

    stateVec[k] += temp;
    for (kBcol = 0; kBcol < 7; kBcol++) {
      temp = 0.0F;
      for (jp = 0; jp < 6; jp++) {
        temp += K[k + 7 * jp] * (float)b_a[jp + 6 * kBcol];
      }

      fv1[k + 7 * kBcol] = (float)iv0[k + 7 * kBcol] - temp;
    }

    for (kBcol = 0; kBcol < 7; kBcol++) {
      fv2[k + 7 * kBcol] = 0.0F;
      for (jp = 0; jp < 7; jp++) {
        fv2[k + 7 * kBcol] += fv1[k + 7 * jp] * P[jp + 7 * kBcol];
      }
    }
  }

  for (k = 0; k < 7; k++) {
    for (kBcol = 0; kBcol < 7; kBcol++) {
      P[kBcol + 7 * k] = fv2[kBcol + 7 * k];
    }
  }

  /*  保证P阵为对称阵，防止由于矩阵病态性引起滤波器发散 */
  /* 'ekf7_gps6_mu:19' P = 0.5*(P + transpose(P)); */
  for (k = 0; k < 7; k++) {
    for (kBcol = 0; kBcol < 7; kBcol++) {
      fv1[kBcol + 7 * k] = 0.5F * (P[kBcol + 7 * k] + P[k + 7 * kBcol]);
    }
  }

  for (k = 0; k < 7; k++) {
    for (kBcol = 0; kBcol < 7; kBcol++) {
      P[kBcol + 7 * k] = fv1[kBcol + 7 * k];
    }
  }

  /*  % % 保证P阵对角线元素为正 */
  /* 'ekf7_gps6_mu:22' for i=1:length(P) */
  for (ijA = 0; ijA < 7; ijA++) {
    /* 'ekf7_gps6_mu:23' if P(i,i) < 0 */
    if (P[ijA + 7 * ijA] < 0.0F) {
      /* 'ekf7_gps6_mu:24' P(i,i) = 0; */
      P[ijA + 7 * ijA] = 0.0F;
    }
  }

  /* 状态量更新     */
  /* 'ekf7_gps6_mu:29' pos = pos + stateVec(1:3); */
  /* 'ekf7_gps6_mu:30' vel = vel + stateVec(4:6); */
  for (k = 0; k < 3; k++) {
    pos[k] += stateVec[k];
    vel[k] += stateVec[3 + k];
  }

  /* 'ekf7_gps6_mu:31' yaw = yaw + stateVec(7); */
  *yaw += stateVec[6];

  /* 'ekf7_gps6_mu:32' yaw = mod(yaw,2*pi); */
  *yaw /= 6.28318548F;
  *yaw = (*yaw - (float)floor(*yaw)) * 6.28318548F;
}
