/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Function Declarations */
static void NALegendreCosRat(double theta, double scalFactor, double PBarUVals
  [8281], double dPBarUValsdTheta[8281]);
static void calcSinCosTerms(double lambda, double SinVec[13], double CosVec[13]);
static void calcSpherJacob(const double point[3], double J[9]);
static double norm(const double x[3]);
static void spherHarmonicEval(const double C[169], const double S[169], const
  double point[3], double a, double c, double *V, double gradV[3]);

/* Function Definitions */

/*
 * NALEGENDRECOSRAT Evaluate \bar{P}_{nm}(cos(theta))/u^m for all n from
 *                      0 to M and for each n for all m from 0 to n.
 *                      Also evaluate D{\bar{P}_{nm}(cos(theta))}/u^m and
 *                      D2{\bar{P}_{nm}(cos(theta))}/u^m where D{} is the
 *                      first derivative operator with respect to theta and
 *                      D2{} is the second derivative operator with respect
 *                      to theta. \bar{P}_{nm}(x) is the fully normalized
 *                      associated Legendre function of x of degree n and
 *                      order m. u=sin(theta). All of the values can be
 *                      scaled by a factor of scalFac, if desired, to help
 *                      prevent overflows with high degrees and orders.
 *
 * INPUTS: theta      An angle in radians.
 *         maxDeg     The maximum degree and order of the output. This should
 *                    be at least 2.
 *      scalFactor    A scale factor to help prevent overflow of the results.
 *                    In the Holmes and Featherstone paper, discussed below,
 *                    a value of 10^(-280) is used.
 * Arguments    : double theta
 *                double scalFactor
 *                double PBarUVals[8281]
 *                double dPBarUValsdTheta[8281]
 * Return Type  : void
 */
static void NALegendreCosRat(double theta, double scalFactor, double PBarUVals
  [8281], double dPBarUValsdTheta[8281])
{
  double u;
  double t;
  int m;
  int n;
  int i1;
  int b_m;

  /* 'NALegendreCosRat:22' u=sin(theta); */
  u = sin(theta);

  /* 'NALegendreCosRat:23' t=cos(theta); */
  t = cos(theta);

  /* Allocate space for the return variables. n ranges from 0 to M and m */
  /* ranges from 0 to n. Thus, the number of elements for each n ranges */
  /* from 1 to M+1. This means that a total of */
  /* (M+1)*(M+2)/2 elements are necessary. */
  /* 'NALegendreCosRat:29' numPBarU=(M+1)*(M+2)/2; */
  /* 'NALegendreCosRat:30' totalP=zeros(numPBarU,1); */
  /* 'NALegendreCosRat:31' PBarUVals =zeros(length(totalP),length(totalP)); */
  memset(&PBarUVals[0], 0, 8281U * sizeof(double));

  /* The value of PBar_{0,0}(cos(theta)) is independent of theta and is one.  */
  /* 'NALegendreCosRat:34' PBarUVals(0+1,0+1)=1*scalFactor; */
  PBarUVals[0] = scalFactor;

  /* Set the seed value for PBar_{1,1}(cos(theta))/u from which the other */
  /* values will be computed. */
  /* 'NALegendreCosRat:38' PBarUVals(1+1,1+1)=sqrt(3)*scalFactor; */
  PBarUVals[92] = 1.7320508075688772 * scalFactor;

  /* Compute the values along the main diagonal, where m=n starting from */
  /* m=n=2. This implements equation 28 in the first Holmes and */
  /* Featherstone paper for the normalized associated Legendre function  */
  /* ratio. */
  /* 'NALegendreCosRat:44' for m=2:M */
  for (m = 0; m < 11; m++) {
    /* 'NALegendreCosRat:45' PBarUVals(m+1,m+1)=sqrt((2*m+1)/(2*m))*PBarUVals(m-1+1,m-1+1); */
    PBarUVals[m + 91 * (2 + m) + 2] = sqrt((2.0 * (2.0 + (double)m) + 1.0) /
      (2.0 * (2.0 + (double)m))) * PBarUVals[m + 91 * (m + 1) + 1];
  }

  /* Recursively compute the values using Equation 27 from the first Holmes */
  /* and Featherstone paper, taking into account the fact that the first */
  /* element of the recursion only has one term. */
  /* 'NALegendreCosRat:51' jTerm=1/sqrt(2); */
  /* First, deal with the case where n=1, m=0; */
  /* 'NALegendreCosRat:53' n=1; */
  /* 'NALegendreCosRat:53' m=0; */
  /* g is given in Equation 18 of the first Holmes and Featherstone paper. */
  /* 'NALegendreCosRat:55' g=2*(m+1)/sqrt((n-m)*(n+m+1)); */
  /* 'NALegendreCosRat:56' PBarUVals(n+1,m+1)=jTerm*g*t*PBarUVals(n+1,m+1+1); */
  PBarUVals[1] = 0.99999999999999978 * t * PBarUVals[92];

  /* Next, evaluate the values for all other valid n and m. */
  /* 'NALegendreCosRat:59' for n=2:M */
  for (n = 0; n < 11; n++) {
    /* Deal with the first element of the recursion,which is  where m=n-1. */
    /* 'NALegendreCosRat:61' m=n-1; */
    /* 'NALegendreCosRat:62' g=2*(m+1)/sqrt((n-m)*(n+m+1)); */
    /* 'NALegendreCosRat:63' PBarUVals(n+1,m+1)=g*t*PBarUVals(n+1,m+1+1); */
    PBarUVals[n + 91 * (n + 1) + 2] = 2.0 * (2.0 + (double)n - 1.0 + 1.0) / sqrt
      (n + n + 4) * t * PBarUVals[n + 91 * (2 + n) + 2];

    /* Recursively compute the values of the rest of the m terms. */
    /* 'NALegendreCosRat:66' for m=(n-2):-1:1 */
    i1 = (int)((1.0 + (-1.0 - (2.0 + (double)n - 2.0))) / -1.0);
    for (m = 0; m < i1; m++) {
      b_m = n - m;

      /* 'NALegendreCosRat:67' g=2*(m+1)/sqrt((n-m)*(n+m+1)); */
      /* h is given in Equation 19 of the first Holmes and Featherstone */
      /* paper. */
      /* 'NALegendreCosRat:70' h=sqrt((n+m+2)*(n-m-1)/((n-m)*(n+m+1))); */
      /* 'NALegendreCosRat:71' PBarUVals(n+1,m+1)=g*t*PBarUVals(n+1,m+1+1)-h*u^2*PBarUVals(n+1,m+2+1); */
      PBarUVals[n + 91 * b_m + 2] = 2.0 * ((double)b_m + 1.0) / sqrt((n - b_m +
        2) * (n + b_m + 3)) * t * PBarUVals[n + 91 * (b_m + 1) + 2] - sqrt
        ((double)((n + b_m + 4) * (n - b_m + 1)) / (double)((n - b_m + 2) * (n +
           b_m + 3))) * (u * u) * PBarUVals[n + 91 * (b_m + 2) + 2];
    }

    /* Deal with the special m=0 case. */
    /* 'NALegendreCosRat:75' m=0; */
    /* 'NALegendreCosRat:76' g=2*(m+1)/sqrt((n-m)*(n+m+1)); */
    /* 'NALegendreCosRat:77' h=sqrt((n+m+2)*(n-m-1)/((n-m)*(n+m+1))); */
    /* 'NALegendreCosRat:78' PBarUVals(n+1,m+1)=jTerm*(g*t*PBarUVals(n+1,m+1+1)-h*u^2*PBarUVals(n+1,m+2+1)); */
    PBarUVals[2 + n] = 0.70710678118654746 * (2.0 / sqrt((2 + n) * (3 + n)) * t *
      PBarUVals[n + 93] - sqrt((double)((4 + n) * (1 + n)) / (double)((2 + n) *
      (3 + n))) * (u * u) * PBarUVals[n + 184]);
  }

  /* If the first derivative is desired. */
  /* Allocate space. */
  /* 'NALegendreCosRat:83' dPBarUValsdTheta =zeros(length(totalP),length(totalP)); */
  memset(&dPBarUValsdTheta[0], 0, 8281U * sizeof(double));

  /* The first derivative of PBar_{0,0}(cos(theta)) is just zero. */
  /* 'NALegendreCosRat:85' dPBarUValsdTheta(1,1)=0; */
  dPBarUValsdTheta[0] = 0.0;

  /* 'NALegendreCosRat:86' m=1; */
  /* From Equation 30 in the first Holmes and Featherstone paper. This */
  /* is the seed value from which other values will be computed. */
  /* 'NALegendreCosRat:90' dPBarUValsdTheta(1+1,1+1)=m*(t/u)*PBarUVals(1+1,1+1); */
  dPBarUValsdTheta[92] = t / u * PBarUVals[92];

  /* Compute the values along the main diagonal, where m=n starting  */
  /* from m=n=2. This implements Equation 30 in the frist Holmes and */
  /* Featherstone paper for the ratio of the first derivative.  */
  /* 'NALegendreCosRat:95' for m=2:M */
  for (m = 0; m < 11; m++) {
    /* 'NALegendreCosRat:96' dPBarUValsdTheta(m+1,m+1)=m*(t/u)*PBarUVals(m+1,m+1); */
    dPBarUValsdTheta[m + 91 * (2 + m) + 2] = (2.0 + (double)m) * (t / u) *
      PBarUVals[m + 91 * (2 + m) + 2];
  }

  /* 'NALegendreCosRat:99' n=1; */
  /* 'NALegendreCosRat:99' m=0; */
  /* e is given in Equation 22 of the first Holmes and Featherstone paper. */
  /* 'NALegendreCosRat:101' e=sqrt((n+m+1)*(n-m)/2); */
  /* This is Equation 30 of the first Holmes and Featherstone paper for */
  /* 'NALegendreCosRat:103' dPBarUValsdTheta(n+1,m+1)=-e*u*PBarUVals(n+1,m+1+1); */
  dPBarUValsdTheta[1] = -u * PBarUVals[92];

  /* Next, evaluate the values for all other valid n and m. */
  /* 'NALegendreCosRat:106' for n=2:M */
  for (n = 0; n < 11; n++) {
    /* Recursively compute the values of the m terms for m>0. */
    /* 'NALegendreCosRat:108' for m=(n-1):-1:1 */
    i1 = (int)((1.0 + (-1.0 - (2.0 + (double)n - 1.0))) / -1.0);
    for (m = 0; m < i1; m++) {
      b_m = n - m + 1;

      /* 'NALegendreCosRat:109' e=sqrt((n+m+1)*(n-m)); */
      /* 'NALegendreCosRat:110' dPBarUValsdTheta(n+1,m+1)=m*(t/u)*PBarUVals(n+1,m+1)-e*u*PBarUVals(n+1,m+1+1); */
      dPBarUValsdTheta[n + 91 * b_m + 2] = (double)b_m * (t / u) * PBarUVals[n +
        91 * b_m + 2] - sqrt((n + b_m + 3) * (n - b_m + 2)) * u * PBarUVals[n +
        91 * (b_m + 1) + 2];
    }

    /* Deal with the special m=0 case. */
    /* 'NALegendreCosRat:113' m=0; */
    /* 'NALegendreCosRat:114' e=sqrt((n+m+1)*(n-m)/2); */
    /* 'NALegendreCosRat:115' dPBarUValsdTheta(n+1,m+1)=-e*u*PBarUVals(n+1,m+1+1); */
    dPBarUValsdTheta[2 + n] = -sqrt((double)((3 + n) * (2 + n)) / 2.0) * u *
      PBarUVals[n + 93];
  }

  /* If the second derivative is desired */
  /* Allocate space */
  /* 'NALegendreCosRat:120' d2PBarUValsdTheta2 =zeros(length(totalP),length(totalP)); */
  /* 'NALegendreCosRat:121' for n=0:M */
}

