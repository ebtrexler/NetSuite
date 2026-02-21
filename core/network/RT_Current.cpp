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


#pragma hdrstop

#include "RT_Current.h"
#include "RT_Network.h"

// Default constructor
__fastcall TCurrent::TCurrent() : FOwner(NULL) {}

// Specialized constructor 1
// TCurrentUser *owner is the parent (synapse or cell)
__fastcall TCurrent::TCurrent(TCurrentUser * const owner) :
					TRTBase(L"UnNamed", true), FOwner(owner),
					FIsPeriodic(false), FPeriod(DEFPERIOD),  FWaveType(wtNone),
					FDutyCycle(DEFDUTY), FInitialDelay(DEFINITDEL),
					FTrianglePhase(DEFTRIPHASE), FElapsedTime(0.0),
               FParamLoggingEnabled(false)
{
}

// Specialized constructor 2
// TCurrentUser *owner is the parent (synapse or cell)
__fastcall TCurrent::TCurrent(TCurrentUser * const owner, const std::wstring & name) :
	TRTBase(name, true), FOwner(owner),	FIsPeriodic(false), FPeriod(DEFPERIOD),
	FWaveType(wtNone), FDutyCycle(DEFDUTY), FInitialDelay(DEFINITDEL),
	FTrianglePhase(DEFTRIPHASE), FElapsedTime(0.0),
					FParamLoggingEnabled(false)
{
}

// copy constructor
__fastcall TCurrent::TCurrent(const TCurrent & source) :
	TRTBase(source.Name(), source.IsActive()), FOwner(source.FOwner),
	FIsPeriodic(source.FIsPeriodic), FPeriod(source.FPeriod),
	FWaveType(source.FWaveType), FDutyCycle(source.FDutyCycle),
	FInitialDelay(source.FInitialDelay), FTrianglePhase(source.FTrianglePhase),
	FElapsedTime(source.FElapsedTime),
   FParamLoggingEnabled(source.FParamLoggingEnabled)
{
}

// overloaded assignment operator
TCurrent & TCurrent::operator = (const TCurrent & source )
{
   if (this != &source) {  // make sure not same object
      this->TRTBase::operator=(source); //copy base class members
      // now copy TCurrent fields
		FOwner = source.FOwner;
		FIsPeriodic = (source.FIsPeriodic);
		FPeriod = (source.FPeriod);
		FWaveType = (source.FWaveType);
		FDutyCycle = (source.FDutyCycle);
		FInitialDelay = (source.FInitialDelay);
		FTrianglePhase = (source.FTrianglePhase);
		FElapsedTime = (source.FElapsedTime);
      FParamLoggingEnabled = (source.FParamLoggingEnabled);
   }
   return *this;
}

// Writes data members to a stream
void TCurrent::WriteToStream(std::ostream &stream) const
{
	stream.write((char *)&FIsPeriodic, sizeof(bool));
	stream.write((char *)&FPeriod, sizeof(double));
	stream.write((char *)&FWaveType, sizeof(int));
	stream.write((char *)&FDutyCycle, sizeof(double));
	stream.write((char *)&FInitialDelay, sizeof(double));
	stream.write((char *)&FTrianglePhase, sizeof(double));
   stream.write((char *)&FParamLoggingEnabled, sizeof(bool));
}

// Reads data members from a stream
void TCurrent::ReadFromStream(std::istream &stream)
{
	stream.read((char *)&FIsPeriodic, sizeof(bool));
	stream.read((char *)&FPeriod, sizeof(double));
	stream.read((char *)&FWaveType, sizeof(int));
	stream.read((char *)&FDutyCycle, sizeof(double));
	stream.read((char *)&FInitialDelay, sizeof(double));
	stream.read((char *)&FTrianglePhase, sizeof(double));
   stream.read((char *)&FParamLoggingEnabled, sizeof(bool));
}

TCurrent::~TCurrent()
{
}


TCurrentUser * const    TCurrent::Owner() const
{
   return FOwner;
}

double __fastcall 		TCurrent::Update(double step, double Vkin, double Vdrv)
{
   if (!IsActive()) {
		 return 0;
	}


	////////////////////////////////////
	// added 2013-01-02   -- periodicity
	double pergain = 1.0;

	// updated 2014-5-15 for improved parameter logging
	std::vector < double > params; // holds calculated parameters from derived class
	double I = DoUpdate(step, Vkin, Vdrv, params);

	if (FIsPeriodic) {
		pergain = PeriodGain(step);
		I = I * pergain;
	}
	////////////////////////////////////

	////////////////////////////////////
	// added 2014-5-15 for improved parameter logging
	if (GetNet()->IsParamLoggingEnabled()) {  // Global network logging set?
		if (IsParamLoggingEnabled()) {         // Individual current logging set?
			TDataLogger &log = GetNet()->GetParamLogger();

			//log calculated current
			log << I << "\t";

			// log parameters from derived class
			int numparams = params.size();
			for (int i = 0; i < numparams; i++) {
				log << params[i] * pergain << "\t";
			}
		}
	}
	return I;
}

