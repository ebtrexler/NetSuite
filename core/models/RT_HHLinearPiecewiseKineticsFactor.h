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

#ifndef RT_HHLinearPiecewiseKineticsFactorH
#define RT_HHLinearPiecewiseKineticsFactorH
//---------------------------------------------------------------------------

//	 Applications that use this framework must keep with the
//  convention of uS, mV, nA, and ms as units for computation.

/// Kinetic factor used by THHLinearPiecewiseCurrent
/*!
<pre>
	 This class implements a Hodgkin-Huxley based current with the modification
	 that tau and inf are linear and piecewise, where

	xinf(V)    =    a*V+b    if Vlo <= V <= Vhi
				  =    a*Vlo+b  if V < Vlo
				  =    a*Vhi+b  if  V > Vhi

	xtau(V)    =    a*V+b    if Vlo <= V <= Vhi
				  =    a*Vlo+b  if V < Vlo
				  =    a*Vhi+b  if  V > Vhi

	a is the slope and b is the intercept of the line

</pre>

@author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2014
*/


#include "RT_HHKineticsFactor.h"


class THHLinearPiecewiseKineticsFactor : public THHKineticsFactor
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<TRTBase>(*this);
		ar & BOOST_SERIALIZATION_NVP(F_V_lo);
		ar & BOOST_SERIALIZATION_NVP(F_V_hi);
		ar & BOOST_SERIALIZATION_NVP(F_slope);
		ar & BOOST_SERIALIZATION_NVP(F_intcpt);
//		ar & BOOST_SERIALIZATION_NVP(F_inf_lo);
//		ar & BOOST_SERIALIZATION_NVP(F_inf_hi);
		ar & BOOST_SERIALIZATION_NVP(F_t_lo);
		ar & BOOST_SERIALIZATION_NVP(F_t_hi);
	}
#endif

private:
	// Linear interpolation parameters storage Fields
	double 								F_V_lo;		// lower bounds voltage
	double 								F_V_hi;		// upper bounds voltage
	double 								F_slope;		// slope of interpolation line
	double								F_intcpt;	// intercept of interpolation line
//	double								F_inf_lo;	// value of inf below Vlo
//	double								F_inf_hi;	// value of inf above Vhi
	double 								F_t_lo;		// value of tau below Vlo
	double 								F_t_hi;		// value of tau above Vhi

	// calculation variables
	double								F_tau;
	double								F_inf;

	// updated parameters storage Fields
	double F_y_n;

protected:

	/// Writes data members to a stream
	virtual void WriteToStream(std::ostream &stream) const
	{
		stream.write((char *)&F_V_lo, sizeof(double));
		stream.write((char *)&F_V_hi, sizeof(double));
		stream.write((char *)&F_slope, sizeof(double));
		stream.write((char *)&F_intcpt, sizeof(double));
//		stream.write((char *)&F_inf_lo, sizeof(double));
//		stream.write((char *)&F_inf_hi, sizeof(double));
		stream.write((char *)&F_t_lo, sizeof(double));
		stream.write((char *)&F_t_hi, sizeof(double));
	}
	/// Reads data members from a stream
	virtual void ReadFromStream(std::istream &stream)
	{
		stream.read((char *)&F_V_lo, sizeof(double));
		stream.read((char *)&F_V_hi, sizeof(double));
		stream.read((char *)&F_slope, sizeof(double));
		stream.read((char *)&F_intcpt, sizeof(double));
//		stream.read((char *)&F_inf_lo, sizeof(double));
//		stream.read((char *)&F_inf_hi, sizeof(double));
		stream.read((char *)&F_t_lo, sizeof(double));
		stream.read((char *)&F_t_hi, sizeof(double));
	}

public:
	// linear interpolation parameters
	/// lower bounds voltage
	// __property double 				V_lo  = {read = F_V_lo, write = F_V_lo};
	/// upper bounds voltage
	// __property double 				V_hi  = {read = F_V_hi, write = F_V_hi};
	/// slope of interpolation line
	// __property double 				slope = {read = F_slope, write = F_slope};
	/// intercept of interpolation line
	// __property double 				intcpt= {read = F_intcpt, write = F_intcpt};
