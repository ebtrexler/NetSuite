/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E. Brady Trexler, Ph.D.

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please direct correspondence to ebtrexler _at_ hudsonvalleysci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#pragma hdrstop

#include "Fit_LevenbergMaquardt.h"
#include <math.h>
#include <float.h>
#include <SysUtils.hpp>

#include "f2c.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


__fastcall TModelBase::TModelBase()
{
	NumCalls = 0;
	EveryOther=1;
	Tol = 0.001;
}

TModelBase::~TModelBase()
{
}

int __fastcall ModelFittingModule::Fit(TModelBase &model)
{
	int i, j;

	Model = &model;
	numdatapoints = Model->NumDataPoints;
	tolerance = Model->Tol;

	numparms = 0;
	for (i=0; i<(Model->NumParams); i++) numparms += Model->DoFit[i];
	if (numparms == 0) return STOP_FIT; //nothing to fit
    if (CalledBefore) CleanUp();
	if ((iwa = new integer[numparms]) == NULL)return ERROR_BADMEMALLOC;
	if ((parms = new doublereal[numparms]) == NULL)return ERROR_BADMEMALLOC;
	if ((fvec = new doublereal[numdatapoints]) == NULL)return ERROR_BADMEMALLOC;
	lwa = numdatapoints*numparms +5*numparms+ numdatapoints;
	if ((wa = new doublereal[lwa]) == NULL)return ERROR_BADMEMALLOC;

	for (i=0,j=0; i<(Model->NumParams); i++){
		if (Model->DoFit[i] == 1) parms[j++] = Model->FitParams[i];
	}
	CalledBefore = 1;
	try {
		LevMaqDiffWrapper(&numdatapoints,&numparms,parms,fvec,&tolerance,
							  &info,iwa,wa,&lwa);
	} catch (...) {
		return -1;
	}

	return info; //fit done, return "success"
}

__fastcall ModelFittingModule::ModelFittingModule()
{
	c__1 = 1;
	c__2 = 2;
	CalledBefore = 0;
	Terminated = false;
}

ModelFittingModule::~ModelFittingModule()
{
	if (CalledBefore) CleanUp();
}

void __fastcall ModelFittingModule::CleanUp()
{
	CalledBefore = 0;
	delete[] parms; delete[] fvec; delete[] iwa; delete[] wa;
}

int __fastcall ModelFittingModule::LevMaqDiffWrapper(int *m, int *n,
										  double *x, double *fvec,
										  double *tol, int *info,
										  int *iwa, double *wa,
										  int *lwa)
{
	/* Initialized data */

	static double factor = 100.;
	static double zero = 0.;

	static int mode, nfev;
	static double ftol, gtol, xtol;
//    extern /* Subroutine */ int lmdif_(); //ebt
	static double epsfcn;
	static int maxfev, nprint, mp5n;

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

//     slightly modified January 1995 by e. brady trexler

/*     ********** */
	/* Parameter adjustments */
	--wa;
	--iwa;
	--fvec;
	--x;

	/* Function Body */
	*info = 0;

/*     check the input parameters for errors. */

	if (*n <= 0 || *m < *n || *tol < zero || *lwa < *m * *n + *n * 5 + *m) {
	goto L10;
	}

/*     call lmdif. */

	maxfev = (*n + 1) * 200;
	ftol = *tol;
	xtol = *tol;
	gtol = zero;
	epsfcn = zero;
	mode = 1;
	nprint = 0;
	mp5n = *m + *n * 5;
	//call member function LevMaqDiff, with no (*func) as first parameter
	LevMaqDiff(m, n, &x[1], &fvec[1], &ftol, &xtol, &gtol, &maxfev, &epsfcn,
		&wa[1], &mode, &factor, &nprint, info, &nfev, &wa[mp5n + 1], m, &
		iwa[1], &wa[*n + 1], &wa[(*n << 1) + 1], &wa[*n * 3 + 1], &wa[(*n
		<< 2) + 1], &wa[*n * 5 + 1]);
	if (*info == 8) {
	*info = 4;
	}
L10:
	return 0;

/*     last card of subroutine lmdif1. */

} /* lmdif1_ */

int __fastcall ModelFittingModule::LevMaqDiff(int *m, int *n,
									double *x, double *fvec,
									double *ftol, double *xtol,
									double *gtol, int *maxfev,
									double *epsfcn, double *diag,
									int *mode, double *factor,
									int *nprint, int *info,
									int *nfev, double *fjac,
									int *ldfjac, int *ipvt,
									double *qtf, double *wa1,
									double *wa2, double *wa3,
									double *wa4)
