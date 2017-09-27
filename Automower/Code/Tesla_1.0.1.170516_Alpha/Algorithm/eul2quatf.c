/* Include Files */
#include "imu.h"
/* Function Definitions */

/*
 * Compute sines and cosines of half angles
 * Arguments    : const float eul[3]
 *                float q[4]
 * Return Type  : void
 */
void eul2quatf(float eul[3], float q[4])
{
  float c[3];
  float s[3];
  int k;

  /* 'eul2quatf:5' c = cos(eul/2); */
  /* 'eul2quatf:6' s = sin(eul/2); */
  for (k = 0; k < 3; k++) {
    c[k] = (float)cos(eul[k] / 2.0F);
    s[k] = (float)sin(eul[k] / 2.0F);
  }

  /*  Construct quaternion */
  /* 'eul2quatf:9' q = [c(1).*c(2).*c(3)+s(1).*s(2).*s(3);... */
  /* 'eul2quatf:10'      c(1).*c(2).*s(3)-s(1).*s(2).*c(3);... */
  /* 'eul2quatf:11'      c(1).*s(2).*c(3)+s(1).*c(2).*s(3);... */
  /* 'eul2quatf:12'      s(1).*c(2).*c(3)-c(1).*s(2).*s(3)]; */
  q[0] = c[0] * c[1] * c[2] + s[0] * s[1] * s[2];
  q[1] = c[0] * c[1] * s[2] - s[0] * s[1] * c[2];
  q[2] = c[0] * s[1] * c[2] + s[0] * c[1] * s[2];
  q[3] = s[0] * c[1] * c[2] - c[0] * s[1] * s[2];
}