/*
 * Compute sin(m*lambda) and cos(m*lambda) for m=0 to m=M.
 * Arguments    : double lambda
 *                double SinVec[13]
 *                double CosVec[13]
 * Return Type  : void
 */
static void calcSinCosTerms(double lambda, double SinVec[13], double CosVec[13])
{
  double x;
  int m;

  /* 'spherHarmonicEval:120' SinVec=zeros(M+1,1); */
  /* 'spherHarmonicEval:121' CosVec=zeros(M+1,1); */
  memset(&SinVec[0], 0, 13U * sizeof(double));
  memset(&CosVec[0], 0, 13U * sizeof(double));

  /* Explicitely set the first two terms. */
  /* 'spherHarmonicEval:123' SinVec(0+1)=0; */
  SinVec[0] = 0.0;

  /* 'spherHarmonicEval:124' CosVec(0+1)=1; */
  CosVec[0] = 1.0;

  /* 'spherHarmonicEval:125' SinVec(1+1)=sin(lambda); */
  x = sin(lambda);
  SinVec[1] = sin(lambda);

  /* 'spherHarmonicEval:126' CosVec(1+1)=cos(lambda); */
  CosVec[1] = cos(lambda);

  /* Use a double angle identity to get the second order term. */
  /* 'spherHarmonicEval:128' SinVec(2+1)=2*SinVec(1+1)*CosVec(1+1); */
  SinVec[2] = 2.0 * x * cos(lambda);

  /* 'spherHarmonicEval:129' CosVec(2+1)=1-2*SinVec(1+1)^2; */
  CosVec[2] = 1.0 - 2.0 * (x * x);

  /* Use a two-part recursion for the rest of the terms. */
  /* 'spherHarmonicEval:131' for m=3:M */
  for (m = 0; m < 10; m++) {
    /* 'spherHarmonicEval:132' SinVec(m+1)=2*CosVec(1+1)*SinVec(m-1+1)-SinVec(m-2+1); */
    SinVec[3 + m] = 2.0 * CosVec[1] * SinVec[m + 2] - SinVec[1 + m];

    /* 'spherHarmonicEval:133' CosVec(m+1)=2*CosVec(1+1)*CosVec(m-1+1)-CosVec(m-2+1); */
    CosVec[3 + m] = 2.0 * CosVec[1] * CosVec[m + 2] - CosVec[1 + m];
  }
}

