/* bieval.f -- translated by f2c (version 19991025).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* ii_bieval:  Evaluation routines for interpolation kernels. */
/*             Called by 'mrievl' from INTER2D.F */

/* All routines converted from SPP to Fortran using 'xc -f' then hand */
/*  edited to include all comments and to use F77 constructs. */
/*  Warren Hack, 15 June 2004 */


/* II_BINEAREST -- Procedure to evaluate the nearest neighbour interpolant. */
/* The real array coeff contains the coefficients of the 2D interpolant. */
/* The procedure assumes that 1 <= x <= nxpix and 1 <= y <= nypix and that */
/* coeff(1+first_point) = datain(1,1). */

/* Subroutine */ int iibint_(coeff, firstt, lencof, x, y, zfit, npts)
real *coeff;
integer *firstt, *lencof;
real *x, *y, *zfit;
integer *npts;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, index, nx, ny;


/* real  coeff[ARB]   # 1D coefficient array */
/* int   firstt       # offset of first data point */
/* int   lencof       # row length of coeff */
/* real  x[npts]      # array of x values */
/* real  y[npts]      # array of y values */
/* real  zfit[npts]	# array of interpolated values */
/* int	  npts		   # number of points to be evaluated */

    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;

    /* Function Body */
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nx = (integer) (x[i__] + (float).5);
	ny = (integer) (y[i__] + (float).5);
/* 	      define pointer to data[nx,ny] */
	index = *firstt + (ny - 1) * *lencof + nx;
	zfit[i__] = coeff[index];
    }
    return 0;
} /* iibint_ */

/* II_BILINEAR -- Procedure to evaluate the bilinear interpolant. */
/* The real array coeff contains the coefficients of the 2D interpolant. */
/* The procedure assumes that 1 <= x <= nxpix and 1 <= y <= nypix */
/* and that coeff(1+first_point) = datain(1,1). */

/* Subroutine */ int iibilr_(coeff, firstt, lencof, x, y, zfit, npts)
real *coeff;
integer *firstt, *lencof;
real *x, *y, *zfit;
integer *npts;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, index, nx, ny;
    static real sx, sy, tx, ty;

/* real   coeff[ARB]     # 1D array of coefficients */
/* int    firstt         # offset of first data point */
/* int    lencof         # row length of coeff */
/* real   x[npts]        # array of x values */
/* real   y[npts]        # array of y values */
/* real   zfit[npts]	   # array of interpolated values */
/* int	   npts		      # number of data points */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;

    /* Function Body */
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nx = (integer) x[i__];
	ny = (integer) y[i__];
	sx = x[i__] - nx;
	tx = (float)1. - sx;
	sy = y[i__] - ny;
	ty = (float)1. - sy;
	index = *firstt + (ny - 1) * *lencof + nx;
	zfit[i__] = tx * ty * coeff[index] + sx * ty * coeff[index + 1] + sy *
		 tx * coeff[index + *lencof] + sx * sy * coeff[index + *
		lencof + 1];
    }
    return 0;
} /* iibilr_ */

/* II_BIPOLY3 -- Procedure to evaluate the bicubic polynomial interpolant. */
/* The real array coeff contains the coefficients of the 2D interpolant. */
/* The procedure assumes that 1 <= x <= nxpix and  1 <= y <= nypix */
/* and that coeff[1+first_point] = datain[1,1]. The interpolant is */
/* evaluated using Everett's central difference formula. */
/* Subroutine */ int iibip3_(coeff, firstt, lencof, x, y, zfit, npts)
real *coeff;
integer *firstt, *lencof;
real *x, *y, *zfit;
integer *npts;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real cd20y, cd21y, sx2m1, tx2m1;
    static integer i__, j, index, nxold, nyold;
    static real ztemp[4];
    static integer nx, ny;
    static real sx, sy, tx, ty;
    static integer firstw;
    static real cd20[4], cd21[4];

