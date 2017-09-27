/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
static double medianf(double x_data[], int x_size[1]);
static void nullAssignment(double x_data[], int x_size[1]);

/* Function Definitions */

/*
 * Arguments    : double x_data[]
 *                int x_size[1]
 * Return Type  : double
 */
static double medianf(double x_data[], int x_size[1])
{
  double y;
  int loop_ub;
  int i1;
  double xsort_data[11];
  int m;
  int n;
  double temp;

  /* 'medianf:3' x = x(:); */
  /* 'medianf:4' x(isnan(x)) = []; */
  nullAssignment(x_data, x_size);

  /* 'medianf:5' xsort = sortf(x); */
  loop_ub = x_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    xsort_data[i1] = x_data[i1];
  }

  /* 'sortf:3' len = length(x); */
  loop_ub = x_size[0] - 3;

  /* 'sortf:4' for m = 1:len-1 */
  for (m = 0; m <= loop_ub + 1; m++) {
    /* 'sortf:5' for n = 1:len-m */
    i1 = loop_ub - m;
    for (n = 0; n <= i1 + 1; n++) {
      /* 'sortf:6' if x(n+1)<x(n) */
      if (xsort_data[1 + n] < xsort_data[n]) {
        /* 'sortf:7' temp = x(n); */
        temp = xsort_data[n];

        /* 'sortf:8' x(n) = x(n+1); */
        xsort_data[n] = xsort_data[1 + n];

        /* 'sortf:9' x(n+1) = temp; */
        xsort_data[1 + n] = temp;
      }
    }
  }

  /* 'medianf:6' len = length(xsort); */
  /* 'medianf:7' if mod(len,2)==1 */
  if (x_size[0] - ((int)floor((double)x_size[0] / 2.0) << 1) == 1) {
    /* 'medianf:8' y = xsort((len+1)/2) ; */
    y = xsort_data[(int)(((double)x_size[0] + 1.0) / 2.0) - 1];
  } else {
    /* 'medianf:9' else */
    /* 'medianf:10' y = mean([xsort(len/2),xsort(len/2+1)]); */
    y = (xsort_data[(int)((double)x_size[0] / 2.0) - 1] + xsort_data[(int)
         ((double)x_size[0] / 2.0 + 1.0) - 1]) / 2.0;
  }

  return y;
}

/*
 * Arguments    : double x_data[]
 *                int x_size[1]
 * Return Type  : void
 */
static void nullAssignment(double x_data[], int x_size[1])
{
  int nxin;
  int k0;
  int k;
  double b_x_data[11];
  nxin = x_size[0];
  k0 = -1;
  for (k = 1; k <= nxin; k++) {
    k0++;
    x_data[k0] = x_data[k - 1];
  }

  if (1 > nxin) {
    nxin = 0;
  }

  for (k0 = 0; k0 < nxin; k0++) {
    b_x_data[k0] = x_data[k0];
  }

  x_size[0] = nxin;
  for (k0 = 0; k0 < nxin; k0++) {
    x_data[k0] = b_x_data[k0];
  }
}

/*
 * Y = HAMPEL(X) replaces any element in vector X that is more than three
 *    standard deviations from the median of itself and up to three
 *    neighboring elements with that median value.
 *    This hampel11 is specified on 11 data elements
 * Arguments    : const double x[11]
 *                double xfilt[11]
 * Return Type  : void
 */
