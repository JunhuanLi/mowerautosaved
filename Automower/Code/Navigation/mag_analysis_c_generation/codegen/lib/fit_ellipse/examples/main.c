#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "fit_ellipse_types.h"

void main(void)
{

  double x[500];
  double y[500];
  struct0_T ellipse_t;

  fit_ellipse(x, y, &ellipse_t);

}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