/*
 * #codgen
 * CALCSPHERJACOB  Compute the Jacobian matrix for a point in spherical
 *                  [range;azimuth;elevation] coordinates.
 *
 * INPUTS: point   A point in the format [range;azimuth;elevation], where the
 *                 two angles are given in radians.
 *
 * OUTPUTS: J     The 3X3 Jacobian matrix. Each row is a components of range,
 *                azimuth and elevation (in that order by row) with
 *                derivatives taken with respect to [x,y,z] by column.
 * Arguments    : const double point[3]
 *                double J[9]
 * Return Type  : void
 */
static void calcSpherJacob(const double point[3], double J[9])
{
  double CartPoint_idx_0;
  double CartPoint_idx_1;
  double CartPoint_idx_2;

  /* 'calcSpherJacob:13' CartPoint=spher2Cart(point); */
  /* SPHER2CART  Convert points from spherical coordinates to Cartesian */
  /*              coordinates */
  /*  */
  /* INPUTS:  points  One or more points given in terms of range, azimuth and */
  /*                  elevation, with the angles in radians. To convert */
  /*                  N points, points is a 3XN matrix with each column */
  /*                  having the format [range;azimuth; elevation]. */
  /*  */
  /* OUTPUTS:   cartPoints For N points, cartPoints is a 3XN matrix of the */
  /*                       converted points with each column having the format */
  /*                       [x;y;z]. */
  /* Extract the coordinates */
  /* 'spher2Cart:15' r=points(1,:); */
  /* 'spher2Cart:16' azimuth=points(2,:); */
  /* 'spher2Cart:17' elevation=points(3,:); */
  /* 'spher2Cart:19' x=r.*cos(azimuth).*cos(elevation); */
  /* 'spher2Cart:20' y=r.*sin(azimuth).*cos(elevation); */
  /* 'spher2Cart:21' z=r.*sin(elevation); */
  /* 'spher2Cart:23' cartPoints=[x;y;z]; */
  CartPoint_idx_0 = point[0] * cos(point[1]) * cos(point[2]);
  CartPoint_idx_1 = point[0] * sin(point[1]) * cos(point[2]);
  CartPoint_idx_2 = point[0] * sin(point[2]);

  /* 'calcSpherJacob:14' x=CartPoint(1); */
  /* 'calcSpherJacob:15' y=CartPoint(2); */
  /* 'calcSpherJacob:16' z=CartPoint(3); */
  /* 'calcSpherJacob:18' r=point(1); */
  /* 'calcSpherJacob:19' J=zeros(3,3); */
  /* Derivatives with respect to x. */
  /* 'calcSpherJacob:22' J(1,1)=x/r; */
  J[0] = CartPoint_idx_0 / point[0];

  /* 'calcSpherJacob:23' J(2,1)=-y/(x^2+y^2); */
  J[1] = -CartPoint_idx_1 / (CartPoint_idx_0 * CartPoint_idx_0 + CartPoint_idx_1
    * CartPoint_idx_1);

  /* 'calcSpherJacob:24' J(3,1)=-x*z/(r^2*sqrt(x^2+y^2)); */
  J[2] = -CartPoint_idx_0 * CartPoint_idx_2 / (point[0] * point[0] * sqrt
    (CartPoint_idx_0 * CartPoint_idx_0 + CartPoint_idx_1 * CartPoint_idx_1));

  /* Derivatives with respect to y. */
  /* 'calcSpherJacob:27' J(1,2)=y/r; */
  J[3] = CartPoint_idx_1 / point[0];

  /* 'calcSpherJacob:28' J(2,2)=x/(x^2+y^2); */
  J[4] = CartPoint_idx_0 / (CartPoint_idx_0 * CartPoint_idx_0 + CartPoint_idx_1 *
    CartPoint_idx_1);

  /* 'calcSpherJacob:29' J(3,2)=-y*z/(r^2*sqrt(x^2+y^2)); */
  J[5] = -CartPoint_idx_1 * CartPoint_idx_2 / (point[0] * point[0] * sqrt
    (CartPoint_idx_0 * CartPoint_idx_0 + CartPoint_idx_1 * CartPoint_idx_1));

  /* Derivatives with respect to z. */
  /* 'calcSpherJacob:32' J(1,3)=z/r; */
  J[6] = CartPoint_idx_2 / point[0];

  /* 'calcSpherJacob:33' J(2,3)=0; */
  J[7] = 0.0;

  /* 'calcSpherJacob:34' J(3,3)=sqrt(x^2+y^2)/r^2; */
  J[8] = sqrt(CartPoint_idx_0 * CartPoint_idx_0 + CartPoint_idx_1 *
              CartPoint_idx_1) / (point[0] * point[0]);
}

/*
 * Arguments    : const double x[3]
 * Return Type  : double
 */
