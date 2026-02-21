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

#ifndef RT_NetworkH
#define RT_NetworkH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include <stdexcept>
#include "RT_Base.h"
#include "RT_CurrentUser.h"
#include "RT_Current.h"
#include "RT_Electrode.h"
#include "RT_Synapse.h"
#include "RT_Cell.h"


/// used for communication with GUI and DAQ sections
struct NetDescriptionStruct
   {
      std::wstring AIChans;
      std::wstring AOChans;
      int NumVDepCells;
      int NumTimeCells;
      NetDescriptionStruct(): NumVDepCells(0), NumTimeCells(0) {}
   };

//---------------------------------------------------------------------------
//
//

/// Complete class that handles network creation, editing and running of simulations
/*!
The TNetwork class handles creation of objects.  The relationships between
objects are set in their constructors.  Call the TNetwork::Update function
as it handles updating of child objects.

TNetwork provides methods for accessing all children by their Name member.
User interfaces should make use of this search ability.

There should be no need to derive from this class.  It is self contained and ready to go.

@author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TNetwork  : public TRTBase
{

private:
   // map of cells in network
   TCellsMap                        FCells;
   // map of ionic currents
   TCurrentsMap  						   FCurrents;
   // map of electrode currents
   TElectrodesMap                   FElectrodes;
   // map of synaptic currents
	TSynapsesMap						   FSynapses;

   NetDescriptionStruct             FNetDescription;
   TCell *                          FVmDepCells[10000]; // space for 100x100 network
   TCell *                          FTimeCells[10000]; // space for 100x100 network

// Execution variables/parameters/flags
   // Runge-Kutta time step
   double                           FMaxRK4Timestep;
   // Flag for logging variables to stream
   bool                             FParamLoggingEnabled;
   // Output stream for logging variables
   TDataLogger                      FParamLogger;
   // Keeps track of total time elapsed
   double                           FElapsedTime; // in ms


// Currents functions
   virtual TCurrentPtr __fastcall   AddCurrentToMap(TCurrentPtr c);
   virtual void __fastcall          RemoveCurrentFromMap(const std::wstring & name);

// Cells functions
   TCellPtr __fastcall              AddCellToMap(TCellPtr c);
   void __fastcall                  RemoveCellFromMap(const std::wstring & name);

// Synapses functions
   TSynapsePtr __fastcall           AddSynapseToMap(TSynapsePtr s);
   void __fastcall                  RemoveSynapseFromMap(const std::wstring & name);

// Electrodes functions
   TElectrodePtr __fastcall         AddElectrodeToMap(TElectrodePtr e);
   void __fastcall                  RemoveElectrodeFromMap(const std::wstring & name);

// override from TRTBase, and hide in private
#ifndef NO_VCL
   /// Throws exception if called, because no separate edit form exists for this type
   virtual void* const GetEditForm()
   {
      throw std::runtime_error("No Edit Form");
      return NULL;
   }
   /// Throws exception if called, because no separate edit form exists for this type
   virtual void PopulateEditForm()
   {
      throw std::runtime_error("No Edit Form");
   }
   /// Throws exception if called, because no separate edit form exists for this type
   virtual bool ValidateEditForm()
   {
      throw std::runtime_error("No Edit Form");
      return false;
   }
#endif // NO_VCL
   /// Throws exception because stream operators can't handle graphs
   virtual void WriteToStream(std::ostream &stream) const
   {
      throw std::runtime_error("Use JSON save/load for TNetwork");
   }
   /// Throws exception because stream operators can't handle graphs
   virtual void ReadFromStream(std::istream &stream)
   {
      throw std::runtime_error("Use JSON save/load for TNetwork");
   }
protected:

public:
// Access to maps of cells, synapses, and electrodes
   /// Returns reference to Cells map
   virtual const TCellsMap & __fastcall      GetCells();
   /// Returns reference to Synapses map
   virtual const TSynapsesMap & __fastcall   GetSynapses();
   /// Returns reference to Electrodes map
   virtual const TElectrodesMap & __fastcall GetElectrodes();

// Access to xecution variables/parameters/flags
   /// Runge-Kutta time step getter method
   virtual const double __fastcall           GetMaxRK4Timestep() const;
   /// Runge-Kutta time step setter method
   virtual void __fastcall                   SetMaxRK4Timestep(double rk4step);

   virtual const double __fastcall           ElapsedTime() const;

   /// Determines if parameter logging to a stream is enabled
   const bool __fastcall         IsParamLoggingEnabled() const;
   /// Determines if parameter logging to a stream is enabled
   void __fastcall               SetParamLoggingEnabled(bool enabled);

   /// Writes tab-delimited column names for paramter logging
   /*!
      Loops through cells and synapses in same order as Update.  Creates stream
      that is used by currents for logging their parameters
   */
   virtual void __fastcall       WriteParamLoggingHeader();

   /// Writes headers for intrinsic and synaptic currents for each cell
   virtual void __fastcall       WriteHeaderForCellCurrents(TCell *cell);

   /// Output stream reference for logging variables getter method
   virtual TDataLogger & __fastcall
                                 GetParamLogger();

   /// Terminate logging
   virtual void __fastcall       CloseLoggingStream(const wchar_t * filename);


