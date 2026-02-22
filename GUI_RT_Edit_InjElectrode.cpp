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

#include "GUI_RT_Edit_InjElectrode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInjectionElectrodeForm *InjectionElectrodeForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TInjectionElectrode methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define TInjectionElectrode

#include "RT_Network.h"

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TInjectionElectrode   ***************************
//
//
/// Implementation of a simple square-wave current pulse
/*!
<pre>
 Class TInjectionElectrode produces a square current in a cell.  The usual
 parameter settings are available, such as
   InitialDelay,
   Delay,
   Duration,
   Amplitude, and
   NumberOfRepeats
</pre>

   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TInjectionElectrode: public TElectrode
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TElectrode>(*this);

      // 11/30/2011 added streaming of FInitDelay. -- also must add
      // BOOST_CLASS_VERSION(TInjectionElectrode, 1) below
      if (version >= 1) {
         ar & BOOST_SERIALIZATION_NVP(FInitDelay);
      }
      ar & BOOST_SERIALIZATION_NVP(FDelay);
      ar & BOOST_SERIALIZATION_NVP(FDuration);
      ar & BOOST_SERIALIZATION_NVP(FAmplitude);
      ar & BOOST_SERIALIZATION_NVP(FNumRepeats);
	}
#endif

private:
   double                        FInitDelay; // ms
   double                        FDelay;     // ms
   double                        FDuration;  // ms
   double                        FAmplitude; // nA
   int                           FNumRepeats;

   UnicodeString                 DelayTxt, DurationTxt, AmplitudeTxt, RepeatsTxt,
                                 InitDelayTxt;

protected:

	/// Writes data members to a stream
	virtual void const __fastcall WriteToStream(ostream &stream) const
	{
		stream.write((char *)&FInitDelay, sizeof(double));
		stream.write((char *)&FDelay, sizeof(double));
		stream.write((char *)&FDuration, sizeof(double));
		stream.write((char *)&FAmplitude, sizeof(double));
		stream.write((char *)&FNumRepeats, sizeof(int));
//		TElectrode::WriteToStream(stream); // write base class members
	}

	/// Reads data members from a stream
	virtual void const __fastcall ReadFromStream(istream &stream)
	{
		stream.read((char *)&FInitDelay, sizeof(double));
		stream.read((char *)&FDelay, sizeof(double));
		stream.read((char *)&FDuration, sizeof(double));
		stream.read((char *)&FAmplitude, sizeof(double));
		stream.read((char *)&FNumRepeats, sizeof(int));
//		TElectrode::ReadFromStream(stream); // read base class members
	}

public:

	// implement pure virtual
	/// Initializes ....
	bool __fastcall               Initialize(bool Reset)
	{
		return TElectrode::Initialize(Reset);
//      return true;
	}

	// implement pure virtual
	#define TInjectionElectrode_KEY L"Square Pulse Iinj Electrode"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(TInjectionElectrode_KEY);
		return Key;
	}

/// Calculation of injected current based on step
/*!
	InjectionElectrode remembers the elapsed time and injects
	current according to desired waveform
*/
	virtual double __fastcall DoUpdate(double step)
	{
		if (FNumRepeats == 0) {   // no repeats, effectively disabled
			return 0.0;
		}

		// step is ms, I is in nA
		double I = 0.0;       // uS * mV = nA
		double period = (FDelay + FDuration);
		double et = ElapsedTime() - FInitDelay;
		if (et > 0.0) {
			double whichrepeat = floor(et/period);
			double elapsed = fmod(et, period);
			if ((whichrepeat < FNumRepeats) || (FNumRepeats == -1) /*forever*/ ) {
				if (elapsed >= FDelay && elapsed < period) {     // in the pulse duration
					I = FAmplitude;
				}
			}
		}
		return I;
	}

	/// Called by GUI to synchronize edit form with current values of object params
	virtual void __fastcall       PopulateEditForm()
	{
		TInjectionElectrodeForm *form = (TInjectionElectrodeForm * )GetEditForm();

		InitDelayTxt = L"Initial Delay (ms)";
		DelayTxt = L"Interpulse Interval (ms)";
		DurationTxt = L"Pulse Duration (ms)";
		AmplitudeTxt = L"Amplitude (nA)";
		RepeatsTxt = L"NumRepeats (-1 to N)";

		form->ValueListEditor1->Values[InitDelayTxt] = FInitDelay;
		form->ValueListEditor1->Values[DelayTxt] = FDelay;
		form->ValueListEditor1->Values[DurationTxt] = FDuration;
		form->ValueListEditor1->Values[AmplitudeTxt] = FAmplitude;
		form->ValueListEditor1->Values[RepeatsTxt] = FNumRepeats;
	}


	/// Called by GUI to check if changed values are satisfactory
	virtual bool __fastcall       ValidateEditForm()
	{

		TInjectionElectrodeForm *form = (TInjectionElectrodeForm * )GetEditForm();
		bool ok = true;
		double temp;
		TValueListEditor *editor = form->ValueListEditor1;
		UnicodeString message;
		try {

			// Initial Delay
			temp = StrToFloat(editor->Values[InitDelayTxt]);
			if (temp < 0.0) {
				message = L"Initial Delay must be greater than or equal to zero ms";
				Application->MessageBox(message.c_str(), L"Please correct input");
				ok = false;
			} else {
				FInitDelay = temp;
			}

			// Delay
			temp = StrToFloat(editor->Values[DelayTxt]);
			if (temp < 0.0) {
				message = L"Delay must be greater than or equal to zero ms";
				Application->MessageBox(message.c_str(), L"Please correct input");
				ok = false;
			} else {
				FDelay = temp;
			}

			// Duration
			temp = StrToFloat(editor->Values[DurationTxt]);
			if (temp < 0.0) {
				message = L"Duration must be greater than or equal to zero ms";
				Application->MessageBox(message.c_str(), L"Please correct input");
				ok = false;
			} else {
				FDuration = temp;
			}

			// Amplitude
			temp = StrToFloat(editor->Values[AmplitudeTxt]);
			FAmplitude = temp;

			// Number of Repeats
			temp = StrToFloat(editor->Values[RepeatsTxt]);
			FNumRepeats = temp;

		} catch (EConvertError & e) {
			ok = false;
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		}

		return ok;
	}

   /// Returns downcasted TInjectionElectrodeForm* that is used to set values for this object
   virtual void* const __fastcall
                                 GetEditForm()
   {
      static TInjectionElectrodeForm *form = new TInjectionElectrodeForm(NULL);
      form->InjElectrode = this;
      return form;
	}

	/// default constructor
	__fastcall TInjectionElectrode():
			TElectrode(NULL, L""),
			FInitDelay(0),
			FDelay(0),
			FDuration(0),
			FAmplitude(0),
			FNumRepeats(1)
	{
   }

   /// specialized constructor 2 param
	__fastcall TInjectionElectrode(TCell * const owner, const std::wstring & name):
         TElectrode(owner, name),
         FInitDelay(0),
         FDelay(0),
         FDuration(0),
         FAmplitude(0),
         FNumRepeats(1)
   {
   }

   /// specialized constructor 1 param
   __fastcall TInjectionElectrode(const std::wstring & name):
         TElectrode(NULL, name),
         FInitDelay(0),
         FDelay(0),
         FDuration(0),
         FAmplitude(0),
         FNumRepeats(1)
   {
   }

   /// copy constructor
   __fastcall TInjectionElectrode( const TInjectionElectrode & source ) :
         TElectrode(source.Owner(), source.Name()),
         FInitDelay(source.FInitDelay),
         FDelay(source.FDelay),
         FDuration(source.FDuration),
         FAmplitude(source.FAmplitude),
         FNumRepeats(source.FNumRepeats)
   {
   }

   /// overloaded assignment operator
   TInjectionElectrode & operator = (const TInjectionElectrode & source)
   {
      if (this != &source) {  // make sure not same object
         this->TElectrode::operator=(source); //copy base class members
         // now copy TInjectionElectrode fields
         FInitDelay = source.FInitDelay;
         FDelay = (source.FDelay);
         FDuration = (source.FDuration);
         FAmplitude = (source.FAmplitude);
         FNumRepeats = (source.FNumRepeats);
      }
      return *this;
   }