static double norm(const double x[3])
{
  double y;
  double scale;
  int k;
  double absxk;
  double t;
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

/*
 * Arguments    : const double C[169]
 *                const double S[169]
 *                const double point[3]
 *                double a
 *                double c
 *                double *V
 *                double gradV[3]
 * Return Type  : void
 */
static void spherHarmonicEval(const double C[169], const double S[169], const
  double point[3], double a, double c, double *V, double gradV[3])
{
  int i;
  double nCoeff[13];
  int n;
  double SinVec[13];
  double CosVec[13];
  double u;
  double PBarUVals[8281];
  double dPBarUValsdTheta[8281];
  int m;
  double XC[13];
  double XS[13];
  int b_n;
  double dVdr;
  double XCdr[13];
  double XSdr[13];
  double dVdLambda;
  double XCdTheta[13];
  double XSdTheta[13];
  double dVdTheta;
  double CScal;
  double SScal;
  double dv2[9];
  double b_c[3];
  int i0;

  /* 'spherHarmonicEval:4' scalFactor=10^(-280); */
  /* 'spherHarmonicEval:5' fullyNormalized=true; */
  /* 'spherHarmonicEval:6' M = 12; */
  /* If the coefficients are Schmidt-quasi-normalized, then convert them to */
  /* fully normalized coefficients. */
  /* 'spherHarmonicEval:10' if(fullyNormalized==false) */
  /* This stores all of the powers of a/r needed for the sum, regardless of */
  /* which algorithm is used. */
  /* 'spherHarmonicEval:25' V=zeros(1,1); */
  *V = 0.0;

  /* 'spherHarmonicEval:26' gradV=zeros(3,1); */
  for (i = 0; i < 3; i++) {
    gradV[i] = 0.0;
  }

  /*  for curPoint=1:numPoints */
  /* 'spherHarmonicEval:29' r=point(1); */
  /* 'spherHarmonicEval:30' lambda=point(2); */
  /* 'spherHarmonicEval:31' thetaCur=point(3); */
  /* 'spherHarmonicEval:33' nCoeff=zeros(M+1,1); */
  memset(&nCoeff[0], 0, 13U * sizeof(double));

  /* 'spherHarmonicEval:34' nCoeff(1)=1; */
  nCoeff[0] = 1.0;

  /* 'spherHarmonicEval:35' for n=1:M */
  for (n = 0; n < 12; n++) {
    /* 'spherHarmonicEval:36' nCoeff(n+1)=nCoeff(n)*(a/r); */
    nCoeff[1 + n] = nCoeff[n] * (a / point[0]);
  }

  /* 'spherHarmonicEval:39' if(abs(thetaCur)<88*pi/180||nargout<2) */
  if (fabs(point[2]) < 1.53588974175501) {
    /* At latitudes that are not near the poles, the algorithm of Holmes and */
    /* Featherstone is used. It can not be used for the gradient near the */
    /* poles, because of the singularity of the spherical coordinate system. */
    /* Compute the sine and cosine terms. */
    /* 'spherHarmonicEval:45' [SinVec,CosVec]=calcSinCosTerms(lambda,M); */
    calcSinCosTerms(point[1], SinVec, CosVec);

    /* 'spherHarmonicEval:47' theta=pi/2-thetaCur; */
    /* 'spherHarmonicEval:48' u=sin(theta); */
    u = sin(1.5707963267948966 - point[2]);

    /* 'spherHarmonicEval:49' [PBarUVals,dPBarUValsdTheta]=NALegendreCosRat(theta,M,scalFactor); */
    NALegendreCosRat(1.5707963267948966 - point[2], 1.0E-280, PBarUVals,
                     dPBarUValsdTheta);

    /* Evaluate Equation 7 from the Holmes and Featherstone paper. */
    /* 'spherHarmonicEval:52' XC=zeros(M+1,1); */
    /* 'spherHarmonicEval:53' XS=zeros(M+1,1); */
    /* 'spherHarmonicEval:55' for m=0:M */
    for (m = 0; m < 13; m++) {
      XC[m] = 0.0;
      XS[m] = 0.0;

      /* 'spherHarmonicEval:56' for n=m:M */
      for (n = 0; n <= 12 - m; n++) {
        b_n = m + n;

        /* 'spherHarmonicEval:57' XC(m+1)=XC(m+1)+nCoeff(n+1)*C(n+1,m+1)*PBarUVals(n+1,m+1); */
        XC[m] += nCoeff[b_n] * C[b_n + 13 * m] * PBarUVals[b_n + 91 * m];

        /* 'spherHarmonicEval:58' XS(m+1)=XS(m+1)+nCoeff(n+1)*S(n+1,m+1)*PBarUVals(n+1,m+1); */
        XS[m] += nCoeff[b_n] * S[b_n + 13 * m] * PBarUVals[b_n + 91 * m];
      }
    }

    /* Use Horner's method to compute V. */
    /* 'spherHarmonicEval:63' V=0; */
    /* 'spherHarmonicEval:64' for m=M:-1:0 */
    /* Multiply by the constant in front of the sum and get rid of the scale */
    /* factor. */
    /* 'spherHarmonicEval:71' V=(c/r)*V/scalFactor; */
    for (m = 0; m < 13; m++) {
      /* 'spherHarmonicEval:65' OmegaRat=XC(m+1)*CosVec(m+1)+XS(m+1)*SinVec(m+1); */
      /* 'spherHarmonicEval:66' V=V*u+OmegaRat; */
      *V = *V * u + (XC[12 - m] * CosVec[12 - m] + XS[12 - m] * SinVec[12 - m]);
      XCdr[m] = 0.0;
      XSdr[m] = 0.0;
      XCdTheta[m] = 0.0;
      XSdTheta[m] = 0.0;

      /* 'spherHarmonicEval:85' for n=m:M */
      for (n = 0; n <= 12 - m; n++) {
        b_n = m + n;

        /* 'spherHarmonicEval:86' CScal=nCoeff(n+1)*C(n+1,m+1); */
        CScal = nCoeff[b_n] * C[b_n + 13 * m];

        /* 'spherHarmonicEval:87' SScal=nCoeff(n+1)*S(n+1,m+1); */
        SScal = nCoeff[b_n] * S[b_n + 13 * m];

        /* 'spherHarmonicEval:89' XCdr(m+1)=XCdr(m+1)+(n+1)*CScal*PBarUVals(n+1,m+1); */
        XCdr[m] += ((double)b_n + 1.0) * CScal * PBarUVals[b_n + 91 * m];

        /* 'spherHarmonicEval:90' XSdr(m+1)=XSdr(m+1)+(n+1)*SScal*PBarUVals(n+1,m+1); */
        XSdr[m] += ((double)b_n + 1.0) * SScal * PBarUVals[b_n + 91 * m];

        /* 'spherHarmonicEval:92' XCdTheta(m+1)=XCdTheta(m+1)+CScal*dPBarUValsdTheta(n+1,m+1); */
        XCdTheta[m] += CScal * dPBarUValsdTheta[b_n + 91 * m];

        /* 'spherHarmonicEval:93' XSdTheta(m+1)=XSdTheta(m+1)+SScal*dPBarUValsdTheta(n+1,m+1); */
        XSdTheta[m] += SScal * dPBarUValsdTheta[b_n + 91 * m];
      }
    }

    *V = c / point[0] * *V / 1.0E-280;

    /* If the gradient is desired. */
    /* 'spherHarmonicEval:74' dVdr=0; */
    dVdr = 0.0;

    /* 'spherHarmonicEval:75' dVdLambda=0; */
    dVdLambda = 0.0;

    /* 'spherHarmonicEval:76' dVdTheta=0; */
    dVdTheta = 0.0;

    /* 'spherHarmonicEval:78' XCdr=zeros(M+1,1); */
    /* 'spherHarmonicEval:79' XSdr=zeros(M+1,1); */
    /* 'spherHarmonicEval:80' XCdTheta=zeros(M+1,1); */
    /* 'spherHarmonicEval:81' XSdTheta=zeros(M+1,1); */
    /* Evaluate Equation 7 from the Holmes and Featherstone paper. */
    /* 'spherHarmonicEval:84' for m=0:M */
    /* 'spherHarmonicEval:97' for m=M:-1:0 */
    for (m = 0; m < 13; m++) {
      /* 'spherHarmonicEval:98' OmegaRat=XCdr(m+1)*CosVec(m+1)+XSdr(m+1)*SinVec(m+1); */
      /* 'spherHarmonicEval:99' dVdr=dVdr*u+OmegaRat; */
      dVdr = dVdr * u + (XCdr[12 - m] * CosVec[12 - m] + XSdr[12 - m] * SinVec
                         [12 - m]);

      /* 'spherHarmonicEval:101' OmegaRat=m*(-XC(m+1)*SinVec(m+1)+XS(m+1)*CosVec(m+1)); */
      /* 'spherHarmonicEval:102' dVdLambda=dVdLambda*u+OmegaRat; */
      dVdLambda = dVdLambda * u + (12.0 + -(double)m) * (-XC[12 - m] * SinVec[12
        - m] + XS[12 - m] * CosVec[12 - m]);

      /* 'spherHarmonicEval:104' OmegaRat=XCdTheta(m+1)*CosVec(m+1)+XSdTheta(m+1)*SinVec(m+1); */
      /* 'spherHarmonicEval:105' dVdTheta=dVdTheta*u+OmegaRat; */
      dVdTheta = dVdTheta * u + (XCdTheta[12 - m] * CosVec[12 - m] + XSdTheta[12
        - m] * SinVec[12 - m]);
    }

    /* 'spherHarmonicEval:108' dVdr=-(c/r^2)*dVdr/scalFactor; */
    /* 'spherHarmonicEval:109' dVdLambda=(c/r)*dVdLambda/scalFactor; */
    /* The minus sign is because the input coordinate was with respect to */
    /* latitude, not the co-latitude that the NALegendreCosRat function uses. */
    /* 'spherHarmonicEval:112' dVdTheta=-(c/r)*dVdTheta/scalFactor; */
    /* 'spherHarmonicEval:114' gradV(:) = calcSpherJacob(point)'*[dVdr;dVdLambda;dVdTheta]; */
    calcSpherJacob(point, dv2);
    b_c[0] = -(c / (point[0] * point[0])) * dVdr / 1.0E-280;
    b_c[1] = c / point[0] * dVdLambda / 1.0E-280;
    b_c[2] = -(c / point[0]) * dVdTheta / 1.0E-280;
    for (i = 0; i < 3; i++) {
      gradV[i] = 0.0;
      for (i0 = 0; i0 < 3; i0++) {
        gradV[i] += dv2[i0 + 3 * i] * b_c[i0];
      }
    }
  }
}

/*
 * MAGHEADING2GEOG Convert a heading in terms of radians clockwise
 *                 (East) of MAGNETIC North to a heading in terms of radians
 *                 clockwise from GEOGRAPHIC North as defined on a particular
 *                 reference ellipsoid. The WMM2010 model for the Earth's
 *                 magnetic field is used. In the rare spots (i.e. near the
 *                 magnetic poles), where the magnetic field vector points
 *                 directly towards the reference ellipsoid, the geographic
 *                 heading will be undefined and a NaN will be returned.
 *
 * INPUTS: points  One or more points given in geodetic latitude and
 *                 longitude, in radians, and height, in meters where the
 *                 magnetic headings apply. To convert N headings, points is
 *                 a 3XN matrix with each column having the format
 *                 [latitude;longitude; height]. All points should be
 *                 associated with the same time. At the geographic poles,
 *                 the longitude value determines the orientation of the
 *                 local coordinate system axes. Thus, geographic headings
 *                 ARE defined at the poles.
 *   PEastOfNorth  An NX1 array of N magnetic headings in radians clockwise
 *                 from North that should be turned into geographic headings.
 *     year        A decimal number indicating a year in the Gregorian
 *                 calendar as specified by UTC. For example, halfway through
 *                 the year 2012 would be represented as 2012.5. The
 *                 precision of the model is not sufficiently fine that leap
 *                 seconds matter. If this parameter is omitted, then the
 *                 last reference epoch of the geomagnetic model is used.
 *            a    The semi-major axis of the reference ellipsoid. If
 *                 this argument is omitted, the value in
 *                 Constants.WGS84SemiMajorAxis is used.
 *            f    The flattening factor of the reference ellipsoid. If
 *                 this argument is omitted, the value in
 *                 Constants.WGS84Flattening is used.
 *
 * OUTPUT: geoEastOfNorth The headings converted to radians clockwise of
 *                        geographic North on the reference ellipse.
 * Arguments    : const double points[3]
 * Return Type  : double
 */
double magHeading2Geog(const double points[3])
{
  double sinP;
  double cosP;
  double Ne;
  double cartPoints_idx_0;
  double cartPoints_idx_1;
  double cartPoints_idx_2;
  double r;
  double b_r[3];
  static const double dv0[169] = { 0.0, -1.70298699501787E-5,
    -1.0717921029752E-6, 5.06510190279666E-7, 3.042E-7, -6.96189694630056E-8,
    2.01910871425983E-8, 2.07850106246465E-8, 5.91786925088652E-9,
    1.23884496290103E-9, -4.36435780471985E-10, 6.25543242171224E-10, -4.4E-10,
    0.0, -9.15850732015503E-7, 1.35331306134242E-6, -8.79220957114064E-7,
    2.69633333333333E-7, 1.07699852283177E-7, 1.90262167305254E-8,
    -1.93907366200118E-8, 1.9645385627943E-9, 2.15650789838328E-9,
    -1.37477270848675E-9, -3.12771621085612E-10, -4.0E-11, 0.0, 0.0,
    7.4622060545123E-7, 4.65614434300067E-7, 5.55666666666667E-8,
    6.03927223189261E-8, 2.10786074565587E-8, -1.21353478181166E-9,
    -3.51676656302683E-9, 7.8001349515991E-10, 1.96396101212393E-10,
    -4.37880269519857E-10, 6.0E-11, 0.0, 0.0, 0.0, 2.3962947588785E-7,
    -1.19033333333333E-7, -4.25432507199225E-8, -3.92173038731237E-8,
    1.16964097055464E-8, -1.35819950020346E-9, -1.19296181612692E-9,
    -2.40039679259592E-10, 3.54474503897027E-10, 2.0E-10, 0.0, 0.0, 0.0, 0.0,
    2.98E-8, -4.91463491661755E-8, -6.32358223696761E-9, 3.58896456748554E-9,
    -4.68093756320123E-9, 7.11188774998742E-10, -4.36435780471985E-11,
    -1.04257207028537E-10, -1.2E-10, 0.0, 0.0, 0.0, 0.0, 0.0,
    -2.35178848770656E-9, 3.66102129508651E-9, 2.68526845337048E-9,
    2.78915968791783E-9, -2.84475509999497E-9, 5.45544725589981E-10,
    1.04257207028537E-10, 1.8E-10, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -2.16055726429727E-8, 4.38938112570174E-10, 2.64363831289603E-9,
    -1.60591013709393E-10, -6.54653670707977E-11, -1.6681153124566E-10, -2.0E-11,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.26517455976109E-9, -3.4197523130123E-9,
    1.92709216451272E-9, 4.80079358519183E-10, 8.34057656228299E-11, 1.0E-10,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -8.97381812634432E-10,
    -1.95003373789978E-9, 6.76475459731577E-10, 3.75325945302735E-10, -8.0E-11,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -2.31709891209267E-9,
    -2.18217890235992E-10, 2.08514414057075E-11, -8.0E-11, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -6.11010092660779E-10, 1.45960089839952E-10,
    4.0E-11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    7.92354773416884E-10, -1.6E-10, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0 };

  static const double dv1[169] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 2.85465067098119E-6, -1.21092025253524E-6,
    -6.05499085760782E-8, 9.54666666666667E-8, 1.34474059681683E-8,
    -5.76888204074238E-9, -1.49497157163606E-8, 2.66789187539966E-9,
    -4.70302254434652E-9, 6.11010092660779E-10, 4.1702882811415E-11, -1.8E-10,
    0.0, 0.0, -2.57639752367526E-7, 9.52092507510243E-8, -7.04E-8,
    5.69554929907396E-8, 1.22311393267663E-8, -5.4479965736651E-9,
    -4.85071250072666E-9, 2.63828093951146E-9, -2.18217890235992E-11,
    3.54474503897027E-10, 6.0E-11, 0.0, 0.0, 0.0, -2.02815736216751E-7,
    5.47666666666667E-8, -3.56386409290917E-8, 1.70570310339258E-8,
    1.67829278335655E-9, 2.88617393793236E-9, 2.93652139354319E-9,
    1.02562408410916E-9, -1.25108648434245E-10, 4.2E-10, 0.0, 0.0, 0.0, 0.0,
    -1.03033333333333E-7, 0.0, -1.83883115048663E-8, 6.42915235470431E-9,
    -4.22011987563219E-9, -1.65179328386804E-9, 9.60158717038367E-10,
    -3.75325945302735E-10, -5.0E-10, 0.0, 0.0, 0.0, 0.0, 0.0,
    3.04224946678964E-8, 8.59785304149105E-10, 1.80739222823013E-9,
    4.05034493810676E-9, -1.69767643064216E-9, -1.57116880969915E-9,
    1.87662972651367E-10, 1.0E-10, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    1.52542553961938E-8, -7.15210924599636E-9, 1.69774937525433E-9,
    1.83532587096449E-9, -2.18217890235992E-10, -8.34057656228299E-11, 1.2E-10,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -8.52056336165632E-10,
    -2.6193847503924E-9, 4.8177304112818E-10, -8.5104977192037E-10,
    -5.21286035142687E-10, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    4.12310562561766E-10, -1.39943597661043E-9, -4.36435780471985E-10,
    -2.71068738274197E-10, 2.0E-11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    1.60591013709393E-9, -4.36435780471985E-10, -4.37880269519857E-10, 6.0E-11,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.81120848895874E-9,
    -3.96177386708442E-10, -1.8E-10, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, -3.75325945302735E-10, -4.0E-11, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 1.8E-10 };

  double unusedU0;
  double gradV[3];
  double sinL;
  double cosL;
  double u3[3];
  double c3;
  double dNedPhi;
  double u2[3];
  double c2;
  double B[3];
  int k;
  double u[9];
  double b_u3;
  double b_u2;
  double vEast;
  double vNorth;

  /* 'magHeading2Geog:38' f = 1/298.257223563; */
  /* flattening */
  /* 'magHeading2Geog:39' a = 6378136.3; */
  /* seminmajor [m] */
  /* 'magHeading2Geog:40' aH = 6371200; */
  /* 'magHeading2Geog:41' cH = 40592189440000.0; */
  /* 'magHeading2Geog:42' C = [0,0,0,0,0,0,0,0,0,0,0,0,0;-1.70298699501787e-05,-9.15850732015503e-07,0,0,0,0,0,0,0,0,0,0,0;-1.07179210297520e-06,1.35331306134242e-06,7.46220605451230e-07,0,0,0,0,0,0,0,0,0,0;5.06510190279666e-07,-8.79220957114064e-07,4.65614434300067e-07,2.39629475887850e-07,0,0,0,0,0,0,0,0,0;3.04200000000000e-07,2.69633333333333e-07,5.55666666666667e-08,-1.19033333333333e-07,2.98000000000000e-08,0,0,0,0,0,0,0,0;-6.96189694630056e-08,1.07699852283177e-07,6.03927223189261e-08,-4.25432507199225e-08,-4.91463491661755e-08,-2.35178848770656e-09,0,0,0,0,0,0,0;2.01910871425983e-08,1.90262167305254e-08,2.10786074565587e-08,-3.92173038731237e-08,-6.32358223696761e-09,3.66102129508651e-09,-2.16055726429727e-08,0,0,0,0,0,0;2.07850106246465e-08,-1.93907366200118e-08,-1.21353478181166e-09,1.16964097055464e-08,3.58896456748554e-09,2.68526845337048e-09,4.38938112570174e-10,1.26517455976109e-09,0,0,0,0,0;5.91786925088652e-09,1.96453856279430e-09,-3.51676656302683e-09,-1.35819950020346e-09,-4.68093756320123e-09,2.78915968791783e-09,2.64363831289603e-09,-3.41975231301230e-09,-8.97381812634432e-10,0,0,0,0;1.23884496290103e-09,2.15650789838328e-09,7.80013495159910e-10,-1.19296181612692e-09,7.11188774998742e-10,-2.84475509999497e-09,-1.60591013709393e-10,1.92709216451272e-09,-1.95003373789978e-09,-2.31709891209267e-09,0,0,0;-4.36435780471985e-10,-1.37477270848675e-09,1.96396101212393e-10,-2.40039679259592e-10,-4.36435780471985e-11,5.45544725589981e-10,-6.54653670707977e-11,4.80079358519183e-10,6.76475459731577e-10,-2.18217890235992e-10,-6.11010092660779e-10,0,0;6.25543242171224e-10,-3.12771621085612e-10,-4.37880269519857e-10,3.54474503897027e-10,-1.04257207028537e-10,1.04257207028537e-10,-1.66811531245660e-10,8.34057656228299e-11,3.75325945302735e-10,2.08514414057075e-11,1.45960089839952e-10,7.92354773416884e-10,0;-4.40000000000000e-10,-4.00000000000000e-11,6.00000000000000e-11,2.00000000000000e-10,-1.20000000000000e-10,1.80000000000000e-10,-2.00000000000000e-11,1.00000000000000e-10,-8.00000000000000e-11,-8.00000000000000e-11,4.00000000000000e-11,-1.60000000000000e-10,0]; */
  /* 'magHeading2Geog:43' S = [0,0,0,0,0,0,0,0,0,0,0,0,0;0,2.85465067098119e-06,0,0,0,0,0,0,0,0,0,0,0;0,-1.21092025253524e-06,-2.57639752367526e-07,0,0,0,0,0,0,0,0,0,0;0,-6.05499085760782e-08,9.52092507510243e-08,-2.02815736216751e-07,0,0,0,0,0,0,0,0,0;0,9.54666666666667e-08,-7.04000000000000e-08,5.47666666666667e-08,-1.03033333333333e-07,0,0,0,0,0,0,0,0;0,1.34474059681683e-08,5.69554929907396e-08,-3.56386409290917e-08,0,3.04224946678964e-08,0,0,0,0,0,0,0;0,-5.76888204074238e-09,1.22311393267663e-08,1.70570310339258e-08,-1.83883115048663e-08,8.59785304149105e-10,1.52542553961938e-08,0,0,0,0,0,0;0,-1.49497157163606e-08,-5.44799657366510e-09,1.67829278335655e-09,6.42915235470431e-09,1.80739222823013e-09,-7.15210924599636e-09,-8.52056336165632e-10,0,0,0,0,0;0,2.66789187539966e-09,-4.85071250072666e-09,2.88617393793236e-09,-4.22011987563219e-09,4.05034493810676e-09,1.69774937525433e-09,-2.61938475039240e-09,4.12310562561766e-10,0,0,0,0;0,-4.70302254434652e-09,2.63828093951146e-09,2.93652139354319e-09,-1.65179328386804e-09,-1.69767643064216e-09,1.83532587096449e-09,4.81773041128180e-10,-1.39943597661043e-09,1.60591013709393e-09,0,0,0;0,6.11010092660779e-10,-2.18217890235992e-11,1.02562408410916e-09,9.60158717038367e-10,-1.57116880969915e-09,-2.18217890235992e-10,-8.51049771920370e-10,-4.36435780471985e-10,-4.36435780471985e-10,-1.81120848895874e-09,0,0;0,4.17028828114150e-11,3.54474503897027e-10,-1.25108648434245e-10,-3.75325945302735e-10,1.87662972651367e-10,-8.34057656228299e-11,-5.21286035142687e-10,-2.71068738274197e-10,-4.37880269519857e-10,-3.96177386708442e-10,-3.75325945302735e-10,0;0,-1.80000000000000e-10,6.00000000000000e-11,4.20000000000000e-10,-5.00000000000000e-10,1.00000000000000e-10,1.20000000000000e-10,0,2.00000000000000e-11,6.00000000000000e-11,-1.80000000000000e-10,-4.00000000000000e-11,1.80000000000000e-10]; */
  /* 'magHeading2Geog:45' [~,gradV]=spherHarmonicEval(C,S,ellips2Sphere(points),aH,cH); */
  /* ELLIPS2SPHERE Convert ellipsoidal coordinates to spherical coordinates. */
  /*                If the ellipsoidal coordinates are not full coordinates, */
  /*                then it is assumed that one is just converting latitudes on */
  /*                the surface of the reference ellipsoid. */
  /*  */
  /* INPUTS:    points  One or more points given in geodetic latitude and */
  /*                    longitude, in radians, and height, in meters that are */
  /*                    to be converted to Cartesian coordinates. To convert */
  /*                    N points, points is a 3XN matrix with each column */
  /*                    having the format [latitude;longitude; height]. */
  /*                    Alternately, one can just provide points as */
  /*                    [latitude;longitude], where height is implied as zero, */
  /*                    and the return values are 2D geocentric (latitude */
  /*                    longitude pairs). Also, one can just provide points as */
  /*                    latitudes, in which case spherical latitudes for points */
  /*                    on the surface of the reference ellipsoid are returned. */
  /*            a       The semi-major axis of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84SemiMajorAxis is used. */
  /*            f       The flattening factor of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84Flattening is used. */
  /*  */
  /* OUTPUTS:   points  A matrix of the converted points. Each column of the */
  /*                    matrix has the format [r;azimuth;elevation], */
  /*                    with azimuth and elevation given in radians, unless the */
  /*                    input points were given as 2D [latitude;longitude] */
  /*                    points, in which case the output points are of the form */
  /*                    [azimuth;elevation], unless the input points were just */
  /*                    given as latitudes, in which case the output points are */
  /*                    just elevations (geocentric latitudes). */
  /* 'ellips2Sphere:35' f = 1/298.257223563; */
  /* flattening */
  /* 'ellips2Sphere:36' a = 6378136.3; */
  /* seminmajor [m] */
  /* 'ellips2Sphere:38' pDim=size(points,1); */
  /* 'ellips2Sphere:39' if(pDim<3) */
  /* 'ellips2Sphere:51' else */
  /* 'ellips2Sphere:53' cartPoints = ellips2Cart(points,a,f); */
  /* #codgen */
  /* ELLIPS2CART Convert ellipsoidal coordinates to ECEF Cartesian */
  /*              coordinates. */
  /*  */
  /* INPUTS:    points  One or more points given in geodetic latitude and */
  /*                    longitude, in radians, and height, in meters that are */
  /*                    to be converted to Cartesian coordinates. To convert */
  /*                    N points, points is a 3XN matrix with each column */
  /*                    having the format [latitude;longitude; height]. */
  /*            a       The semi-major axis of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84SemiMajorAxis is used. */
  /*            f       The flattening factor of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84Flattening is used. */
  /*  */
  /* OUTPUTS:   cartPoints For N points, cartPoints is a 3XN matrix of the */
  /*                       converted points with each column having the format */
  /*                       [x;y;z]. */
  /*  */
  /* The conversions are mentioned in the paper "Simulating Aerial Targets in */
  /* 3D Accounting for the Earth's Curvature" by David F. Crouse. */
  /*  */
  /* September 2013 David F. Crouse, Naval Research Laboratory, Washington D.C. */
  /* (UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release. */
  /*  f = 1/298.257223563;    %flattening */
  /*  a = 6378136.3;          %seminmajor [m] */
  /* The geodetic latitudes */
  /* 'ellips2Cart:32' phi=points(1,:); */
  /* The longitudes */
  /* 'ellips2Cart:34' lambda=points(2,:); */
  /* The altitudes */
  /* 'ellips2Cart:36' h=points(3,:); */
  /* 'ellips2Cart:38' sinP=sin(phi); */
  sinP = sin(points[0]);

  /* 'ellips2Cart:39' cosP=cos(phi); */
  cosP = cos(points[0]);

  /* 'ellips2Cart:40' sinL=sin(lambda); */
  /* 'ellips2Cart:41' cosL=cos(lambda); */
  /* The square of the first numerical eccentricity */
  /* 'ellips2Cart:44' e2=2*f-f^2; */
  /* The normal radii of curvature. */
  /* 'ellips2Cart:46' Ne=a./sqrt(1-e2*sinP.^2); */
  Ne = 6.3781363E+6 / sqrt(1.0 - 0.0066943799901413165 * (sinP * sinP));

  /* 'ellips2Cart:48' x=(Ne+h).*cosP.*cosL; */
  /* 'ellips2Cart:49' y=(Ne+h).*cosP.*sinL; */
  /* 'ellips2Cart:50' z=(Ne*(1-e2)+h).*sinP; */
  /* 'ellips2Cart:52' cartPoints=[x;y;z]; */
  cartPoints_idx_0 = (Ne + points[2]) * cosP * cos(points[1]);
  cartPoints_idx_1 = (Ne + points[2]) * cosP * sin(points[1]);
  cartPoints_idx_2 = (Ne * 0.99330562000985867 + points[2]) * sinP;

  /* 'ellips2Sphere:54' x=cartPoints(1,:); */
  /* 'ellips2Sphere:55' y=cartPoints(2,:); */
  /* 'ellips2Sphere:56' z=cartPoints(3,:); */
  /* 'ellips2Sphere:57' r=sqrt(x.^2+y.^2+z.^2); */
  r = sqrt(cartPoints_idx_0 * cartPoints_idx_0 + cartPoints_idx_1 *
           cartPoints_idx_1 + cartPoints_idx_2 * cartPoints_idx_2);

  /* 'ellips2Sphere:58' azimuth=atan2(y,x); */
  /* 'ellips2Sphere:59' elevation=asin(z./r); */
  /* 'ellips2Sphere:60' points=[r;azimuth;elevation]; */
  b_r[0] = r;
  b_r[1] = atan2(cartPoints_idx_1, cartPoints_idx_0);
  b_r[2] = asin(cartPoints_idx_2 / r);
  spherHarmonicEval(dv0, dv1, b_r, 6.3712E+6, 4.059218944E+13, &unusedU0, gradV);

  /* 'magHeading2Geog:45' ~ */
  /* 'magHeading2Geog:46' B=-gradV; */
  /* B is now a matrix of vectors of the magnetic flux of the Earth's field at */
  /* the points. The direction of B in the local tangent plane to the reference */
  /* ellipsoid approximately defined magnetic north. */
  /* 'magHeading2Geog:50' numPoints=size(points,2); */
  /* 'magHeading2Geog:51' geoEastOfNorth=zeros(numPoints,1); */
  /* Allocate space */
  /* 'magHeading2Geog:52' for curPoint=1:numPoints */
  /* -B(:,curPoint) defines the direction of magnetic North at the current */
  /* point. We want the direction vector PEastOfNorth(curPoint). To go East */
  /* of North, one must rotate that number of degrees about the local */
  /* "Down" vector. */
  /* 'magHeading2Geog:57' u=getENUAxes(points(:,curPoint),a,f); */
  /* GETENUAXES Find the East-North-Up unit vectors at the given point as well */
  /*             as the magnitudes of the derivatives of a position vector with */
  /*             respect to latitude, longitude and height. */
  /*  */
  /* INPUTS:   plhPoint The point at which the axes are to be found given in */
  /*                    terms of [latitude;longitude;height] with the geodetic  */
  /*                    latitude and longitude in radians and the height in */
  /*                    meters. The latitude should be between -pi/2 and pi/2. */
  /*       justVertical An optional parameter. If this is given and is */
  /*                    true, then u and c only for the Up direction will be */
  /*                    returned. */
  /*            a       The semi-major axis of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84SemiMajorAxis is used. */
  /*            f       The flattening factor of the reference ellipsoid. If */
  /*                    this argument is omitted, the value in */
  /*                    Constants.WGS84Flattening is used. */
  /*  */
  /* OUTPUTS:   u       u(:,1), u(:,2) and u(:,3) are respectively the East, */
  /*                    North and Up unit vectors. */
  /*            c       c(1), c(2) and c(3) are the respective magnitudes of */
  /*                    the derivative of the Cartesian position with respect */
  /*                    to latitude, longitude and height. */
  /* The latitude */
  /* 'getENUAxes:28' phi=plhPoint(1); */
  /* The longitude */
  /* 'getENUAxes:30' lambda=plhPoint(2); */
  /* The height */
  /* 'getENUAxes:32' h=plhPoint(3); */
  /* 'getENUAxes:34' sinP=sin(phi); */
  sinP = sin(points[0]);

  /* 'getENUAxes:35' cosP=cos(phi); */
  cosP = cos(points[0]);

  /* 'getENUAxes:36' sinL=sin(lambda); */
  sinL = sin(points[1]);

  /* 'getENUAxes:37' cosL=cos(lambda); */
  cosL = cos(points[1]);

  /* u3 is dr/dh (Up) */
  /* 'getENUAxes:40' u3=[cosP*cosL; */
  /* 'getENUAxes:41'     cosP*sinL; */
  /* 'getENUAxes:42'     sinP]; */
  u3[0] = cosP * cosL;
  u3[1] = cosP * sinL;
  u3[2] = sinP;

  /* 'getENUAxes:43' c3=norm(u3); */
  c3 = norm(u3);

  /* Barring precision problems, this is always one. */
  /* 'getENUAxes:44' u3=u3/c3; */
  /* The square of the first numerical eccentricity */
  /* 'getENUAxes:47' e2=2*f-f^2; */
  /* The normal radius of curvature. */
  /* 'getENUAxes:49' Ne=a/sqrt(1-e2*sinP^2); */
  Ne = 6.3781363E+6 / sqrt(1.0 - 0.0066943799901413165 * (sinP * sinP));

  /* The derivative of the normal radius of curvature with respect to phi. */
  /* 'getENUAxes:51' dNedPhi=a*e2*cosP*sinP/(1-e2*sinP^2)^(3/2); */
  dNedPhi = 42697.668021113968 * cosP * sinP / pow(1.0 - 0.0066943799901413165 *
    (sinP * sinP), 1.5);

  /* u1 is dr/dlambda, normalized (East). */
  /* 'getENUAxes:54' u1=[-(Ne+h)*cosP*sinL; */
  /* 'getENUAxes:55'      (Ne+h)*cosP*cosL; */
  /* 'getENUAxes:56'     0]; */
  /* 'getENUAxes:57' c1=norm(u1); */
  /* u2 is dr/dphi, normalized (North) */
  /* 'getENUAxes:60' u2=[(cosP*dNedPhi-(Ne+h)*sinP)*cosL; */
  /* 'getENUAxes:61'     (cosP*dNedPhi-(Ne+h)*sinP)*sinL; */
  /* 'getENUAxes:62'     (Ne*(1-e2)+h)*cosP+(1-e2)*dNedPhi*sinP]; */
  u2[0] = (cosP * dNedPhi - (Ne + points[2]) * sinP) * cosL;
  u2[1] = (cosP * dNedPhi - (Ne + points[2]) * sinP) * sinL;
  u2[2] = (Ne * 0.99330562000985867 + points[2]) * cosP + 0.99330562000985867 *
    dNedPhi * sinP;

  /* 'getENUAxes:63' c2=norm(u2); */
  c2 = norm(u2);

  /* 'getENUAxes:64' u2=u2/c2; */
  /* If the point is too close to the poles, then it is possible that c1 is */
  /* nearly equal to zero. However, u1 can just be found by orthogonality: */
  /* it is orthogonal to u3 and u2. */
  /* 'getENUAxes:69' u1=cross(u2,u3); */
  /* 'getENUAxes:70' u=[u1,u2,u3]; */
  for (k = 0; k < 3; k++) {
    B[k] = -gradV[k];
    b_u3 = u3[k] / c3;
    b_u2 = u2[k] / c2;
    u[3 + k] = b_u2;
    u[6 + k] = b_u3;
    u3[k] = b_u3;
    u2[k] = b_u2;
  }

  u[0] = u2[1] * u3[2] - u2[2] * u3[1];
  u[1] = u2[2] * u3[0] - u2[0] * u3[2];
  u[2] = u2[0] * u3[1] - u2[1] * u3[0];

  /* 'getENUAxes:71' c=[c1;c2;c3]; */
  /*      %The negative of u(:,3) is a unit vector in the down direction.  */
  /* 'magHeading2Geog:60' vRot = B(:,curPoint); */
  /* Given the rotated field vector, only the components in the local */
  /* tangent plane matter. */
  /* 'magHeading2Geog:64' vEast=dot(vRot,u(:,1)); */
  vEast = 0.0;

  /* 'magHeading2Geog:65' vNorth=dot(vRot,u(:,2)); */
  vNorth = 0.0;
  for (k = 0; k < 3; k++) {
    vEast += B[k] * u[k];
    vNorth += B[k] * u[3 + k];
  }

  /* Find the angle East of North. */
  /* 'magHeading2Geog:67' geoEastOfNorth(curPoint)=atan2(vEast,vNorth); */
  return atan2(vEast, vNorth);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void magHeading2Geog_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void magHeading2Geog_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for magHeading2Geog.c
 *
 * [EOF]
 */
