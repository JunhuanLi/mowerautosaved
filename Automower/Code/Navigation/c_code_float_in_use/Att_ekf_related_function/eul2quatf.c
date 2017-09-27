/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Function Definitions */
/*
 * Input:euler angle 3*1
 * Output:quaternion 4*1
 *  Compute sines and cosines of half angles
 * Arguments    : const float eul[3]
 *                float q[4]
 * Return Type  : void
 */
void eul2quatf(float eul[3], float q[4])
{
  float c[3];
  float s[3];
  int k;

  /* 'eul2quatf:8' c = cos(eul/2); */
  /* 'eul2quatf:9' s = sin(eul/2); */
  for (k = 0; k < 3; k++) {
    c[k] = cos(eul[k] / 2.0);
    s[k] = sin(eul[k] / 2.0);
  }

  q[0] = c[0] * c[1] * c[2] + s[0] * s[1] * s[2];
  q[1] = c[0] * c[1] * s[2] - s[0] * s[1] * c[2];
  q[2] = c[0] * s[1] * c[2] + s[0] * c[1] * s[2];
  q[3] = s[0] * c[1] * c[2] - c[0] * s[1] * s[2];
}