/* real   coeff[ARB]     # 1D array of coefficients */
/* int    firstt         # offset of first data point */
/* int    lencof         # row length of coeff */
/* real   x[npts]        # array of x values */
/* real   y[npts]        # array of y values */
/* real   zfit[npts]	   # array of interpolated values */
/* int	   npts		      # number of data points */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;

    /* Function Body */
    nxold = -1;
    nyold = -1;
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nx = (integer) x[i__];
	sx = x[i__] - nx;
	tx = (float)1. - sx;
	sx2m1 = sx * sx - (float)1.;
	tx2m1 = tx * tx - (float)1.;
	ny = (integer) y[i__];
	sy = y[i__] - ny;
	ty = (float)1. - sy;
/* 	         define pointer to datain[nx,ny-1] */
	firstw = *firstt + (ny - 2) * *lencof + nx;
/* 	         loop over the 4 surrounding rows of data */
/* 	         calculate the  central differences at each value of y */

/* 	         if new data point caculate the central differnences in x */
/* 	         for each y */
	index = firstw;
	if (nx != nxold || ny != nyold) {
	    for (j = 1; j <= 4; ++j) {
		cd20[j - 1] = (coeff[index + 1] - coeff[index] * (float)2. + 
			coeff[index - 1]) * (float).16666666666666666;
		cd21[j - 1] = (coeff[index + 2] - coeff[index + 1] * (float)
			2. + coeff[index]) * (float).16666666666666666;
		index += *lencof;
	    }
	}
/* 	         interpolate in x at each value of y */
	index = firstw;
	for (j = 1; j <= 4; ++j) {
	    ztemp[j - 1] = sx * (coeff[index + 1] + sx2m1 * cd21[j - 1]) + tx 
		    * (coeff[index] + tx2m1 * cd20[j - 1]);
	    index += *lencof;
	}
/*  	      calculate y central differences */
	cd20y = (ztemp[2] - ztemp[1] * (float)2. + ztemp[0]) * (float)
		.16666666666666666;
	cd21y = (ztemp[3] - ztemp[2] * (float)2. + ztemp[1]) * (float)
		.16666666666666666;
/* 	         interpolate in y */
	zfit[i__] = sy * (ztemp[2] + (sy * sy - (float)1.) * cd21y) + ty * (
		ztemp[1] + (ty * ty - (float)1.) * cd20y);
	nxold = nx;
	nyold = ny;
    }
    return 0;
} /* iibip3_ */

/* II_BIPOLY5 -- Procedure to evaluate a biquintic polynomial. */
/* The real array coeff contains the coefficents of the 2D interpolant. */
/* The routine assumes that 1 <= x <= nxpix and 1 <= y <= nypix */
/* and that coeff[1+first_point] = datain[1,1]. The interpolant is evaluated */
/* using Everett's central difference formula. */

/* Subroutine */ int iibip5_(coeff, firstt, lencof, x, y, zfit, npts)
real *coeff;
integer *firstt, *lencof;
real *x, *y, *zfit;
integer *npts;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real cd20y, cd21y, cd40y, cd41y, sx2m1, tx2m1, sx2m4, tx2m4;
    static integer i__, j, index, nxold, nyold;
    static real ztemp[6];
    static integer nx, ny;
    static real sx, tx, sy, ty;
    static integer firstw;
    static real sx2, tx2, sy2, ty2, cd20[6], cd21[6], cd40[6], cd41[6];

/* real   coeff[ARB]     # 1D array of coefficients */
/* int    firstt         # offset of first data point */
/* int    lencof         # row length of coeff */
/* real   x[npts]        # array of x values */
/* real   y[npts]        # array of y values */
/* real   zfit[npts]	   # array of interpolated values */
/* int	   npts		      # number of data points */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;

    /* Function Body */
    nxold = -1;
    nyold = -1;
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nx = (integer) x[i__];
	sx = x[i__] - nx;
	sx2 = sx * sx;
	sx2m1 = sx2 - (float)1.;
	sx2m4 = sx2 - (float)4.;
	tx = (float)1. - sx;
	tx2 = tx * tx;
	tx2m1 = tx2 - (float)1.;
	tx2m4 = tx2 - (float)4.;
	ny = (integer) y[i__];
	sy = y[i__] - ny;
	sy2 = sy * sy;
	ty = (float)1. - sy;
	ty2 = ty * ty;