///* Subroutine */ int (*fcn) (); //from model class now (ebt)
{
	/* Initialized data */

	static double one = 1.;
	static double p1 = .1;
	static double p5 = .5;
	static double p25 = .25;
	static double p75 = .75;
	static double p0001 = 1e-4;
	static double zero = 0.;

	/* System generated locals */
	int fjac_dim1, fjac_offset, i__1, i__2;
	double d__1, d__2, d__3;

	/* Builtin functions */
	//double sqrt();

	/* Local variables */
	static int iter;
	static double temp, temp1, temp2;
	static int i, j, l, iflag;
	static double delta;
//    extern /* Subroutine */ int qrfac_(), lmpar_();(now member functions--ebt)
	static double ratio;
//	extern double enorm_(); (now member function --ebt)
	static double fnorm, gnorm;
//    extern /* Subroutine */ int fdjac2_(); (now member function --ebt)
	static double pnorm, xnorm, fnorm1, actred, dirder, epsmch, prered;
//    extern double dpmpar_(); (now member function --ebt)
	static double par, sum;

/*     ********** */
/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

//     modified January 1995 by e. brady trexler

/*     ********** */
	/* Parameter adjustments */
	--wa4;
	--wa3;
	--wa2;
	--wa1;
	--qtf;
	--ipvt;
	fjac_dim1 = *ldfjac;
	fjac_offset = fjac_dim1 + 1;
	fjac -= fjac_offset;
	--diag;
	--fvec;
	--x;

	/* Function Body */

/*     epsmch is the machine precision. */

	epsmch = dpmpar_(&c__1);

	*info = 0;
	iflag = 0;
	*nfev = 0;

/*     check the input parameters for errors. */

	if (*n <= 0 || *m < *n || *ldfjac < *m || *ftol < zero || *xtol < zero ||
		*gtol < zero || *maxfev <= 0 || *factor <= zero) {
	goto L300;
	}
	if (*mode != 2) {
	goto L20;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	if (diag[j] <= zero) {
		goto L300;
	}
/* L10: */
	}
L20:
	if (Terminated){
		return 1001;
	}
/*     evaluate the function at the starting point */
/*     and calculate its norm. */

	iflag = 1;
	// (*func) no longer called --call from model class (ebt)
	Model->fitGen(m, n, &x[1], &fvec[1], &iflag);
	*nfev = 1;
	if (iflag < 0) {
	goto L300;
	}
	fnorm = enorm_(m, &fvec[1]);

/*     initialize levenberg-marquardt parameter and iteration counter. */

	par = zero;
	iter = 1;

/*     beginning of the outer loop. */

L30:

/*        calculate the jacobian matrix. */

	iflag = 2;
	fdjac2_(m, n, &x[1], &fvec[1], &fjac[fjac_offset], ldfjac, &iflag,
		epsfcn, &wa4[1]);
	*nfev += *n;
	if (iflag < 0) {
	goto L300;
	}

/*        if requested, call fcn to enable printing of iterates. */
   if (Terminated){
		return 1001;
	}

	if (*nprint <= 0) {
	goto L40;
	}
	iflag = 0;
	if ((iter - 1) % *nprint == 0) {
	// (*func) no longer called --call from model class (ebt)
	Model->fitGen(m, n, &x[1], &fvec[1], &iflag);
	}
	if (iflag < 0) {
	goto L300;
	}
L40:

/*        compute the qr factorization of the jacobian. */

	qrfac_(m, n, &fjac[fjac_offset], ldfjac, &c__1, &ipvt[1], n, &wa1[1], &
		wa2[1], &wa3[1]);

/*        on the first iteration and if mode is 1, scale according */
/*        to the norms of the columns of the initial jacobian. */

	if (iter != 1) {
	goto L80;
	}
	if (*mode == 2) {
	goto L60;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	diag[j] = wa2[j];
	if (wa2[j] == zero) {
		diag[j] = one;
	}
/* L50: */
	}
L60:

/*        on the first iteration, calculate the norm of the scaled x */
/*        and initialize the step bound delta. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa3[j] = diag[j] * x[j];
/* L70: */
	}
	xnorm = enorm_(n, &wa3[1]);
	delta = *factor * xnorm;
	if (delta == zero) {
	delta = *factor;
	}
L80:

