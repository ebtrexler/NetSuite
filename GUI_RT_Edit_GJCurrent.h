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

#ifndef GUI_RT_Edit_GJCurrentH
#define GUI_RT_Edit_GJCurrentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------

#include "RT_Network.h"

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGapJunctionCurrent   ***************************
//
//
/// Implements a current for an electrical synapse
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TGapJunctionCurrent : public TCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCurrent>(*this);
      ar & BOOST_SERIALIZATION_NVP(F_Gmax);
	}
#endif

private:
   double 								F_Gmax;
   double                        Ielect;

protected:
	/// Writes data members to a stream
	virtual void const __fastcall WriteToStream(ostream &stream) const;
	/// Reads data members from a stream
	virtual void const __fastcall ReadFromStream(istream &stream);

	const void __fastcall			GetParamLogHeader(
												std::vector<std::string>& params) const;

public:
	const double __fastcall			Gmax();
	void __fastcall               SetGmax(double set);

	// override TCurrent::DoUpdate
	virtual double __fastcall DoUpdate(double step, double Vpre, double Vpost,
														std::vector<double> & params);

	// override TRTBase::PopulateEditForm
	virtual void __fastcall PopulateEditForm();

	// override TRTBase::ValidateEditForm
	virtual bool __fastcall ValidateEditForm();

	// override TRTBase::GetEditForm
	virtual void* const __fastcall
											GetEditForm();

	// override TRTBase::Initialize
	virtual bool __fastcall Initialize(bool Reset);

	// override TRTBase::ClassKey
	virtual const std::wstring & __fastcall ClassKey() const;

   /// Default Constructor
   __fastcall TGapJunctionCurrent();

   /// Specialized Constructor 2 param
   __fastcall TGapJunctionCurrent(TCurrentUser *owner, const std::wstring & name);

   /// Specialized Constructor 1 param
   __fastcall TGapJunctionCurrent(const std::wstring & name);

   /// copy constructor
   __fastcall TGapJunctionCurrent( const TGapJunctionCurrent & source);

   /// overloaded assignment operator
   TGapJunctionCurrent & operator = (const TGapJunctionCurrent & source);

   /// overloaded method for duplicating currents without complete assignment
   virtual void __fastcall CopyParamsFrom(const TCurrent * const source );

	virtual ~TGapJunctionCurrent();

};

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGJCurrentForm   ***************************
//
//
/// GUI Editor for TGapJunctionCurrent
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TGJCurrentForm : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TEdit *Edit1;
   TLabel *Label2;
   void __fastcall Edit1KeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
public:		// User declarations
   TGapJunctionCurrent *GJCurrent;
   __fastcall TGJCurrentForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGJCurrentForm *GJCurrentForm;
//---------------------------------------------------------------------------
#endif