/* 	      calculate value of pointer to data[nx,ny-2] */
	firstw = *firstt + (ny - 3) * *lencof + nx;
/* 	      calculate the central differences in x at each value of y */
	index = firstw;
	if (nx != nxold || ny != nyold) {
	    for (j = 1; j <= 6; ++j) {
		cd20[j - 1] = (coeff[index + 1] - coeff[index] * (float)2. + 
			coeff[index - 1]) * (float).16666666666666666;
		cd21[j - 1] = (coeff[index + 2] - coeff[index + 1] * (float)
			2. + coeff[index]) * (float).16666666666666666;
		cd40[j - 1] = (coeff[index - 2] - coeff[index - 1] * (float)
			4. + coeff[index] * (float)6. - coeff[index + 1] * (
			float)4. + coeff[index + 2]) * (float)
			.0083333333333333332;
		cd41[j - 1] = (coeff[index - 1] - coeff[index] * (float)4. + 
			coeff[index + 1] * (float)6. - coeff[index + 2] * (
			float)4. + coeff[index + 3]) * (float)
			.0083333333333333332;
		index += *lencof;
/* L200: */
	    }
	}
/* 	      interpolate in x at each value of y */
	index = firstw;
	for (j = 1; j <= 6; ++j) {
	    ztemp[j - 1] = sx * (coeff[index + 1] + sx2m1 * (cd21[j - 1] + 
		    sx2m4 * cd41[j - 1])) + tx * (coeff[index] + tx2m1 * (
		    cd20[j - 1] + tx2m4 * cd40[j - 1]));
	    index += *lencof;
/* L300: */
	}
/* 	      central differences in y */
	cd20y = (ztemp[3] - ztemp[2] * (float)2. + ztemp[1]) * (float)
		.16666666666666666;
	cd21y = (ztemp[4] - ztemp[3] * (float)2. + ztemp[2]) * (float)
		.16666666666666666;
	cd40y = (ztemp[0] - ztemp[1] * (float)4. + ztemp[2] * (float)6. - 
		ztemp[3] * (float)4. + ztemp[4]) * (float)
		.0083333333333333332;
	cd41y = (ztemp[1] - ztemp[2] * (float)4. + ztemp[3] * (float)6. - 
		ztemp[4] * (float)4. + ztemp[5]) * (float)
		.0083333333333333332;
/* 	      interpolate in y */
	zfit[i__] = sy * (ztemp[3] + (sy2 - (float)1.) * (cd21y + (sy2 - (
		float)4.) * cd41y)) + ty * (ztemp[2] + (ty2 - (float)1.) * (
		cd20y + (ty2 - (float)4.) * cd40y));
	nxold = nx;
	nyold = ny;
/* L110: */
    }
    return 0;
} /* iibip5_ */

/* II_BISPLINE3 -- Procedure to evaluate a bicubic spline. */
/* The real array coeff contains the B-spline coefficients. */
/* The procedure assumes that 1 <= x <= nxpix and 1 <= y <= nypix */
/* and that coeff[1+first_point] = B-spline[2]. */