/*        form (q transpose)*fvec and store the first n components in */
/*        qtf. */

	i__1 = *m;
	for (i = 1; i <= i__1; ++i) {
	wa4[i] = fvec[i];
/* L90: */
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	if (fjac[j + j * fjac_dim1] == zero) {
		goto L120;
	}
	sum = zero;
	i__2 = *m;
	for (i = j; i <= i__2; ++i) {
		sum += fjac[i + j * fjac_dim1] * wa4[i];
/* L100: */
	}
	temp = -sum / fjac[j + j * fjac_dim1];
	i__2 = *m;
	for (i = j; i <= i__2; ++i) {
		wa4[i] += fjac[i + j * fjac_dim1] * temp;
/* L110: */
	}
L120:
	fjac[j + j * fjac_dim1] = wa1[j];
	qtf[j] = wa4[j];
/* L130: */
	}

/*        compute the norm of the scaled gradient. */

	gnorm = zero;
	if (fnorm == zero) {
	goto L170;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	if (wa2[l] == zero) {
		goto L150;
	}
	sum = zero;
	i__2 = j;
	for (i = 1; i <= i__2; ++i) {
		sum += fjac[i + j * fjac_dim1] * (qtf[i] / fnorm);
/* L140: */
	}
/* Computing MAX */
	d__2 = gnorm, d__3 = (d__1 = sum / wa2[l], abs(d__1));
	gnorm = max(d__2,d__3);
L150:
/* L160: */
	;
	}
L170:

/*        test for convergence of the gradient norm. */

	if (gnorm <= *gtol) {
	*info = 4;
	}
	if (*info != 0) {
	goto L300;
	}

/*        rescale if necessary. */

	if (*mode == 2) {
	goto L190;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	d__1 = diag[j], d__2 = wa2[j];
	diag[j] = max(d__1,d__2);
/* L180: */
	}
L190:

/*        beginning of the inner loop. */

L200:

/*           determine the levenberg-marquardt parameter. */

	lmpar_(n, &fjac[fjac_offset], ldfjac, &ipvt[1], &diag[1], &qtf[1], &delta,
		 &par, &wa1[1], &wa2[1], &wa3[1], &wa4[1]);

/*           store the direction p and x + p. calculate the norm of p. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa1[j] = -wa1[j];
	wa2[j] = x[j] + wa1[j];
	wa3[j] = diag[j] * wa1[j];
/* L210: */
	}
	pnorm = enorm_(n, &wa3[1]);

/*           on the first iteration, adjust the initial step bound. */

	if (iter == 1) {
	delta = min(delta,pnorm);
	}

/*           evaluate the function at x + p and calculate its norm. */
   if (Terminated){
		return 1001;
	}
	iflag = 1;
	// (*func) no longer called --call from model class (ebt)
	Model->fitGen(m, n, &wa2[1], &wa4[1], &iflag);
	++(*nfev);
	if (iflag < 0) {
	goto L300;
	}
	fnorm1 = enorm_(m, &wa4[1]);

/*           compute the scaled actual reduction. */

	actred = -one;
	if (p1 * fnorm1 < fnorm) {
/* Computing 2nd power */
	d__1 = fnorm1 / fnorm;
	actred = one - d__1 * d__1;
	}

/*           compute the scaled predicted reduction and */
/*           the scaled directional derivative. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa3[j] = zero;
	l = ipvt[j];
	temp = wa1[l];
	i__2 = j;
	for (i = 1; i <= i__2; ++i) {
		wa3[i] += fjac[i + j * fjac_dim1] * temp;
/* L220: */
	}
/* L230: */
	}
	temp1 = enorm_(n, &wa3[1]) / fnorm;
	temp2 = sqrt(par) * pnorm / fnorm;
/* Computing 2nd power */
	d__1 = temp1;
/* Computing 2nd power */
	d__2 = temp2;
	prered = d__1 * d__1 + d__2 * d__2 / p5;
/* Computing 2nd power */
	d__1 = temp1;
/* Computing 2nd power */
	d__2 = temp2;
	dirder = -(d__1 * d__1 + d__2 * d__2);

/*           compute the ratio of the actual to the predicted */
/*           reduction. */

	ratio = zero;
	if (prered != zero) {
	ratio = actred / prered;
	}

/*           update the step bound. */

	if (ratio > p25) {
	goto L240;
	}
	if (actred >= zero) {
	temp = p5;
	}
	if (actred < zero) {
	temp = p5 * dirder / (dirder + p5 * actred);
	}
	if (p1 * fnorm1 >= fnorm || temp < p1) {
	temp = p1;
	}