double __fastcall     	TCurrent::PeriodGain(double step)
{

	// step is ms, gain is 0 or 1
	double gain = 0.0;
	FElapsedTime += step;
	double et = FElapsedTime - FInitialDelay;
	double elapsed = fmod(et, FPeriod);
	if (et > 0.0) {  // past Initial Delay
		switch (FWaveType){
			case wtPulse: {
				double duration = FDutyCycle * FPeriod;
				if (elapsed <= duration ) {     // in the pulse duration
					gain = 1.0;
				}
			} break;
			case wtSine: {
				gain = (1.0 + sin(2.0*3.14*(elapsed/FPeriod))) / 2.0;
			} break;
			case wtTriangle: {
				double duration = FDutyCycle * FPeriod;
				double risetime = duration * FTrianglePhase;
				double falltime = duration * (1.0 - FTrianglePhase);
				if (elapsed <= risetime ) {     // in the rising phase
					gain = elapsed / risetime;
				}
				else if (elapsed <= duration) { // in the falling phase
					double time_falling = elapsed - risetime;
					gain = 1.0 - (time_falling/falltime);
				}
			} break;
         default: gain = 1.0;
      }
	}
	return gain;

}


const bool __fastcall   TCurrent::IsParamLoggingEnabled() const
{
   return FParamLoggingEnabled;
}

void __fastcall         TCurrent::SetParamLoggingEnabled(bool enabled)
{
   FParamLoggingEnabled = enabled;
}

// Writes tab-delimited column names for paramter logging to supplied stream
void __fastcall         TCurrent::WriteParamLoggingHeader(TDataLogger &log) const
{
	if (!IsActive()) {
		return;
	}
	if (!GetNet()->IsParamLoggingEnabled()) {
		return;
	}
	if (!IsParamLoggingEnabled()) {
		return;
	}

	std::wstring ownerWName = Owner()->Name();
	std::string theOwnerName(ownerWName.begin(), ownerWName.end());
	const char * OwnerName(theOwnerName.c_str());
	std::wstring currentWName = Name();
	std::string theCurrentName(currentWName.begin(), currentWName.end());
	const char * CurrentName(theCurrentName.c_str());

	// each column header of the form "OWNER_CURRENT_VAR"
	log << OwnerName << "_" << CurrentName << "_I (nA)" << "\t";

	// call derived class' GetParamLogHeader for names of paramters & write them
	std::vector <std::string> params;
	GetParamLogHeader(params);
	int numparams = params.size();
	for (int i = 0; i < numparams; i++) {
		log << OwnerName << "_" << CurrentName << params[i].c_str() << "\t";	
	}

	return;
}


bool __fastcall     		TCurrent::Initialize(bool reset)
{
	// regardless of Reset, FElapsedTime must be set to zero
	FElapsedTime = 0;
	return true;
}

double __fastcall       TCurrent::ElapsedTime() const
{
   return FElapsedTime;
}

// Gets if current is modulated according to set periodicity params
const bool __fastcall  	TCurrent::IsPeriodic() const
{
	return FIsPeriodic;
}
// Sets if current is modulated according to set periodicity params
void __fastcall         TCurrent::SetPeriodic(bool isperiodic)
{
	FIsPeriodic = isperiodic;
}

// Gets the period in ms
const double __fastcall TCurrent::GetPeriod() const
{
	return FPeriod;
}
// Sets the period in ms
void __fastcall         TCurrent::SetPeriod(double period)
{
	FPeriod = period;
}

// Gets the period type -- square or sine
const int __fastcall    TCurrent::GetWaveType() const
{
	return FWaveType;
}
// Sets the period type -- square or sine
void __fastcall         TCurrent::SetWaveType(int wavetype)
{
	FWaveType = wavetype;
}

// Gets the duty cycle
const double __fastcall TCurrent::GetDutyCycle() const
{
	return FDutyCycle;
}
// Sets the duty cycle
void __fastcall         TCurrent::SetDutyCycle(double dutycycle)
{
	FDutyCycle = 0.0;
	if (dutycycle > 0.0 && dutycycle <= 1.0)
		FDutyCycle = dutycycle;
}

// Sets the midpoint of the triangle wave
const double __fastcall TCurrent::GetTrianglePhase() const
{	
	return FTrianglePhase;
}
// Gets the initial delay before waveform starts
void __fastcall         TCurrent::SetTrianglePhase(double phase)
{
	FTrianglePhase = 0.0;
	if (phase > 0.0 && phase <= 1.0) {
		FTrianglePhase = phase;   
	}
}

// Sets the initial delay before waveform starts
const double __fastcall TCurrent::GetInitialDelay() const
{
	return FInitialDelay;
}
// Gets the initial delay before waveform starts
void __fastcall         TCurrent::SetInitialDelay(double initdelay)
{
	FInitialDelay = 0.0;
	if (initdelay >= 0.0)
		FInitialDelay = initdelay;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