/* Subroutine */ int iibis3_(coeff, firstt, lencof, x, y, zfit, npts)
real *coeff;
integer *firstt, *lencof;
real *x, *y, *zfit;
integer *npts;
{
    /* System generated locals */
    integer i__1;
    real r__1;

    /* Local variables */
    static integer i__, j;
    static real accum;
    static integer index;
    static real bx[4], by[4];
    static integer nx, ny;
    static real sx, tx, sy, ty;
    static integer firstw;
    static real sum;

/* real   coeff[ARB]     # 1D array of coefficients */
/* int    firstt         # offset of first data point */
/* int    lencof         # row length of coeff */
/* real   x[npts]        # array of x values */
/* real   y[npts]        # array of y values */
/* real   zfit[npts]	   # array of interpolated values */
/* int	   npts		      # number of data points */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;

    /* Function Body */
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nx = (integer) x[i__];
	sx = x[i__] - nx;
	tx = (float)1. - sx;
	ny = (integer) y[i__];
	sy = y[i__] - ny;
	ty = (float)1. - sy;
/*        calculate the x B-splines */
/* Computing 3rd power */
	r__1 = tx;
	bx[0] = r__1 * (r__1 * r__1);
	bx[1] = tx * (tx * ((float)3. - tx * (float)3.) + (float)3.) + (float)
		1.;
	bx[2] = sx * (sx * ((float)3. - sx * (float)3.) + (float)3.) + (float)
		1.;
/* Computing 3rd power */
	r__1 = sx;
	bx[3] = r__1 * (r__1 * r__1);
/*        calculate the y B-splines */
/* Computing 3rd power */
	r__1 = ty;
	by[0] = r__1 * (r__1 * r__1);
	by[1] = ty * (ty * ((float)3. - ty * (float)3.) + (float)3.) + (float)
		1.;
	by[2] = sy * (sy * ((float)3. - sy * (float)3.) + (float)3.) + (float)
		1.;
/* Computing 3rd power */
	r__1 = sy;
	by[3] = r__1 * (r__1 * r__1);
/* 	      calculate the pointer to data[nx,ny-1] */
	firstw = *firstt + (ny - 2) * *lencof + nx;
	accum = (float)0.;
	index = firstw;
	for (j = 1; j <= 4; ++j) {
	    sum = coeff[index - 1] * bx[0] + coeff[index] * bx[1] + coeff[
		    index + 1] * bx[2] + coeff[index + 2] * bx[3];
	    accum += sum * by[j - 1];
	    index += *lencof;
	}
	zfit[i__] = accum;
    }
    return 0;
} /* iibis3_ */

/* II_NISINC -- Procedure to evaluate the 2D sinc function.  The real array */
/* coeff contains the data. The procedure assumes that 1 <= x <= nxpix and */
/* 1 <= y <= nypix and that coeff[1+first_point] = datain[1,1]. The since */
/* truncation length is nsinc. The taper is a cosbell function which is */
/* valid for 0 <= x <= PI / 2 (Abramowitz and Stegun, 1972, Dover Publications, */
/* p 76). If the point to be interpolated is less than mindx and mindy from */
/* a data point no interpolation is done and the data point is returned. This */
/* routine does not use precomputed arrays. */

/* This version, renamed II_NISINC allows for a scaling of the */
/* interpolations array. */

/* Interface modified to accept pre-initialized arrays for taper, ac, ar. */
/*   Original version dynamically allocated these arrays in the code. */
/*  WJH, 15 June 2004 */

/* Subroutine */ int iinisc_(coeff, firstt, lencof, lenary, x, y, zfit, npts, 
	nconv, taper, ac, ar, mindx, mindy, scale)
real *coeff;
integer *firstt, *lencof, *lenary;
real *x, *y, *zfit;
integer *npts, *nconv;
real *taper, *ac, *ar, *mindx, *mindy, *scale;
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    real r__1;

    /* Builtin functions */
    integer i_nint();

    /* Local variables */
    static integer offj, offk, minj, mink, maxj, maxk;
    static real sumx, sumy;
    static integer i__, j, k, index, nsinc;
    static real a2, a4, ax, ay, dx, dy;
    static integer nx, ny;
    static real px, py;
    static integer lastpt;
    static real sconst, dx2, dxn, dyn, sdx, sum;

