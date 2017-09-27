/* Include Files */
#include "imu.h"

/* Function Definitions */
/*
 * Input:quaternion 4*1
 * Output:euler angle 3*1
 * Arguments    : float q[4]
 *                float eul[3]
 * Return Type  : void
 */
void quat2eulf(float q[4], float eul[3])
{
  float x;
  int i;
  float aSinInput;
  float a,b;

  /*  Normalize the quaternions */
  /* 'quat2eulf:8' q = q/sqrt(q(1)*q(1)+q(2)*q(2)+q(3)*q(3)+q(4)*q(4)); */
  x = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
  for (i = 0; i < 4; i++) {
    q[i] /= x;
  }

  /* 'quat2eulf:9' qw = q(1); */
  /* 'quat2eulf:10' qx = q(2); */
  /* 'quat2eulf:11' qy = q(3); */
  /* 'quat2eulf:12' qz = q(4); */
  /* 'quat2eulf:14' aSinInput = -2*(qx.*qz-qw.*qy); */
  aSinInput = -2.0 * (q[1] * q[3] - q[0] * q[2]);

  /* 'quat2eulf:15' aSinInput(aSinInput > 1) = 1; */
  x = aSinInput;
  for (i = 0; i < 1; i++) {
    if (aSinInput > 1.0) {
      x = 1.0;
    }
  }

  /* 'quat2eulf:17' eul = [ atan2( 2*(qx.*qy+qw.*qz), qw.^2 + qx.^2 - qy.^2 - qz.^2 ); ... */
  /* 'quat2eulf:18'         asin( aSinInput );... */
  /* 'quat2eulf:19'         atan2( 2*(qy.*qz+qw.*qx), qw.^2 - qx.^2 - qy.^2 + qz.^2 )]; */
  a = 2.0 * (q[1] * q[2] + q[0] * q[3]);
  b = q[0] * q[0] + q[1] * q[1] -q[2] * q[2] - q[3] * q[3];
  eul[0] = atan2(a, b);
  eul[1] = asin(x);

  a = 2.0 * (q[2] * q[3] + q[0] * q[1]);
  b = q[0] * q[0] - q[1] * q[1] -q[2] * q[2] + q[3] * q[3];	  
  eul[2] = atan2(a, b);


  /*  Check for complex numbers */
  /* 'quat2eulf:22' if ~isreal(eul) */
}

