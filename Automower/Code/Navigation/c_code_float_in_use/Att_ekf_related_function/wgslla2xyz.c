/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Function Definitions */

/*
 * ellipsoid constants
 * Arguments    : double lat_rad
 *                double lon_rad
 *                double alt_m
 *                double p_e[3]
 * Return Type  : void
 */
void wgslla2xyz(double lat_rad, double lon_rad, double alt_m, double p_e[3])
{
  double a;
  double R_E;

  /* 'wgslla2xyz:5' f = 1/298.257223563; */
  /*    WGS-84 Flattening. */
  /* 'wgslla2xyz:6' e = sqrt(f*(2 - f)); */
  /*    Eccentricity. */
  /* 'wgslla2xyz:7' R_0 = 6378137; */
  /*    WGS-84 equatorial radius (m).                             */
  /*    Compute East-West Radius of curvature at current position */
  /* 'wgslla2xyz:10' R_E = R_0/(sqrt(1 - (e*sin(lat_rad))^2)); */
  a = 0.081819190842621486 * sin(lat_rad);
  R_E = 6.378137E+6 / sqrt(1.0 - a * a);

  /*    Compute ECEF coordinates */
  /* 'wgslla2xyz:13' p_e = zeros(3,1); */
  /* 'wgslla2xyz:14' p_e(1) = (R_E + alt_m)*cos(lat_rad)*cos(lon_rad); */
  p_e[0] = (R_E + alt_m) * cos(lat_rad) * cos(lon_rad);

  /* 'wgslla2xyz:15' p_e(2) = (R_E + alt_m)*cos(lat_rad)*sin(lon_rad); */
  p_e[1] = (R_E + alt_m) * cos(lat_rad) * sin(lon_rad);

  /* 'wgslla2xyz:16' p_e(3) = ((1 - e^2)*R_E + alt_m)*sin(lat_rad); */
  p_e[2] = (0.99330562000985867 * R_E + alt_m) * sin(lat_rad);
}