/* real   coeff[ARB]     # 1D array of coefficients */
/* int    first_point    # offset to first data point */
/* int    len_coeff      # row length of coeff */
/* int    len_array      # column length of coeff */
/* real   x[npts]        # array of x values */
/* real   y[npts]        # array of y values */
/* real   zfit[npts]     # array of interpolated values */
/* real   scale          # stretch factor */
/* int    npts           # the number of input points. */
/* int    nconv          # number of points in interpolation arrays */
/*          where nconv = 2 * nsinc + 1 and nsinc = sinc truncation length */
/* real   taper[NCONV]   # taper array */
/* real   ac[NCONV]      # ac array */
/* real   ar[NCONV]      # ar array */
/* real   mindx          # interpolation minimum in x */
/* real   mindy		      # interpolation minimum in y */

/*     Compute original sinc truncation length */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;
    --ar;
    --ac;
    --taper;

    /* Function Body */
    nsinc = (*nconv - 1) / 2;
/*     Compute the constants for the cosine bell taper. */
/* Computing 2nd power */
    r__1 = (float)1.5707963267948966192 / nsinc;
    sconst = r__1 * r__1;
    a2 = (float)-.4967;
    a4 = (float).03705;
/* 	   Precompute the taper array. Incorporate the sign change portion */
/* 	   of the sinc interpolator into the taper array. */
    if (nsinc % 2 == 0) {
	sdx = (float)1.;
    } else {
	sdx = (float)-1.;
	i__1 = nsinc;
	for (j = -nsinc; j <= i__1; ++j) {
	    dx2 = sconst * j * j;
/* Computing 2nd power */
	    r__1 = a2 * dx2 + (float)1. + a4 * dx2 * dx2;
	    taper[j + nsinc] = sdx * (r__1 * r__1);
	    sdx = -sdx;
	}
    }
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*     	   define the fractional pixel interpolation. */
	nx = i_nint(&x[i__]);
	ny = i_nint(&y[i__]);
	if (nx < 1 || nx > *lencof || ny < 1 || ny > *lenary) {
	    zfit[i__] = (float)0.;
/*              go onto next point */
	    goto L130;
	}
/* ### Scale the offset */
	dx = (x[i__] - nx) * *scale;
	dy = (y[i__] - ny) * *scale;
/* 	         define pointer to data[nx,ny] */
	if (dabs(dx) < *mindx && dabs(dy) < *mindy) {
	    index = *firstt + (ny - 1) * *lencof + nx;
	    zfit[i__] = coeff[index];
	    goto L130;
	}
/* 	         Compute the x and y sinc arrays using a cosbell taper. */
	dxn = nsinc + 1 + dx;
	dyn = nsinc + 1 + dy;
	sumx = (float)0.;
	sumy = (float)0.;
	i__2 = *nconv;
	for (j = 1; j <= i__2; ++j) {
	    ax = dxn - j;
	    ay = dyn - j;
	    if (ax == (float)0.) {
		px = (float)1.;
	    } else if (dx == (float)0.) {
		px = (float)0.;
	    } else {
		px = taper[j - 1] / ax;
	    }
	    if (ay == (float)0.) {
		py = (float)1.;
	    } else if (dy == (float)0.) {
		py = (float)0.;
	    } else {
		py = taper[j - 1] / ay;
	    }
	    ac[j - 1] = px;
	    ar[j - 1] = py;
	    sumx += px;
	    sumy += py;
	}
/* 	         Compute the limits of the convolution. */
/* Computing MAX */
	i__2 = 1, i__3 = ny - nsinc;
	minj = max(i__2,i__3);
/* Computing MIN */
	i__2 = *lenary, i__3 = ny + nsinc;
	maxj = min(i__2,i__3);
/*           Get the index into ar */
	offj = nsinc - ny;
/* Computing MAX */
	i__2 = 1, i__3 = nx - nsinc;
	mink = max(i__2,i__3);
/* Computing MIN */
	i__2 = *lencof, i__3 = nx + nsinc;
	maxk = min(i__2,i__3);
