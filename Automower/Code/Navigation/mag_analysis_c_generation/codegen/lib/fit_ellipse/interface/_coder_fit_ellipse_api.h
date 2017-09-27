/*
 * File: _coder_fit_ellipse_api.h
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 04-Jul-2017 16:45:48
 */

#ifndef _CODER_FIT_ELLIPSE_API_H
#define _CODER_FIT_ELLIPSE_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_fit_ellipse_api.h"

/* Type Definitions */
#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  real_T a;
  real_T b;
  real_T phi;
  real_T X0;
  real_T Y0;
  real_T X0_in;
  real_T Y0_in;
  real_T long_axis;
  real_T short_axis;
  real_T status;
} struct0_T;

#endif                                 /*typedef_struct0_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void fit_ellipse(real_T x[500], real_T y[500], struct0_T *ellipse_t);
extern void fit_ellipse_api(const mxArray *prhs[2], const mxArray *plhs[1]);
extern void fit_ellipse_atexit(void);
extern void fit_ellipse_initialize(void);
extern void fit_ellipse_terminate(void);
extern void fit_ellipse_xil_terminate(void);

#endif

/*
 * File trailer for _coder_fit_ellipse_api.h
 *
 * [EOF]
 */
