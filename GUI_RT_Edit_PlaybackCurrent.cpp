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

#include "GUI_RT_Edit_PlaybackCurrent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPlaybackCurrentForm *PlaybackCurrentForm;

#include "RT_Network.h"
#include "GUI_PlaybackWaveform.h"
#include "RT_Utilities.h"

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TPlaybackCurrent methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region RT classes with methods that define TPlaybackCurrent



////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackCurrent   ***************************
//
//
/// Implements a current for an electrical synapse
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TPlaybackCurrent : public TCurrent
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<TCurrent>(*this);
      ar & BOOST_SERIALIZATION_NVP(F_E);
      ar & BOOST_SERIALIZATION_NVP(F_Gmax);
		ar & BOOST_SERIALIZATION_NVP(F_Gnoise);
	}
#endif

private:
	double 								F_E;
	double 								F_Gmax;
	double                        F_Gnoise;

	long                          idum; // for random variables

	TPlaybackWaveform*	         FPlaybackWaveform;

protected:
	void const __fastcall 			WriteToStream(ostream &stream) const
	{
		stream.write((char *)&F_Gmax, sizeof(double));
		TCurrent::WriteToStream(stream); //Write base class members
	}

	void const __fastcall 			ReadFromStream(istream &stream)
	{
		stream.read((char *)&F_Gmax, sizeof(double));
		TCurrent::ReadFromStream(stream); //Read base class members
	}

	// implement pure virtual
	const void __fastcall			GetParamLogHeader(
													std::vector<std::string>& params) const
	{
		params.clear();  // do nothing, set size to zero

		params.push_back("_G");
	}