/*           Get the index into ac */
	offk = nsinc - nx;
/* 	         Do the convolution. */
	zfit[i__] = (float)0.;
	i__2 = minj - 1;
	for (j = ny - nsinc; j <= i__2; ++j) {
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = nx - nsinc; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[*firstt + 1];
/* L220: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[*firstt + k];
/* L230: */
	    }
	    i__3 = nx + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[*firstt + *lencof];
/* L240: */
	    }
	    zfit[i__] += ar[j + offj] * sum;
/* L210: */
	}
	i__2 = maxj;
	for (j = minj; j <= i__2; ++j) {
	    index = *firstt + (j - 1) * *lencof;
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = nx - nsinc; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[index + 1];
/* L260: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[index + k];
/* L270: */
	    }
	    i__3 = nx + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[index + *lencof];
/* L280: */
	    }
	    zfit[i__] += ar[j + offj] * sum;
/* L250: */
	}
	i__2 = ny + nsinc;
	for (j = maxj + 1; j <= i__2; ++j) {
	    lastpt = *firstt + (*lenary - 1) * *lencof;
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = nx - nsinc; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[lastpt + 1];
/* L300: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[lastpt + k];
/* L310: */
	    }
	    i__3 = nx + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ac[k + offk] * coeff[lastpt + *lencof];
/* L320: */
	    }
	    zfit[i__] += ar[j + offj] * sum;
/* L290: */
	}
	zfit[i__] = zfit[i__] / sumx / sumy;
L130:
	;
    }
/* L131: */
    return 0;
} /* iinisc_ */

/* II_NILSINC -- Procedure to evaluate the 2D sinc function.  The real array */
/* coeff contains the data. The procedure assumes that 1 <= x <= nxpix and */
/* 1 <= y <= nypix and that coeff[1+first_point] = datain[1,1]. The since */
/* truncation length is nsinc. The taper is a cosbell function which is */
/* valid for 0 <= x <= PI / 2 (Abramowitz and Stegun, 1972, Dover Publications, */
/* p 76). If the point to be interpolated is less than mindx and mindy from */
/* a data point no interpolation is done and the data point is returned. This */
/* routine does use precomputed arrays. */

/* Renamed II_NILSINC when scale was added, */
/*  Richard Hook, ST-ECF/STScI, December 2003 */
/* Subroutine */ int iinilc_(coeff, firstt, lencof, lenary, x, y, zfit, npts, 
	ltable, nconv, nxincr, nyincr, mindx, mindy, scale)