// Network Access Methods -- text based

// cell access
   /// Adds cell to network
   /*!
      \param CellType is the C++ class type to add, derived from TCell
      \param CellName is the unique name of the cell
      \param X is the distance of GUI representation from the left
      \param Y is the distance of GUI representation from the top
   */
   virtual TCell * __fastcall
   AddCellToNet          ( const std::wstring & CellType,
                           const std::wstring & CellName,
                           int X = 0, int Y = 0);

   /// Removes cell from network
   /*!
      \param CellName is the unique name of the cell
   */
   virtual void __fastcall
   RemoveCellFromNet     ( const std::wstring & CellName);

   /// Determines whether cell is included in calculations
   /*!
      \param CellName is the unique name of the cell
      \param Active is true if cell is included, false otherwise
   */
   virtual void __fastcall
   SetCellActiveState    ( const std::wstring & CellName,
                           bool Active);

   /// Rename the cell
   /*!
      \param oldname is the old unique name of the cell
      \param newname is the new unique name of the cell
   */
   virtual void __fastcall
   RenameCell            ( const std::wstring & oldname,
                           const std::wstring & newname);

   /// Copy currents from one cell to one or more other cells
   /*!
		\param fromname is the cell whose currents will be copied
		\param tonames is the vector of target cell names
		\param ClearAndReplace determines if currents in target cells are deleted and replaced
	*/
   virtual void __fastcall
   CopyCurrentsFromCell  ( const std::wstring & fromname,
									const std::vector<std::wstring> tonames,
									bool ClearAndReplace);

   // synapse access
   /// Adds synapse to network between two cells
   /*!
      \param SynapseType is the C++ class type to add, derived from TSynapse
      \param SynapseName is the unique name of the synapse
      \param PreCellName is the presynaptic cell
      \param PostCellName is the postsynaptic cell
      \param X is the distance of GUI representation from the left
      \param Y is the distance of GUI representation from the top
   */
   virtual TSynapse * __fastcall
   AddSynapseBetweenCells( const std::wstring & SynapseType,
                           const std::wstring & SynapseName,
                           const std::wstring & PreCellName,
                           const std::wstring & PostCellName,
                           int X = 0, int Y = 0);

   /// Removes synapse from network and disconnects two cells
   /*!
      \param SynapseName is the unique name of the synapse
   */
   virtual void __fastcall
   RemoveSynapseFromNet  ( const std::wstring & SynapseName);

   /// Determines whether synapse is included in calculations
   /*!
      \param SynapseName is the unique name of the synapse
      \param Active is true if synapse is included, false otherwise
   */
   virtual void __fastcall
   SetSynapseActiveState ( const std::wstring & SynapseName,
                           bool Active);

   /// Rename the synapse
   /*!
      \param oldname is the old unique name of the synapse
      \param newname is the new unique name of the synapse
   */
   virtual void __fastcall
   RenameSynapse         ( const std::wstring & oldname,
                           const std::wstring & newname);




   // electrode access
   /// Adds electrode to a cell in the network
   /*!
      \param ElectrodeType is the C++ class type to add, derived from TElectrode
      \param ElectrodeName is the unique name of the electrode
      \param CellName is the cell "impaled" by the electrode
      \param X is the distance of GUI representation from the left
      \param Y is the distance of GUI representation from the top
   */
   virtual TElectrode * __fastcall
   AddElectrodeToCell    ( const std::wstring & ElectrodeType,
                           const std::wstring & ElectrodeName,
                           const std::wstring & CellName,
                           int X = 0, int Y = 0);

   /// Removes electrode from network and its cell
   /*!
      \param ElectrodeName is the unique name of the electrode
   */
   virtual void __fastcall
   RemoveElectrodeFromNet( const std::wstring & ElectrodeName);

   /// Determines whether electrode is included in calculations
   /*!
      \param ElectrodeName is the unique name of the electrode
      \param Active is true if electrode is included, false otherwise
   */
   virtual void __fastcall
   SetElectrodeActiveState(const std::wstring & ElectrodeName,
                           bool Active);

   /// Rename the electrode
   /*!
      \param oldname is the old unique name of the electrode
      \param newname is the new unique name of the electrode
   */
   virtual void __fastcall
   RenameElectrode       ( const std::wstring & oldname,
                           const std::wstring & newname);




   // currents access
   /// Adds current to a synapse in the network
   /*!
      \param SynapseName is the unique name of the synapse
      \param CurrentType is the C++ class type to add, derived from TCurrent
      \param CurrentName is the unique name of the current
      \param CellName is cell to which current is applied, either the pre- or
               postsynaptic cell of the synapse
   */
   virtual TCurrent * __fastcall
   AddCurrentToSynapse   ( const std::wstring & SynapseName,
                           const std::wstring & CurrentType,
                           const std::wstring & CurrentName,
                           const std::wstring & CellName);

   /// Removes current from synapse
   /*!
      \param CurrentName is the unique name of the current
      \param SynapseName is the unique name of the synapse
   */
   virtual void __fastcall
   RemoveCurrentFromSynapse(
                           const std::wstring & CurrentName,
                           const std::wstring & SynapseName);

   /// Adds current to a cell in the network
   /*!
      \param CurrentType is the C++ class type to add, derived from TCurrent
      \param CurrentName is the unique name of the current
      \param CellName is cell to which current is applied
   */
   virtual TCurrent * __fastcall
   AddCurrentToCell      ( const std::wstring & CurrentType,
                           const std::wstring & CurrentName,
                           const std::wstring & CellName);

   /// Removes current from cell
   /*!
      \param CurrentName is the unique name of the current
      \param CellName is the unique name of the cell
   */
   virtual void __fastcall
   RemoveCurrentFromCell ( const std::wstring & CurrentName,
                           const std::wstring & CellName);

   /// Determines whether current is included in calculations
   /*!
      \param CurrentName is the unique name of the current
      \param Active is true if current is included, false otherwise
   */
   virtual void __fastcall
   SetCurrentActiveState ( const std::wstring & CurrentName,
                           bool Active);

   /// Rename the current
   /*!
      \param oldname is the old unique name of the current
      \param newname is the new unique name of the current
   */
   virtual void __fastcall
   RenameCurrent         ( const std::wstring & oldname,
                           const std::wstring & newname);





   /// GUI friendly function that determines the proximity of a position to a cell
   /*!
      \param X is the distance of GUI representation from the left
      \param Y is the distance of GUI representation from the top
      \param tol is the tolerance, in pixels by which proximity is judged
   */
   virtual std::wstring __fastcall
   CellGridHitTest       ( const int X, const int Y, int tol = 0);

   /// Default constructor
	__fastcall TNetwork();
   /// Specialized constructor
   __fastcall TNetwork(const std::wstring & name);
	virtual ~TNetwork();