//	/// Inf = inf_low if V < V_lo
//	// __property double             inf_lo= {read = F_inf_lo, write = F_inf_lo};
//	/// Inf = inf_low if V > V_hi
//	// __property double             inf_hi= {read = F_inf_hi, write = F_inf_hi};
	/// Tau = t_lo if V < V_lo
	// __property double 				t_lo  = {read = F_t_lo, write = F_t_lo};
	/// Tau = t_hi if V > V_hi
	// __property double 				t_hi  = {read = F_t_hi, write = F_t_hi};

	/// Value of the updated parameter at the end of a step
	// __property double             y_n   = {read = F_y_n, write = F_y_n};


   // implement pure virtual
	#define THHLinearPiecewiseKineticsFactor_KEY L"HH Linear Piecewise Kinetics Factor"
   const std::wstring & ClassKey() const
   {
		static std::wstring Key(THHLinearPiecewiseKineticsFactor_KEY);
      return Key;
   }

	/// Calculates new value of tau from given voltage, V
	virtual inline double tau(double V)
	{

		if(V > F_V_hi) return F_t_hi;
		else if (V < F_V_lo) return F_t_lo;

		// do this ?
//		else return F_slope * V + F_intcpt;

		// or do this?
		else {
			double vRange = (F_V_hi - F_V_lo);
			if (vRange == 0.0) {
				return 0.0;
			}
			return ((V - F_V_lo)/vRange)*(F_t_hi - F_t_lo) + F_t_lo;
		}

	}

	/// Calculates new value of inf from given voltage, V
	virtual inline double inf(double V)
	{
		double retval = 0.0;
		if(V > F_V_hi) retval =  F_slope * F_V_hi + F_intcpt; //F_inf_hi;
		else if (V < F_V_lo) retval = F_slope * F_V_lo + F_intcpt; //F_inf_lo;
		else{
			retval = F_slope * V + F_intcpt;
		}

/*                                   // Farzan said let it go negative 7-9-2014
   		if (retval < 0.0) {
   			retval = 0.0;
   		}
   		if (retval > 1.0) {
				retval = 1.0;
			}
*/
		return retval;
	}

	/// default constructor
	THHLinearPiecewiseKineticsFactor(): //TRTBase(L"HHLinearPiecewiseKineticsFactor"),
												F_V_lo(0), F_V_hi(0), F_slope(0), F_intcpt(0),
												/*	F_inf_lo(0), F_inf_hi(0), */
												F_t_lo(0), F_t_hi(0),
												F_tau(0), F_inf(0),F_y_n(0)
	{
#ifndef NO_VCL
		HelpText =
			L"V_lo is the lower bounds voltage, \n"
			L"V_hi is the upper bounds voltage \n"
			L"slope is the slope of the interpolating line \n"
			L"intercept is the y-intercept \n"
			L"tau_lo is the time constant when V < V_lo \n"
			L"tau_hi is the time constant when V > V_hi \n"
			L"p,q or r is the exponent for the entire calculation \n";
#endif
	}

	/// copy constructor
	THHLinearPiecewiseKineticsFactor(const THHLinearPiecewiseKineticsFactor &source):
//												TRTBase(source.Name()),
												F_V_lo(source.F_V_lo),
												F_V_hi(source.F_V_hi),
												F_slope(source.F_slope),
												F_intcpt(source.F_intcpt),
//												F_inf_lo(source.F_inf_lo),
//												F_inf_hi(source.F_inf_hi),
												F_t_lo(source.F_t_lo),
												F_t_hi(source.F_t_hi),
												F_tau(source.F_tau),
												F_inf(source.F_inf),
												F_y_n(source.F_y_n){};

	/// overloaded assignment operator
	THHLinearPiecewiseKineticsFactor& operator = (const THHLinearPiecewiseKineticsFactor & source)
   {
		if (this != &source) {  // make sure not same object
			this->THHKineticsFactor::operator=(source); //copy base class members
			// now copy THHLinearPiecewiseKineticsFactor fields
			F_V_lo = (source.F_V_lo);
			F_V_hi = (source.F_V_hi);
			F_slope = (source.F_slope);
			F_intcpt = (source.F_intcpt);
//			F_inf_lo = (source.F_inf_lo);
//			F_inf_hi = (source.F_inf_hi);
			F_t_lo = (source.F_t_lo);
			F_t_hi = (source.F_t_hi);
			F_tau = (source.F_tau);
			F_inf = (source.F_inf);
			F_y_n = (source.F_y_n);
      }
      return *this;
	}

	virtual ~THHLinearPiecewiseKineticsFactor() {};

	/// Fill in param names for GUI
	virtual void PopulateParams(void * guiElement);

	///
	virtual bool KineticFactorsValidate(
										THHKineticsFactor &f, wchar_t *factorname,
										void *ed, double &the_exp,
										wchar_t *exptext);

};
//---------------------------------------------------------------------------
#endif
