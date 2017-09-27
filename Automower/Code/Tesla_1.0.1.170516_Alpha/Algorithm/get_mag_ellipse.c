/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: get_ellipse
  Author	  	: Rui.fang
  Version  		: V1.0.0      
  Date		  	: 2017/06/07
  Description	: filter
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/
#include <rtthread.h>
#include <stdio.h>
#include "global.h"
#include "math.h"
#include "hardware.h"

/* Function Definitions */

/*
 * Arguments    : const float x[500]
 * Return Type  : float
 */
static s16 mean300(s16 x[mag_cali_len])
{
  float y;
  int k;
  y = x[0];
  for (k = 0; k < mag_cali_len-1; k++) {
    y += x[k + 1];
  }
  y /= mag_cali_len;
  return y;
}

/*
 * Arguments    : const float a[500]
 *                float y[500]
 * Return Type  : void
 */
static void power300(s16 a[mag_cali_len], float y[mag_cali_len])
{
  int k;
  for (k = 0; k < mag_cali_len; k++) {
    y[k] = a[k] * a[k];
  }
}

/*
 * Arguments    : const float x[2500]
 *                float y[5]
 * Return Type  : void
 */
static void sum300(float x[mag_cali_len*5], float y[5])
{
  int i,k;
  int xoffset;
  float s;
  
  for (i = 0; i < 5; i++) {
    xoffset = i * mag_cali_len;
    s = x[xoffset];
    for (k = 0; k < mag_cali_len-1; k++) {
      s += x[xoffset + k + 1];
    }
    y[i] = s;
  }
}


/*
 * Arguments    : float x[500]
 *                float y[500]
 *                struct0_T *mag_cali_ellipse
 * Return Type  : void
 */
