/*
 * File: normK.c
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 07-Sep-2017 17:30:01
 */

/* Include Files */
#include "normK.h"

/* Function Declarations */
static void b_xaxpy(int n, float a, const float x[12], int ix0, float y[4], int
                    iy0);
static float b_xnrm2(int n, const float x[3], int ix0);
static void c_xaxpy(int n, float a, const float x[4], int ix0, float y[12], int
                    iy0);
static void svd(float A[12], float U[3]);
static void xaxpy(int n, float a, int ix0, float y[12], int iy0);
static float xdotc(int n, const float x[12], int ix0, const float y[12], int iy0);
static float xnrm2(int n, const float x[12], int ix0);
static void xrotg(float *a, float *b, float *c, float *s);

/* Function Definitions */

/*
 * Arguments    : int n
 *                float a
 *                const float x[12]
 *                int ix0
 *                float y[4]
 *                int iy0
 * Return Type  : void
 */
static void b_xaxpy(int n, float a, const float x[12], int ix0, float y[4], int
                    iy0)
{
  int ix;
  int iy;
  int k;
  if (n < 1 || a == 0.0F) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                const float x[3]
 *                int ix0
 * Return Type  : float
 */
static float b_xnrm2(int n, const float x[3], int ix0)
{
  float y;
  float scale;
  int kend;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  if (!(n < 1)) {
    if (n == 1) {
      y = (float)fabs(x[ix0 - 1]);
    } else {
      scale = 1.17549435E-38F;
      kend = ix0 + n - 1;
      for (k = ix0; k <= kend; k++) {
        absxk = (float)fabs(x[k - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          y = 1.0F + y * t * t;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * (float)sqrt(y);
    }
  }

  return y;
}

/*
 * Arguments    : int n
 *                float a
 *                const float x[4]
 *                int ix0
 *                float y[12]
 *                int iy0
 * Return Type  : void
 */
static void c_xaxpy(int n, float a, const float x[4], int ix0, float y[12], int
                    iy0)
{
  int ix;
  int iy;
  int k;
  if (n < 1 || a == 0.0F) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : const float A[12]
 *                float U[3]
 * Return Type  : void
 */
static void svd(float A[12], float U[3])
{
  float b_A[12];
  int kase;
  float s[3];
  float e[3];
  float work[4];
  int q;
  int m;
  int qs;
  boolean_T apply_transform;
  float ztest0;
  int iter;
  float snorm;
  float f;
  int qjj;
  float ztest;
  int exitg2;
  boolean_T exitg1;
  float varargin_1[5];
  float mtmp;
  float sqds;
  float b;
  for (kase = 0; kase < 12; kase++) {
    b_A[kase] = A[kase];
  }

  for (kase = 0; kase < 3; kase++) {
    s[kase] = 0.0F;
    e[kase] = 0.0F;
  }

  for (kase = 0; kase < 4; kase++) {
    work[kase] = 0.0F;
  }

  for (q = 0; q < 3; q++) {
    qs = q + (q << 2);
    apply_transform = false;
    ztest0 = xnrm2(4 - q, b_A, qs + 1);
    if (ztest0 > 0.0F) {
      apply_transform = true;
      if (b_A[qs] < 0.0F) {
        s[q] = -ztest0;
      } else {
        s[q] = ztest0;
      }

      if ((float)fabs(s[q]) >= 9.86076132E-32F) {
        ztest = 1.0F / s[q];
        kase = qs - q + 4;
        for (qjj = qs; qjj + 1 <= kase; qjj++) {
          b_A[qjj] *= ztest;
        }
      } else {
        kase = qs - q + 4;
        for (qjj = qs; qjj + 1 <= kase; qjj++) {
          b_A[qjj] /= s[q];
        }
      }

      b_A[qs]++;
      s[q] = -s[q];
    } else {
      s[q] = 0.0F;
    }

    for (kase = q + 1; kase + 1 < 4; kase++) {
      qjj = q + (kase << 2);
      if (apply_transform) {
        xaxpy(4 - q, -(xdotc(4 - q, b_A, qs + 1, b_A, qjj + 1) / b_A[q + (q << 2)]),
              qs + 1, b_A, qjj + 1);
      }

      e[kase] = b_A[qjj];
    }

    if (q + 1 <= 1) {
      ztest0 = b_xnrm2(2, e, 2);
      if (ztest0 == 0.0F) {
        e[0] = 0.0F;
      } else {
        if (e[1] < 0.0F) {
          ztest = -ztest0;
        } else {
          ztest = ztest0;
        }

        if (e[1] < 0.0F) {
          e[0] = -ztest0;
        } else {
          e[0] = ztest0;
        }

        if ((float)fabs(e[0]) >= 9.86076132E-32F) {
          ztest = 1.0F / e[0];
          for (qjj = 1; qjj + 1 < 4; qjj++) {
            e[qjj] *= ztest;
          }
        } else {
          for (qjj = 1; qjj + 1 < 4; qjj++) {
            e[qjj] /= ztest;
          }
        }

        e[1]++;
        e[0] = -e[0];
        for (qjj = 2; qjj < 5; qjj++) {
          work[qjj - 1] = 0.0F;
        }

        for (kase = 1; kase + 1 < 4; kase++) {
          b_xaxpy(3, e[kase], b_A, (kase << 2) + 2, work, 2);
        }

        for (kase = 1; kase + 1 < 4; kase++) {
          c_xaxpy(3, -e[kase] / e[1], work, 2, b_A, (kase << 2) + 2);
        }
      }
    }
  }

  m = 1;
  e[1] = b_A[9];
  e[2] = 0.0F;
  iter = 0;
  snorm = 0.0F;
  for (q = 0; q < 3; q++) {
    f = e[q];
    if (s[q] != 0.0F) {
      ztest = (float)fabs(s[q]);
      ztest0 = s[q] / ztest;
      s[q] = ztest;
      if (q + 1 < 3) {
        f = e[q] / ztest0;
      }
    }

    if (q + 1 < 3 && f != 0.0F) {
      ztest = (float)fabs(f);
      ztest0 = f;
      f = ztest;
      s[q + 1] *= ztest / ztest0;
    }

    ztest0 = (float)fabs(s[q]);
    ztest = (float)fabs(f);
    if (ztest0 >= ztest) {
    } else {
      ztest0 = ztest;
    }

    if (!(snorm >= ztest0)) {
      snorm = ztest0;
    }

    e[q] = f;
  }

  while (m + 2 > 0 && !(iter >= 75)) {
    qjj = m;
    do {
      exitg2 = 0;
      q = qjj + 1;
      if (qjj + 1 == 0) {
        exitg2 = 1;
      } else {
        ztest0 = (float)fabs(e[qjj]);
        if (ztest0 <= 1.1920929E-7F * ((float)fabs(s[qjj]) + (float)fabs(s[qjj +
              1])) || ztest0 <= 9.86076132E-32F || iter > 20 && ztest0 <=
            1.1920929E-7F * snorm) {
          e[qjj] = 0.0F;
          exitg2 = 1;
        } else {
          qjj--;
        }
      }
    } while (exitg2 == 0);

    if (qjj + 1 == m + 1) {
      kase = 4;
    } else {
      qs = m + 2;
      kase = m + 2;
      exitg1 = false;
      while (!exitg1 && kase >= qjj + 1) {
        qs = kase;
        if (kase == qjj + 1) {
          exitg1 = true;
        } else {
          ztest0 = 0.0F;
          if (kase < m + 2) {
            ztest0 = (float)fabs(e[kase - 1]);
          }

          if (kase > qjj + 2) {
            ztest0 += (float)fabs(e[kase - 2]);
          }

          ztest = (float)fabs(s[kase - 1]);
          if (ztest <= 1.1920929E-7F * ztest0 || ztest <= 9.86076132E-32F) {
            s[kase - 1] = 0.0F;
            exitg1 = true;
          } else {
            kase--;
          }
        }
      }

      if (qs == qjj + 1) {
        kase = 3;
      } else if (qs == m + 2) {
        kase = 1;
      } else {
        kase = 2;
        q = qs;
      }
    }

    switch (kase) {
     case 1:
      f = e[m];
      e[m] = 0.0F;
      for (qjj = m; qjj + 1 >= q + 1; qjj--) {
        xrotg(&s[qjj], &f, &ztest0, &ztest);
        if (qjj + 1 > q + 1) {
          f = -ztest * e[0];
          e[0] *= ztest0;
        }
      }
      break;

     case 2:
      f = e[q - 1];
      e[q - 1] = 0.0F;
      while (q + 1 <= m + 2) {
        xrotg(&s[q], &f, &ztest0, &ztest);
        f = -ztest * e[q];
        e[q] *= ztest0;
        q++;
      }
      break;

     case 3:
      varargin_1[0] = (float)fabs(s[m + 1]);
      varargin_1[1] = (float)fabs(s[m]);
      varargin_1[2] = (float)fabs(e[m]);
      varargin_1[3] = (float)fabs(s[q]);
      varargin_1[4] = (float)fabs(e[q]);
      mtmp = varargin_1[0];
      for (kase = 0; kase < 4; kase++) {
        if (varargin_1[kase + 1] > mtmp) {
          mtmp = varargin_1[kase + 1];
        }
      }

      f = s[m + 1] / mtmp;
      ztest0 = s[m] / mtmp;
      ztest = e[m] / mtmp;
      sqds = s[q] / mtmp;
      b = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0F;
      ztest0 = f * ztest;
      ztest0 *= ztest0;
      if (b != 0.0F || ztest0 != 0.0F) {
        ztest = (float)sqrt(b * b + ztest0);
        if (b < 0.0F) {
          ztest = -ztest;
        }

        ztest = ztest0 / (b + ztest);
      } else {
        ztest = 0.0F;
      }

      f = (sqds + f) * (sqds - f) + ztest;
      b = sqds * (e[q] / mtmp);
      for (qjj = q + 1; qjj <= m + 1; qjj++) {
        xrotg(&f, &b, &ztest0, &ztest);
        if (qjj > q + 1) {
          e[0] = f;
        }

        f = ztest0 * s[qjj - 1] + ztest * e[qjj - 1];
        e[qjj - 1] = ztest0 * e[qjj - 1] - ztest * s[qjj - 1];
        b = ztest * s[qjj];
        s[qjj] *= ztest0;
        s[qjj - 1] = f;
        xrotg(&s[qjj - 1], &b, &ztest0, &ztest);
        f = ztest0 * e[qjj - 1] + ztest * s[qjj];
        s[qjj] = -ztest * e[qjj - 1] + ztest0 * s[qjj];
        b = ztest * e[qjj];
        e[qjj] *= ztest0;
      }

      e[m] = f;
      iter++;
      break;

     default:
      if (s[q] < 0.0F) {
        s[q] = -s[q];
      }

      kase = q + 1;
      while (q + 1 < 3 && s[q] < s[kase]) {
        ztest = s[q];
        s[q] = s[kase];
        s[kase] = ztest;
        q = kase;
        kase++;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (qjj = 0; qjj < 3; qjj++) {
    U[qjj] = s[qjj];
  }
}

/*
 * Arguments    : int n
 *                float a
 *                int ix0
 *                float y[12]
 *                int iy0
 * Return Type  : void
 */
static void xaxpy(int n, float a, int ix0, float y[12], int iy0)
{
  int ix;
  int iy;
  int k;
  if (n < 1 || a == 0.0F) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                const float x[12]
 *                int ix0
 *                const float y[12]
 *                int iy0
 * Return Type  : float
 */
static float xdotc(int n, const float x[12], int ix0, const float y[12], int iy0)
{
  float d;
  int ix;
  int iy;
  int k;
  d = 0.0F;
  if (!(n < 1)) {
    ix = ix0;
    iy = iy0;
    for (k = 1; k <= n; k++) {
      d += x[ix - 1] * y[iy - 1];
      ix++;
      iy++;
    }
  }

  return d;
}

/*
 * Arguments    : int n
 *                const float x[12]
 *                int ix0
 * Return Type  : float
 */
static float xnrm2(int n, const float x[12], int ix0)
{
  float y;
  float scale;
  int kend;
  int k;
  float absxk;
  float t;
  y = 0.0F;
  if (!(n < 1)) {
    if (n == 1) {
      y = (float)fabs(x[ix0 - 1]);
    } else {
      scale = 1.17549435E-38F;
      kend = ix0 + n - 1;
      for (k = ix0; k <= kend; k++) {
        absxk = (float)fabs(x[k - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          y = 1.0F + y * t * t;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * (float)sqrt(y);
    }
  }

  return y;
}

/*
 * Arguments    : float *a
 *                float *b
 *                float *c
 *                float *s
 * Return Type  : void
 */
static void xrotg(float *a, float *b, float *c, float *s)
{
  float roe;
  float absa;
  float absb;
  float scale;
  float ads;
  float bds;
  roe = *b;
  absa = (float)fabs(*a);
  absb = (float)fabs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0F) {
    *s = 0.0F;
    *c = 1.0F;
    scale = 0.0F;
    *b = 0.0F;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    scale *= (float)sqrt(ads * ads + bds * bds);
    if (roe < 0.0F) {
      scale = -scale;
    }

    *c = *a / scale;
    *s = *b / scale;
    if (absa > absb) {
      *b = *s;
    } else if (*c != 0.0F) {
      *b = 1.0F / *c;
    } else {
      *b = 1.0F;
    }
  }

  *a = scale;
}

/*
 * Arguments    : const float in[12]
 * Return Type  : float
 */
float normK(float in[12])
{
  float s[3];

  /* 'normK:3' out = norm(in,2); */
  svd(in, s);
  return s[0];
}
