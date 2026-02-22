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

#ifndef RT_CurrentH
#define RT_CurrentH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "RT_Base.h"
#include "RT_CurrentUser.h"

#define wtNone    -1
#define wtPulse   0
#define wtSine    1
#define wtTriangle 2
#define DEFDUTY   0.5
#define DEFPERIOD 1000
#define DEFINITDEL 0.0
#define DEFTRIPHASE 0.5

// used for kinetic parameter logging

#include "datalogger.h"

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//   *****************    TCurrent   ************************
//
//
/// Base class for all intrinsic and synaptic currents
/*!
TCurrent is the polymorphic workhorse.  It has a virtual member function,
  TCurrent::DoUpdate(step, Vkin, Vdrv), that allows two voltages to determine
  the current response.  Vkin governs kinetics/conductance, whereas Vdrv
  governs the driving force.  TCurrent::DoUpdate is called by the nonvirtual
  TCurrent::Update, which first verifies that the current is active, then
  multiplies the current obtained from DoUpdate by a periodically changing gain.

  The two voltage parameters allow for the same class to serve as both an
  intrinsic cellular current or a synaptic current.  For example, when a synapse
  updates its currents, it might call Update(step, Vpre, Vpost), where the Vkin
  paramter is set by the presynaptic cell's membrane potential (Vpre) and the
  Vdrv paramter is set by the postsynaptic cell's membrane potential (Vpost).
  Intrinsic cellular currents might call Update(step, Vm,   Vm), with the cell's
  membrane potential passed for both parameters.

  <STRONG>
  --Derived classes must override DoUpdate(step, Vkin, Vdrv) and return the
	 new current value based on the passed parameters.
  --Derived classes must handle parameter logging in DoWriteParamLoggingHeader()
	 and DoUpdate().  TCurrent handles writing G (conductance) and I (current).
  </STRONG>

  @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2014
*/

class TCurrent : public TRTBase
{

private:
	TCurrentUser                  *FOwner;  // Field for owner

	////////////////////////////////////////////
	// added 2013-01-02
	bool									FIsPeriodic;
	double								FPeriod;
	int									FWaveType;
	double								FDutyCycle;
	double								FInitialDelay;

	double								FElapsedTime;
	////////////////////////////////////////////

	////////////////////////////////////////////
	// added 2014-05-12
	double								FTrianglePhase;
	////////////////////////////////////////////

	// added 2013-02-21
	// Field holding ParamLoggingEnabled property --
	//    if yes, then DoUpdate can save vars to stream
	bool                          FParamLoggingEnabled;

protected:

	/// Determines a gain from 0 to 1 dependent upon elapsed time and period params.
	/*!
		\param step = milliseconds passed since last call
	*/
	virtual double __fastcall     PeriodGain(double step);

	/// Override in derived classes to implement calculations.
	/*!
		\param step = milliseconds passed since last call
		\param Vkin = voltage governing kinetics of conductance
		\param Vdrv = voltage governing ionic flow through conductance
		\param params = vector of calculated parameters for logging
	*/
	virtual double __fastcall     DoUpdate(double step,
														double Vkin,
														double Vdrv,
														std::vector < double > & params ) = 0;

	/// Supplies column names for parameter logging
	/*!
		\param params = vector of parameter names for logging
		Override in derived classes to add their parameters to the header
	*/
	const virtual void __fastcall	GetParamLogHeader(
												std::vector<std::string>& params) const = 0;

public:
	/////////////// Property set - get methods

	/// Gets if current is modulated according to set periodicity params
	const bool __fastcall         IsPeriodic() const;
	/// Sets if current is modulated according to set periodicity params
	void __fastcall               SetPeriodic(bool isperiodic);

	/// Gets the period in ms
	const double __fastcall      	GetPeriod() const;
	/// Sets the period in ms
	void __fastcall               SetPeriod(double period);

	/// Gets the period type -- square or sine
	const int __fastcall         	GetWaveType() const;
	/// Sets the period type -- square or sine
	void __fastcall               SetWaveType(int wavetype);

	/// Gets the duty cycle
	const double __fastcall      	GetDutyCycle() const;
	/// Sets the duty cycle
	void __fastcall               SetDutyCycle(double dutycycle);

	/// Sets the initial delay before waveform starts
	const double __fastcall       GetInitialDelay() const;
	/// Gets the initial delay before waveform starts
	void __fastcall               SetInitialDelay(double initdelay);

	/// Sets the midpoint of the triangle wave
	const double __fastcall       GetTrianglePhase() const;
	/// Gets the initial delay before waveform starts
	void __fastcall               SetTrianglePhase(double phase);

   /// Determines if parameter logging to a stream is enabled
   const bool __fastcall         IsParamLoggingEnabled() const;
   /// Determines if parameter logging to a stream is enabled
   void __fastcall               SetParamLoggingEnabled(bool enabled);

	/// Writes tab-delimited column names for paramter logging to supplied stream
	/*!
		This method writes G (conductance) and I (current) to a tab delimited log,
		then calls GetParamLogHeader(std::vector <std::string>) from derived class.
	*/
	void __fastcall       WriteParamLoggingHeader(TDataLogger &log) const;

	/// Sets ElapsedTime to zero. Must be called before first Update.
	/*!
		\param reset = flag for derived classes
	*/
	virtual bool __fastcall     	Initialize(bool reset);

	/// Returns the time since last call to Initialize
	double __fastcall       		ElapsedTime() const;

	/// Returns pointer to the TSynapse or TCell (TCurrentUser) that owns this current.
	TCurrentUser * const          Owner() const;

	/// Checks if Active, and calls DoUpdate accordingly
	double __fastcall             Update(double step, double Vkin, double Vdrv);

	/// Default constructor
   __fastcall TCurrent();

   /// Specialized constructor 1
   /*! TCurrentUser *owner is the parent (synapse or cell) */
   __fastcall TCurrent(TCurrentUser * const owner);

   /// Specialized constructor 2
   /*! TCurrentUser *owner is the parent (synapse or cell) */
	__fastcall TCurrent(TCurrentUser * const owner, const std::wstring & name);

   /// copy constructor
   __fastcall TCurrent(const TCurrent & source);

   /// overloaded assignment operator
	TCurrent & operator = (const TCurrent & source );

	/// Writes data members to a stream
	virtual void WriteToStream(std::ostream &stream) const;

	/// Reads data members from a stream
	virtual void ReadFromStream(std::istream &stream);

	/// Override in derived classes to allow copying currents between cells
	/*!
      This method is like the assignment operator, but TRTBase::Name and
      TCell::Owner are not copied.
   */
   virtual void __fastcall CopyParamsFrom(const TCurrent * const source ) = 0;

	virtual ~TCurrent();
};


//---------------------------------------------------------------------------
#endif
