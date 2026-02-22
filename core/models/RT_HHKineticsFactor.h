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

Please direct correspondence to ebtrexler _at_ gothamsci _dot_ com
///////////////////////////////////////////////////////////////////////////// */


//---------------------------------------------------------------------------

#ifndef RT_HHKineticsFactorH
#define RT_HHKineticsFactorH

#include "RT_Network.h"
#include <limits>
#include <cfloat>

#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif

//	 Applications that use this framework must keep with the
//  convention of uS, mV, nA, and ms as units for computation.

/// Kinetic factor used by THHCurrent
/*!
<pre>
 This class implements the kinetic factor used by F. Nadim (Tohidi and Nadim, 2009)
 where,               dy(t,V)/dt = __(y_inf(V) - y(t,V))__
                                           tau_y(V)

 and
         inf(V) =   _______1 - inf_min___________  + inf_min
                     / 1 + exp ( __(V_ - V0)__ ) \^w
                     \         (       (k)     ) /

         tau(V) = t_hi + ________(t_lo - t_hi)_________
                          / 1 + exp( __(V - V0)__) \^w
                          \        (   abs(k)    ) /

      k = "steepness" and V0 = midpoint

      see Tohidi and Nadim 2009 J Neurosci. and Willms et al 1999 J Comp Neurosci

      additional parameters (w and inf_min) added according to Cataldo et al 2006
</pre>

@author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class THHKineticsFactor : public TRTBase
{

private:
	// Boltzmann parameters storage Fields
	double 								F_V0;
	double 								F_k;
	double 								F_t_lo;
	double 								F_t_hi;
	double								F_tau;
	double								F_inf;
   double                        F_infMin;
	double                        F_w;

	// updated parameters storage Fields
	double F_y_n;

#ifndef NO_VCL
   // Override from TRTBase, but hide in private
   /// Throws exception if called, because no separate edit form exists for this type
   virtual void* const __fastcall
                                 GetEditForm()
   {
      throw Exception("No Edit Form");
      return NULL;
   }
   /// Throws exception if called, because no separate edit form exists for this type
   virtual void       PopulateEditForm()
   {
      throw Exception("No Edit Form");
   }
   /// Throws exception if called, because no separate edit form exists for this type
   virtual bool       ValidateEditForm()
   {
      throw Exception("No Edit Form");
      return false;
   }
#endif


protected:

   /// "derivs" function for rk4
   /*!
      y' = f(t,y) --> ODE to solve numerically
      dy/dt = - [y - yInf(V)] / yTau(V)
   */
	virtual inline double f(double y)
	{
      double t;
      if (F_tau == 0) {
         t = 1;
      } else {
         t = 1.0 / F_tau;
      }
		return (F_inf - y) * t; // dy/dt = - [y - yInf(V)] / yTau(V)
	}

   /// returns y_n+1 by 4th order Runge-Kutta
	virtual inline double rk4(double y_n, double step)
	{
		double h=step/2.0,                 /* the midpoint */
		t1, t2, t3,            /* temporary storage arrays */
		k1, k2, k3,k4;                  /* for Runge-Kutta */

		if (fabs(y_n) > MAXFLOAT){
      	return 1.0;
		}

		t1=y_n+0.5*(k1=step*f(y_n));
		t2=y_n+0.5*(k2=step*f(t1));
		t3=y_n+    (k3=step*f(t2));
		k4=            step*f(t3);

		return y_n+=(k1+2*k2+2*k3+k4)/6.0;
	}

   /// Writes data members to a stream
   virtual void WriteToStream(std::ostream &stream) const
   {
      stream.write((char *)&F_V0, sizeof(double));
      stream.write((char *)&F_k, sizeof(double));
      stream.write((char *)&F_t_lo, sizeof(double));
      stream.write((char *)&F_t_hi, sizeof(double));
      stream.write((char *)&F_infMin, sizeof(double));
      stream.write((char *)&F_w, sizeof(double));
   }
   /// Reads data members from a stream
   virtual void ReadFromStream(std::istream &stream)
   {
      stream.read((char *)&F_V0, sizeof(double));
      stream.read((char *)&F_k, sizeof(double));
      stream.read((char *)&F_t_lo, sizeof(double));
      stream.read((char *)&F_t_hi, sizeof(double));
      stream.read((char *)&F_infMin, sizeof(double));
      stream.read((char *)&F_w, sizeof(double));
   }

public:
	// Boltzmann parameters - use getters/setters instead of __property
   double V0() const { return F_V0; }
   void V0(double v) { F_V0 = v; }
   
   double k() const { return F_k; }
   void k(double v) { F_k = v; }
   
   double t_lo() const { return F_t_lo; }
   void t_lo(double v) { F_t_lo = v; }
   
   double t_hi() const { return F_t_hi; }
   void t_hi(double v) { F_t_hi = v; }
   
   double infMin() const { return F_infMin; }
   void infMin(double v) { F_infMin = v; }
   
   double w() const { return F_w; }
   void w(double v) { F_w = v; }

	double y_n() const { return F_y_n; }
	void y_n(double v) { F_y_n = v; }

