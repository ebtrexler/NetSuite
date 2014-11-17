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

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_BiologicalCell.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBiologicalCellForm *BiologicalCellForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TBiologicalCell methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define TBiologicalCell

#include "RT_Network.h"
#include "NI_DAQmx_Devices.h"
//-----------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//   *****************    TBiologicalCell   ************************
//
//
/// Derived class that implements "dynamically clamped" cell
/*!

SetVm() called to update membrane voltage.  Then all Update() is called so that
intrinsic currents, synaptic currents, and electrode currents are calculated and
summed.  This value is used by data acquisition classes to inject current.

  @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TBiologicalCell : public TCell
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	  ar & boost::serialization::base_object<TCell>(*this);

	  // 02/21/2012 added streaming of FPosCurrentLimit and FNegCurrentLimit.
	  // -- also must add
		// BOOST_CLASS_VERSION(TBiologicalCell, 1) below
      if (version >= 1) {
			ar & BOOST_SERIALIZATION_NVP(FPosCurrentLimit);
			ar & BOOST_SERIALIZATION_NVP(FNegCurrentLimit);
		}
	}
#endif

private:
	double FPosCurrentLimit;
	double FNegCurrentLimit;

protected:

public:

   virtual void* const __fastcall
                                 GetEditForm()
   {
      static TBiologicalCellForm *form = new TBiologicalCellForm(NULL);
      form->BiologicalCell = this;
      return form;
   }

   virtual void __fastcall       PopulateEditForm()
   {
      TBiologicalCellForm *form = (TBiologicalCellForm * )GetEditForm();

      int aiIndex = form->lbxAIChannels->Items->IndexOf(UnicodeString(FAIChannel.c_str()));
      int aoIndex = form->lbxAOChannels->Items->IndexOf(UnicodeString(FAOChannel.c_str()));

      form->lbxAIChannels->ItemIndex = aiIndex;
      form->lbxAOChannels->ItemIndex = aoIndex;
      form->AIGainEdit->Text = FloatToStr(FAIGain);
      form->AOGainEdit->Text = FloatToStr(FAOGain);

      form->ADCGainEdit->Text = FloatToStrF(1000.0/FAIGain, ffGeneral, 3, 5);
		form->DACGainEdit->Text = FloatToStrF(1.0/FAOGain, ffGeneral, 3, 5);

		form->PosLimitEdit->Text = FloatToStr(FPosCurrentLimit);
		form->NegLimitEdit->Text = FloatToStr(FNegCurrentLimit);
	}

   virtual bool __fastcall       ValidateEditForm()
   {
      TBiologicalCellForm *form = (TBiologicalCellForm * )GetEditForm();
      bool ok = true;
      bool ok1 = false;
		bool ok2 = false;
		bool ok3 = false;
		bool ok4 = false;

      try {
         int aiIndex = form->lbxAIChannels->ItemIndex;
         int aoIndex = form->lbxAOChannels->ItemIndex;
         if (aiIndex > -1) {
            FAIChannel = form->lbxAIChannels->Items->Strings[aiIndex].c_str();
         } else {
				Application->MessageBox(L"Please choose an input and output channel",
                                    L"Please correct input");
            ok = false;
         }

         if (aoIndex > -1) {
            FAOChannel = form->lbxAOChannels->Items->Strings[aoIndex].c_str();
         } else {
				Application->MessageBox(L"Please choose an input and output channel",
                                    L"Please correct input");
            ok = false;
         }

         double aigain = StrToFloat(form->AIGainEdit->Text);
         if (aigain < 0.0){
            Application->MessageBox(L"Input Gain must be greater than or equal to zero",
                                    L"Please correct input");
         } else {
            FAIGain = aigain;
            ok1 = true;
         }

         double aogain = StrToFloat(form->AOGainEdit->Text);
         if (aogain < 0.0){
            Application->MessageBox(L"Output Gain must be greater than or equal to zero",
                                    L"Please correct input");
         } else {
            ok2 = true;
            FAOGain = aogain;
			}

			double neg, pos;

			pos = StrToFloat(form->PosLimitEdit->Text);
			if (pos < 0.0) {
				Application->MessageBoxA(L"Positive Current limit must be greater than or equal to zero",
												L"Please correct input");
			}
			else {
				FPosCurrentLimit = pos;
				ok3 = true;
			}

			neg = StrToFloat(form->NegLimitEdit->Text);
			if (neg > 0.0) {
				Application->MessageBoxA(L"Negative Current limit must be less than or equal to zero",
												L"Please correct input");
			}
			else {
				FNegCurrentLimit = neg;
				ok4 = true;
			}

      } catch (EConvertError & e) {
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      }
		return (ok && ok1 && ok2 && ok3 && ok4);
   }

   /// Default constructor
	__fastcall TBiologicalCell() : TCell(),
													FPosCurrentLimit(10), FNegCurrentLimit(-10)
	{
	}

	/// Specialized constructor
	__fastcall TBiologicalCell(const std::wstring & name) : TCell(name),
													FPosCurrentLimit(10), FNegCurrentLimit(-10)
	{
	}

	/// Copy constructor
	__fastcall TBiologicalCell(const TBiologicalCell &source) :  TCell(source.Name()),
													FPosCurrentLimit(source.FPosCurrentLimit),
													FNegCurrentLimit(source.FNegCurrentLimit)
   {
   }

   /// Overloaded assignment operator
   TBiologicalCell& operator = (const TBiologicalCell & source)
   {
      if (this != &source) {  // make sure not same object
			this->TCell::operator=(source); //copy base class members
			// now copy TBiologicalCell fields
			FPosCurrentLimit = source.FPosCurrentLimit;
			FNegCurrentLimit = source.FNegCurrentLimit;
      }
      return *this;
   }

	~TBiologicalCell()
	{
	}

   // pure virtual functions overidden

   /// Sets the membrane voltage
   virtual double __fastcall     SetVm(double Vm)
   {
      return TCell::SetVm(Vm);
   }

   /// overridden, but not used because cell is voltage dependent
   virtual double __fastcall     CalcVm(double step)
   {
      return TCell::CalcVm(step);
   }

   /// Returns true, because this cell needs voltage set by SetVm()
   bool __fastcall               IsVoltageDependent()
   {
      return true;
	}

	/// Worker function that updates the current -- overrides TCell::DoUpdate to allow for current limits
	virtual double __fastcall     DoUpdate(double step)
	{
		double I = TCell::DoUpdate(step);
		if (I > FPosCurrentLimit * FAOGain) I = FPosCurrentLimit * FAOGain;
		if (I < FNegCurrentLimit * FAOGain) I = FNegCurrentLimit * FAOGain;
		return I;
	}

   // implement pure virtual
   bool __fastcall               Initialize(bool Reset)
   {
      return true;
   }

	// implement pure virtual
   #define TBiologicalCell_KEY L"Biological Cell"
   const std::wstring & __fastcall ClassKey() const
   {
      static std::wstring Key(TBiologicalCell_KEY);
      return Key;
	}

	// implement pure virtual
	// Informs caller whether can accept currents or not
	virtual const bool __fastcall AcceptsCurrents() const
	{
		return true;
	}
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TBiologicalCell)
BOOST_CLASS_VERSION(TBiologicalCell, 1) //Feb-22-2012
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

#ifndef SIM_ONLY_NO_NIDAQ
namespace TBiologicalCellspace
{
   static struct regTBiologicalCell {
	  regTBiologicalCell(){
		 GetCellFactory().registerBuilder(
								 TBiologicalCell_KEY, //L"Biological Cell",
								 TypeID<TBiologicalCell>(),
								 TypeID<const std::wstring>());
      }
   } RegTBiologicalCell;
}
#endif

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TBiologicalCellForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================



//---------------------------------------------------------------------------
__fastcall TBiologicalCellForm::TBiologicalCellForm(TComponent* Owner)
   : TForm(Owner)
{
   lbxAIChannels->Clear();
   lbxAOChannels->Clear();

#ifndef SIM_ONLY_NO_NIDAQ
   for (int i=0; i<GetNIDAQmxSetup().NumDevices; i++){
      TNIDAQmxDevice *dev = GetNIDAQmxSetup().Devices[i];
      lbxAIChannels->Items->AddStrings(dev->AIChannels);
      lbxAOChannels->Items->AddStrings(dev->AOChannels);
	}
#endif
}
//---------------------------------------------------------------------------


void __fastcall TBiologicalCellForm::AmpGainEditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
      double adcGain = StrToFloat(ADCGainEdit->Text);
      double dacGain = StrToFloat(DACGainEdit->Text);
      AIGainEdit->Text = FloatToStrF(1000.0/adcGain, ffGeneral, 3, 5);
      AOGainEdit->Text = FloatToStrF(1.0/dacGain, ffGeneral, 3, 5);

      Key = 0;

      BiologicalCell->ValidateEditForm();
   }
}
//---------------------------------------------------------------------------

void __fastcall TBiologicalCellForm::NetworkGainEditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
      double aiGain = StrToFloat(AIGainEdit->Text);
      double aoGain = StrToFloat(AOGainEdit->Text);
      ADCGainEdit->Text = FloatToStrF(1000.0/aiGain, ffGeneral, 3, 5);
      DACGainEdit->Text = FloatToStrF(1.0/aoGain, ffGeneral, 3, 5);

      Key = 0;

      BiologicalCell->ValidateEditForm();
   }
}
//---------------------------------------------------------------------------

void __fastcall TBiologicalCellForm::lbxAIAOChannelsClick(TObject *Sender)
{
   BiologicalCell->ValidateEditForm();
}
//---------------------------------------------------------------------------

void __fastcall TBiologicalCellForm::PosNegLimitEditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
      Key = 0;

      BiologicalCell->ValidateEditForm();
   }
}
//---------------------------------------------------------------------------