/* Computing MIN */
	d__1 = delta, d__2 = pnorm / p1;
	delta = temp * min(d__1,d__2);
	par /= temp;
	goto L260;
L240:
	if (par != zero && ratio < p75) {
	goto L250;
	}
	delta = pnorm / p5;
	par = p5 * par;
L250:
L260:

/*           test for successful iteration. */

	if (ratio < p0001) {
	goto L290;
	}

/*           successful iteration. update x, fvec, and their norms. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	x[j] = wa2[j];
	wa2[j] = diag[j] * x[j];
/* L270: */
	}
	i__1 = *m;
	for (i = 1; i <= i__1; ++i) {
	fvec[i] = wa4[i];
/* L280: */
	}
	xnorm = enorm_(n, &wa2[1]);
	fnorm = fnorm1;
	++iter;
L290:

/*           tests for convergence. */

	if (abs(actred) <= *ftol && prered <= *ftol && p5 * ratio <= one) {
	*info = 1;
	}
	if (delta <= *xtol * xnorm) {
	*info = 2;
	}
	if (abs(actred) <= *ftol && prered <= *ftol && p5 * ratio <= one && *info
		== 2) {
	*info = 3;
	}
	if (*info != 0) {
	goto L300;
	}

/*           tests for termination and stringent tolerances. */

	if (*nfev >= *maxfev) {
	*info = 5;
	}
	if (abs(actred) <= epsmch && prered <= epsmch && p5 * ratio <= one) {
	*info = 6;
	}
	if (delta <= epsmch * xnorm) {
	*info = 7;
	}
	if (gnorm <= epsmch) {
	*info = 8;
	}
	if (*info != 0) {
	goto L300;
	}

/*           end of the inner loop. repeat if iteration unsuccessful. */

	if (ratio < p0001) {
	goto L200;
	}

/*        end of the outer loop. */

	goto L30;
L300:

/*     termination, either normal or user imposed. */
	if (Terminated){
		return 1001;
	}
	if (iflag < 0) {
	*info = iflag;
	}
	iflag = 0;
	if (*nprint > 0) {
	// (*func) no longer called --call from model class (ebt)
	Model->fitGen(m, n, &x[1], &fvec[1], &iflag);
	}
	return 0;

/*     last card of subroutine lmdif. */

} /* lmdif_ */

int __fastcall ModelFittingModule::fdjac2_(int *m, int *n,
								double *x, double *fvec,
								double *fjac, int *ldfjac,
								int *iflag, double *epsfcn,
								double *wa)
{
	/* Initialized data */

	static double zero = 0.;

	/* System generated locals */
	int fjac_dim1, fjac_offset, i__1, i__2;

	/* Builtin functions */
	//double sqrt();

	/* Local variables */
	static double temp, h;
	static int i, j;
	static double epsmch;
//	extern double dpmpar_(); (now member function --ebt)
	static double eps;

/*     ********** */
/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

//     modified January 1995 by e. brady trexler

/*     ********** */
	/* Parameter adjustments */
	--wa;
	fjac_dim1 = *ldfjac;
	fjac_offset = fjac_dim1 + 1;
	fjac -= fjac_offset;
	--fvec;
	--x;

	/* Function Body */

/*     epsmch is the machine precision. */

	epsmch = dpmpar_(&c__1);

	eps = sqrt((max(*epsfcn,epsmch)));
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	temp = x[j];
	h = eps * abs(temp);
	if (h == zero) {
		h = eps;
	}
	x[j] = temp + h;
	// (*func) no longer called --call from model class (ebt)
	Model->fitGen(m, n, &x[1], &wa[1], iflag);//&iflag);
	if (*iflag < 0) {
		goto L30;
	}
	x[j] = temp;
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
		fjac[i + j * fjac_dim1] = (wa[i] - fvec[i]) / h;
/* L10: */
	}
/* L20: */
	}
L30:
	return 0;

/*     last card of subroutine fdjac2. */

} /* fdjac2_ */


double __fastcall ModelFittingModule::dpmpar_(int *i)
{
	double dmach[3];

/*     Machine constants for IEEE -- right?*/
	dmach[0] = FLT_EPSILON;
	dmach[1] = FLT_MIN;
	dmach[2] = FLT_MAX;


    return dmach[*i - 1];

/*     Last card of function dpmpar. */

} /* dpmpar_ */

