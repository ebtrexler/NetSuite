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

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_VoltageClampPIDCurrent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVoltageClamp_PID_CurrentForm *VoltageClamp_PID_CurrentForm;
//---------------------------------------------------------------------------

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TVoltageClamp_PID_Current methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define TVoltageClamp_PID_Current

#include "RT_Network.h"

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TVoltageClamp_PID_Current   ***************************
//
//
/**
   Uses Proportional-Integral-Differential method to determine injected current necessary
   to hold a cell at a command potential

   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
   @brief Implementation of current designed to clamp a cells voltage to a command potential
*/
class TVoltageClamp_PID_Current: public TCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCurrent>(*this);
      ar & BOOST_SERIALIZATION_NVP(FpGain);
      ar & BOOST_SERIALIZATION_NVP(FiGain);
      ar & BOOST_SERIALIZATION_NVP(FdGain);
      ar & BOOST_SERIALIZATION_NVP(FiMax);
      ar & BOOST_SERIALIZATION_NVP(FiMin);
      ar & BOOST_SERIALIZATION_NVP(FiState);
      ar & BOOST_SERIALIZATION_NVP(FdState);
      ar & BOOST_SERIALIZATION_NVP(Ftau);
      ar & BOOST_SERIALIZATION_NVP(FVCommand);
	}
#endif

private:
	double								FpGain;
	double 								FiGain;
   double                        FdGain;
	double 								FiMax, FiMin;
	double 								FiState, FdState;
   double                        Ftau;
   double                        FVCommand;

protected:

// moved to public -- 1/24/14 -- ebt (shouldn't have been hidden in protected anyway)
//   /// Returns downcasted TVoltageClamp_PID_CurrentForm* that is used to set values for this object
//	virtual void* const __fastcall
//											GetEditForm()
//	{
//		static TVoltageClamp_PID_CurrentForm *form = new TVoltageClamp_PID_CurrentForm(NULL);
////      form->HHTNCurrent = this;
//		return form;
//	}

	/// Writes data members to a stream
   virtual void const __fastcall WriteToStream(ostream &stream) const
   {
      stream.write((char *)&FpGain, sizeof(double));
      stream.write((char *)&FiGain, sizeof(double));
      stream.write((char *)&FdGain, sizeof(double));
      stream.write((char *)&FiMax, sizeof(double));
      stream.write((char *)&FiMin, sizeof(double));
      stream.write((char *)&FiState, sizeof(double));
      stream.write((char *)&FdState, sizeof(double));
      stream.write((char *)&Ftau, sizeof(double));
      stream.write((char *)&FVCommand, sizeof(double));
   }

   /// Reads data members from a stream
   virtual void const __fastcall ReadFromStream(istream &stream)
   {
      stream.read((char *)&FpGain, sizeof(double));
      stream.read((char *)&FiGain, sizeof(double));
      stream.read((char *)&FdGain, sizeof(double));
      stream.read((char *)&FiMax, sizeof(double));
      stream.read((char *)&FiMin, sizeof(double));
      stream.read((char *)&FiState, sizeof(double));
      stream.read((char *)&FdState, sizeof(double));
      stream.read((char *)&Ftau, sizeof(double));
      stream.read((char *)&FVCommand, sizeof(double));
   }

