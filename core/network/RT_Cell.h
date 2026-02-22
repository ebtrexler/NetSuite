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

#ifndef RT_CellH
#define RT_CellH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "RT_Base.h"
#include "RT_CurrentUser.h"
#include "RT_Current.h"
#include "RT_Electrode.h"
#include "RT_Synapse.h"

//-----------------------------------------------------------------------
//
//
/// Base class for biological and model and other specialized cells
/*!
TCell owns one array each of Currents, Electrodes, and Synapses.
 TCell's DoUpdate(double step) member function calls the Update functions
  of its Currents, Electrodes, and Synapses in a loop, summing each
  contribution to calculate the total current and storing the value.
 TCell has a virtual member function--> bool CalcVm() that will allow derived
  cells such as TModelCell and TPlaybackCell to update F_Vm independently
  of data acquisition.  In addition, CalcVm() can be used to compare against
  the sampled Vm from biological cells to find a best fit for the membrane
  capacitance of model cells.

  @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/

class TCell : public TCurrentUser
{

private:
   /// vector of ionic currents
  	TCurrentsArray						FCurrents;
   /// vector of electrode currents
   TElectrodesArray              FElectrodes;
   /// vector of synaptic currents
	TSynapsesArray						FSynapses;

protected:
   // these 4 members are not used by some cells, but need to be here in TCell
   std::wstring                  FAIChannel;
   std::wstring                  FAOChannel;
   double                        FAIGain; // in mV / V
   double                        FAOGain; // in V / nA

   double 								F_Vm;   // The membrane voltage (in mV)
   double                        F_I;    // The updated current (in nA)

   /// Writes data members to a stream
   virtual void WriteToStream(std::ostream &stream) const;
   /// Reads data members from a stream
   virtual void ReadFromStream(std::istream &stream);

public:
   /// Returns the Analog Input Channel for this cell, if assigned
   virtual std::wstring __fastcall AIChannel();
   /// Returns the Analog Output Channel for this cell, if assigned
   virtual std::wstring __fastcall AOChannel();
   /// Returns the Analog Input Gain for this cell, if assigned
   virtual double __fastcall     AIGain();
   /// Returns the Analog Output Gain for this cell, if assigned
   virtual double __fastcall     AOGain();

   // DAQ channel/gain setters
   void SetAIChannel(const std::wstring &ch) { FAIChannel = ch; }
   void SetAOChannel(const std::wstring &ch) { FAOChannel = ch; }
   void SetAIGain(double g) { FAIGain = g; }
   void SetAOGain(double g) { FAOGain = g; }

   /// Returns the current value of the calculated current
   virtual double __fastcall     I();
   /// Returns the stored value of the membrane potential
	virtual double __fastcall     Vm();
   /// Sets membrane voltage -- must be overriden
   virtual double __fastcall     SetVm(double Vm) = 0;
   /// Calculates membrane voltage -- must be overridden
   virtual double __fastcall     CalcVm(double step) = 0;
   /// Used to classify cell as biological (true) or other (false)
   virtual bool __fastcall       IsVoltageDependent() = 0;

   /// Worker function that updates the current -- no need to override
   virtual double __fastcall     DoUpdate(double step);

   /// Checks if Active, and if so, calls DoUpdate to calculate the new current.
   /*! Before calling Update, caller must SetVm() or CalcVm().
   */
 	double __fastcall		Update(double step);


   // add and remove currents
   // override pure virtual methods in TCurrentUser
   /// Adds a TCurrent * to the array of Currents
   virtual const TCurrent * __fastcall AddCurrent(TCurrent * const c, TCell * cell = NULL);
   /// Removes a TCurrent * from the array of Currents
   virtual void __fastcall       RemoveCurrent(TCurrent * c);
   /// Returns the array of Currents
   virtual const TCurrentsArray __fastcall GetCurrents() const;
	/// Deletes all currents from the cell
	void __fastcall ClearCurrents();
	/// Informs caller whether can accept currents or not
	virtual const bool __fastcall AcceptsCurrents() const = 0;

   // add remove synapses
   /// Adds a TSynapse * to the array of Synapses
   virtual const TSynapse * __fastcall  AddSynapse(TSynapse * s);
   /// Removes a TSynapse * from the array of Synapses
   virtual void __fastcall   RemoveSynapse(TSynapse * s);
   /// Returns the array of Synapses
   virtual const TSynapsesArray __fastcall GetSynapses() const;

   // add remove electrodes
   /// Adds a TElectrode * to the array of Electrodes
   virtual const TElectrode *__fastcall AddElectrode(TElectrode * e);
   /// Removes a TElectrode * from the array of Electrodes
   virtual void  __fastcall    RemoveElectrode(TElectrode * e);
   /// Returns the array of Electrodes
   virtual const TElectrodesArray __fastcall GetElectrodes() const;


   /// Default constructor
   __fastcall TCell();
   /// Specialized constructor
	__fastcall TCell(const std::wstring & name);
   /// Copy constructor
   __fastcall TCell(const TCell &source);
   /// Overloaded assignment operator
   TCell& operator = (const TCell & source);

	virtual ~TCell();
};

//---------------------------------------------------------------------------
#endif