double __fastcall ModelFittingModule::enorm_(int *n,double *x)
{
	/* Initialized data */

	 static double one = 1.;
	 static double zero = 0.;
	 static double rdwarf = 3.834e-20;
	 static double rgiant = 1.304e19;

    /* System generated locals */
	int i__1;
	 double ret_val, d__1;

    /* Builtin functions */
    //double sqrt();

    /* Local variables */
	 static double xabs, x1max, x3max;
	 static int i;
	 static double s1, s2, s3, agiant, floatn;

/*     ********** */
/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
	/* Parameter adjustments */
	--x;

	/* Function Body */
	s1 = zero;
	s2 = zero;
	s3 = zero;
    x1max = zero;
    x3max = zero;
	 floatn = (double) (*n);
    agiant = rgiant / floatn;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	xabs = (d__1 = x[i], abs(d__1));
	if (xabs > rdwarf && xabs < agiant) {
	    goto L70;
	}
	if (xabs <= rdwarf) {
	    goto L30;
	}

/*              sum for large components. */

	if (xabs <= x1max) {
		goto L10;
	}
/* Computing 2nd power */
	d__1 = x1max / xabs;
	s1 = one + s1 * (d__1 * d__1);
	x1max = xabs;
	goto L20;
L10:
/* Computing 2nd power */
	d__1 = xabs / x1max;
	s1 += d__1 * d__1;
L20:
	goto L60;
L30:

/*              sum for small components. */

	if (xabs <= x3max) {
	    goto L40;
	}
/* Computing 2nd power */
	d__1 = x3max / xabs;
	s3 = one + s3 * (d__1 * d__1);
	x3max = xabs;
	goto L50;
L40:
	if (xabs != zero) {
/* Computing 2nd power */
	    d__1 = xabs / x3max;
	    s3 += d__1 * d__1;
	}
L50:
L60:
	goto L80;
L70:

/*           sum for intermediate components. */

/* Computing 2nd power */
	d__1 = xabs;
	s2 += d__1 * d__1;
L80:
/* L90: */
	;
    }

/*     calculation of norm. */

	if (s1 == zero) {
	goto L100;
    }
    ret_val = x1max * sqrt(s1 + s2 / x1max / x1max);
    goto L130;
L100:
    if (s2 == zero) {
	goto L110;
    }
    if (s2 >= x3max) {
	ret_val = sqrt(s2 * (one + x3max / s2 * (x3max * s3)));
	}
    if (s2 < x3max) {
	ret_val = sqrt(x3max * (s2 / x3max + x3max * s3));
    }
    goto L120;
L110:
    ret_val = x3max * sqrt(s3);
L120:
L130:
    return ret_val;

/*     last card of function enorm. */

} /* enorm_ */

int __fastcall ModelFittingModule::lmpar_(int *n,double *r,
								int *ldr,int *ipvt,
								double *diag,double *qtb,
								double *delta,double *par,
								double *x,double *sdiag,
								double *wa1,double *wa2)
{
	/* Initialized data */

	static double p1 = .1;
	static double p001 = .001;
	static double zero = 0.;

	/* System generated locals */
	int r_dim1, r_offset, i__1, i__2;
	double d__1, d__2;

	/* Builtin functions */
	//double sqrt();

	/* Local variables */
	static double parc, parl;
	static int iter;
	static double temp, paru;
	static int i, j, k, l;
	static double dwarf;
	static int nsing;
	static double gnorm, fp;
	static double dxnorm;
	static int jm1, jp1;
	static double sum;

/*     ********** */
/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
	/* Parameter adjustments */
	--wa2;
	--wa1;
	--sdiag;
	--x;
	--qtb;
	--diag;
	--ipvt;
	r_dim1 = *ldr;
	r_offset = r_dim1 + 1;
	r -= r_offset;

	/* Function Body */

/*     dwarf is the smallest positive magnitude. */

	dwarf = dpmpar_(&c__2);

/*     compute and store in x the gauss-newton direction. if the */
/*     jacobian is rank-deficient, obtain a least squares solution. */

	nsing = *n;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa1[j] = qtb[j];
	if (r[j + j * r_dim1] == zero && nsing == *n) {
		nsing = j - 1;
	}
	if (nsing < *n) {
		wa1[j] = zero;
	}
/* L10: */
	}
	if (nsing < 1) {
	goto L50;
	}
	i__1 = nsing;
	for (k = 1; k <= i__1; ++k) {
	j = nsing - k + 1;
	wa1[j] /= r[j + j * r_dim1];
	temp = wa1[j];
	jm1 = j - 1;
	if (jm1 < 1) {
		goto L30;
	}
	i__2 = jm1;
	for (i = 1; i <= i__2; ++i) {
		wa1[i] -= r[i + j * r_dim1] * temp;
/* L20: */
	}
L30:
/* L40: */
	;
	}