ellipse get_mag_ellipse(s16 x[mag_cali_len],s16 y[mag_cali_len])
{
  s16 value,b_value;
	float c_value[5];
  float mean_x,mean_y;
  float fv0[mag_cali_len],fv1[mag_cali_len];
	float X[mag_cali_len*5];
  signed char ipiv[5];
  int i0,j,jy,c,jAcol,ix,b_j,ijA;
  float a[5],A[25];
  float temp,s,e,d,b_c,b_a;
  float b_temp,orientation_rad,cos_phi,sin_phi,test;
  int status;
  float X0,Y0,F,b_cos_phi[4],b_X0[2],P_in[2],b_s,c_s;
	ellipse mag_cali_ellipse0;

  //elseif(cnt==len){
  /* ':3' single status; */
  /*  initializeget_ellipse */
  /* 'get_ellipse:5' orientation_tolerance = 1e-3; */
  /*  prepare vectors, must be column vectors */
  /* 'get_ellipse:8' x = x(:); */
  /* 'get_ellipse:9' y = y(:); */
  /*  remove bias of the ellipse - to make matrix inversion more accurate. (will be added later on). */
  /* 'get_ellipse:12' mean_x = mean(x); */
  value = mean300(x);
  mean_x = value;

  /* 'get_ellipse:13' mean_y = mean(y); */
  b_value = mean300(y);
  mean_y = b_value;

  /* 'get_ellipse:14' x = x-mean_x; */
  /* 'get_ellipse:15' y = y-mean_y; */
  for (i0 = 0; i0 < mag_cali_len; i0++) {
    x[i0] -= value;
    y[i0] -= b_value;
  }

  /*  the estimation for the conic equation of the ellipse */
  /* 'get_ellipse:18' X = [x.^2, x.*y, y.^2, x, y ]; */
  power300(x, fv0);
  power300(y, fv1);
  for (i0 = 0; i0 < mag_cali_len; i0++) {
    X[i0] = fv0[i0];
    X[mag_cali_len + i0] = x[i0] * y[i0];
    X[mag_cali_len*2 + i0] = fv1[i0];
    X[mag_cali_len*3 + i0] = x[i0];
    X[mag_cali_len*4 + i0] = y[i0];
  }

  /* 'get_ellipse:19' a = sum(X)/(X'*X); */
  sum300(X, c_value);
  for (i0 = 0; i0 < 5; i0++) {
    for (jy = 0; jy < 5; jy++) {
      A[i0 + 5 * jy] = 0.0F;
      for (jAcol = 0; jAcol < mag_cali_len; jAcol++) {
        A[i0 + 5 * jy] += X[jAcol + mag_cali_len * i0] * X[jAcol + mag_cali_len * jy];
      }
    }
    ipiv[i0] = (signed char)(1 + i0);
  }

  for (j = 0; j < 4; j++) {
    c = j * 6;
    jAcol = 0;
    ix = c;
    temp = (float)fabs(A[c]);
    for (jy = 2; jy <= 5 - j; jy++) {
      ix++;
      s = (float)fabs(A[ix]);
      if (s > temp) {
        jAcol = jy - 1;
        temp = s;
      }
    }

    if (A[c + jAcol] != 0.0F) {
      if (jAcol != 0) {
        ipiv[j] = (signed char)(j + jAcol + 1);
        ix = j;
        jAcol += j;
        for (jy = 0; jy < 5; jy++) {
          temp = A[ix];
          A[ix] = A[jAcol];
          A[jAcol] = temp;
          ix += 5;
          jAcol += 5;
        }
      }

      i0 = c - j + 5;
      for (jAcol = c + 1; jAcol + 1 <= i0; jAcol++) {
        A[jAcol] /= A[c];
      }
    }

    jAcol = c;
    jy = c + 5;
    for (b_j = 1; b_j <= 4 - j; b_j++) {
      temp = A[jy];
      if (A[jy] != 0.0F) {
        ix = c + 1;
        i0 = jAcol - j + 10;
        for (ijA = 6 + jAcol; ijA + 1 <= i0; ijA++) {
          A[ijA] += A[ix] * -temp;
          ix++;
        }
      }

      jy += 5;
      jAcol += 5;
    }
  }

  for (i0 = 0; i0 < 5; i0++) {
    a[i0] = c_value[i0];
  }

  for (j = 0; j < 5; j++) {
    jAcol = 5 * j;
    for (jy = 0; jy + 1 <= j; jy++) {
      if (A[jy + jAcol] != 0.0F) {
        a[j] -= A[jy + jAcol] * a[jy];
      }
    }

    a[j] *= 1.0F / A[j + jAcol];
  }

  for (j = 4; j >= 0; j += -1) {
    jAcol = 5 * j;
    for (jy = j + 1; jy + 1 < 6; jy++) {
      if (A[jy + jAcol] != 0.0F) {
        a[j] -= A[jy + jAcol] * a[jy];
      }
    }
  }

  for (jAcol = 3; jAcol >= 0; jAcol += -1) {
    if (ipiv[jAcol] != jAcol + 1) {
      temp = a[jAcol];
      a[jAcol] = a[ipiv[jAcol] - 1];
      a[ipiv[jAcol] - 1] = temp;
    }
  }

  /*  extract parameters from the conic equation */
  /* 'get_ellipse:22' [a,b,c,d,e] = deal( a(1),a(2),a(3),a(4),a(5) ); */
  e = a[4];
  d = a[3];
  b_c = a[2];
  b_a = a[0];

  /*  remove the orientation from the ellipse */
  /* 'get_ellipse:25' if ( min(abs(b/a),abs(b/c)) > orientation_tolerance ) */
  temp = (float)fabs(a[1] / a[0]);
  s = (float)fabs(a[1] / a[2]);
  if (temp <= s) {
    b_temp = temp;
  } else {
    b_temp = s;
  }

  if (b_temp > 0.001F) {
    /* 'get_ellipse:27' orientation_rad = 1/2 * atan( b/(c-a) ); */
    orientation_rad = 0.5F * (float)atan(a[1] / (a[2] - a[0]));

    /* 'get_ellipse:28' cos_phi = cos( orientation_rad ); */
    s = (float)cos(orientation_rad);
    cos_phi = s;

    /* 'get_ellipse:29' sin_phi = sin( orientation_rad ); */
    temp = (float)sin(orientation_rad);
    sin_phi = temp;

    /* 'get_ellipse:30' [a,~,c,d,e] = deal(... */
    /* 'get_ellipse:31'         a*cos_phi^2 - b*cos_phi*sin_phi + c*sin_phi^2,... */
    /* 'get_ellipse:32'         0,... */
    /* 'get_ellipse:33'         a*sin_phi^2 + b*cos_phi*sin_phi + c*cos_phi^2,... */
    /* 'get_ellipse:34'         d*cos_phi - e*sin_phi,... */
    /* 'get_ellipse:35'         d*sin_phi + e*cos_phi ); */
    e = a[3] * temp + a[4] * s;
    d = a[3] * s - a[4] * temp;
    b_c = a[0] * (temp * temp) + a[1] * s * temp + a[2] * (s * s);
    b_a = a[0] * (s * s) - a[1] * s * temp + a[2] * (temp * temp);

    /* 'get_ellipse:30' ~ */
    /* 'get_ellipse:36' [mean_x,mean_y] = deal( ... */
    /* 'get_ellipse:37'         cos_phi*mean_x - sin_phi*mean_y,... */
    /* 'get_ellipse:38'         sin_phi*mean_x + cos_phi*mean_y ); */
    mean_y = temp * value + s * b_value;
    mean_x = s * value - temp * b_value;
  } else {
    /* 'get_ellipse:39' else */
    /* 'get_ellipse:40' orientation_rad = 0; */
    orientation_rad = 0.0F;

    /* 'get_ellipse:41' cos_phi = cos( orientation_rad ); */
    cos_phi = 1.0F;

    /* 'get_ellipse:42' sin_phi = sin( orientation_rad ); */
    sin_phi = 0.0F;
  }

  /*  check if conic equation represents an ellipse */
  /* 'get_ellipse:46' test = a*c; */
  test = b_a * b_c;

  /*  switch (1) */
  /*  case (test>0),  status = 1;%ellipse fount; */
  /*  case (test==0), status = 0;%'Parabola found';   */
  /*  case (test<0),  status = -1;%'Hyperbola found';  */
  /*  end */
  /* 'get_ellipse:52' if(test>0) */
  if (test > 0.0F) {
    /* 'get_ellipse:53' status = 1; */
    status = 1;
  } else {
    /* 'get_ellipse:54' else */
    /* 'get_ellipse:55' status = -1; */
    status = -1;
  }

  /*  if we found an ellipse return it's data */
  /* 'get_ellipse:59' if (test>0) */
  if (test > 0.0F) {
    /*  make sure coefficients are positive as required */
    /* 'get_ellipse:61' if (a<0) */
    if (b_a < 0.0F) {
      /* 'get_ellipse:61' [a,c,d,e] = deal( -a,-c,-d,-e ); */
      e = -e;
      d = -d;
      b_c = -b_c;
      b_a = -b_a;
    }

    /*  final ellipse parameters */
    /* 'get_ellipse:63' X0          = mean_x - d/2/a; */
    X0 = mean_x - d / 2.0F / b_a;

    /* 'get_ellipse:64' Y0          = mean_y - e/2/c; */
    Y0 = mean_y - e / 2.0F / b_c;

    /* 'get_ellipse:65' F           = 1 + (d^2)/(4*a) + (e^2)/(4*c); */
    F = 1.0F + d * d / (4.0F * b_a) + e * e / (4.0F * b_c);

    /* 'get_ellipse:66' [a,b]       = deal( sqrt( F/a ),sqrt( F/c ) ); */
    s = (float)sqrt(F / b_a);
    temp = (float)sqrt(F / b_c);

    /* 'get_ellipse:67' long_axis   = 2*max(a,b); */
    /* 'get_ellipse:68' short_axis  = 2*min(a,b); */
    /*  rotate the axes backwards to find the center point of the original TILTED ellipse */
    /* 'get_ellipse:71' R           = [ cos_phi sin_phi; -sin_phi cos_phi ]; */
    /* 'get_ellipse:72' P_in        = R * [X0;Y0]; */
    b_cos_phi[0] = cos_phi;
    b_cos_phi[2] = sin_phi;
    b_cos_phi[1] = -sin_phi;
    b_cos_phi[3] = cos_phi;
    b_X0[0] = X0;
    b_X0[1] = Y0;
    for (i0 = 0; i0 < 2; i0++) {
      P_in[i0] = 0.0F;
      for (jy = 0; jy < 2; jy++) {
        P_in[i0] += b_cos_phi[i0 + (jy << 1)] * b_X0[jy];
      }
    }

    mag_cali_ellipse0.a = s;
    mag_cali_ellipse0.b = temp;
    mag_cali_ellipse0.phi = orientation_rad;
    mag_cali_ellipse0.X0 = X0;
    mag_cali_ellipse0.Y0 = Y0;
    mag_cali_ellipse0.X0_in = P_in[0];
    mag_cali_ellipse0.Y0_in = P_in[1];
    if (s >= temp) {
      b_s = s;
    } else {
      b_s = temp;
    }

    mag_cali_ellipse0.long_axis = 2.0F * b_s;
    if (s <= temp) {
      c_s = s;
    } else {
      c_s = temp;
    }

    mag_cali_ellipse0.short_axis = 2.0F * c_s;
    mag_cali_ellipse0.status = (float)status;
		
  } else {
    mag_cali_ellipse0.a = 0.0F;
    mag_cali_ellipse0.b = 0.0F;
    mag_cali_ellipse0.phi = 0.0F;
    mag_cali_ellipse0.X0 = 0.0F;
    mag_cali_ellipse0.Y0 = 0.0F;
    mag_cali_ellipse0.X0_in = 0.0F;
    mag_cali_ellipse0.Y0_in = 0.0F;
    mag_cali_ellipse0.long_axis = 0.0F;
    mag_cali_ellipse0.short_axis = 0.0F;
    mag_cali_ellipse0.status = (float)status;
  }
	
	return mag_cali_ellipse0;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
