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

#include "GUI_RT_Edit_PlaybackCell.h"
#include "GUI_PlaybackWaveform.h"
#include "RT_Network.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PLOTPanel"
#pragma resource "*.dfm"
TPlaybackCellForm *PlaybackCellForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TPlaybackCell methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define TPlaybackCell

//-----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackCell   ***************************
//
//
/// Derived class that implements model cell that plays back pre-recorded Vm
/*!
   It can contain no currents.  Appropriate methods are overridden.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TPlaybackCell : public TCell
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	  ar & boost::serialization::base_object<TCell>(*this);
	  ar & BOOST_SERIALIZATION_NVP(FPlaybackWaveform);
	}
#endif

private:
	// fields
	TPlaybackWaveform*	          	FPlaybackWaveform;

protected:

public:
	__fastcall TPlaybackCell() : TCell(), FPlaybackWaveform(NULL)
	{
	}
	__fastcall TPlaybackCell(const std::wstring & name) : TCell(name),
																			FPlaybackWaveform(NULL)
	{
	}
	~TPlaybackCell()
	{
		if (FPlaybackWaveform) {
			delete FPlaybackWaveform;
		}
	}

	virtual TPlaybackWaveform* const __fastcall  GetPlaybackWaveform()
	{
		if (FPlaybackWaveform == NULL){
			FPlaybackWaveform = new TPlaybackWaveform();
		}
		return FPlaybackWaveform;
	}

	// pure virtual functions implemented

	// implement pure virtual
	virtual void* const __fastcall	GetEditForm()
	{
		return GetPlaybackWaveform()->GetEditForm();
	}

	// implement pure virtual
	virtual void __fastcall       PopulateEditForm()
	{
		GetPlaybackWaveform()->SetUnits(L"mV");
		GetPlaybackWaveform()->PopulateEditForm();
	}

	// implement pure virtual
	virtual bool __fastcall       ValidateEditForm()
	{
		return GetPlaybackWaveform()->ValidateEditForm();
	}

	/// Sets the membrane voltage
	virtual double __fastcall     SetVm(double Vm)
	{
		return TCell::SetVm(Vm);
	}

	/// Calculates the voltage given the step in ms
	virtual double __fastcall     CalcVm(double step)
	{
		double val = FPlaybackWaveform->EvaluateAtNext(step);
		SetVm(val);
		return val;
	}

	/// Returns false, because this cell exists only in software
	bool __fastcall               IsVoltageDependent()
	{
		return false;
	}

	bool __fastcall               Initialize(bool Reset)
	{
		return FPlaybackWaveform->Initialize(Reset);
	}

	// implement pure virtual
	#define TPlaybackCell_KEY L"Vm Playback Cell"
	const std::wstring & __fastcall ClassKey() const
	{
      static std::wstring Key(TPlaybackCell_KEY);
      return Key;
   }

   // virtual functions overridden
   /// Overrides and disables adding a TCurrent * to the array of Currents
   virtual const TCurrent * __fastcall AddCurrent(TCurrent * const c, TCell * cell = NULL)
   {
      throw Exception(L"Adding currents disallowed for this cell");
      return NULL;
   }
   /// Overrides and disables adding a TElectrode * to the array of Electrodes
   virtual const TElectrode *__fastcall AddElectrode(TElectrode * e)
	{
      throw Exception(L"Adding electrodes disallowed for this cell");
		return NULL;
	}

	// implement pure virtual
	/// Informs caller whether can accept currents or not
	virtual const bool __fastcall AcceptsCurrents() const
	{
		return false;  // playback, no current calculations
	}

   /// Disables current calculations because there aren't any.
   virtual double __fastcall DoUpdate(double step)
   {
      return 0.0;
   }
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TPlaybackCell)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace TPlaybackCellspace
{
   static struct regTPlaybackCell {
	  regTPlaybackCell(){
		 GetCellFactory().registerBuilder(
								 TPlaybackCell_KEY, // L"Vm Playback Cell"
								 TypeID<TPlaybackCell>(),
								 TypeID<const std::wstring>());
      }
   } RegTPlaybackCell;
}

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TPlaybackCellForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================
//---------------------------------------------------------------------------
__fastcall TPlaybackCellForm::TPlaybackCellForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPlaybackCellForm::BrowseBitBtnClick(TObject *Sender)
{
   if (OpenDialog1->Execute()) {
		Cell->GetPlaybackWaveform()->SetFileName(OpenDialog1->FileName.c_str());
   }
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackCellForm::ChannelComboBoxChange(TObject *Sender)
{
	Cell->GetPlaybackWaveform()->SetChannel(ChannelComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackCellForm::ConvertmVEditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
		Cell->GetPlaybackWaveform()->SetScaleFactor(StrToFloat(ConvertmVEdit->Text));
      Key = 0;
   }
}
//---------------------------------------------------------------------------

