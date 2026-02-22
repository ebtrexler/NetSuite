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

#pragma hdrstop

#include "RT_Network.h"


// static factory Getter functions

TCellFactory &GetCellFactory()
{
   static TCellFactory     myCellFactory;
   return myCellFactory;
}

TSynapseFactory &GetSynapseFactory()
{
   static TSynapseFactory     mySynapseFactory;
   return mySynapseFactory;
}

TElectrodeFactory &GetElectrodeFactory()
{
   static TElectrodeFactory     myElectrodeFactory;
   return myElectrodeFactory;
}

TCurrentFactory &GetCurrentFactory()
{
   static TCurrentFactory     myCurrentFactory;
   return myCurrentFactory;
}


////////////////////////////////////////////////////////////////////////////////
// TNetwork methods


// Default constructor
__fastcall TNetwork::TNetwork() :
                              TRTBase(L"UnNamed", true),
										FMaxRK4Timestep(0.01),
										FParamLoggingEnabled(false)
{
}

// Specialized constructor
__fastcall TNetwork::TNetwork(const std::wstring & name) :
                              TRTBase(name, true),
										FMaxRK4Timestep(0.01),
										FParamLoggingEnabled(false)
{
}

TNetwork::~TNetwork()
{
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*
This function gets an array of voltages sampled
  from all the biological cells in the network.  This array could be NULL
  if the program is running in modeling mode rather than dynamic clamp mode.
  Modeling mode means that all cells in the network are Model cells.

 The steps of the TNetwork::Update are as follows:<br>
   -# Set the voltages of the biological cells according to the acquired Vm's.
   -# Calculate the new voltages of the TModelCells and TPlayback cells
   -# Update the intrinsic, electrode and synaptic currents of all the cells in a loop,
         calling Update() for each cell.
   -# Return the updated currents in I_nA
*/

double * __fastcall	TNetwork::Update(double step, double* Vm_in, double *Vm_out, double *I_nA)
{

   double *vin = Vm_in;                         // in from ADCs as Volts
   double *vout = Vm_out;                       // out from SetVm as mV
   double *Iptr = I_nA;                         // storage for outputs

   // update Elapsed Time
   FElapsedTime += step;


   // set voltages of Vm dependent cells
   for (int i = 0; i < FNetDescription.NumVDepCells; i++) {
      *vout = FVmDepCells[i]->SetVm(*vin); // get scaled voltage (in mV) from SetVm
      ++vin;
      ++vout;
   }

   // calculate voltages of time dependent cells
   for (int i=0; i < FNetDescription.NumTimeCells; i++) {
      *vout = FTimeCells[i]->CalcVm(step);// get scaled voltage (in mV) from CalcVm
      ++vout;
   }


   // added 2/21/2013 -- kinetic parameter logging
   if (FParamLoggingEnabled) {
      GetParamLogger() << (FElapsedTime / 1000.0) << "\t";  // start the line with the time in seconds
   }


   // call update for all cells, but separately
   // first VmDep cells, because they return a current

   for (int i = 0; i < FNetDescription.NumVDepCells; i++) {
      *Iptr = FVmDepCells[i]->Update(step);// get scaled DAC voltages from Update
      ++Iptr;
   }

   // next, Time cells
   for (int i=0; i < FNetDescription.NumTimeCells; i++) {
      FTimeCells[i]->Update(step);
   }

   // added 2/21/2013 -- kinetic parameter logging
   if (FParamLoggingEnabled) {
      GetParamLogger() << "\n";  // finish the line
   }

   return I_nA; // now filled with appropriately scaled voltages to be applied
               // to DACs for current injection by the amplifier
}

bool __fastcall TNetwork::Initialize(bool Reset)
{
   // iterators for cell array
   TCellsMapIterator celliter = FCells.begin();
//   TCellsMapIterator cellend = FCells.end();

   // iterators for synapse array
   TSynapsesMapIterator syniter = FSynapses.begin();
//   TSynapsesMapIterator synend = FSynapses.end();

   // iterators for electrode array
   TElectrodesMapIterator trodeiter = FElectrodes.begin();
//   TElectrodesMapIterator trodeend = FElectrodes.end();

   // iterators for currents array
   TCurrentsMapIterator curriter = FCurrents.begin();
//   TCurrentsMapIterator currend = FCurrents.end();

   // initialize cells
   for (; celliter != FCells.end(); ++celliter) {
      if (!celliter->second->Initialize(Reset)) return false;    // initialize cell
   }

   // initialize synapses
   for (; syniter != FSynapses.end(); ++syniter) {
      if (!syniter->second->Initialize(Reset)) return false;
   }

   // initialize electrodes
   for (; trodeiter != FElectrodes.end(); ++trodeiter) {
      if (!trodeiter->second->Initialize(Reset)) return false;
   }

   // initialize currents
   for (; curriter != FCurrents.end(); ++curriter) {
      if (!curriter->second->Initialize(Reset)) return false;
   }

   FElapsedTime = 0.0;

   return true;
}

NetDescriptionStruct __fastcall TNetwork::DescribeNetwork()
{
   // iterators for cell array
   TCellsMapIterator celliter = FCells.begin();
   TCellsMapIterator cellend = FCells.end();

//   // iterators for synapse array
//   TSynapsesMapIterator syniter = FSynapses.begin();
//   TSynapsesMapIterator synend = FSynapses.end();
//
//   // iterators for electrode array
//   TElectrodesMapIterator trodeiter = FElectrodes.begin();
//   TElectrodesMapIterator trodeend = FElectrodes.end();

   //  Reset FInit for changes in network
   FNetDescription.AIChans.clear();
   FNetDescription.AOChans.clear();
   FNetDescription.NumVDepCells = 0;
   FNetDescription.NumTimeCells = 0;

   // repopulate FInit
   for (; celliter != cellend; ++celliter) {
       if (celliter->second->IsActive()) {
         if (celliter->second->IsVoltageDependent()) {
            FVmDepCells[FNetDescription.NumVDepCells] = (celliter->second).get();
            if (FNetDescription.NumVDepCells == 0) {
               FNetDescription.AIChans = celliter->second->AIChannel();
               FNetDescription.AOChans = celliter->second->AOChannel();
            } else {
               FNetDescription.AIChans = FNetDescription.AIChans + L"," + celliter->second->AIChannel();
               FNetDescription.AOChans = FNetDescription.AOChans + L"," + celliter->second->AOChannel();
            }
            ++FNetDescription.NumVDepCells;
         } else {
            FTimeCells[FNetDescription.NumTimeCells] = (celliter->second).get();
            ++FNetDescription.NumTimeCells;
         }
      }
   }
   return FNetDescription;
}

const std::wstring & __fastcall  TNetwork::ClassKey() const
{
   return L"Not in factory";
}

//---------------------------------------------------------------------------

const TCellsMap & __fastcall     TNetwork::GetCells()
{
   return FCells;
}

const TSynapsesMap & __fastcall  TNetwork::GetSynapses()
{
   return FSynapses;
}

const TElectrodesMap & __fastcall TNetwork::GetElectrodes()
{
   return FElectrodes;
}


// Access to execution variables/parameters/flags

// Runge-Kutta time step getter method
const double __fastcall          TNetwork::GetMaxRK4Timestep() const
{
   return FMaxRK4Timestep;
}
// Runge-Kutta time step setter method
void __fastcall                  TNetwork::SetMaxRK4Timestep(double rk4step)
{
   if (rk4step < 0) {
      rk4step = 0.01;
   }
   FMaxRK4Timestep = rk4step;
}

const double __fastcall          TNetwork::ElapsedTime() const
{
   return FElapsedTime;
}

// Output stream reference for logging variables getter method
TDataLogger & __fastcall        TNetwork::GetParamLogger()
{
   return FParamLogger;
}

const bool __fastcall            TNetwork::IsParamLoggingEnabled() const
{
   return FParamLoggingEnabled;
}

void __fastcall         TNetwork::SetParamLoggingEnabled(bool enabled)
{
   FParamLoggingEnabled = enabled;
}

// Writes tab-delimited column names for paramter logging to supplied stream
void __fastcall         TNetwork::WriteParamLoggingHeader()
{
   if (!IsParamLoggingEnabled()) {
      return;
   }

   // call WriteParamLoggingHeader for all cells, but separately
   // first VmDep cells, then TimeDep cells
   try {
      FParamLogger.Reset();

      FParamLogger << "Time (s)\t";

      for (int i = 0; i < FNetDescription.NumVDepCells; i++) {
         WriteHeaderForCellCurrents(FVmDepCells[i]);
      }
      for (int i = 0; i < FNetDescription.NumTimeCells; i++) {
         WriteHeaderForCellCurrents(FTimeCells[i]);
      }

      FParamLogger << "\n"; //new line to finish header
   } catch (...) {
   }


/*
// loop over currents
         if (FTimeCells[i]->GetCurrents().size() > 0) {
            TCurrentsArrayConstIterator
               currIter = FTimeCells[i]->GetCurrents().begin(),
               currend = FTimeCells[i]->GetCurrents().end();
            for (currIter; currIter != currend; currIter++) {
               (*currIter)->WriteParamLoggingHeader(FParamLogger);
            }
         }

         // loop over synapses
         if (FTimeCells[i]->GetSynapses().size() > 0) {
            TSynapsesArrayConstIterator
               synpIter = FTimeCells[i]->GetSynapses().begin(),
               synpend = FTimeCells[i]->GetSynapses().end();
            for (synpIter; synpIter != synpend; synpIter++) {
               TSynapse *syn = (*synpIter);
               bool hascurrents = false;
               TCurrentsArrayIterator currIter, currend;
               if (syn->IsPostSynaptic(FTimeCells[i])) {
                  if (syn->PreToPostCurrents().size() > 0) {
                     currIter = syn->PreToPostCurrents().begin();
                     currend = syn->PreToPostCurrents().end();
                     hascurrents = true;
                  }
               }
               else {
                  if (syn->PostToPreCurrents().size() > 0) {
                     currIter = syn->PostToPreCurrents().begin();
                     currend = syn->PostToPreCurrents().end();
                     hascurrents = true;
                  }
               }
               if (hascurrents) {
                  for (currIter; currIter != currend; currIter++) {
                     (*currIter)->WriteParamLoggingHeader(FParamLogger);
                  }
               }
            }
         }
*/
}

void __fastcall         TNetwork::WriteHeaderForCellCurrents(TCell *cell)
{
   // loop over currents
   if (cell->GetCurrents().size() > 0) {
      TCurrentsArrayConstIterator
         currIter = cell->GetCurrents().begin(),
         currend = cell->GetCurrents().end();
      for (currIter; currIter != currend; currIter++) {
         (*currIter)->WriteParamLoggingHeader(FParamLogger);
      }
   }

   // loop over synapses
   if (cell->GetSynapses().size() > 0) {
      TSynapsesArrayConstIterator
         synpIter = cell->GetSynapses().begin(),
         synpend = cell->GetSynapses().end();
      for (synpIter; synpIter != synpend; synpIter++) {
         TSynapse *syn = (*synpIter);
         bool hascurrents = false;
         TCurrentsArrayIterator currIter, currend;
         if (syn->IsPostSynaptic(cell)) {
            if (syn->PreToPostCurrents().size() > 0) {
               currIter = syn->PreToPostCurrents().begin();
               currend = syn->PreToPostCurrents().end();
               hascurrents = true;
            }
         }
         else {
            if (syn->PostToPreCurrents().size() > 0) {
               currIter = syn->PostToPreCurrents().begin();
               currend = syn->PostToPreCurrents().end();
               hascurrents = true;
            }
         }
         if (hascurrents) {
            for (currIter; currIter != currend; currIter++) {
               (*currIter)->WriteParamLoggingHeader(FParamLogger);
            }
         }
      }
   }
}

/// Terminate logging
void __fastcall         TNetwork::CloseLoggingStream(const wchar_t * filename)
{
   FParamLogger.WriteTo(filename);
   FParamLogger.Reset();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// Currents functions
TCurrentPtr __fastcall  TNetwork::AddCurrentToMap(TCurrentPtr c)
{
   TCurrentsMapIterator it = FCurrents.find(c->Name());
   if (it != FCurrents.end())
      throw std::runtime_error("duplicate");
   FCurrents.insert(std::pair<std::wstring, TCurrentPtr>(c->Name(), c));
   return c;
}

void __fastcall         TNetwork::RemoveCurrentFromMap(const std::wstring & name)
{
   TCurrentsMapIterator toRemove = FCurrents.find(name);
   if (toRemove != FCurrents.end()) FCurrents.erase(toRemove);
}

// Cells functions
TCellPtr __fastcall     TNetwork::AddCellToMap(TCellPtr c)
{
   TCellsMapIterator it = FCells.find(c->Name());
   if (it != FCells.end())
      throw std::runtime_error("duplicate");
   FCells.insert(std::pair<std::wstring, TCellPtr>(c->Name(), c));
   return c;
}
void __fastcall         TNetwork::RemoveCellFromMap(const std::wstring & name)
{
   TCellsMapIterator toRemove = FCells.find(name);
   if (toRemove != FCells.end()) FCells.erase(toRemove);
}


// Synapses functions
TSynapsePtr __fastcall  TNetwork:: AddSynapseToMap(TSynapsePtr s)
{
   TSynapsesMapIterator it = FSynapses.find(s->Name());
   if (it != FSynapses.end())
      throw std::runtime_error("duplicate");
   FSynapses.insert(std::pair<std::wstring, TSynapsePtr>(s->Name(), s));
   return s;
}
void __fastcall         TNetwork::RemoveSynapseFromMap(const std::wstring & name)
{
   TSynapsesMapIterator toRemove = FSynapses.find(name);
   if (toRemove != FSynapses.end()) FSynapses.erase(toRemove);
}

// Electrodes functions
TElectrodePtr __fastcall TNetwork::AddElectrodeToMap(TElectrodePtr e)
{
   TElectrodesMapIterator it = FElectrodes.find(e->Name());
   if (it != FElectrodes.end())
      throw std::runtime_error("duplicate");
   FElectrodes.insert(std::pair<std::wstring, TElectrodePtr>(e->Name(), e));
   return e;
}
void __fastcall          TNetwork::RemoveElectrodeFromMap(const std::wstring & name)
{
   TElectrodesMapIterator toRemove = FElectrodes.find(name);
   if (toRemove != FElectrodes.end()) FElectrodes.erase(toRemove);
}


///////////////////////////////////////////////////////////////////////////////
// Network Access Functions
// Text Based access methods


TCell * __fastcall TNetwork::AddCellToNet
                      ( const std::wstring & CellType,
                        const std::wstring & CellName,
                        int X, int Y)
{

   TCellPtr cell(GetCellFactory().buildObj(CellType, CellName));
   cell->SetX(X);
   cell->SetY(Y);
   AddCellToMap(cell);
   return cell.get();
}


void __fastcall TNetwork::RemoveCellFromNet
                      ( const std::wstring & CellName)
{
   RemoveCellFromMap(CellName);
}


void __fastcall TNetwork::SetCellActiveState
                      ( const std::wstring & CellName,
                        bool Active)
{
   TCell * cell = FCells[CellName].get();
   cell->SetActive(Active);
}

void __fastcall TNetwork::RenameCell
                      ( const std::wstring & oldname,
                        const std::wstring & newname)
{
   // get ptr to cell
   TCellPtr cellptr(FCells[oldname]);

   // erase cell from map
   RemoveCellFromMap(oldname);

   // rename cell
   cellptr->SetName(newname);

   // add cell back to map with new name
   AddCellToMap(cellptr);
}

void __fastcall TNetwork::CopyCurrentsFromCell
                       ( const std::wstring & fromname,
								 const std::vector<std::wstring> tonames,
								 bool ClearAndReplace)
{
   TCellsMapConstIterator try_cell_from = FCells.find(fromname);
   if (try_cell_from == FCells.end()) {
      throw std::runtime_error("From cell doesn't exist");
   }
   TCellPtr cell_from(FCells[fromname]);
   int tonamessize = tonames.size();
   for (int j=0; j<tonamessize; j++) {
      std::wstring CellName = tonames[j];
		TCurrentsArray fromCurrents = cell_from->GetCurrents();
		int fromCurrentssize = fromCurrents.size();
		if (ClearAndReplace){
			TCurrentsArray toCurrents = FCells[CellName]->GetCurrents();
			for (unsigned int i=0; i<toCurrents.size(); i++)
				RemoveCurrentFromCell(toCurrents[i]->Name(), CellName);
		}
      for (int i = 0; i < fromCurrentssize; i++) {
         TCurrent *theCurrent = fromCurrents[i];
         std::wstring CurrentKey = theCurrent->ClassKey();
         std::wostringstream CurrentName;
         std::wstring currname(theCurrent->Name());
         CurrentName << currname << L" Copy# " << j + 1;
         TCurrent *current = AddCurrentToCell(CurrentKey, CurrentName.str(), CellName);
         current->CopyParamsFrom(theCurrent);
      }
   }
}

TSynapse * __fastcall TNetwork::AddSynapseBetweenCells
                      ( const std::wstring & SynapseType,
                        const std::wstring & SynapseName,
                        const std::wstring & PreCellName,
                        const std::wstring & PostCellName,
                        int X, int Y)
{
   TSynapsePtr synapse(GetSynapseFactory().buildObj(
            SynapseType,
            SynapseName,
            FCells[PreCellName].get(),
            FCells[PostCellName].get()));
   synapse->SetX(X);
   synapse->SetY(Y);
   AddSynapseToMap(synapse);
   return synapse.get();
}


void __fastcall TNetwork::RemoveSynapseFromNet
                      ( const std::wstring & SynapseName)
{
   TSynapsePtr synapse(FSynapses[SynapseName]);
   synapse->Pre()->RemoveSynapse(synapse.get());
   synapse->Post()->RemoveSynapse(synapse.get());
   RemoveSynapseFromMap(SynapseName);
}


void __fastcall TNetwork::SetSynapseActiveState
                      ( const std::wstring & SynapseName,
                        bool Active)
{
   TSynapsePtr synapse(FSynapses[SynapseName]);
   synapse->SetActive(Active);
}

void __fastcall TNetwork::RenameSynapse
                      ( const std::wstring & oldname,
                        const std::wstring & newname)
{
   // get ptr
   TSynapsePtr synapseptr(FSynapses[oldname]);

   // erase from map
   RemoveSynapseFromMap(oldname);

   // rename
   synapseptr->SetName(newname);

   // add back to map with new name
   AddSynapseToMap(synapseptr);
}

TElectrode * __fastcall TNetwork::AddElectrodeToCell
                      ( const std::wstring & ElectrodeType,
                        const std::wstring & ElectrodeName,
                        const std::wstring & CellName,
                        int X, int Y)
{
   TCell * cell = FCells[CellName].get();
   TElectrodePtr electrode(GetElectrodeFactory().buildObj(
            ElectrodeType,
            cell,
            ElectrodeName));
   electrode->SetX(X);
   electrode->SetY(Y);
   AddElectrodeToMap(electrode);
   return electrode.get();
}


void __fastcall TNetwork::RemoveElectrodeFromNet
                      ( const std::wstring & ElectrodeName)
{
   TElectrodePtr electrode(FElectrodes[ElectrodeName]);
   TCell * cell(electrode->Owner());
   cell->RemoveElectrode(electrode.get());
   RemoveElectrodeFromMap(ElectrodeName);
}


void __fastcall TNetwork::SetElectrodeActiveState
                      ( const std::wstring & ElectrodeName,
                        bool Active)
{
   TElectrodePtr electrode(FElectrodes[ElectrodeName]);
   electrode->SetActive(Active);
}

void __fastcall TNetwork::RenameElectrode
                      ( const std::wstring & oldname,
                        const std::wstring & newname)
{
   // get ptr
   TElectrodePtr electrodeptr(FElectrodes[oldname]);

   // erase from map
   RemoveElectrodeFromMap(oldname);

   // rename
   electrodeptr->SetName(newname);

   // add back to map with new name
   AddElectrodeToMap(electrodeptr);
}

TCurrent * __fastcall TNetwork::AddCurrentToSynapse
                      ( const std::wstring & SynapseName,
                        const std::wstring & CurrentType,
                        const std::wstring & CurrentName,
                        const std::wstring & CellName)
{
   TSynapsePtr synapse(FSynapses[SynapseName]);
   TCurrentPtr curr(GetCurrentFactory().buildObj(
            CurrentType,
            (TCurrentUser *)synapse.get(),
            CurrentName));
   synapse->AddCurrent(curr.get(), (TCell *)FCells[CellName].get());
   AddCurrentToMap(curr);
   return curr.get();
}

void __fastcall TNetwork::RemoveCurrentFromSynapse
                      ( const std::wstring & CurrentName,
                        const std::wstring & SynapseName)
{
   TSynapsePtr synapse(FSynapses[SynapseName]);
   TCurrentPtr curr(FCurrents[CurrentName]);
   synapse->RemoveCurrent(curr.get());
   RemoveCurrentFromMap(CurrentName);
}

TCurrent * __fastcall TNetwork::AddCurrentToCell
                      ( const std::wstring & CurrentType,
                        const std::wstring & CurrentName,
                        const std::wstring & CellName)
{
   TCellPtr cell(FCells[CellName]);
   TCurrentPtr curr(GetCurrentFactory().buildObj(
            CurrentType,
            (TCurrentUser *)cell.get(), //TCurrentUser subclass
            CurrentName));
   cell->AddCurrent(curr.get(), (TCell *)cell.get());
   AddCurrentToMap(curr);
   return curr.get();
}


void __fastcall TNetwork::RemoveCurrentFromCell
                      ( const std::wstring & CurrentName,
                        const std::wstring & CellName)
{
   TCellPtr cell(FCells[CellName]);
   TCurrentPtr curr = FCurrents[CurrentName];
   cell->RemoveCurrent(curr.get());
   RemoveCurrentFromMap(CurrentName);
}


void __fastcall TNetwork::SetCurrentActiveState
                      ( const std::wstring & CurrentName,
                        bool Active)
{
   TCurrentPtr current(FCurrents[CurrentName]);
   current->SetActive(Active);
}

void __fastcall TNetwork::RenameCurrent
                      ( const std::wstring & oldname,
                        const std::wstring & newname)
{
   // get ptr
   TCurrentPtr currptr(FCurrents[oldname]);

   // erase from map
   RemoveCurrentFromMap(oldname);

   // rename
   currptr->SetName(newname);

   // add back to map with new name
   AddCurrentToMap(currptr);
}

////////////////////////////////////////////////////////////////////////////////
///////
//             Other network access methods
///////
////////////////////////////////////////////////////////////////////////////////

TNetwork * GetNet(bool reset)
{
	static TNetwork *net = NULL;
	if (reset) {
		delete net;
		net = NULL;
	}
	if (!net) {
		net = new TNetwork(L"Singleton Network");
	}
	return net;
}

std::wstring __fastcall TNetwork::CellGridHitTest
                     ( const int X, const int Y, int tol)
{
   TCellsMapIterator iter = FCells.begin();
   std::wstring str = L"";
   for (; iter != FCells.end(); ++iter) {
      if (iter->second->HitTest(X, Y, tol)) {
         str = iter->second->Name();
         break;
      }
   }
   return str;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