#ifndef NO_VCL
	/// Supplied text for hint in GUI
	std::wstring HelpText;
#endif

   /// Override pure virtual
   virtual bool Initialize(bool Reset)
   {
      if (Reset) {
         return Restart( 0 );
      }
      return true;
   }

	/// Sets variables (tau, inf) to steady state values before new run
   virtual bool Restart( double V )
   {
      F_tau = tau(V);
      F_inf = inf(V);
      F_y_n = F_inf;
      return true;
   }

   // implement pure virtual
   #define THHKineticsFactor_KEY L"HH Kinetics Factor"
   const std::wstring & ClassKey() const
   {
      static std::wstring Key(THHKineticsFactor_KEY);
      return Key;
   }

   /// Called by THHCurrent to get new value of kinetic parameter
	/*! Given the new voltage sample (V) and the interval (step) elapsed since
	 the last sample, computes updated value
    \param V (in mV) is the new voltage
    \param step (in ms) is the time to integrate
   */
	double Update(double V, double step) //step is ms, V is mV.
	{
		/* DONE -oebt-farzan : Here is where we can loop to increase number of
										integrations per step.  Should add stability. */
		F_tau = tau(V);
		F_inf = inf(V);

      double max_step = GetNet()->GetMaxRK4Timestep();
      if (step > max_step){
         int numRK4calls = floor(step / max_step);
         double leftover = fmod (step , max_step);
         for (int i = 0; i < numRK4calls; i++) {
            F_y_n = rk4(F_y_n, max_step);
         }
         if (leftover > 0.0) {
            F_y_n = rk4(F_y_n, leftover);
         }
      }
      else {
    		F_y_n = rk4(F_y_n, step);
      }

		return F_y_n;
	}

   /// Calculates new value of tau from given voltage, V
   virtual inline double tau(double V)
	{
      // with error checks and constraints
      double arg, k = fabs(F_k);
      if ( F_k == 0 ) return F_t_lo;
      arg = (V - F_V0) / k;
      if(arg > 10) return F_t_hi;
      else if (arg < -10) return F_t_lo;
      else {
         if(F_w == 1.0) return ( F_t_hi + ( F_t_lo - F_t_hi ) / (1+exp(arg)) );
			else if (F_w > 0) return ( F_t_hi + ( F_t_lo - F_t_hi ) / pow(1+exp(arg), F_w) );
			else return 1.0;
      }
	}

   /// Calculates new value of inf from given voltage, V
	virtual inline double inf(double V)
	{
      // with error checks and constraints
      double arg;
      if(F_k == 0) return F_infMin;
		arg=(V-F_V0)/F_k;
      if(arg > 10) return F_infMin;
      else if (arg < -10) return 1.0;
		if (F_w != 1.0 && F_w > 0) arg = pow(1+exp(arg), F_w);
      else arg = 1 + exp(arg);
      return ( (1 - F_infMin)/arg + F_infMin);
	}

	/// default constructor
	THHKineticsFactor(): TRTBase(L"HHKineticsFactor"),
   											F_V0(0), F_k(0), F_t_lo(0), F_t_hi(0),
												F_infMin(0), F_w(1.0),F_y_n(0)
	{
#ifndef NO_VCL
		HelpText =
			L"V_1/2 is the half maximal voltage, \n"
			L"k is the steepness \n"
			L"tau_lo is the time constant to the left of V_1/2 \n"
			L"tau_hi is the time constant to the right of V_1/2 \n"
			L"SSmin is the minimum steady state value \n"
			L"den. exponent is the power of the denominator \n"
			L"p,q or r is the exponent for the entire calculation \n";
#endif
	}

   /// copy constructor
   THHKineticsFactor(const THHKineticsFactor &source):
                                    TRTBase(source.Name()),
                                    F_V0(source.F_V0),
                                    F_k(source.F_k),
                                    F_t_lo(source.F_t_lo),
                                    F_t_hi(source.F_t_hi),
                                    F_infMin(source.F_infMin),
												F_w(source.F_w),
												F_y_n(source.F_y_n)
												{};

   /// overloaded assignment operator
   THHKineticsFactor& operator = (const THHKineticsFactor & source)
   {
		if (this != &source) {  // make sure not same object
         this->TRTBase::operator=(source); //copy base class members
         // now copy THHKineticsFactor fields
         F_V0 = source.F_V0;
         F_k = source.F_k;
         F_t_lo = source.F_t_lo;
         F_t_hi = source.F_t_hi;
         F_infMin = source.F_infMin;
			F_w = source.F_w;
			F_y_n = source.F_y_n;
      }
      return *this;
   }

	virtual ~THHKineticsFactor() {};

#ifndef NO_VCL
	/// Fill in param names for GUI
	virtual void PopulateParams(void * guiElement);

	///
	virtual bool KineticFactorsValidate(
										THHKineticsFactor &f, wchar_t *factorname,
										void *ed, double &the_exp,
										wchar_t *exptext);
#endif

};
//---------------------------------------------------------------------------
#endif