L50:
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	x[l] = wa1[j];
/* L60: */
	}

/*     initialize the iteration counter. */
/*     evaluate the function at the origin, and test */
/*     for acceptance of the gauss-newton direction. */

	iter = 0;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
/* L70: */
	}
	dxnorm = enorm_(n, &wa2[1]);
	fp = dxnorm - *delta;
	if (fp <= p1 * *delta) {
	goto L220;
	}

/*     if the jacobian is not rank deficient, the newton */
/*     step provides a lower bound, parl, for the zero of */
/*     the function. otherwise set this bound to zero. */

	parl = zero;
	if (nsing < *n) {
	goto L120;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
/* L80: */
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	sum = zero;
	jm1 = j - 1;
	if (jm1 < 1) {
		goto L100;
	}
	i__2 = jm1;
	for (i = 1; i <= i__2; ++i) {
		sum += r[i + j * r_dim1] * wa1[i];
/* L90: */
	}
L100:
	wa1[j] = (wa1[j] - sum) / r[j + j * r_dim1];
/* L110: */
	}
	temp = enorm_(n, &wa1[1]);
	parl = fp / *delta / temp / temp;
L120:

/*     calculate an upper bound, paru, for the zero of the function. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	sum = zero;
	i__2 = j;
	for (i = 1; i <= i__2; ++i) {
		sum += r[i + j * r_dim1] * qtb[i];
/* L130: */
	}
	l = ipvt[j];
	wa1[j] = sum / diag[l];
/* L140: */
	}
	gnorm = enorm_(n, &wa1[1]);
	paru = gnorm / *delta;
	if (paru == zero) {
	paru = dwarf / min(*delta,p1);
	}

/*     if the input par lies outside of the interval (parl,paru), */
/*     set par to the closer endpoint. */

	*par = max(*par,parl);
	*par = min(*par,paru);
	if (*par == zero) {
	*par = gnorm / dxnorm;
	}

/*     beginning of an iteration. */

L150:
	++iter;

/*        evaluate the function at the current value of par. */

	if (*par == zero) {
/* Computing MAX */
	d__1 = dwarf, d__2 = p001 * paru;
	*par = max(d__1,d__2);
	}
	temp = sqrt(*par);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa1[j] = temp * diag[j];
/* L160: */
	}
	qrsolv_(n, &r[r_offset], ldr, &ipvt[1], &wa1[1], &qtb[1], &x[1], &sdiag[1]
		, &wa2[1]);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
/* L170: */
	}
	dxnorm = enorm_(n, &wa2[1]);
	temp = fp;
	fp = dxnorm - *delta;

/*        if the function is small enough, accept the current value */
/*        of par. also test for the exceptional cases where parl */
/*        is zero or the number of iterations has reached 10. */

	if (abs(fp) <= p1 * *delta || (parl == zero && fp <= temp && temp < zero)
			||	iter == 10) {
	goto L220;
	}

/*        compute the newton correction. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
/* L180: */
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	wa1[j] /= sdiag[j];
	temp = wa1[j];
	jp1 = j + 1;
	if (*n < jp1) {
		goto L200;
	}
	i__2 = *n;
	for (i = jp1; i <= i__2; ++i) {
		wa1[i] -= r[i + j * r_dim1] * temp;
/* L190: */
	}
L200:
/* L210: */
	;
	}
	temp = enorm_(n, &wa1[1]);
	parc = fp / *delta / temp / temp;

/*        depending on the sign of the function, update parl or paru. */

	if (fp > zero) {
	parl = max(parl,*par);
	}
	if (fp < zero) {
	paru = min(paru,*par);
	}

/*        compute an improved estimate for par. */

/* Computing MAX */
	d__1 = parl, d__2 = *par + parc;
	*par = max(d__1,d__2);

/*        end of an iteration. */

	goto L150;
L220:

/*     termination. */

	if (iter == 0) {
	*par = zero;
	}
	return 0;

/*     last card of subroutine lmpar. */

} /* lmpar_ */

