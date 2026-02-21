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

#include "RT_HHCurrent.h"
#ifndef NO_VCL
#include "GUI_RT_Edit_HHCurrent.h"
#endif
#include "RT_Utilities.h"

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- THHCurrent methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region RT classes with methods that define THHCurrent

	// activation kinetic factor
	THHKineticsFactor& THHCurrent::get_m()
	{
		return _m;
	}
	// inactivation kinetic factor
	THHKineticsFactor& THHCurrent::get_h()
	{
		return _h;
	}
	// third kinetic factor
	THHKineticsFactor& THHCurrent::get_n()
	{
		return _n;
	}

#ifndef NO_VCL
	/// Returns downcasted THHCurrentForm* that is used to set values for this object
	void* const THHCurrent::GetEditForm()
	{
		static THHCurrentForm *form = new THHCurrentForm(NULL);
		form->HHCurrent = this;
		return form;
	}
#endif

	/// Writes data members to a stream
	void THHCurrent::WriteToStream(std::ostream &stream) const
	{
		stream.write((char *)&F_p, sizeof(double));
		stream.write((char *)&F_q, sizeof(double));
		stream.write((char *)&F_E, sizeof(double));
		stream.write((char *)&F_Gmax, sizeof(double));
		stream.write((char *)&F_Gnoise, sizeof(double));
		stream << _m;
		stream << _h;
		stream << _n;
		TCurrent::WriteToStream(stream); //Write base class members
	}

	/// Reads data members from a stream
	void THHCurrent::ReadFromStream(std::istream &stream)
	{
		stream.read((char *)&F_p, sizeof(double));
		stream.read((char *)&F_q, sizeof(double));
		stream.read((char *)&F_E, sizeof(double));
		stream.read((char *)&F_Gmax, sizeof(double));
		stream.read((char *)&F_Gnoise, sizeof(double));
		stream >> _m;
		stream >> _h;
		stream >> _n;
		TCurrent::ReadFromStream(stream); //Read base class members
	}

	// implement pure virtual
	bool               THHCurrent::Initialize(bool Reset)
	{
		if (Reset) return Restart( -60 ) ; /* TODO : Need to find way to choose initial voltage for HHCurrent */
		return true;
	}

	/// Initializes kinetic parameters m, h, and n
	bool               THHCurrent::Restart( double V )
	{
		get_m().Restart(V);
		get_h().Restart(V);
		get_n().Restart(V);
		srand(time(NULL));
		idum = -1 * rand();
		return true;
	}

	// implement pure virtual
	#define THHCurrent_KEY L"HH Current"
	const std::wstring & THHCurrent::ClassKey() const
	{
		static std::wstring Key(THHCurrent_KEY);
		return Key;
	}

	// override for parameter logging
	const void __fastcall			THHCurrent::GetParamLogHeader(
												std::vector <std::string> & params) const
	{
		params.clear();

		// each column header of the form "OWNER_CURRENT_VAR"
		if (p() != 0) {
			params.push_back("_m");
		}
		if (q() != 0) {
			params.push_back("_h");
		}
		if (r() != 0) {
			params.push_back("_n");
		};

		params.push_back("_G");
	}

/// Calculation of HH type current based on two voltages
/*!
<pre>
*                                    p         q         r
*         computes I = Gmax * m(Vkin) * h(Vkin) * n(Vkin) * (Vdrv - E)
</pre>
*/
	double 				THHCurrent::DoUpdate(double step, double Vkin,
																		double Vdrv,
																  std::vector <double> & params)
	{
		// step is ms, V's are mV, G is in microsiemens, I is in nA
		//                      p   q   r
		// computes I = Gmax * m * h * n * (V - E)
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
			if (M >= 0) {     // ebt 9/18/2013 -- to stop domain errors in pow
				mp = pow(M,p());
			}
			params.push_back(M);
		}
		if (q() > 0) {
			H = get_h().Update(Vkin, step);
			if (H >= 0) {     // ebt 9/18/2013 -- to stop domain errors in pow
				hq = pow(H,q());
			}
			params.push_back(H);
		}
		if (r() > 0) {
			N = get_n().Update(Vkin, step);
			if (N >= 0) {    // ebt 9/18/2013 -- to stop domain errors in pow
				nr = pow(N,r());
			}
			params.push_back(N);
		}

		if (F_Gnoise == 0.0){
			G = F_Gmax*mp*hq*nr;
		}
		else {
			double noise = 0.01 * F_Gnoise * F_Gmax * gasdev(&idum);
			G = (F_Gmax + noise)*mp*hq*nr;
		}

		I = G * (Vdrv - F_E);       // uS * mV = nA

		params.push_back(G);

		return I;
	}

	/// Called by GUI to synchronize edit form with current values of object params
