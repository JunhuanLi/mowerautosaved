/* Include Files */
#include "imu.h"

/* Function Declarations */
static float medianf(float x_data[], int x_size[1]);
static void nullAssignment(float x_data[], int x_size[1]);

/* Function Definitions */

/*
 * Arguments    : float x_data[]
 *                int x_size[1]
 * Return Type  : float
 */
static float medianf(float x_data[], int x_size[1])
{
  float y;
  int loop_ub;
  int i1;
  float xsort_data[FILTER_LEN];
  int m;
  int n;
  float temp;

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
  if (x_size[0] - ((int)floor((float)x_size[0] / 2.0) << 1) == 1) {
    /* 'medianf:8' y = xsort((len+1)/2) ; */
    y = xsort_data[(int)(((float)x_size[0] + 1.0) / 2.0) - 1];
  } else {
    /* 'medianf:9' else */
    /* 'medianf:10' y = mean([xsort(len/2),xsort(len/2+1)]); */
    y = (xsort_data[(int)((float)x_size[0] / 2.0) - 1] + xsort_data[(int)
         ((float)x_size[0] / 2.0 + 1.0) - 1]) / 2.0;
  }

  return y;
}

/*
 * Arguments    : float x_data[]
 *                int x_size[1]
 * Return Type  : void
 */