void hampel11(double x[11], double xfilt[11])
{
  double xmad[11];
  double xmedian[11];
  double m;
  int exitg1;
  int loop_ub;
  int x_window_size[1];
  int k;
  int i0;
  double x_window_data[11];
  double b_x_window_data[11];
  int b_x_window_size[1];
  double x_data[11];
  int y_size[1];
  double xf;

  /* 'hampel11:8' k = 3; */
  /* 'hampel11:9' nsigma = 3; */
  /* 'hampel11:10' x = x(:); */
  /* filter size will be 2*k+1 */
  /* 'hampel11:13' [xmad,xmedian] = movmadf(x,k); */
  /* 'movmadf:3' x = x(:); */
  /* data head tructat */
  /* 'movmadf:6' len = length(x); */
  /* 'movmadf:7' xmad = zeros(len,1); */
  /* 'movmadf:8' xmedian = zeros(len,1); */
  memset(&xmad[0], 0, 11U * sizeof(double));
  memset(&xmedian[0], 0, 11U * sizeof(double));

  /* 'movmadf:10' m = 1; */
  m = 1.0;

  /* 'movmadf:11' while(1) */
  do {
    exitg1 = 0;

    /* 'movmadf:12' if (1<=m)&&(m<=k) */
    if (m <= 3.0) {
      /* 'movmadf:13' x_window = x(1:m+k); */
      loop_ub = (int)m;
      x_window_size[0] = (int)m + 3;
      for (k = 0; k <= loop_ub + 2; k++) {
        x_window_data[k] = x[k];
      }
    } else if (11.0 - m < 3.0) {
      /* 'movmadf:14' elseif ((len-m)>=0)&&((len-m)<k) */
      /* 'movmadf:15' x_window = x(m-k:end); */
      x_window_size[0] = 12 - (int)(m - 3.0);
      loop_ub = 12 - (int)(m - 3.0);
      for (k = 0; k < loop_ub; k++) {
        x_window_data[k] = x[((int)(m - 3.0) + k) - 1];
      }
    } else {
      /* 'movmadf:16' else */
      /* 'movmadf:17' x_window = x(m-k:m+k); */
      if (m - 3.0 > m + 3.0) {
        k = 0;
        i0 = 0;
      } else {
        k = (int)(m - 3.0) - 1;
        i0 = (int)(m + 3.0);
      }

      x_window_size[0] = i0 - k;
      loop_ub = i0 - k;
      for (i0 = 0; i0 < loop_ub; i0++) {
        x_window_data[i0] = x[k + i0];
      }
    }

    /* ignore samples that contain NaN and truncate at the borders of the input */
    /* 'movmadf:20' x_window(isnan(x_window)) = []; */
    nullAssignment(x_window_data, x_window_size);

    /* 'movmadf:21' xmedian(m) = medianf(x_window); */
    b_x_window_size[0] = x_window_size[0];
    loop_ub = x_window_size[0];
    for (k = 0; k < loop_ub; k++) {
      b_x_window_data[k] = x_window_data[k];
    }

    xmedian[(int)m - 1] = medianf(b_x_window_data, b_x_window_size);

    /* 'movmadf:22' xmad(m) = medianf(abs(x_window-xmedian(m))); */
    loop_ub = x_window_size[0];
    for (k = 0; k < loop_ub; k++) {
      x_data[k] = x_window_data[k] - xmedian[(int)m - 1];
    }

    y_size[0] = (signed char)x_window_size[0];
    for (k = 0; k + 1 <= x_window_size[0]; k++) {
      b_x_window_data[k] = fabs(x_data[k]);
    }

    xmad[(int)m - 1] = medianf(b_x_window_data, y_size);

    /* 'movmadf:23' if (m == len) */
    if (m == 11.0) {
      exitg1 = 1;
    } else {
      /* 'movmadf:26' m = m+1; */
      m++;
    }
  } while (exitg1 == 0);

  /*  % scale the MAD by ~1.4826 as an estimate of its standard deviation */
  /* 'hampel11:16' scale = 1.482602218505602; */
  /* 'hampel11:17' xsigma = scale*xmad; */
  /*  identify points that are either NaN or beyond the desired threshold */
  /* 'hampel11:20' xi = ~(abs(x-xmedian) <= nsigma*xsigma); */
  /*  replace identified points with the corresponding median value */
  /* 'hampel11:23' xf = x; */
  /* 'hampel11:24' xf(xi) = xmedian(xi); */
  /* 'hampel11:26' xfilt = xf; */
  for (k = 0; k < 11; k++) {
    xf = x[k];
    if (!(fabs(x[k] - xmedian[k]) <= 3.0 * (1.482602218505602 * xmad[k]))) {
      xf = xmedian[k];
    }

    xfilt[k] = xf;
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void hampel11_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void hampel11_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for hampel11.c
 *
 * [EOF]
 */
