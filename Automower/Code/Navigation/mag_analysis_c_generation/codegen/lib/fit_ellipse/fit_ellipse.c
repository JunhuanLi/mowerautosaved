/*
 * File: fit_ellipse.c
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 04-Jul-2017 16:45:48
 */

/* Include Files */
#include "fit_ellipse.h"

/* Function Declarations */
static double mean(const double x[500]);
static void mrdivide(double A[5], const double B[25]);
static void power(const double a[500], double y[500]);
static void sum(const double x[2500], double y[5]);

/* Function Definitions */

/*
 * Arguments    : const double x[500]
 * Return Type  : double
 */
static double mean(const double x[500])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 499; k++) {
    y += x[k + 1];
  }

  y /= 500.0;
  return y;
}

/*
 * Arguments    : double A[5]
 *                const double B[25]
 * Return Type  : void
 */
static void mrdivide(double A[5], const double B[25])
{
  double b_A[25];
  signed char ipiv[5];
  int i3;
  int j;
  int c;
  int jAcol;
  int ix;
  int jy;
  double temp;
  double s;
  int b_j;
  int ijA;
  memcpy(&b_A[0], &B[0], 25U * sizeof(double));
  for (i3 = 0; i3 < 5; i3++) {
    ipiv[i3] = (signed char)(1 + i3);
  }

  for (j = 0; j < 4; j++) {
    c = j * 6;
    jAcol = 0;
    ix = c;
    temp = fabs(b_A[c]);
    for (jy = 2; jy <= 5 - j; jy++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        jAcol = jy - 1;
        temp = s;
      }
    }

    if (b_A[c + jAcol] != 0.0) {
      if (jAcol != 0) {
        ipiv[j] = (signed char)(j + jAcol + 1);
        ix = j;
        jAcol += j;
        for (jy = 0; jy < 5; jy++) {
          temp = b_A[ix];
          b_A[ix] = b_A[jAcol];
          b_A[jAcol] = temp;
          ix += 5;
          jAcol += 5;
        }
      }

      i3 = c - j + 5;
      for (jAcol = c + 1; jAcol + 1 <= i3; jAcol++) {
        b_A[jAcol] /= b_A[c];
      }
    }

    jAcol = c;
    jy = c + 5;
    for (b_j = 1; b_j <= 4 - j; b_j++) {
      temp = b_A[jy];
      if (b_A[jy] != 0.0) {
        ix = c + 1;
        i3 = jAcol - j + 10;
        for (ijA = 6 + jAcol; ijA + 1 <= i3; ijA++) {
          b_A[ijA] += b_A[ix] * -temp;
          ix++;
        }
      }

      jy += 5;
      jAcol += 5;
    }
  }

  for (j = 0; j < 5; j++) {
    jAcol = 5 * j;
    for (jy = 0; jy + 1 <= j; jy++) {
      if (b_A[jy + jAcol] != 0.0) {
        A[j] -= b_A[jy + jAcol] * A[jy];
      }
    }

    A[j] *= 1.0 / b_A[j + jAcol];
  }

  for (j = 4; j >= 0; j += -1) {
    jAcol = 5 * j;
    for (jy = j + 1; jy + 1 < 6; jy++) {
      if (b_A[jy + jAcol] != 0.0) {
        A[j] -= b_A[jy + jAcol] * A[jy];
      }
    }
  }

  for (jAcol = 3; jAcol >= 0; jAcol += -1) {
    if (ipiv[jAcol] != jAcol + 1) {
      temp = A[jAcol];
      A[jAcol] = A[ipiv[jAcol] - 1];
      A[ipiv[jAcol] - 1] = temp;
    }
  }
}

/*
 * Arguments    : const double a[500]
 *                double y[500]
 * Return Type  : void
 */
static void power(const double a[500], double y[500])
{
  int k;
  for (k = 0; k < 500; k++) {
    y[k] = a[k] * a[k];
  }
}

/*
 * Arguments    : const double x[2500]
 *                double y[5]
 * Return Type  : void
 */