public:

   // implement pure virtual
	bool __fastcall               Initialize(bool Reset)
	{
		if (Reset) {
			FiState = 0;
			FdState = 0;
		}
		return true;
	}

	// implement pure virtual
	#define TVoltageClamp_PID_Current_KEY L"Voltage Clamp (PID) Current"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(TVoltageClamp_PID_Current_KEY);
		return Key;
	}

	// implement pure virtual
	const void __fastcall			GetParamLogHeader(
													std::vector<std::string>& params) const
	{
		params.clear();  // do nothing, set size to zero
	}


   /// Compute current based on PID control
   /*!
   <pre>
   *
   *         computes pTerm + iTerm + (dTerm * step/tau)  ;
   </pre>
   */
   double UpdatePID(double step, double error, double position)
   {
      double pTerm, dTerm, iTerm;

      // calculate the proportional term
      pTerm = FpGain * error;

      // calculate the integral state with appropriate limiting
      FiState += error;
      if (FiState > FiMax) FiState = FiMax;
      else if (FiState < FiMin) FiState = FiMin;

      // calculate the integral term
      iTerm = FiGain * FiState;

      // calculate the derivative term
      dTerm = FdGain * (position - FdState);

      FdState = position;

      return pTerm + iTerm + dTerm * step / Ftau ;
   }


   /// Uses Proportional-Integral-Differential method to determine injected current
   /*!
   <pre>
   *
   *         Calls UpdatePID with error = Vcommand - Vkin.
   *           Uses Vkin so a synapse can clamp a cell.  Vdrv is ignored.
   </pre>
   */
	virtual double __fastcall DoUpdate(double step, double Vkin, double Vdrv,
													std::vector<double> & params)
	{
		double error = FVCommand - Vkin;
		params.clear(); // no logging

		return UpdatePID(step, error, Vkin) * -15;  // don't know why 15 is magic number
	}

   /// Called by GUI to synchronize edit form with current values of object params
   virtual void __fastcall       PopulateEditForm()
   {
      TVoltageClamp_PID_CurrentForm *form =
         (TVoltageClamp_PID_CurrentForm * )GetEditForm();

      form->PGainEdit->Text = FpGain;
      form->IGainEdit->Text = FiGain;
      form->DGainEdit->Text = FdGain;
      form->TauEdit->Text = Ftau;
      form->VCommandEdit->Text = FVCommand;
      form->ImaxEdit->Text = FiMax;
      form->IminEdit->Text = FiMin;
   }


   /// Called by GUI to check if changed values are satisfactory
	virtual bool __fastcall       ValidateEditForm()
   {

      TVoltageClamp_PID_CurrentForm *form = (TVoltageClamp_PID_CurrentForm * )GetEditForm();
      bool ok = true;
      try {
         FpGain = StrToFloat(form->PGainEdit->Text);
         FiGain = StrToFloat(form->IGainEdit->Text);
         FdGain = StrToFloat(form->DGainEdit->Text);
         Ftau = StrToFloat(form->TauEdit->Text);
         FVCommand = StrToFloat(form->VCommandEdit->Text);
         FiMax = StrToFloat(form->ImaxEdit->Text);
         FiMin = StrToFloat(form->IminEdit->Text);
      } catch (EConvertError & e) {
         ok = false;
         Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      }
      return ok;
	}

	/// Returns downcasted TVoltageClamp_PID_CurrentForm* that is used to set values for this object
	virtual void* const __fastcall
											GetEditForm()
	{
		static TVoltageClamp_PID_CurrentForm *form = new TVoltageClamp_PID_CurrentForm(NULL);
		return form;
	}

   /// default constructor
   __fastcall TVoltageClamp_PID_Current():
         TCurrent(NULL, L""),
         FpGain(0),
	   	FiGain(0),
         FdGain(0),
	      FiMax(1),
         FiMin(-1),
	      FiState(0),
         FdState(0),
         Ftau(1),
         FVCommand(0)

   {
   }

   /// specialized constructor 2 param
	__fastcall TVoltageClamp_PID_Current(TCurrentUser *owner, const std::wstring & name):
         TCurrent(owner, name),
         FpGain(0),
	   	FiGain(0),
         FdGain(0),
	      FiMax(1),
         FiMin(-1),
	      FiState(0),
         FdState(0),
         Ftau(1),
         FVCommand(0)
   {
   }

   /// specialized constructor 1 param
   __fastcall TVoltageClamp_PID_Current(const std::wstring & name):
         TCurrent(NULL, name),
         FpGain(0),
	   	FiGain(0),
         FdGain(0),
	      FiMax(1),
         FiMin(-1),
	      FiState(0),
         FdState(0),
         Ftau(1),
         FVCommand(0)
   {
   }

   /// copy constructor
   __fastcall TVoltageClamp_PID_Current( const TVoltageClamp_PID_Current & source ) :
         TCurrent(source.Owner(), source.Name()),
         FpGain(source.FpGain),
	   	FiGain(source.FiGain),
         FdGain(source.FdGain),
	      FiMax(source.FiMax),
         FiMin(source.FiMin),
	      FiState(source.FiState),
         FdState(source.FdState),
         Ftau(source.Ftau),
         FVCommand(source.FVCommand)
   {
   }

   /// overloaded assignment operator
   TVoltageClamp_PID_Current & operator = (const TVoltageClamp_PID_Current & source)
   {
      if (this != &source) {  // make sure not same object
         this->TCurrent::operator=(source); //copy base class members
         // now copy TVoltageClamp_PID_Current fields
         FpGain = (source.FpGain),
	   	FiGain = (source.FiGain),
         FdGain = (source.FdGain),
	      FiMax = (source.FiMax),
         FiMin = (source.FiMin),
	      FiState = (source.FiState),
         FdState = (source.FdState),
         Ftau = (source.Ftau),
         FVCommand = (source.FVCommand);
      }
      return *this;
   }

   /// overloaded method for duplicating currents without complete assignment
   virtual void __fastcall CopyParamsFrom(const TCurrent * const source )
   {
      if (this != source) {  // make sure not same object
         if (this->ClassType() == source->ClassType()) { // make sure same type
            TVoltageClamp_PID_Current *cast_source = (TVoltageClamp_PID_Current *)source;

            FpGain = (cast_source->FpGain),
            FiGain = (cast_source->FiGain),
            FdGain = (cast_source->FdGain),
            FiMax = (cast_source->FiMax),
            FiMin = (cast_source->FiMin),
            FiState = (cast_source->FiState),
            FdState = (cast_source->FdState),
            Ftau = (cast_source->Ftau),
            FVCommand = (cast_source->FVCommand);
         }
      }
   }

   virtual ~TVoltageClamp_PID_Current(){;}
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TVoltageClamp_PID_Current)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
   static struct regVoltageClamp_PID_Current {
      regVoltageClamp_PID_Current(){
         GetCurrentFactory().registerBuilder(
                                 TVoltageClamp_PID_Current_KEY, //L"Voltage Clamp (PID) Current",
                                 TypeID<TVoltageClamp_PID_Current>(),
                                 TypeID<TCurrentUser*const>(),
                                 TypeID<const std::wstring>());
      }
   } RegVoltageClamp_PID_Current;
}

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TVoltageClamp_PID_CurrentForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================



#pragma region GUI methods of TVoltageClamp_PID_CurrentForm

//---------------------------------------------------------------------------


__fastcall TVoltageClamp_PID_CurrentForm::TVoltageClamp_PID_CurrentForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#pragma end_region