real *coeff;
integer *firstt, *lencof, *lenary;
real *x, *y, *zfit;
integer *npts;
real *ltable;
integer *nconv, *nxincr, *nyincr;
real *mindx, *mindy, *scale;
{
    /* System generated locals */
    integer ltable_dim1, ltable_dim2, ltable_dim3, ltable_offset, i__1, i__2, 
	    i__3;
    real r__1;

    /* Builtin functions */
    integer i_nint();

    /* Local variables */
    static integer offj, offk, minj, mink, maxj, maxk, lutx, luty, i__, j, k, 
	    index, nsinc, xc, yc;
    static real dx, dy;
    static integer lastpt;
    static real sum;

/* real   coeff[ARB]           # 1D array of coefficients */
/* int    first_point          # offset to first data point */
/* int    len_coeff            # row length of coeff */
/* int    len_array            # column length of coeff */
/* real   x[npts]              # array of x values */
/* real   y[npts]              # array of y values */
/* real   zfit[npts]           # array of interpolated values */
/* real   scale                # scale for offset */
/* int    npts                 # the number of input points. */
/* real   ltable[nconv,nconv,nxincr,nyincr]  # the pre-computed look-up table */
/* int    nconv                # the sinc truncation full width */
/* int	   nxincr				   # the interpolation resolution in x */
/* int	   nyincr				   # the interpolation resolution in y */
/* real	mindx				      # interpolation mininmum in x */
/* real	mindy				      # interpolation mininmum in y */
    /* Parameter adjustments */
    --coeff;
    --zfit;
    --y;
    --x;
    ltable_dim1 = *nconv;
    ltable_dim2 = *nconv;
    ltable_dim3 = *nxincr;
    ltable_offset = 1 + ltable_dim1 * (1 + ltable_dim2 * (1 + ltable_dim3 * 1)
	    );
    ltable -= ltable_offset;

    /* Function Body */
    nsinc = (*nconv - 1) / 2;
    i__1 = *npts;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* 	      Return zero outside of data. */
	xc = i_nint(&x[i__]);
	yc = i_nint(&y[i__]);
	if (xc < 1 || xc > *lencof || yc < 1 || yc > *lenary) {
	    zfit[i__] = (float)0.;
	    goto L110;
	}
	dx = (x[i__] - xc) * *scale;
	dy = (y[i__] - yc) * *scale;
	if (dabs(dx) < *mindx && dabs(dy) < *mindy) {
	    index = *firstt + (yc - 1) * *lencof + xc;
	    zfit[i__] = coeff[index];
	}
	if (*nxincr == 1) {
	    lutx = 1;
	} else {
	    r__1 = (-dx + (float).5) * (*nxincr - 1);
	    lutx = i_nint(&r__1) + 1;
	}
	if (*nyincr == 1) {
	    luty = 1;
	} else {
	    r__1 = (-dy + (float).5) * (*nyincr - 1);
	    luty = i_nint(&r__1) + 1;
	}
/* 	         Compute the convolution limits. */
/* Computing MAX */
	i__2 = 1, i__3 = yc - nsinc;
	minj = max(i__2,i__3);
/* Computing MIN */
	i__2 = *lenary, i__3 = yc + nsinc;
	maxj = min(i__2,i__3);
	offj = 1 - yc + nsinc;
/* Computing MAX */
	i__2 = 1, i__3 = xc - nsinc;
	mink = max(i__2,i__3);
/* Computing MIN */
	i__2 = *lencof, i__3 = xc + nsinc;
	maxk = min(i__2,i__3);
	offk = 1 - xc + nsinc;
/* 	         Initialize */
	zfit[i__] = (float)0.;
/* 	         Do the convolution. */
	i__2 = minj - 1;
	for (j = yc - nsinc; j <= i__2; ++j) {
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = xc - nsinc; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[*
			firstt + 1];
/* L170: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[*
			firstt + k];
/* L180: */
	    }
	    i__3 = xc + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[*
			firstt + *lencof];
/* L190: */
	    }
	    zfit[i__] += sum;
/* L160: */
	}
	i__2 = maxj;
	for (j = minj; j <= i__2; ++j) {
	    index = *firstt + (j - 1) * *lencof;
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = xc - nsinc; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			index + 1];
/* L210: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			index + k];
/* L220: */
	    }
	    i__3 = xc + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			index + *lencof];
/* L230: */
	    }
	    zfit[i__] += sum;
/* L200: */
	}
	i__2 = yc + nsinc;
	for (j = maxj + 1; j <= i__2; ++j) {
	    lastpt = *firstt + (*lenary - 1) * *lencof;
	    sum = (float)0.;
	    i__3 = mink - 1;
	    for (k = xc - nsinc; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			lastpt + 1];
/* L250: */
	    }
	    i__3 = maxk;
	    for (k = mink; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			lastpt + k];
/* L260: */
	    }
	    i__3 = xc + nsinc;
	    for (k = maxk + 1; k <= i__3; ++k) {
		sum += ltable[k + offk + (j + offj + (lutx + luty * 
			ltable_dim3) * ltable_dim2) * ltable_dim1] * coeff[
			lastpt + *lencof];
/* L270: */
	    }
	    zfit[i__] += sum;
/* L240: */
	}
L110:
	;
    }
/* L111: */
    return 0;
} /* iinilc_ */

