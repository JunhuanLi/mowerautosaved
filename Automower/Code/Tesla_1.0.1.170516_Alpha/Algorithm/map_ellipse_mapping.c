/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: mag_ellipse_mapping
  Author		: Rui     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: filter
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/
/* Include Files */
#include "hardware.h"
#include "math.h"

/* Function Definitions 
 * Arguments : ellipse *ellipse_t
 *             float imu_mx
 *             float imu_my
 *             float imu_mz
 *             float mag_body[3]
 * Return Type: void
 */
 void mag_ellipse_mapping(ellipse *ellipse_t, T_mag *mag0,float mag_body[3])
{
  float b_c,c,v[2],d[4];  
	int i0,j;
  float fv0[4],fv1[2],fv2[2],b_ellipse_t[2];
	float t1,t2;
	
  t1 = (float)mag0->mag_x;
  t2 = (float)mag0->mag_y;

	//imu_mx = mag0.mag_x;
	//imu_my = mag0.mag_y;
	//imu_mz = mag0.mag_z;
	
  /*  %ellipse fitting */
  /* 'mag_ellipse_mapping:3' mag_body = zeros(3,1); */
  /* mapping to circle */
  /* 'mag_ellipse_mapping:5' phi = ellipse_t.phi; */
  /* 'mag_ellipse_mapping:6' B = [ellipse_t.X0;ellipse_t.Y0]; */
  /* 'mag_ellipse_mapping:7' Xf = max(1,ellipse_t.b/ellipse_t.a); */
  c = ellipse_t->b / ellipse_t->a;

  /* 'mag_ellipse_mapping:8' Yf = max(1,ellipse_t.a/ellipse_t.b); */
  b_c = ellipse_t->a / ellipse_t->b;

  /* 'mag_ellipse_mapping:9' T = diag([Xf,Yf]); */
  if (1.0F >= c) {
    v[0] = 1.0F;
  } else {
    v[0] = c;
  }

  if (1.0F >= b_c) {
    v[1] = 1.0F;
  } else {
    v[1] = b_c;
  }

  for (i0 = 0; i0 < 4; i0++) {
    d[i0] = 0.0F;
  }

  /*  T = diag([1,ellipse_t.a/ellipse_t.b]); */
  /* 'mag_ellipse_mapping:11' A = [cos(phi) -sin(phi);sin(phi) cos(phi)]; */
  /* 'mag_ellipse_mapping:13' mag_body(1:2) = T*((A * [imu_mx;imu_my]) - B); */
  fv0[0] = (float)cos(ellipse_t->phi);
  fv0[2] = -(float)sin(ellipse_t->phi);
  fv0[1] = (float)sin(ellipse_t->phi);
  fv0[3] = (float)cos(ellipse_t->phi);

	
//  b_ellipse_t[0] = ellipse_t->X0;
//  b_ellipse_t[1] = ellipse_t->Y0;
//  for (j = 0; j < 2; j++) {
//    d[j + (j << 1)] = v[j];
//    fv1[j] = 0.0F;
//    for (i0 = 0; i0 < 2; i0++) {
//      fv1[j] += fv0[j + (i0 << 1)] * b_imu_mx[i0];
//    }
//    fv2[j] = fv1[j] - b_ellipse_t[j];
//  }

//  for (i0 = 0; i0 < 2; i0++) {
//    v[i0] = 0.0F;
//    for (j = 0; j < 2; j++) {
//      v[i0] += d[i0 + (j << 1)] * fv2[j];
//    }
//    mag_body[i0] = v[i0];
//  }

  mag_body[0] = ((fv0[0]*t1+fv0[2]*t2)-ellipse_t->X0)*v[0];
  mag_body[1] = ((fv0[1]*t1+fv0[3]*t2)-ellipse_t->Y0)*v[1];
	mag_body[2] = mag0->mag_z;
}