//	// friend insertion << operator
//	friend ostream &operator<<(ostream &stream, const TInjectionElectrode &electrode)
//	{
//		stream.write((char *)&electrode.FInitDelay, sizeof(double));
//		stream.write((char *)&electrode.FDelay, sizeof(double));
//		stream.write((char *)&electrode.FDuration, sizeof(double));
//		stream.write((char *)&electrode.FAmplitude, sizeof(double));
//		stream.write((char *)&electrode.FNumRepeats, sizeof(double));
//		return stream;
//	}
//
//	// friend extraction >> operator
//	friend istream &operator>>(istream &stream, TInjectionElectrode &electrode)
//	{
//		stream.read((char *)&electrode.FInitDelay, sizeof(double));
//		stream.read((char *)&electrode.FDelay, sizeof(double));
//      stream.read((char *)&electrode.FDuration, sizeof(double));
//		stream.read((char *)&electrode.FAmplitude, sizeof(double));
//      stream.read((char *)&electrode.FNumRepeats, sizeof(double));
//		return stream;
//   }

	virtual ~TInjectionElectrode(){;}
};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TInjectionElectrode)
BOOST_CLASS_VERSION(TInjectionElectrode, 1) //added 11/30/2011 for streaming FInitDelay
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace InjElectrode
{
   static struct regInjectionElectrode {
      regInjectionElectrode(){
         GetElectrodeFactory().registerBuilder(
                                 TInjectionElectrode_KEY, // L"Square Pulse Iinj Electrode"
                                 TypeID<TInjectionElectrode>(),
                                 TypeID<TCell* const>(),
                                 TypeID<const std::wstring>());
      }
   } RegInjectionElectrode;
}

#pragma end_region


//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TInjectionElectrodeForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region GUI methods of TInjectionElectrodeForm
//---------------------------------------------------------------------------
__fastcall TInjectionElectrodeForm::TInjectionElectrodeForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------


#pragma end_region

void __fastcall TInjectionElectrodeForm::ValueListEditor1KeyPress(TObject *Sender,
          wchar_t &Key)
{
   if (Key == VK_RETURN) {
      InjElectrode->ValidateEditForm();
      Key = 0;
   }
}
//---------------------------------------------------------------------------

