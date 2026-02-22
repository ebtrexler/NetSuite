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

#ifndef Fit_LevenbergMaquardtH
#define Fit_LevenbergMaquardtH

#include <string>
#include <vector>


const int STOP_FIT = -1000;
const int ERROR_BADMEMALLOC = -1001;

class TModelBase
{
public:
	__fastcall TModelBase();
	virtual ~TModelBase();
protected:
public:
	virtual int __fastcall OutFit() = 0;
	int NumDataPoints;
	double Tol;
	std::string Description; // description of model
	std::vector<double> SeedParams; //array of seed parameters (float[numparams])
	std::vector<double> FitParams; //array of fit parameters (float[numparams])
	std::vector<std::string> ParamDesc; // array of strings describing parameters (char*[numparams])
	std::vector<int> DoFit; //array controlling which params to fit (int[numparams]) (y/n, 1/0)
	int NumParams; //number of parameters
	int NumCalls, EveryOther; //determines how often to call OutFit()
	virtual double __fastcall GetChiSq() = 0; // calculates/returns ChiSq   --measure of goodness of fit
	//this function will be called by ModelFittingModule to generate data
	virtual int __fastcall fitGen(int *numdatapoints, int *numparms,
						double *parms, double *fvec, int *iflag) = 0;
};

//class ModelContainer
//{
//public:
//	int numModels;
//	int modelSelected;
////	ModelClass **Model;
//	ModelContainer(){numModels = 0; modelSelected = 0;}
//	~ModelContainer(){};
//};


class ModelFittingModule
{
private:
	int numdatapoints;
	double tolerance;
	int numparms;
	double *parms, *fvec; // fvec contains differences of fit and data
	int *iwa;
	double *wa;
	int lwa;
	int CalledBefore;
	void __fastcall CleanUp();
	TModelBase *Model;
	int c__1, c__2;
	//the first paramter (*func) has been omitted:
	//from minpack's lmdif1_(...)
	int __fastcall LevMaqDiffWrapper(int *m, int *n,
						  double *x, double *fvec,
						  double *tol, int *info,
						  int *iwa, double *wa,
						  int *lwa);
	//from minpack's lmdif_(...)
	int __fastcall LevMaqDiff(int *m, int *n,
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
					double *wa4);
	//from minpack's fdjac2_(...)
	int __fastcall fdjac2_(int *m, int *n,
				double *x, double *fvec,
				double *fjac, int *ldfjac,
				int *iflag, double *epsfcn,
				double *wa);
	double __fastcall dpmpar_(int *i);
	double __fastcall enorm_(int *n,double *x);
	int __fastcall lmpar_(int *n,double *r,
				int *ldr,int *ipvt,
				double *diag,double *qtb,
				double *delta,double *par,
				double *x,double *sdiag,
				double *wa1,double *wa2);
	int __fastcall qrsolv_(int *n,  double *r,
				int *ldr, int *ipvt,
				double *diag, double *qtb,
				double *x, double *sdiag,
				double *wa);
	int __fastcall qrfac_(int *m,int *n,
				double *a, int *lda,
				int *pivot,int *ipvt,
				int *lipvt, double *rdiag,
				double *acnorm,double *wa);
public:
	int error;
	int info;
	bool Terminated;
	int __fastcall Fit(TModelBase & model);
	__fastcall ModelFittingModule();
	virtual ~ModelFittingModule();

};

//---------------------------------------------------------------------------
#endif
