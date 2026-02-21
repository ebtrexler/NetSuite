/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E(). Brady Trexler, Ph.D.

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

#pragma hdrstop

#include "RT_HHLinearPiecewiseCurrent.h"
//---------------------------------------------------------------------------

#pragma hdrstop

#include "RT_Network.h"

#include "RT_HHCurrent.h"
#ifndef NO_VCL
#include "GUI_RT_Edit_HHCurrent.h"
#endif
#include "RT_Utilities.h"
#include "RT_HHLinearPiecewiseKineticsFactor.h"

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- THHLinearPiecewiseCurrent methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define THHLinearPiecewiseCurrent

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   THHLinearPiecewiseCurrent   ***************************
//
//
/// Implementation of Hodgkin-Huxley type current with Tohidi-Nadim shortcuts
/*!
<pre>
 Classes THHLinearPiecewiseCurrent gives ionic current as a function of several kinetic
 parameters and the input voltages V_kin and V_drv.  For intrinsic currents V_drv = V_kin,
 and for synaptic currents, V_kin is the voltage of the presynaptic cell.

 A new parameter, UseVdrv, is introduced that determines whether the current is
 dependent on the driving force, V_drv - E().

 if UseVdrv = 1
							 p   q   r
 computes I = Gmax() * m * h * n * (V_drv - E())

 if UseVdrv = 0
							 p   q   r
 computes I = Gmax() * m * h * n

</pre>

@author E(). Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2014
*/
class THHLinearPiecewiseCurrent: public THHCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<THHCurrent>(*this);
		ar & BOOST_SERIALIZATION_NVP(m_lin);
		ar & BOOST_SERIALIZATION_NVP(h_lin);
		ar & BOOST_SERIALIZATION_NVP(n_lin);
		if (version >= 1) {
			ar & BOOST_SERIALIZATION_NVP(F_UseVdrv);
		}
	}
#endif

private:
	/// activation kinetic factor
	THHLinearPiecewiseKineticsFactor             m_lin;
	/// inactivation kinetic factor
	THHLinearPiecewiseKineticsFactor             h_lin;
	/// third kinetic factor
	THHLinearPiecewiseKineticsFactor             n_lin;

	int														F_UseVdrv;

public:
	int									get_UseVdrv()
	{
		return F_UseVdrv;
	}
	void									set_UseVdrv(int set)
	{
		F_UseVdrv = set;
	}

	// activation kinetic factor
	THHKineticsFactor& get_m()
	{
		return m_lin;
	}
	// inactivation kinetic factor
	THHKineticsFactor& get_h()
	{
		return h_lin;
	}
	// third kinetic factor
	THHKineticsFactor& get_n()
	{
		return n_lin;
	}

	/// Returns downcasted THHCurrentForm* that is used to set values for this object
#ifndef NO_VCL
	void* const        GetEditForm()
	{
		static THHCurrentForm *form = new THHCurrentForm(NULL);
		form->UseVdrvComboBox->Visible = true; // only need this for piecewise
		form->HHCurrent = this;
		return form;
	}
#endif

   /// Calculation of HH type current based on two voltages