static void sum(const double x[2500], double y[5])
{
  int i;
  int xoffset;
  double s;
  int k;
  for (i = 0; i < 5; i++) {
    xoffset = i * 500;
    s = x[xoffset];
    for (k = 0; k < 499; k++) {
      s += x[xoffset + k + 1];
    }

    y[i] = s;
  }
}

/*
 * Arguments    : double x[500]
 *                double y[500]
 *                struct0_T *ellipse_t
 * Return Type  : void
 */
void fit_ellipse(double x[500], double y[500], struct0_T *ellipse_t)
{
  double mean_x;
  double mean_y;
  int i0;
  double dv0[500];
  double dv1[500];
  double X[2500];
  double dv2[5];
  double b_X[25];
  double a[5];
  int i1;
  double b_a;
  double c;
  int i2;
  double d;
  double e;
  double varargin_1;
  double varargin_2;
  double b_varargin_1;
  double orientation_rad;
  double cos_phi;
  double sin_phi;
  double test;
  int status;
  double X0;
  double Y0;
  double F;
  double b_cos_phi[4];
  double b_X0[2];
  double P_in[2];
  double b_varargin_2;
  double c_varargin_2;

  /* 'fit_ellipse:3' single status; */
  /*  initialize */
  /* 'fit_ellipse:5' orientation_tolerance = 1e-3; */
  /*  prepare vectors, must be column vectors */
  /* 'fit_ellipse:8' x = x(:); */
  /* 'fit_ellipse:9' y = y(:); */
  /*  remove bias of the ellipse - to make matrix inversion more accurate. (will be added later on). */
  /* 'fit_ellipse:12' mean_x = mean(x); */
  mean_x = mean(x);

  /* 'fit_ellipse:13' mean_y = mean(y); */
  mean_y = mean(y);

  /* 'fit_ellipse:14' x = x-mean_x; */
  /* 'fit_ellipse:15' y = y-mean_y; */
  for (i0 = 0; i0 < 500; i0++) {
    x[i0] -= mean_x;
    y[i0] -= mean_y;
  }

  /*  the estimation for the conic equation of the ellipse */
  /* 'fit_ellipse:18' X = [x.^2, x.*y, y.^2, x, y ]; */
  power(x, dv0);
  power(y, dv1);
  for (i0 = 0; i0 < 500; i0++) {
    X[i0] = dv0[i0];
    X[500 + i0] = x[i0] * y[i0];
    X[1000 + i0] = dv1[i0];
    X[1500 + i0] = x[i0];
    X[2000 + i0] = y[i0];
  }

  /* 'fit_ellipse:19' a = sum(X)/(X'*X); */
  sum(X, dv2);
  for (i0 = 0; i0 < 5; i0++) {
    a[i0] = dv2[i0];
    for (i1 = 0; i1 < 5; i1++) {
      b_X[i0 + 5 * i1] = 0.0;
      for (i2 = 0; i2 < 500; i2++) {
        b_X[i0 + 5 * i1] += X[i2 + 500 * i0] * X[i2 + 500 * i1];
      }
    }
  }

  mrdivide(a, b_X);

  /*  extract parameters from the conic equation */
  /* 'fit_ellipse:22' [a,b,c,d,e] = deal( a(1),a(2),a(3),a(4),a(5) ); */
  b_a = a[0];
  c = a[2];
  d = a[3];
  e = a[4];

  /*  remove the orientation from the ellipse */
  /* 'fit_ellipse:25' if ( min(abs(b/a),abs(b/c)) > orientation_tolerance ) */
  varargin_1 = fabs(a[1] / a[0]);
  varargin_2 = fabs(a[1] / a[2]);
  if (varargin_1 <= varargin_2) {
    b_varargin_1 = varargin_1;
  } else {
    b_varargin_1 = varargin_2;
  }

  if (b_varargin_1 > 0.001) {
    /* 'fit_ellipse:27' orientation_rad = 1/2 * atan( b/(c-a) ); */
    orientation_rad = 0.5 * atan(a[1] / (a[2] - a[0]));

    /* 'fit_ellipse:28' cos_phi = cos( orientation_rad ); */
    cos_phi = cos(orientation_rad);

    /* 'fit_ellipse:29' sin_phi = sin( orientation_rad ); */
    sin_phi = sin(orientation_rad);

    /* 'fit_ellipse:30' [a,~,c,d,e] = deal(... */
    /* 'fit_ellipse:31'         a*cos_phi^2 - b*cos_phi*sin_phi + c*sin_phi^2,... */
    /* 'fit_ellipse:32'         0,... */
    /* 'fit_ellipse:33'         a*sin_phi^2 + b*cos_phi*sin_phi + c*cos_phi^2,... */
    /* 'fit_ellipse:34'         d*cos_phi - e*sin_phi,... */
    /* 'fit_ellipse:35'         d*sin_phi + e*cos_phi ); */
    b_a = a[0] * (cos_phi * cos_phi) - a[1] * cos_phi * sin_phi + a[2] *
      (sin_phi * sin_phi);
    c = a[0] * (sin_phi * sin_phi) + a[1] * cos_phi * sin_phi + a[2] * (cos_phi *
      cos_phi);
    d = a[3] * cos_phi - a[4] * sin_phi;
    e = a[3] * sin_phi + a[4] * cos_phi;

    /* 'fit_ellipse:30' ~ */
    /* 'fit_ellipse:36' [mean_x,mean_y] = deal( ... */
    /* 'fit_ellipse:37'         cos_phi*mean_x - sin_phi*mean_y,... */
    /* 'fit_ellipse:38'         sin_phi*mean_x + cos_phi*mean_y ); */
    varargin_2 = sin_phi * mean_x + cos_phi * mean_y;
    mean_x = cos_phi * mean_x - sin_phi * mean_y;
    mean_y = varargin_2;
  } else {
    /* 'fit_ellipse:39' else */
    /* 'fit_ellipse:40' orientation_rad = 0; */
    orientation_rad = 0.0;

    /* 'fit_ellipse:41' cos_phi = cos( orientation_rad ); */
    cos_phi = 1.0;

    /* 'fit_ellipse:42' sin_phi = sin( orientation_rad ); */
    sin_phi = 0.0;
  }

  /*  check if conic equation represents an ellipse */
  /* 'fit_ellipse:46' test = a*c; */
  test = b_a * c;

  /*  switch (1) */
  /*  case (test>0),  status = 1;%ellipse fount; */
  /*  case (test==0), status = 0;%'Parabola found';   */
  /*  case (test<0),  status = -1;%'Hyperbola found';  */
  /*  end */
  /* 'fit_ellipse:52' if(test>0) */
  if (test > 0.0) {
    /* 'fit_ellipse:53' status = 1; */
    status = 1;
  } else {
    /* 'fit_ellipse:54' else */
    /* 'fit_ellipse:55' status = -1; */
    status = -1;
  }

  /*  if we found an ellipse return it's data */
  /* 'fit_ellipse:59' if (test>0) */
  if (test > 0.0) {
    /*  make sure coefficients are positive as required */
    /* 'fit_ellipse:62' if (a<0) */
    if (b_a < 0.0) {
      /* 'fit_ellipse:62' [a,c,d,e] = deal( -a,-c,-d,-e ); */
      b_a = -b_a;
      c = -c;
      d = -d;
      e = -e;
    }

    /*  final ellipse parameters */
    /* 'fit_ellipse:65' X0          = mean_x - d/2/a; */
    X0 = mean_x - d / 2.0 / b_a;

    /* 'fit_ellipse:66' Y0          = mean_y - e/2/c; */
    Y0 = mean_y - e / 2.0 / c;

    /* 'fit_ellipse:67' F           = 1 + (d^2)/(4*a) + (e^2)/(4*c); */
    F = 1.0 + d * d / (4.0 * b_a) + e * e / (4.0 * c);

    /* 'fit_ellipse:68' [a,b]       = deal( sqrt( F/a ),sqrt( F/c ) ); */
    varargin_2 = sqrt(F / b_a);
    varargin_1 = sqrt(F / c);

    /* 'fit_ellipse:69' long_axis   = 2*max(a,b); */
    /* 'fit_ellipse:70' short_axis  = 2*min(a,b); */
    /*  rotate the axes backwards to find the center point of the original TILTED ellipse */
    /* 'fit_ellipse:73' R           = [ cos_phi sin_phi; -sin_phi cos_phi ]; */
    /* 'fit_ellipse:74' P_in        = R * [X0;Y0]; */
    b_cos_phi[0] = cos_phi;
    b_cos_phi[2] = sin_phi;
    b_cos_phi[1] = -sin_phi;
    b_cos_phi[3] = cos_phi;
    b_X0[0] = X0;
    b_X0[1] = Y0;
    for (i0 = 0; i0 < 2; i0++) {
      P_in[i0] = 0.0;
      for (i1 = 0; i1 < 2; i1++) {
        P_in[i0] += b_cos_phi[i0 + (i1 << 1)] * b_X0[i1];
      }
    }

    /* 'fit_ellipse:75' X0_in       = P_in(1); */
    /* 'fit_ellipse:76' Y0_in       = P_in(2); */
    /*  pack ellipse into a structure */
    /* 'fit_ellipse:79' ellipse_t = struct( ... */
    /* 'fit_ellipse:80'         'a',a,... */
    /* 'fit_ellipse:81'         'b',b,... */
    /* 'fit_ellipse:82'         'phi',orientation_rad,... */
    /* 'fit_ellipse:83'         'X0',X0,... */
    /* 'fit_ellipse:84'         'Y0',Y0,... */
    /* 'fit_ellipse:85'         'X0_in',X0_in,... */
    /* 'fit_ellipse:86'         'Y0_in',Y0_in,... */
    /* 'fit_ellipse:87'         'long_axis',long_axis,... */
    /* 'fit_ellipse:88'         'short_axis',short_axis,... */
    /* 'fit_ellipse:89'         'status',status ); */
    ellipse_t->a = varargin_2;
    ellipse_t->b = varargin_1;
    ellipse_t->phi = orientation_rad;
    ellipse_t->X0 = X0;
    ellipse_t->Y0 = Y0;
    ellipse_t->X0_in = P_in[0];
    ellipse_t->Y0_in = P_in[1];
    if (varargin_2 >= varargin_1) {
      b_varargin_2 = varargin_2;
    } else {
      b_varargin_2 = varargin_1;
    }

    ellipse_t->long_axis = 2.0 * b_varargin_2;
    if (varargin_2 <= varargin_1) {
      c_varargin_2 = varargin_2;
    } else {
      c_varargin_2 = varargin_1;
    }

    ellipse_t->short_axis = 2.0 * c_varargin_2;
    ellipse_t->status = status;
  } else {
    /* 'fit_ellipse:90' else */
    /*  report an empty structure */
    /* 'fit_ellipse:92' ellipse_t = struct( ... */
    /* 'fit_ellipse:93'         'a',0,... */
    /* 'fit_ellipse:94'         'b',0,... */
    /* 'fit_ellipse:95'         'phi',0,... */
    /* 'fit_ellipse:96'         'X0',0,... */
    /* 'fit_ellipse:97'         'Y0',0,... */
    /* 'fit_ellipse:98'         'X0_in',0,... */
    /* 'fit_ellipse:99'         'Y0_in',0,... */
    /* 'fit_ellipse:100'         'long_axis',0,... */
    /* 'fit_ellipse:101'         'short_axis',0,... */
    /* 'fit_ellipse:102'         'status',status ); */
    ellipse_t->a = 0.0;
    ellipse_t->b = 0.0;
    ellipse_t->phi = 0.0;
    ellipse_t->X0 = 0.0;
    ellipse_t->Y0 = 0.0;
    ellipse_t->X0_in = 0.0;
    ellipse_t->Y0_in = 0.0;
    ellipse_t->long_axis = 0.0;
    ellipse_t->short_axis = 0.0;
    ellipse_t->status = status;
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fit_ellipse_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fit_ellipse_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for fit_ellipse.c
 *
 * [EOF]
 */