static void nullAssignment(float x_data[], int x_size[1])
{
  int nxin;
  int k0;
  int k;
  float b_x_data[FILTER_LEN];
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
 *    This hampel11 is specified on FILTER_LEN data elements
 * Arguments    : const float x[FILTER_LEN]
 *                float xfilt[FILTER_LEN]
 * Return Type  : void
 */
float hampel(float x[FILTER_LEN])
{
  float xmad[FILTER_LEN];
  float xmedian[FILTER_LEN];
	float xfilt[FILTER_LEN];
	float m;
  int exitg1;
  int loop_ub;
  int x_window_size[1];
  int k;
  int i0;
  float x_window_data[FILTER_LEN];
  float b_x_window_data[FILTER_LEN];
  int b_x_window_size[1];
  float x_data[FILTER_LEN];
  int y_size[1];
  float xf;

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
  memset(&xmad[0], 0, FILTER_LEN * sizeof(float));
  memset(&xmedian[0], 0, FILTER_LEN * sizeof(float));

  /* 'movmadf:10' m = 1; */
  m = 1.0;

  /* 'movmadf:FILTER_LEN' while(1) */
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
    } else if (FILTER_LEN - m < 3.0) {
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
    if (m == FILTER_LEN) {
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
  for (k = 0; k < FILTER_LEN; k++) {
    xf = x[k];
    if (!(fabs(x[k] - xmedian[k]) <= 3.0 * (1.482602218505602 * xmad[k]))) {
      xf = xmedian[k];
    }
    xfilt[k] = xf;
  }
  	return xfilt[FILTER_LEN-1];
}




///* Type Definitions */
//#ifndef struct_emxArray_int32_T_11
//#define struct_emxArray_int32_T_11

//struct emxArray_int32_T_11
//{
//  int data[FILTER_LEN];
//  int size[1];
//};

//#endif                                 /*struct_emxArray_int32_T_11*/

//#ifndef typedef_emxArray_int32_T_11
//#define typedef_emxArray_int32_T_11

//typedef struct emxArray_int32_T_11 emxArray_int32_T_11;

//#endif                                 /*typedef_emxArray_int32_T_11*/

///* Function Declarations */
//static float medianf(float x_data[], int x_size[1]);
//static void merge(int idx_data[], float x_data[], int np, int nq, int
//                  iwork_data[], float xwork_data[]);
//static void nullAssignment(float x_data[], int x_size[1]);
//static void sort(float x_data[], int x_size[1]);
//static void sortIdx(float x_data[], int x_size[1], int idx_data[], int idx_size
//                    [1]);

///* Function Definitions */

///*
// * Arguments    : float x_data[]
// *                int x_size[1]
// * Return Type  : float
// */
//static float medianf(float x_data[], int x_size[1])
//{
//  float y;
//  int xsort_size[1];
//  int loop_ub;
//  int i1;
//  float xsort_data[FILTER_LEN];

//  /* 'medianf:3' x = x(:); */
//  /* 'medianf:4' x(isnan(x)) = []; */
//  nullAssignment(x_data, x_size);

//  /* 'medianf:5' xsort = sort(x); */
//  xsort_size[0] = x_size[0];
//  loop_ub = x_size[0];
//  for (i1 = 0; i1 < loop_ub; i1++) {
//    xsort_data[i1] = x_data[i1];
//  }

//  sort(xsort_data, xsort_size);

//  /* 'medianf:6' len = length(xsort); */
//  /* 'medianf:7' if mod(len,2)==1 */
//  if (xsort_size[0] - ((int)(float)floor((float)xsort_size[0] / 2.0F) << 1) == 1)
//  {
//    /* 'medianf:8' y = xsort((len+1)/2) ; */
//    y = xsort_data[(int)(((float)xsort_size[0] + 1.0F) / 2.0F) - 1];
//  } else {
//    /* 'medianf:9' else */
//    /* 'medianf:10' y = mean([xsort(len/2),xsort(len/2+1)]); */
//    y = (xsort_data[(int)((float)xsort_size[0] / 2.0F) - 1] + xsort_data[(int)
//         ((float)xsort_size[0] / 2.0F + 1.0F) - 1]) / 2.0F;
//  }

//  return y;
//}

///*
// * Arguments    : int idx_data[]
// *                float x_data[]
// *                int np
// *                int nq
// *                int iwork_data[]
// *                float xwork_data[]
// * Return Type  : void
// */
//static void merge(int idx_data[], float x_data[], int np, int nq, int
//                  iwork_data[], float xwork_data[])
//{
//  int n;
//  int qend;
//  int p;
//  int iout;
//  int exitg1;
//  if (np == 0 || nq == 0) {
//  } else {
//    n = np + nq;
//    for (qend = 0; qend + 1 <= n; qend++) {
//      iwork_data[qend] = idx_data[qend];
//      xwork_data[qend] = x_data[qend];
//    }

//    p = 0;
//    n = np;
//    qend = np + nq;
//    iout = -1;
//    do {
//      exitg1 = 0;
//      iout++;
//      if (xwork_data[p] <= xwork_data[n]) {
//        idx_data[iout] = iwork_data[p];
//        x_data[iout] = xwork_data[p];
//        if (p + 1 < np) {
//          p++;
//        } else {
//          exitg1 = 1;
//        }
//      } else {
//        idx_data[iout] = iwork_data[n];
//        x_data[iout] = xwork_data[n];
//        if (n + 1 < qend) {
//          n++;
//        } else {
//          n = iout - p;
//          while (p + 1 <= np) {
//            idx_data[n + p + 1] = iwork_data[p];
//            x_data[n + p + 1] = xwork_data[p];
//            p++;
//          }

//          exitg1 = 1;
//        }
//      }
//    } while (exitg1 == 0);
//  }
//}

///*
// * Arguments    : float x_data[]
// *                int x_size[1]
// * Return Type  : void
// */
//static void nullAssignment(float x_data[], int x_size[1])
//{
//  int nxin;
//  int k0;
//  int k;
//  float b_x_data[FILTER_LEN];
//  nxin = x_size[0];
//  k0 = -1;
//  for (k = 1; k <= nxin; k++) {
//    k0++;
//    x_data[k0] = x_data[k - 1];
//  }

//  if (1 > nxin) {
//    nxin = 0;
//  }

//  for (k0 = 0; k0 < nxin; k0++) {
//    b_x_data[k0] = x_data[k0];
//  }

//  x_size[0] = nxin;
//  for (k0 = 0; k0 < nxin; k0++) {
//    x_data[k0] = b_x_data[k0];
//  }
//}

///*
// * Arguments    : float x_data[]
// *                int x_size[1]
// * Return Type  : void
// */
//static void sort(float x_data[], int x_size[1])
//{
//  int dim;
//  int i2;
//  float vwork_data[FILTER_LEN];
//  int vwork_size[1];
//  int vstride;
//  int k;
//  emxArray_int32_T_11 b_vwork_data;
//  dim = 2;
//  if (x_size[0] != 1) {
//    dim = 1;
//  }

//  if (dim <= 1) {
//    i2 = x_size[0];
//  } else {
//    i2 = 1;
//  }

//  vwork_size[0] = (signed char)i2;
//  vstride = 1;
//  k = 1;
//  while (k <= dim - 1) {
//    vstride *= x_size[0];
//    k = 2;
//  }

//  for (dim = 0; dim + 1 <= vstride; dim++) {
//    for (k = 0; k + 1 <= i2; k++) {
//      vwork_data[k] = x_data[dim + k * vstride];
//    }

//    sortIdx(vwork_data, vwork_size, b_vwork_data.data, b_vwork_data.size);
//    for (k = 0; k + 1 <= i2; k++) {
//      x_data[dim + k * vstride] = vwork_data[k];
//    }
//  }
//}

///*
// * Arguments    : float x_data[]
// *                int x_size[1]
// *                int idx_data[]
// *                int idx_size[1]
// * Return Type  : void
// */
//static void sortIdx(float x_data[], int x_size[1], int idx_data[], int idx_size
//                    [1])
//{
//  signed char unnamed_idx_0;
//  int x_size_idx_0;
//  int nBlocks;
//  int ib;
//  float b_x_data[FILTER_LEN];
//  int n;
//  float x4[4];
//  signed char idx4[4];
//  int iwork_data[FILTER_LEN];
//  float xwork_data[FILTER_LEN];
//  int k;
//  signed char perm[4];
//  int i3;
//  int i4;
//  unnamed_idx_0 = (signed char)x_size[0];
//  x_size_idx_0 = x_size[0];
//  nBlocks = x_size[0];
//  for (ib = 0; ib < nBlocks; ib++) {
//    b_x_data[ib] = x_data[ib];
//  }

//  idx_size[0] = unnamed_idx_0;
//  nBlocks = unnamed_idx_0;
//  for (ib = 0; ib < nBlocks; ib++) {
//    idx_data[ib] = 0;
//  }

//  n = x_size[0];
//  for (nBlocks = 0; nBlocks < 4; nBlocks++) {
//    x4[nBlocks] = 0.0F;
//    idx4[nBlocks] = 0;
//  }

//  nBlocks = unnamed_idx_0;
//  for (ib = 0; ib < nBlocks; ib++) {
//    iwork_data[ib] = 0;
//  }

//  nBlocks = (signed char)x_size[0];
//  for (ib = 0; ib < nBlocks; ib++) {
//    xwork_data[ib] = 0.0F;
//  }

//  ib = 0;
//  for (k = 0; k + 1 <= n; k++) {
//    ib++;
//    idx4[ib - 1] = (signed char)(k + 1);
//    x4[ib - 1] = b_x_data[k];
//    if (ib == 4) {
//      if (x4[0] <= x4[1]) {
//        nBlocks = 1;
//        ib = 2;
//      } else {
//        nBlocks = 2;
//        ib = 1;
//      }

//      if (x4[2] <= x4[3]) {
//        i3 = 3;
//        i4 = 4;
//      } else {
//        i3 = 4;
//        i4 = 3;
//      }

//      if (x4[nBlocks - 1] <= x4[i3 - 1]) {
//        if (x4[ib - 1] <= x4[i3 - 1]) {
//          perm[0] = (signed char)nBlocks;
//          perm[1] = (signed char)ib;
//          perm[2] = (signed char)i3;
//          perm[3] = (signed char)i4;
//        } else if (x4[ib - 1] <= x4[i4 - 1]) {
//          perm[0] = (signed char)nBlocks;
//          perm[1] = (signed char)i3;
//          perm[2] = (signed char)ib;
//          perm[3] = (signed char)i4;
//        } else {
//          perm[0] = (signed char)nBlocks;
//          perm[1] = (signed char)i3;
//          perm[2] = (signed char)i4;
//          perm[3] = (signed char)ib;
//        }
//      } else if (x4[nBlocks - 1] <= x4[i4 - 1]) {
//        if (x4[ib - 1] <= x4[i4 - 1]) {
//          perm[0] = (signed char)i3;
//          perm[1] = (signed char)nBlocks;
//          perm[2] = (signed char)ib;
//          perm[3] = (signed char)i4;
//        } else {
//          perm[0] = (signed char)i3;
//          perm[1] = (signed char)nBlocks;
//          perm[2] = (signed char)i4;
//          perm[3] = (signed char)ib;
//        }
//      } else {
//        perm[0] = (signed char)i3;
//        perm[1] = (signed char)i4;
//        perm[2] = (signed char)nBlocks;
//        perm[3] = (signed char)ib;
//      }

//      idx_data[k - 3] = idx4[perm[0] - 1];
//      idx_data[k - 2] = idx4[perm[1] - 1];
//      idx_data[k - 1] = idx4[perm[2] - 1];
//      idx_data[k] = idx4[perm[3] - 1];
//      b_x_data[k - 3] = x4[perm[0] - 1];
//      b_x_data[k - 2] = x4[perm[1] - 1];
//      b_x_data[k - 1] = x4[perm[2] - 1];
//      b_x_data[k] = x4[perm[3] - 1];
//      ib = 0;
//    }
//  }

//  if (ib > 0) {
//    for (nBlocks = 0; nBlocks < 4; nBlocks++) {
//      perm[nBlocks] = 0;
//    }

//    if (ib == 1) {
//      perm[0] = 1;
//    } else if (ib == 2) {
//      if (x4[0] <= x4[1]) {
//        perm[0] = 1;
//        perm[1] = 2;
//      } else {
//        perm[0] = 2;
//        perm[1] = 1;
//      }
//    } else if (x4[0] <= x4[1]) {
//      if (x4[1] <= x4[2]) {
//        perm[0] = 1;
//        perm[1] = 2;
//        perm[2] = 3;
//      } else if (x4[0] <= x4[2]) {
//        perm[0] = 1;
//        perm[1] = 3;
//        perm[2] = 2;
//      } else {
//        perm[0] = 3;
//        perm[1] = 1;
//        perm[2] = 2;
//      }
//    } else if (x4[0] <= x4[2]) {
//      perm[0] = 2;
//      perm[1] = 1;
//      perm[2] = 3;
//    } else if (x4[1] <= x4[2]) {
//      perm[0] = 2;
//      perm[1] = 3;
//      perm[2] = 1;
//    } else {
//      perm[0] = 3;
//      perm[1] = 2;
//      perm[2] = 1;
//    }

//    for (k = 1; k <= ib; k++) {
//      idx_data[n - ib + k - 1] = idx4[perm[k - 1] - 1];
//      b_x_data[n - ib + k - 1] = x4[perm[k - 1] - 1];
//    }
//  }

//  if (x_size[0] > 1) {
//    nBlocks = x_size[0] >> 2;
//    ib = 4;
//    while (nBlocks > 1) {
//      merge(idx_data, b_x_data, ib, ib, iwork_data, xwork_data);
//      ib <<= 1;
//      nBlocks = 1;
//    }

//    if (x_size[0] > ib) {
//      merge(idx_data, b_x_data, ib, x_size[0] - ib, iwork_data, xwork_data);
//    }
//  }

//  x_size[0] = x_size_idx_0;
//  for (ib = 0; ib < x_size_idx_0; ib++) {
//    x_data[ib] = b_x_data[ib];
//  }
//}

///*
// * Arguments    : const float x[FILTER_LEN]
// *                float xfilt[FILTER_LEN]
// * Return Type  : void
// */
//float hampel(float x[FILTER_LEN])
//{
//  float xmad[FILTER_LEN];
//  float xmedian[FILTER_LEN];
//	float xfilt[FILTER_LEN];
//  int k;
//  float m;
//  int exitg1;
//  int loop_ub;
//  int x_window_size[1];
//  int i0;
//  float x_window_data[FILTER_LEN];
//  float b_x_window_data[FILTER_LEN];
//  int b_x_window_size[1];
//  float c_data[FILTER_LEN];
//  int y_size[1];
//  float xf;

//  /*  function [xfilt, xi, xmedian, xsigma] = hampel_my(x) */
//  /* 'hampelf:4' k = 3; */
//  /* 'hampelf:5' nsigma = 3; */
//  /* 'hampelf:6' x = x(:); */
//  /* filter size will be 2*k+1 */
//  /* 'hampelf:9' [xmad,xmedian] = movmadf(x,k); */
//  /* 'movmadf:3' x = x(:); */
//  /* data head tructat */
//  /* 'movmadf:6' len = length(x); */
//  /* 'movmadf:7' xmad = zeros(len,1); */
//  /* 'movmadf:8' xmedian = zeros(len,1); */
//  for (k = 0; k < FILTER_LEN; k++) {
//    xmad[k] = 0.0F;
//    xmedian[k] = 0.0F;
//  }

//  /* 'movmadf:10' m = 1; */
//  m = 1.0F;

//  /* 'movmadf:FILTER_LEN' while(1) */
//  do {
//    exitg1 = 0;

//    /* 'movmadf:12' if (1<=m)&&(m<=k) */
//    if (m <= 3.0F) {
//      /* 'movmadf:13' x_window = x(1:m+k); */
//      loop_ub = (int)m;
//      x_window_size[0] = (int)m + 3;
//      for (k = 0; k <= loop_ub + 2; k++) {
//        x_window_data[k] = x[k];
//      }
//    } else if (FILTER_LEN.0F - m >= 0.0F && FILTER_LEN.0F - m < 3.0F) {
//      /* 'movmadf:14' elseif ((len-m)>=0)&&((len-m)<k) */
//      /* 'movmadf:15' x_window = x(m-k:end); */
//      if (m - 3.0F > FILTER_LEN.0F) {
//        k = 0;
//        i0 = -1;
//      } else {
//        k = (int)(m - 3.0F) - 1;
//        i0 = 10;
//      }

//      x_window_size[0] = i0 - k + 1;
//      loop_ub = i0 - k;
//      for (i0 = 0; i0 <= loop_ub; i0++) {
//        x_window_data[i0] = x[k + i0];
//      }
//    } else {
//      /* 'movmadf:16' else */
//      /* 'movmadf:17' x_window = x(m-k:m+k); */
//      if (m - 3.0F > m + 3.0F) {
//        k = 0;
//        i0 = 0;
//      } else {
//        k = (int)(m - 3.0F) - 1;
//        i0 = (int)(m + 3.0F);
//      }

//      x_window_size[0] = i0 - k;
//      loop_ub = i0 - k;
//      for (i0 = 0; i0 < loop_ub; i0++) {
//        x_window_data[i0] = x[k + i0];
//      }
//    }

//    /* ignore samples that contain NaN and truncate at the borders of the input */
//    /* 'movmadf:20' x_window(isnan(x_window)) = []; */
//    nullAssignment(x_window_data, x_window_size);

//    /* 'movmadf:21' xmedian(m) = medianf(x_window); */
//    b_x_window_size[0] = x_window_size[0];
//    loop_ub = x_window_size[0];
//    for (k = 0; k < loop_ub; k++) {
//      b_x_window_data[k] = x_window_data[k];
//    }

//    xmedian[(int)m - 1] = medianf(b_x_window_data, b_x_window_size);

//    /* 'movmadf:22' xmad(m) = medianf(abs(x_window-xmedian(m))); */
//    loop_ub = x_window_size[0];
//    for (k = 0; k < loop_ub; k++) {
//      c_data[k] = x_window_data[k] - xmedian[(int)m - 1];
//    }

//    y_size[0] = (signed char)x_window_size[0];
//    for (k = 0; k + 1 <= x_window_size[0]; k++) {
//      b_x_window_data[k] = (float)fabs(c_data[k]);
//    }

//    xmad[(int)m - 1] = medianf(b_x_window_data, y_size);

//    /* 'movmadf:23' if (m == len) */
//    if (m == FILTER_LEN.0F) {
//      exitg1 = 1;
//    } else {
//      /* 'movmadf:26' m = m+1; */
//      m++;
//    }
//  } while (exitg1 == 0);

//  /*  % scale the MAD by ~1.4826 as an estimate of its standard deviation */
//  /* 'hampelf:12' scale = 1.482602218505602; */
//  /* 'hampelf:13' xsigma = scale*xmad; */
//  /*  identify points that are either NaN or beyond the desired threshold */
//  /* 'hampelf:16' xi = ~(abs(x-xmedian) <= nsigma*xsigma); */
//  /*  replace identified points with the corresponding median value */
//  /* 'hampelf:19' xf = x; */
//  /* 'hampelf:20' xf(xi) = xmedian(xi); */
//  /* 'hampelf:22' xfilt = xf; */
//  for (k = 0; k < FILTER_LEN; k++) {
//    xf = x[k];
//    if (!((float)fabs(x[k] - xmedian[k]) <= 3.0F * (1.48260224F * xmad[k]))) {
//      xf = xmedian[k];
//    }
//    xfilt[k] = xf;
//  }
//	return xfilt[10];
//}
