/*
 * File: movvarf.c
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 07-Sep-2017 15:59:44
 */

/* Include Files */
#include "movvarf.h"

/* Function Declarations */
static float var(const float x[2]);

/* Function Definitions */

/*
 * Arguments    : const float x[2]
 * Return Type  : float
 */
static float var(const float x[2])
{
  float xbar;
  float r;
  xbar = (x[0] + x[1]) / 2.0F;
  r = x[0] - xbar;
  xbar = x[1] - xbar;
  return r * r + xbar * xbar;
}

/*
 * Arguments    : const float in[7]
 *                float out[7]
 * Return Type  : void
 */

float movvarf(float in)
{
	static float normK_group[7] = {0};
	float out[7];
  float xbar;
  int k;
  int i0;
  int i1;
  int i2;
  float y;
  int ix;
  int b_k;
  float r;
  int i3;
  u8 i;

	
  for(i=0;i<7;i++) {
	normK_group[i] = normK_group[i+1]; 
  }
			
  normK_group[6] = in;  
	
  if(normK_group[0]!=0){
  /* 'movvarf:3' out = zeros(1,length(in)); */
  /* 'movvarf:4' len = 3; */
  /* 'movvarf:5' halflen = 0.5*(len-1); */ 
  /* 'movvarf:6' for k = 1:halflen */
  /* 'movvarf:7' out(k) = var(in(k:k+halflen)); */
  xbar = var(*(float (*)[2])&normK_group[0]);
  out[0] = xbar;

  /* 'movvarf:10' for k = halflen+1:length(in)-halflen */
  for (k = 0; k < 5; k++) {
    /* 'movvarf:11' out(k) = var(in(k-halflen:k+halflen)); */
    if (1 + k > 3 + k) {
      i0 = 0;
      i1 = 1;
    } else {
      i0 = k;
      i1 = 4 + k;
    }

    i2 = i1 - i0 - 1;
    if (i1 - i0 - 1 == 0) {
      y = 0.0F;
    } else if (i1 - i0 - 1 == 0) {
      y = 0.0F;
    } else {
      ix = 0;
      xbar = normK_group[i0];
      for (b_k = 2; b_k <= i2; b_k++) {
        ix++;
        xbar += normK_group[i0 + ix];
      }

      xbar /= (float)(i1 - i0) - 1.0F;
      ix = 0;
      r = normK_group[i0] - xbar;
      y = r * r;
      for (b_k = 2; b_k <= i2; b_k++) {
        ix++;
        r = normK_group[i0 + ix] - xbar;
        y += r * r;
      }

      if (i2 > 1) {
        i3 = i2 - 1;
      } else {
        i3 = i2;
      }

      y /= (float)i3;
    }

    out[k + 1] = y;
  }

  /* 'movvarf:14' for k = length(in)-halflen+1:length(in) */
  /* 'movvarf:15' out(k) = var(in(k-halflen:end)); */
  xbar = var(*(float (*)[2])&normK_group[5]);
  out[6] = xbar;
	
	return out[6];
	}
	else 
		return 0;
	
}
