/*
 * File: _coder_fit_ellipse_api.c
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 04-Jul-2017 16:45:48
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_fit_ellipse_api.h"
#include "_coder_fit_ellipse_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131435U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "fit_ellipse",                       /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[500];
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[500];
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x, const
  char_T *identifier))[500];
static const mxArray *emlrt_marshallOut(const struct0_T *u);

/* Function Definitions */

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[500]
 */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[500]
{
  real_T (*y)[500];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[500]
 */
  static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[500]
{
  real_T (*ret)[500];
  static const int32_T dims[1] = { 500 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 1U, dims);
  ret = (real_T (*)[500])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *x
 *                const char_T *identifier
 * Return Type  : real_T (*)[500]
 */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x, const
  char_T *identifier))[500]
{
  real_T (*y)[500];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}
/*
 * Arguments    : const struct0_T *u
 * Return Type  : const mxArray *
 */
  static const mxArray *emlrt_marshallOut(const struct0_T *u)
{
  const mxArray *y;
  const mxArray *b_y;
  const mxArray *m0;
  const mxArray *c_y;
  const mxArray *d_y;
  const mxArray *e_y;
  const mxArray *f_y;
  const mxArray *g_y;
  const mxArray *h_y;
  const mxArray *i_y;
  const mxArray *j_y;
  const mxArray *k_y;
  y = NULL;
  emlrtAssign(&y, emlrtCreateStructMatrix(1, 1, 0, NULL));
  b_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->a);
  emlrtAssign(&b_y, m0);
  emlrtAddField(y, b_y, "a", 0);
  c_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->b);
  emlrtAssign(&c_y, m0);
  emlrtAddField(y, c_y, "b", 0);
  d_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->phi);
  emlrtAssign(&d_y, m0);
  emlrtAddField(y, d_y, "phi", 0);
  e_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->X0);
  emlrtAssign(&e_y, m0);
  emlrtAddField(y, e_y, "X0", 0);
  f_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->Y0);
  emlrtAssign(&f_y, m0);
  emlrtAddField(y, f_y, "Y0", 0);
  g_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->X0_in);
  emlrtAssign(&g_y, m0);
  emlrtAddField(y, g_y, "X0_in", 0);
  h_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->Y0_in);
  emlrtAssign(&h_y, m0);
  emlrtAddField(y, h_y, "Y0_in", 0);
  i_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->long_axis);
  emlrtAssign(&i_y, m0);
  emlrtAddField(y, i_y, "long_axis", 0);
  j_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->short_axis);
  emlrtAssign(&j_y, m0);
  emlrtAddField(y, j_y, "short_axis", 0);
  k_y = NULL;
  m0 = emlrtCreateDoubleScalar(u->status);
  emlrtAssign(&k_y, m0);
  emlrtAddField(y, k_y, "status", 0);
  return y;
}

/*
 * Arguments    : const mxArray *prhs[2]
 *                const mxArray *plhs[1]
 * Return Type  : void
 */
void fit_ellipse_api(const mxArray *prhs[2], const mxArray *plhs[1])
{
  real_T (*x)[500];
  real_T (*y)[500];
  struct0_T ellipse_t;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);

  /* Marshall function inputs */
  x = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x");
  y = emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "y");

  /* Invoke the target function */
  fit_ellipse(*x, *y, &ellipse_t);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(&ellipse_t);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fit_ellipse_atexit(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  fit_ellipse_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fit_ellipse_initialize(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fit_ellipse_terminate(void)
{
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_fit_ellipse_api.c
 *
 * [EOF]
 */