#ifndef NO_VCL
	void       			THHCurrent::PopulateEditForm()
	{
		THHCurrentForm *form = (THHCurrentForm * )GetEditForm();


		ptext =     L"p (exponent)";
		qtext =     L"q (exponent)";
		rtext =     L"r (exponent)";

		get_m().PopulateParams(form->ValueListEditor_m);
		form->ValueListEditor_m->Values[ptext] = FloatToStrF(p(), ffFixed, 5, 5);
		form->ValueListEditor_m->Hint = get_m().HelpText;

		get_h().PopulateParams(form->ValueListEditor_h);
		form->ValueListEditor_h->Values[qtext] = FloatToStrF(q(), ffFixed, 5, 5);
		form->ValueListEditor_h->Hint = get_h().HelpText;

		get_n().PopulateParams(form->ValueListEditor_n);
		form->ValueListEditor_n->Values[rtext] = FloatToStrF(r(), ffFixed, 5, 5);
		form->ValueListEditor_n->Hint = get_n().HelpText;


		form->GmaxEdit->Text = FloatToStr(F_Gmax);
		form->GnoiseEdit->Text = FloatToStr(F_Gnoise);
		form->ErevEdit->Text = FloatToStr(F_E);

		form->UpdatePlots();

		// kinetic parameters logging
		form->ParamLoggingCheckBox->Checked = IsParamLoggingEnabled();

		// period settings form
		PeriodTxt =    L"Period (ms)";
		DutyCycleTxt = L"Duty Cycle (0..1)";
		InitDelayTxt = L"Initial Delay (ms)";
		TrianglePhaseTxt = L"Triangle Phase (0..1)";
		form->PeriodicCheckBox->Checked = IsPeriodic();
		TPeriodicityForm *periodform = form->CurrentPeriodForm;
		periodform->WaveformTypeComboBox->Clear();
		periodform->WaveformTypeComboBox->AddItem(L"Pulse", NULL);
		periodform->WaveformTypeComboBox->AddItem(L"Sine (Duty cycle ignored)", NULL);
		periodform->WaveformTypeComboBox->AddItem(L"Triangle", NULL);
		periodform->WaveformTypeComboBox->ItemIndex = GetWaveType();
		periodform->ValueListEditor1->Values[InitDelayTxt] = GetInitialDelay();
		periodform->ValueListEditor1->Values[PeriodTxt] = GetPeriod();
		periodform->ValueListEditor1->Values[DutyCycleTxt] = GetDutyCycle();
		periodform->ValueListEditor1->Values[TrianglePhaseTxt] = GetTrianglePhase();
	}

	/// Called by GUI to check if changed values are satisfactory
	bool       			THHCurrent::ValidateEditForm()
   {

		THHCurrentForm *form = (THHCurrentForm * )GetEditForm();
      bool ok = true;

	  ok = get_m().KineticFactorsValidate(get_m(), L"m", form->ValueListEditor_m, p(), L"p");
		if (ok == false) {
			return ok;
		}
		ok = get_h().KineticFactorsValidate(get_h(), L"h", form->ValueListEditor_h, q(), L"q");
		if (ok == false) {
			return ok;
		}
		ok = get_n().KineticFactorsValidate(get_n(), L"n", form->ValueListEditor_n, r(), L"r");
      if (ok == false) {
         return ok;
      }

      double temp;
      try {
         temp = StrToFloat(form->GmaxEdit->Text);
//         if (temp < 0.0){
//            Application->MessageBox(L"Gmax conductance must be greater than or equal to zero",
//                                    L"Please correct input");
//            ok = false;
//         } else {
//            F_Gmax = temp;
//         }

         // for subtracting conductance, Gmax has to be negative
         F_Gmax = temp;

         temp = StrToFloat(form->ErevEdit->Text);
         F_E = temp;

         temp = StrToFloat(form->GnoiseEdit->Text);
         F_Gnoise = temp;

      } catch (EConvertError & e) {
         ok = false;
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      }

      if (!ok) {
         return ok;
      }

      // kinetic parameters logging
      SetParamLoggingEnabled(form->ParamLoggingCheckBox->Checked);

      TPeriodicityForm *periodform = form->CurrentPeriodForm;
      TValueListEditor *editor = periodform->ValueListEditor1;
      UnicodeString message;
      try {

         // Initial Delay
         temp = StrToFloat(editor->Values[InitDelayTxt]);
         if (temp < 0.0) {
            message = L"Initial Delay must be greater than or equal to zero ms";
            ok = false;
         } else {
            SetInitialDelay(temp);
         }

			// Period
         temp = StrToFloat(editor->Values[PeriodTxt]);
         if (temp < 0.0) {
            message = L"Period must be greater than or equal to zero ms";
            ok = false;
         } else {
            SetPeriod(temp);
         }

			// Duty Cycle
			temp = StrToFloat(editor->Values[DutyCycleTxt]);
			if (temp < 0.001 || temp > 0.999) {
				message = L"Duty Cycle  must be between 0.001 and 0.999";
				ok = false;
			} else {
				SetDutyCycle(temp);
			}

			// Triangle Phase
			temp = StrToFloat(editor->Values[TrianglePhaseTxt]);
			if (temp < 0.001 || temp > 0.999) {
				message = L"Triangle Phase  must be between 0.001 and 0.999";
				ok = false;
			} else {
				SetTrianglePhase(temp);
			}

         SetWaveType(periodform->WaveformTypeComboBox->ItemIndex);

         SetPeriodic(form->PeriodicCheckBox->Checked);

      } catch (EConvertError & e) {
         ok = false;
         Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      }

      if (!ok) {
         Application->MessageBox(message.c_str(), L"Please correct input");
      }
      return ok;
   }
