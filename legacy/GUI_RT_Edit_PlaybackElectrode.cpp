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

#include "GUI_RT_Edit_PlaybackElectrode.h"
#include "GUI_PlaybackWaveform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PLOTPanel"
#pragma resource "*.dfm"
TPlaybackElectrodeForm *PlaybackElectrodeForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TPlaybackElectrode methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define TPlaybackCell

#include "RT_Network.h"

//-----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackElectrode   ***************************
//
//
/// Derived class that implements an electrode that plays back pre-recorded waveform as nA.
/*!
	   Gains and offset alter waveform.  Appropriate methods are overridden.

   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TPlaybackElectrode : public TElectrode
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	  ar & boost::serialization::base_object<TElectrode>(*this);
	  ar & BOOST_SERIALIZATION_NVP(FPlaybackWaveform);
	}
#endif

private:
	// fields
	TPlaybackWaveform*	          	FPlaybackWaveform;

protected:

	/// pure virtual function for derived classes to write to a stream
	virtual void const __fastcall WriteToStream(ostream &stream) const
	{
	}
	/// pure virtual function for derived classes to read from a stream
	virtual void const __fastcall ReadFromStream(istream &stream)
	{
	}

public:
	__fastcall TPlaybackElectrode() : TElectrode(NULL, L""), FPlaybackWaveform(NULL)
	{
	}
	__fastcall TPlaybackElectrode(TCell * const owner, const std::wstring & name) : TElectrode(owner, name), FPlaybackWaveform(NULL)
	{
	}
	~TPlaybackElectrode()
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
	virtual double __fastcall DoUpdate(double step)
	{
		double val = FPlaybackWaveform->EvaluateAtNext(step);
		return val;
	}


	bool __fastcall               Initialize(bool Reset)
	{
		return FPlaybackWaveform->Initialize(Reset);
	}

   virtual void* const __fastcall
											GetEditForm()
	{
		return GetPlaybackWaveform()->GetEditForm();
	}

	virtual void __fastcall       PopulateEditForm()
	{
		GetPlaybackWaveform()->SetUnits(L"nA");
		GetPlaybackWaveform()->PopulateEditForm();
	}

	virtual bool __fastcall       ValidateEditForm()
	{
		return GetPlaybackWaveform()->ValidateEditForm();
	}

	// implement pure virtual
	#define TPlaybackElectrode_KEY L"Playback Electrode"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(TPlaybackElectrode_KEY);
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
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TPlaybackElectrode)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace TPlaybackElectrodespace
{
	static struct regTPlaybackElectrode {
	  regTPlaybackElectrode(){
			GetElectrodeFactory().registerBuilder(
											TPlaybackElectrode_KEY, // L"Playback Electrode"
											TypeID<TPlaybackElectrode>(),
											TypeID<TCell* const>(),
											TypeID<const std::wstring>());
		}
	} RegTPlaybackElectrode;
}

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TPlaybackElectrodeForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================
//---------------------------------------------------------------------------
__fastcall TPlaybackElectrodeForm::TPlaybackElectrodeForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------