/*!
<pre>
*                                    p         q         r
*         computes I = Gmax() * m(Vkin) * h(Vkin) * n(Vkin) * (Vdrv - E())   or
*
*                                    p         q         r
*         computes I = Gmax() * m(Vkin) * h(Vkin) * n(Vkin)
*
*			depending on UseVdrv setting
</pre>
*/
	double 				DoUpdate(double step, double Vkin,
																		double Vdrv,
																  std::vector <double> & params)
	{
		// step is ms, V's are mV, G is in microsiemens, I is in nA
		//                      p   q   r
		// computes I = Gmax() * m * h * n * (V - E())
		double I = 0.0;
		double mp = 1;
		double hq = 1;
		double nr = 1;
		double M = 1;
		double H = 1;
		double N = 1;
		double G = 0.0;

		params.clear();

		if (p() > 0) {
			M = get_m().Update(Vkin, step);
//			if (M >= 0) {     // ebt 9/18/2013 -- to stop domain errors in pow
				mp = pow(M,p());  // ebt 7/9/2014 -- let it go negative
//			}
			params.push_back(M);
		}
		if (q() > 0) {
			H = get_h().Update(Vkin, step);
//			if (H >= 0) {     // ebt 9/18/2013 -- to stop domain errors in pow
				hq = pow(H,q());  // ebt 7/9/2014 -- let it go negative
//			}
			params.push_back(H);
		}
		if (r() > 0) {
			N = get_n().Update(Vkin, step);
//			if (N >= 0) {    // ebt 9/18/2013 -- to stop domain errors in pow
				nr = pow(N,r());  // ebt 7/9/2014 -- let it go negative
//			}
			params.push_back(N);
		}

		if (Gnoise() == 0.0){
			G = Gmax()*mp*hq*nr;
		}
		else {
			double noise = 0.01 * Gnoise() * Gmax() * gasdev(&idum);
			G = (Gmax() + noise)*mp*hq*nr;
		}

		if (F_UseVdrv) {
			I = G * (Vdrv - E());       // uS * mV = nA			
		}
		else {
			I = G;
		}

		params.push_back(G);

		return I;
	}

	/// Called by GUI to synchronize edit form with current values of object params
	void       			PopulateEditForm()
	{
#ifndef NO_VCL
		// call inherited method
		THHCurrent::PopulateEditForm();

		// now set GUI to reflect UseVdrv value
		THHCurrentForm *form = (THHCurrentForm * )GetEditForm();

		form->UseVdrvComboBox->Checked = F_UseVdrv;
#endif

	}

	/// Called by GUI to check if changed values are satisfactory
	bool       			ValidateEditForm()
	{
#ifndef NO_VCL
		// call inherited method
		bool ok = THHCurrent::ValidateEditForm();

		// now get UseVdrv value from checkbox
		THHCurrentForm *form = (THHCurrentForm * )GetEditForm();

		F_UseVdrv = form->UseVdrvComboBox->Checked;
		
		return ok;
#else
		return true;
#endif
	}




	/// Writes data members to a stream
	void WriteToStream(std::ostream &stream) const
	{
		THHCurrent::WriteToStream(stream); //Write base class members
		stream << m_lin;
		stream << h_lin;
		stream << n_lin;
		stream.write((char *)&F_UseVdrv, sizeof(int));
	}

	/// Reads data members from a stream
	void ReadFromStream(std::istream &stream)
	{
		THHCurrent::ReadFromStream(stream); //Read base class members
		stream >> m_lin;
		stream >> h_lin;
		stream >> n_lin;
		stream.read((char *)&F_UseVdrv, sizeof(int));
	}

	// implement pure virtual
	#define THHLinearPiecewiseCurrent_KEY L"HH Linear Piecewise Current"
	const std::wstring & ClassKey() const
	{
		static std::wstring Key(THHLinearPiecewiseCurrent_KEY);
		return Key;
	}

	/// default constructor
THHLinearPiecewiseCurrent():
			THHCurrent(NULL, L"")
	{
	}

	/// specialized constructor 2 param
THHLinearPiecewiseCurrent(TCurrentUser *owner, const std::wstring & name):
			THHCurrent(owner, name)
	{
	}

	/// specialized constructor 1 param
THHLinearPiecewiseCurrent(const std::wstring & name):
			THHCurrent(NULL, name)
	{
	}

	/// copy constructor
THHLinearPiecewiseCurrent( const THHLinearPiecewiseCurrent & source ) :
			THHCurrent(source.Owner(), source.Name()),
			m_lin(source.m_lin),
			h_lin(source.h_lin),
			n_lin(source.n_lin),
			F_UseVdrv(source.F_UseVdrv)
	{
	}

	/// overloaded assignment operator
	THHLinearPiecewiseCurrent & operator = (const THHLinearPiecewiseCurrent & source)
	{
		if (this != &source) {  // make sure not same object
			this->THHCurrent::operator=(source); //copy base class members
			// now copy THHLinearPiecewiseCurrent fields
			m_lin = (source.m_lin);
			h_lin = (source.h_lin);
			n_lin = (source.n_lin);
			F_UseVdrv = (source.F_UseVdrv);
		}
		return *this;
	}

	/// overloaded method for duplicating currents without complete assignment
void CopyParamsFrom(const TCurrent * const source )
	{
		if (this != source) {  // make sure not same object
//         std::wstring this_type(this->ClassType());
//         std::wstring source_type(source->ClassType());
//         if (this->ClassType() == source->ClassType()) { // make sure same type
//         if (this_type == source_type) {
				THHLinearPiecewiseCurrent *cast_source = (THHLinearPiecewiseCurrent *)source;

				m_lin = (cast_source->m_lin);
				h_lin = (cast_source->h_lin);
				n_lin = (cast_source->n_lin);
				F_UseVdrv = (cast_source->F_UseVdrv);
//         }
		}
	}

};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(THHLinearPiecewiseCurrent)
BOOST_CLASS_VERSION(THHLinearPiecewiseCurrent, 1) //added 07/09/2014 for UseVdrv
#endif


////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
	static struct regHHLinearPiecewiseCurrent {
		regHHLinearPiecewiseCurrent(){
			GetCurrentFactory().registerBuilder(
											THHLinearPiecewiseCurrent_KEY, //L"HH Linear Piecewise Current",
											TypeID<THHLinearPiecewiseCurrent>(),
											TypeID<TCurrentUser*const>(),
											TypeID<const std::wstring>());
		}
	} RegHHLinearPiecewiseCurrent;
}

#pragma end_region
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