#endif

   /// default constructor
	THHCurrent::THHCurrent():
         TCurrent(NULL, L""), F_p(0), F_q(0), F_r(0), F_E(0), F_Gmax(5), F_Gnoise(0)
   {
	}

   /// specialized constructor 2 param
	THHCurrent::THHCurrent(TCurrentUser *owner, const std::wstring & name):
         TCurrent(owner, name), F_p(0), F_q(0), F_r(0), F_E(0), F_Gmax(5), F_Gnoise(0)
	{
   }

   /// specialized constructor 1 param
	THHCurrent::THHCurrent(const std::wstring & name):
			TCurrent(NULL, name), F_p(0), F_q(0), F_r(0), F_E(0), F_Gmax(5), F_Gnoise(0)
   {
   }

   /// copy constructor
	THHCurrent::THHCurrent( const THHCurrent & source ) :
         TCurrent(source.Owner(), source.Name()),
         F_p(source.F_p),
         F_q(source.F_q),
         F_r(source.F_r),
			F_E(source.F_E),
         F_Gmax(source.F_Gmax),
         F_Gnoise(source.F_Gnoise),
			_m(source._m),
			_h(source._h),
			_n(source._n)
   {
   }

   /// overloaded assignment operator
	THHCurrent & THHCurrent::operator = (const THHCurrent & source)
   {
      if (this != &source) {  // make sure not same object
         this->TCurrent::operator=(source); //copy base class members
         // now copy THHCurrent fields
			F_p = (source.F_p);
         F_q = (source.F_q);
         F_r = (source.F_r);
         F_E = (source.F_E);
         F_Gmax = (source.F_Gmax);
			F_Gnoise = (source.F_Gnoise);
			_m = (source._m);
			_h = (source._h);
			_n = (source._n);
      }
		return *this;
   }

   /// overloaded method for duplicating currents without complete assignment
	void THHCurrent::CopyParamsFrom(const TCurrent * const source )
	{
      if (this != source) {  // make sure not same object
//         std::wstring this_type(this->ClassType());
//         std::wstring source_type(source->ClassType());
//         if (this->ClassType() == source->ClassType()) { // make sure same type
//         if (this_type == source_type) {
            THHCurrent *cast_source = (THHCurrent *)source;

            F_p = (cast_source->F_p);
            F_q = (cast_source->F_q);
				F_r = (cast_source->F_r);
				F_E = (cast_source->F_E);
            F_Gmax = (cast_source->F_Gmax);
            F_Gnoise = (cast_source->F_Gnoise);
				_m = (cast_source->_m);
				_h = (cast_source->_h);
				_n = (cast_source->_n);
//         }
		}
	}


#ifdef SERIALIZE
BOOST_CLASS_EXPORT(THHCurrent)
#endif


////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
	static struct regHHCurrent {
		regHHCurrent(){
         GetCurrentFactory().registerBuilder(
											THHCurrent_KEY, //L"HH Current",
                                 TypeID<THHCurrent>(),
											TypeID<TCurrentUser*const>(),
											TypeID<const std::wstring>());
      }
	} RegHHCurrent;
}

#pragma end_region

//---------------------------------------------------------------------------
#pragma package(smart_init)