int __fastcall ModelFittingModule::qrsolv_(int *n,  double *r,
								int *ldr, int *ipvt,
								double *diag, double *qtb,
								double *x, double *sdiag,
								double *wa)
{
	/* Initialized data */

	static double p5 = .5;
	static double p25 = .25;
	static double zero = 0.;

	/* System generated locals */
	int r_dim1, r_offset, i__1, i__2, i__3;
	double d__1, d__2;

	/* Builtin functions */
	//double sqrt();

	/* Local variables */
	static double temp;
	static int i, j, k, l;
	static double cotan;
	static int nsing;
	static double qtbpj;
	static int jp1, kp1;
	static double tan_, cos_, sin_, sum;

/*     ********** */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
	/* Parameter adjustments */
	--wa;
	--sdiag;
	--x;
	--qtb;
	--diag;
	--ipvt;
	r_dim1 = *ldr;
	r_offset = r_dim1 + 1;
	r -= r_offset;

	/* Function Body */

/*     copy r and (q transpose)*b to preserve input and initialize s. */
/*     in particular, save the diagonal elements of r in x. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i = j; i <= i__2; ++i) {
		r[i + j * r_dim1] = r[j + i * r_dim1];
/* L10: */
	}
	x[j] = r[j + j * r_dim1];
	wa[j] = qtb[j];
/* L20: */
	}

/*     eliminate the diagonal matrix d using a givens rotation. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {

/*        prepare the row of d to be eliminated, locating the */
/*        diagonal element using p from the qr factorization. */

	l = ipvt[j];
	if (diag[l] == zero) {
		goto L90;
	}
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {
		sdiag[k] = zero;
/* L30: */
	}
	sdiag[j] = diag[l];

/*        the transformations to eliminate the row of d */
/*        modify only a single element of (q transpose)*b */
/*        beyond the first n, which is initially zero. */

	qtbpj = zero;
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {

/*           determine a givens rotation which eliminates the */
/*           appropriate element in the current row of d. */

		if (sdiag[k] == zero) {
		goto L70;
		}
		if ((d__1 = r[k + k * r_dim1], abs(d__1)) >= (d__2 = sdiag[k],
			abs(d__2))) {
		goto L40;
		}
		cotan = r[k + k * r_dim1] / sdiag[k];
/* Computing 2nd power */
		d__1 = cotan;
		sin_ = p5 / sqrt(p25 + p25 * (d__1 * d__1));
		cos_ = sin_ * cotan;
		goto L50;
L40:
		tan_ = sdiag[k] / r[k + k * r_dim1];
/* Computing 2nd power */
		d__1 = tan_;
		cos_ = p5 / sqrt(p25 + p25 * (d__1 * d__1));
		sin_ = cos_ * tan_;
L50:

/*           compute the modified diagonal element of r and */
/*           the modified element of ((q transpose)*b,0). */

		r[k + k * r_dim1] = cos_ * r[k + k * r_dim1] + sin_ * sdiag[k];
		temp = cos_ * wa[k] + sin_ * qtbpj;
		qtbpj = -sin_ * wa[k] + cos_ * qtbpj;
		wa[k] = temp;

/*           accumulate the tranformation in the row of s. */

		kp1 = k + 1;
		if (*n < kp1) {
		goto L70;
		}
		i__3 = *n;
		for (i = kp1; i <= i__3; ++i) {
		temp = cos_ * r[i + k * r_dim1] + sin_ * sdiag[i];
		sdiag[i] = -sin_ * r[i + k * r_dim1] + cos_ * sdiag[i];
		r[i + k * r_dim1] = temp;
/* L60: */
		}
L70:
/* L80: */
		;
	}
L90:

/*        store the diagonal element of s and restore */
/*        the corresponding diagonal element of r. */

	sdiag[j] = r[j + j * r_dim1];
	r[j + j * r_dim1] = x[j];
/* L100: */
	}

/*     solve the triangular system for z. if the system is */
/*     singular, then obtain a least squares solution. */

	nsing = *n;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	if (sdiag[j] == zero && nsing == *n) {
		nsing = j - 1;
	}
	if (nsing < *n) {
		wa[j] = zero;
	}
/* L110: */
	}
	if (nsing < 1) {
	goto L150;
	}
	i__1 = nsing;
	for (k = 1; k <= i__1; ++k) {
	j = nsing - k + 1;
	sum = zero;
	jp1 = j + 1;
	if (nsing < jp1) {
		goto L130;
	}
	i__2 = nsing;
	for (i = jp1; i <= i__2; ++i) {
		sum += r[i + j * r_dim1] * wa[i];
/* L120: */
	}
L130:
	wa[j] = (wa[j] - sum) / sdiag[j];
/* L140: */
	}
L150:

/*     permute the components of z back to components of x. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	x[l] = wa[j];
/* L160: */
	}
	return 0;

/*     last card of subroutine qrsolv. */

} /* qrsolv_ */

int __fastcall ModelFittingModule::qrfac_(int *m,int *n,
								double *a, int *lda,
								int *pivot,int *ipvt,
								int *lipvt, double *rdiag,
								double *acnorm,double *wa)
{
	/* Initialized data */

	static double one = 1.;
	static double p05 = .05;
	static double zero = 0.;

	/* System generated locals */
	int a_dim1, a_offset, i__1, i__2, i__3;
	double d__1, d__2, d__3;

	/* Builtin functions */
	//double sqrt();

	/* Local variables */
	static int kmax;
	static double temp;
	static int i, j, k, minmn;
	static double epsmch;
	static double ajnorm;
	static int jp1;
	static double sum;

/*     ********** */
/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
	/* Parameter adjustments */
	--wa;
	--acnorm;
	--rdiag;
	--ipvt;
	a_dim1 = *lda;
	a_offset = a_dim1 + 1;
	a -= a_offset;

	/* Function Body */

/*     epsmch is the machine precision. */

	epsmch = dpmpar_(&c__1);

/*     compute the initial column norms and initialize several arrays. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	acnorm[j] = enorm_(m, &a[j * a_dim1 + 1]);
	rdiag[j] = acnorm[j];
	wa[j] = rdiag[j];
	if (*pivot) {
		ipvt[j] = j;
	}
/* L10: */
	}

/*     reduce a to r with householder transformations. */

	minmn = min(*m,*n);
	i__1 = minmn;
	for (j = 1; j <= i__1; ++j) {
	if (! (*pivot)) {
		goto L40;
	}

/*        bring the column of largest norm into the pivot position. */


	kmax = j;
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {
		if (rdiag[k] > rdiag[kmax]) {
		kmax = k;
		}
/* L20: */
	}
	if (kmax == j) {
		goto L40;
	}
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
		temp = a[i + j * a_dim1];
		a[i + j * a_dim1] = a[i + kmax * a_dim1];
		a[i + kmax * a_dim1] = temp;
/* L30: */
	}
	rdiag[kmax] = rdiag[j];
	wa[kmax] = wa[j];
	k = ipvt[j];
	ipvt[j] = ipvt[kmax];
	ipvt[kmax] = k;
L40:

/*        compute the householder transformation to reduce the */
/*        j-th column of a to a multiple of the j-th unit vector. */

	i__2 = *m - j + 1;
	ajnorm = enorm_(&i__2, &a[j + j * a_dim1]);
	if (ajnorm == zero) {
		goto L100;
	}
	if (a[j + j * a_dim1] < zero) {
		ajnorm = -ajnorm;
	}
	i__2 = *m;
	for (i = j; i <= i__2; ++i) {
		a[i + j * a_dim1] /= ajnorm;
/* L50: */
	}
	a[j + j * a_dim1] += one;

/*        apply the transformation to the remaining columns */
/*        and update the norms. */

	jp1 = j + 1;
	if (*n < jp1) {
		goto L100;
	}
	i__2 = *n;
	for (k = jp1; k <= i__2; ++k) {
		sum = zero;
		i__3 = *m;
		for (i = j; i <= i__3; ++i) {
		sum += a[i + j * a_dim1] * a[i + k * a_dim1];
/* L60: */
		}
		temp = sum / a[j + j * a_dim1];
		i__3 = *m;
		for (i = j; i <= i__3; ++i) {
		a[i + k * a_dim1] -= temp * a[i + j * a_dim1];
/* L70: */
		}
		if (! (*pivot) || rdiag[k] == zero) {
		goto L80;
		}
		temp = a[j + k * a_dim1] / rdiag[k];
/* Computing MAX */
/* Computing 2nd power */
		d__3 = temp;
		d__1 = zero, d__2 = one - d__3 * d__3;
		rdiag[k] *= sqrt((max(d__1,d__2)));
/* Computing 2nd power */
		d__1 = rdiag[k] / wa[k];
		if (p05 * (d__1 * d__1) > epsmch) {
		goto L80;
		}
		i__3 = *m - j;
		rdiag[k] = enorm_(&i__3, &a[jp1 + k * a_dim1]);
		wa[k] = rdiag[k];
L80:
/* L90: */
		;
	}
L100:
	rdiag[j] = -ajnorm;
/* L110: */
	}
	return 0;

/*     last card of subroutine qrfac. */

} /* qrfac_ */
