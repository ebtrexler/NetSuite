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

#ifndef RT_HHCurrentH
#define RT_HHCurrentH

#include "RT_HHKineticsFactor.h"

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   THHCurrent   ***************************
//
//
/// Implementation of Hodgkin-Huxley type current with Tohidi-Nadim shortcuts
/*!
<pre>
 Classes THHCurrent gives ionic current as a function of several kinetic
 parameters and the input voltages V_kin and V_drv.  For intrinsic currents
 V_drv = V_kin, and for synaptic currents, V_kin is the voltage of the
 presynaptic cell.

                      p   q   r
 computes I = Gmax * m * h * n * (V_drv - E)
</pre>

@author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class THHCurrent: public TCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCurrent>(*this);
      ar & BOOST_SERIALIZATION_NVP(F_p);
      ar & BOOST_SERIALIZATION_NVP(F_q);
      ar & BOOST_SERIALIZATION_NVP(F_r);
      ar & BOOST_SERIALIZATION_NVP(F_E);
      ar & BOOST_SERIALIZATION_NVP(F_Gmax);
      ar & BOOST_SERIALIZATION_NVP(F_Gnoise);
		ar & BOOST_SERIALIZATION_NVP(_m);
		ar & BOOST_SERIALIZATION_NVP(_h);
		ar & BOOST_SERIALIZATION_NVP(_n);
	}
#endif

private:
	double								F_p;
	double 								F_q;
   double                        F_r;
	double 								F_E;
	double 								F_Gmax;
	double                        F_Gnoise;

	/// activation kinetic factor
	THHKineticsFactor             _m;
	/// inactivation kinetic factor
	THHKineticsFactor             _h;
	/// third kinetic factor
	THHKineticsFactor             _n;

	bool                          FEnableLogging;

protected:

	long                          idum; // for random variables

	/// Writes data members to a stream
	virtual void WriteToStream(std::ostream &stream) const;

	/// Reads data members from a stream
	virtual void ReadFromStream(std::istream &stream);

public:
	// Use getters/setters instead of __property
	double p() const { return F_p; }
	void p(double v) { F_p = v; }
	
	double q() const { return F_q; }
	void q(double v) { F_q = v; }
	
	double r() const { return F_r; }
	void r(double v) { F_r = v; }
	
	double E() const { return F_E; }
	void E(double v) { F_E = v; }
	
	double Gmax() const { return F_Gmax; }
	void Gmax(double v) { F_Gmax = v; }
	
	double Gnoise() const { return F_Gnoise; }
	void Gnoise(double v) { F_Gnoise = v; }
	
	/// activation kinetic factor
	virtual THHKineticsFactor& get_m();
	/// inactivation kinetic factor
	virtual THHKineticsFactor& get_h();
	/// third kinetic factor
	virtual THHKineticsFactor& get_n();

	// implement pure virtual
	bool               Initialize(bool Reset);

	/// Initializes kinetic parameters m, h, and n
	bool               Restart( double V );

	// implement pure virtual
	#define THHCurrent_KEY L"HH Current"
	const std::wstring & ClassKey() const;

	// implement pure virtual
	const void 			GetParamLogHeader(
												std::vector <std::string> & params) const;

/// Calculation of HH type current based on two voltages
/*!
<pre>
*                                    p         q         r
*         computes I = Gmax * m(Vkin) * h(Vkin) * n(Vkin) * (Vdrv - E)
</pre>
*/
	virtual double DoUpdate(double step, double Vkin, double Vdrv,
												  std::vector <double> & params);

	/// Called by GUI to synchronize edit form with current values of object params
	virtual void       PopulateEditForm();

	// called by ValidateEditForm -- REFACTOR -- moved to HHKineticsFactor
//	bool KineticFactorsValidate(THHKineticsFactor &f, wchar_t *factorname,
//											void *ed, double &the_exp,
//											wchar_t *exptext);

	/// Called by GUI to check if changed values are satisfactory
	virtual bool       ValidateEditForm();
#ifndef NO_VCL
	/// Returns downcasted THHCurrentForm* that is used to set values for this object
	virtual void* const GetEditForm();
#endif
	/// default constructor
	THHCurrent();

	/// specialized constructor 2 param
	THHCurrent(TCurrentUser *owner, const std::wstring & name);

	/// specialized constructor 1 param
	THHCurrent(const std::wstring & name);

	/// copy constructor
	THHCurrent( const THHCurrent & source );

	/// overloaded assignment operator
	THHCurrent & operator = (const THHCurrent & source);

	/// overloaded method for duplicating currents without complete assignment
	virtual void CopyParamsFrom(const TCurrent * const source );
};

//---------------------------------------------------------------------------
#endif