public:
	const double __fastcall			Gmax()
	{
		return F_Gmax;
	}

	void __fastcall               SetGmax(double set)
	{
		F_Gmax = set;
	}

	virtual TPlaybackWaveform* const __fastcall  GetPlaybackWaveform()
	{
		if (FPlaybackWaveform == NULL){
			throw Exception(L"FPlaybackWaveform not created yet!");
		}
		return FPlaybackWaveform;
	}

	// pure virtual functions implemented

	// implement pure virtual
	virtual void* const __fastcall	GetEditForm()
	{
		static TPlaybackCurrentForm *form = new TPlaybackCurrentForm(NULL);
		if (FPlaybackWaveform == NULL){ // 1st time called
			FPlaybackWaveform = new TPlaybackWaveform();
			FPlaybackWaveform->AttachEditFormTo(form,form->ScrollBox1);
			form->PlaybackCurrent = this;
		}
		return form;
	}

	// implement pure virtual
	virtual void __fastcall       PopulateEditForm()
	{
		TPlaybackCurrentForm *form = (TPlaybackCurrentForm * )GetEditForm();

		form->GmaxEdit->Text = FloatToStrF(F_Gmax, ffFixed, 3, 3);
		form->GnoiseEdit->Text = FloatToStrF(F_Gnoise, ffFixed, 3, 3);
		form->ErevEdit->Text = FloatToStrF(F_E, ffFixed, 3, 3);
		GetPlaybackWaveform()->SetUnits(L"");
		GetPlaybackWaveform()->PopulateEditForm();
	}

	// implement pure virtual
	virtual bool __fastcall       ValidateEditForm()
	{
		TPlaybackCurrentForm *form = (TPlaybackCurrentForm * )GetEditForm();

		bool ok = true;
		double temp;
		try {
			temp = StrToFloat(form->GmaxEdit->Text);
			F_Gmax = temp;

			temp = StrToFloat(form->ErevEdit->Text);
			F_E = temp;

         temp = StrToFloat(form->GnoiseEdit->Text);
         F_Gnoise = temp;

      } catch (EConvertError & e) {
         ok = false;
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		}

		// kinetic parameters logging
		SetParamLoggingEnabled(form->ParamLoggingCheckBox->Checked);

		if (ok) {
			ok = GetPlaybackWaveform()->ValidateEditForm();
		}

		return ok;
	}


	// override TCurrent::DoUpdate
	virtual double __fastcall 		DoUpdate(double step, double Vkin, double Vdrv,
														std::vector<double> & params)
	{
		// step is ms, V's are mV, G is in microsiemens, I is in nA

		double I = 0.0;
		double G = 0.0;

		params.clear();  // set size to zero

		//calculate conductance based on waveform
		double gain = GetPlaybackWaveform()->EvaluateAtNext(step);

		if (F_Gnoise == 0.0){
			G = F_Gmax * gain;
		}
		else {
			double noise = 0.01 * F_Gnoise * F_Gmax * gasdev(&idum);
			G = (F_Gmax + noise) * gain;
		}

		I = G * (Vdrv - F_E);       // uS * mV = nA

		params.push_back(G);

		return I;
	}

	// implement pure virtual
	virtual bool __fastcall Initialize(bool Reset)
	{
      randomize();
		idum = -1 * rand();
		return FPlaybackWaveform->Initialize(Reset);
	}

	// implement pure virtual
	#define TPLAYBACKCURRENT_KEY L"Playback Current"
	const std::wstring & __fastcall TPlaybackCurrent::ClassKey() const
	{
		static std::wstring Key(TPLAYBACKCURRENT_KEY);
		return Key;
	}

	/// Default Constructor
	__fastcall TPlaybackCurrent():
			TCurrent(NULL, L""), F_Gmax(0), FPlaybackWaveform(NULL) {}

	/// Specialized Constructor 2 param
	__fastcall TPlaybackCurrent(TCurrentUser *owner, const std::wstring & name):
			TCurrent(owner, name), F_Gmax(0), FPlaybackWaveform(NULL) {}

	/// Specialized Constructor 1 param
	__fastcall TPlaybackCurrent(const std::wstring & name):
			TCurrent(NULL, name), F_Gmax(0), FPlaybackWaveform(NULL) {}

   /// copy constructor
	__fastcall TPlaybackCurrent( const TPlaybackCurrent & source):
			TCurrent(source.Owner(), source.Name()),
			F_Gmax(source.F_Gmax) {}

   /// overloaded assignment operator
	TPlaybackCurrent & operator = (const TPlaybackCurrent & source)
	{
		if (this != &source) {  // make sure not same object
			this->TCurrent::operator=(source); //copy base class members
			// now copy TPlaybackCurrent fields
			F_Gmax = source.F_Gmax;
		}
		return *this;
	}

	/// overloaded method for duplicating currents without complete assignment
	virtual void __fastcall CopyParamsFrom(const TCurrent * const source )
	{
		if (this != source) {  // make sure not same object
			if (this->ClassType() == source->ClassType()) { // make sure same type
				TPlaybackCurrent *cast_source = (TPlaybackCurrent *)source;
				F_Gmax = cast_source->F_Gmax;
			}
		}
	}

	virtual ~TPlaybackCurrent()
	{
		if (FPlaybackWaveform) {
			delete FPlaybackWaveform;
		};
	}

};

//---------------------------------------------------------------------------


#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TPlaybackCurrent)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
	static struct regPlaybackCurrent {
      regPlaybackCurrent(){
         GetCurrentFactory().registerBuilder(
											TPLAYBACKCURRENT_KEY, //L"Gap Junction Current",
											TypeID<TPlaybackCurrent>(),
                                 TypeID<TCurrentUser*const>(),
                                 TypeID<const std::wstring>());
      }
	} RegPlaybackCurrent;
}

//---------------------------------------------------------------------------

#pragma end_region

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TPlaybackCurrentForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region GUI methods of TPlaybackCurrentForm

//---------------------------------------------------------------------------
__fastcall TPlaybackCurrentForm::TPlaybackCurrentForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackCurrentForm::Edit1KeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == VK_RETURN) {
		PlaybackCurrent->ValidateEditForm();
		Key = 0;
	}
}

//---------------------------------------------------------------------------
#pragma end_region