/// Updates the entire network -- called from DAQ classes
/*!
 The steps of the TNetwork::Update are as follows:<br>
   -# Set the voltages of the biological cells according to the acquired Vm's.
   -# Calculate the new voltages of the TModelCells and TPlayback cells
     by calling CalcVm().
   -# Update the intrinsic and synaptic currents of all the cells in a loop.
   -# Return the updated currents in an array.

   \param step is ms since last call
   \param Vm_in is the array of voltages from ADCs (in Volts), size = NumVDepCells.
            This parameter could be NULL if no cells are voltage dependent.
   \param Vm_out is the array of voltages of all cells in the
         network (in mV), size = NumCells
   \param I_nA  is the returned array of voltages of the voltage-dependent cells
         in the network (in Volts), size = NumVDepCells -- to be applied by DACs
            This parameter could be NULL if there are no voltage dependent cells
            in the network.

*/
   virtual double*  __fastcall		Update(  double step,
                                             double *Vm_in,
                                             double *Vm_out,
                                             double *I_nA);

/// GUI or DAQ classes must call DescribeNetwork before starting a run
/*!
Call DescribeNetwork at the beginning of a run.
Returns NetDescriptionStruct structure with DAQ information
*/
   virtual NetDescriptionStruct __fastcall DescribeNetwork();

   // override pure virtual TRTBase::Initialize
   virtual bool __fastcall          Initialize(bool Reset);

   virtual const std::wstring & __fastcall ClassKey() const;

};



//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
//
//
//
/*

 Class Factories

*/

#include "factory.h"

typedef std::wstring TKeyType;   // use wstring to make interaction with Windows easy


/// Factory method for TCell derived types
typedef TObjFactory1P<  TKeyType,
                        TCell,
                        const std::wstring> TCellFactory;
TCellFactory &GetCellFactory();


/// Factory method for TSynapse derived types
typedef TObjFactory3P<  TKeyType,
                        TSynapse,
                        const std::wstring,
                        TCell * const,
                        TCell * const> TSynapseFactory;
TSynapseFactory &GetSynapseFactory();


/// Factory method for TElectrode derived types
typedef TObjFactory2P<  TKeyType,
                        TElectrode,
                        TCell * const,
                        const std::wstring> TElectrodeFactory;
TElectrodeFactory &GetElectrodeFactory();

/// Factory method for TCurrent derived types
typedef TObjFactory2P<  TKeyType,
                        TCurrent,
                        TCurrentUser* const,
                        const std::wstring> TCurrentFactory;
TCurrentFactory &GetCurrentFactory();


/// Global method for accessing a TNetwork allocated on the heap
TNetwork * GetNet(bool reset = false);
//---------------------------------------------------------------------------
#endif
