/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Function Definitions */

/*
 * Calculate the relative position vector
 * Arguments    : const double p_e[3]
 *                double ref_lat
 *                double ref_lon
 *                double ref_alt
 *                double ned[3]
 * Return Type  : void
 */
void wgsxyz2ned(double p_e[3], double ref_lat, double ref_lon, double
                ref_alt, double ned[3])
{
  double a;
  double R_E;
  double p_e_ref[3];
  double delta_xyz[3];
  int i;
  double enu[3];
  int i0;
  static const signed char b_a[9] = { 0, 1, 0, 1, 0, 0, 0, 0, -1 };

  /* 'wgsxyz2ned:5' p_e_ref = wgslla2xyz(ref_lat, ref_lon, ref_alt); */
  /*  ellipsoid constants */
  /* 'wgslla2xyz:5' f = 1/298.257223563; */
  /*    WGS-84 Flattening. */
  /* 'wgslla2xyz:6' e = sqrt(f*(2 - f)); */
  /*    Eccentricity. */
  /* 'wgslla2xyz:7' R_0 = 6378137; */
  /*    WGS-84 equatorial radius (m).                             */
  /*    Compute East-West Radius of curvature at current position */
  /* 'wgslla2xyz:10' R_E = R_0/(sqrt(1 - (e*sin(lat))^2)); */
  a = 0.081819190842621486 * sin(ref_lat);
  R_E = 6.378137E+6 / sqrt(1.0 - a * a);

  /*    Compute ECEF coordinates */
  /* 'wgslla2xyz:13' p_e = zeros(3,1); */
  /* 'wgslla2xyz:14' p_e(1) = (R_E + alt)*cos(lat)*cos(lon); */
  p_e_ref[0] = (R_E + ref_alt) * cos(ref_lat) * cos(ref_lon);

  /* 'wgslla2xyz:15' p_e(2) = (R_E + alt)*cos(lat)*sin(lon); */
  p_e_ref[1] = (R_E + ref_alt) * cos(ref_lat) * sin(ref_lon);

  /* 'wgslla2xyz:16' p_e(3) = ((1 - e^2)*R_E + alt)*sin(lat); */
  p_e_ref[2] = (0.99330562000985867 * R_E + ref_alt) * sin(ref_lat);

  /* 'wgsxyz2ned:6' delta_xyz = p_e - p_e_ref; */
  for (i = 0; i < 3; i++) {
    delta_xyz[i] = p_e[i] - p_e_ref[i];
  }

  /*    Calculate ENU coordinates */
  /* 'wgsxyz2ned:9' enu = zeros(3,1); */
  /* 'wgsxyz2ned:10' enu(1)= -sin(ref_lon)*delta_xyz(1) + cos(ref_lon)*delta_xyz(2); */
  enu[0] = -sin(ref_lon) * delta_xyz[0] + cos(ref_lon) * delta_xyz[1];

  /* 'wgsxyz2ned:11' enu(2)= -sin(ref_lat)*cos(ref_lon)*delta_xyz(1) - ... */
  /* 'wgsxyz2ned:12'             sin(ref_lat)*sin(ref_lon)*delta_xyz(2) + ... */
  /* 'wgsxyz2ned:13'             cos(ref_lat)*delta_xyz(3); */
  enu[1] = -sin(ref_lat) * cos(ref_lon) * delta_xyz[0] - sin(ref_lat) * sin
    (ref_lon) * delta_xyz[1] + cos(ref_lat) * delta_xyz[2];

  /* 'wgsxyz2ned:14' enu(3)= cos(ref_lat)*cos(ref_lon)*delta_xyz(1) + ... */
  /* 'wgsxyz2ned:15'             cos(ref_lat)*sin(ref_lon)*delta_xyz(2) + ...  */
  /* 'wgsxyz2ned:16'             sin(ref_lat)*delta_xyz(3); */
  enu[2] = cos(ref_lat) * cos(ref_lon) * delta_xyz[0] + cos(ref_lat) * sin
    (ref_lon) * delta_xyz[1] + sin(ref_lat) * delta_xyz[2];

  /*    Define ENU to NED rotation matrix */
  /* 'wgsxyz2ned:19' C = [0 1 0;1 0 0;0 0 -1]; */
  /*    From NED position vector */
  /* 'wgsxyz2ned:22' ned = C*enu; */
  for (i = 0; i < 3; i++) {
    ned[i] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      ned[i] += (double)b_a[i + 3 * i0] * enu[i0];
    }
  }
}
