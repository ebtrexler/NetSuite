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

// ---------------------------------------------------------------------------

#pragma hdrstop

#include "RT_Cell.h"

// Default constructor
__fastcall TCell::TCell() : TCurrentUser(L"UnNamed", true), F_Vm(0), F_I(0),
FAIGain(1), FAOGain(1), FAIChannel(L""), FAOChannel(L"") {
}

// Specialized constructor
__fastcall TCell::TCell(const std::wstring & name) : TCurrentUser(name, true),
F_Vm(0), F_I(0), FAIGain(1), FAOGain(1), FAIChannel(L""), FAOChannel(L"") {
}

// copy constructor
__fastcall TCell::TCell(const TCell &source) : TCurrentUser(source.Name(),
   source.IsActive()), FCurrents(source.FCurrents),
FElectrodes(source.FElectrodes), FSynapses(source.FSynapses),
FAIChannel(source.FAIChannel), FAOChannel(source.FAOChannel),
FAIGain(source.FAIGain), FAOGain(source.FAOGain), F_Vm(source.F_Vm),
F_I(source.F_I) {
}

// Overloaded assignment operator
TCell& TCell:: operator = (const TCell & source) {
   if (this != &source) { // make sure not same object
      this->TCurrentUser:: operator = (source); // copy base class members
      // now copy TCell fields
      FCurrents = source.FCurrents;
      FElectrodes = source.FElectrodes;
      FSynapses = source.FSynapses;
      FAIChannel = source.FAIChannel;
      FAOChannel = source.FAOChannel;
      FAIGain = source.FAIGain;
      FAOGain = source.FAOGain;
      F_Vm = source.F_Vm;
      F_I = source.F_I;
   }
   return *this;
}

TCell::~TCell() {
}

// Writes members to a stream
void TCell::WriteToStream(std::ostream &stream)const
{
/* TODO : Can't implement -- how do you know the current type to create from stream? */
   // int len = FCurrents.size();
   // stream.write((char *)&len, sizeof(int));
   // // loop over currents
   // if (len > 0) {
   // TCurrentsArrayIterator currIter = FCurrents.begin(), currend = FCurrents.end();
   // for (currIter; currIter != currend; currIter++) {
   // stream << *currIter;
   // }
   // }

   // FElectrodes -- leave out
   // FSynapses -- leave out
   int len = FAIChannel.length();
   const wchar_t* data = FAIChannel.data();
   stream.write((char*) & len, sizeof(int));
   stream.write((char*) & data, len*sizeof(wchar_t));

   len = FAOChannel.length();
   data = FAOChannel.data();
   stream.write((char*) & len, sizeof(int));
   stream.write((char*) & data, len*sizeof(wchar_t));

   stream.write((char*) & FAIGain, sizeof(double));
   stream.write((char*) & FAOGain, sizeof(double));
   stream.write((char*) & F_Vm, sizeof(double));
   stream.write((char*) & F_I, sizeof(double));
}

// Reads members from a stream
void TCell::ReadFromStream(std::istream &stream)
{

   // int len;= FCurrents.size();
   // stream.read((char *)&len, sizeof(int));
   // // loop over currents
   // if (len > 0) {
   // for (int i=0; i<len; i++) {
   // stream >> *currIter;
   // }
   // }
   // FElectrodes -- leave out
   // FSynapses -- leave out
   int len;
   stream.read((char*) & len, sizeof(int));
   wchar_t* data = new wchar_t[len];
   stream.read((char*) & data, len*sizeof(wchar_t));
   FAIChannel.assign(data);
   delete[]data;
   stream.read((char*) & len, sizeof(int));
   data = new wchar_t[len];
   stream.read((char*) & data, len*sizeof(wchar_t));
   FAOChannel.assign(data);

   stream.read((char*) & FAIGain, sizeof(double));
   stream.read((char*) & FAOGain, sizeof(double));
   stream.read((char*) & F_Vm, sizeof(double));
   stream.read((char*) & F_I, sizeof(double));
}

std::wstring __fastcall TCell::AIChannel() {
   return FAIChannel;
}

std::wstring __fastcall TCell::AOChannel() {
   return FAOChannel;
}

double __fastcall TCell::AIGain() {
   return FAIGain;
}

double __fastcall TCell::AOGain() {
   return FAOGain;
}

double __fastcall TCell::Vm() {
   return F_Vm;
}

double __fastcall TCell::SetVm(double Vm) {
   F_Vm = FAIGain * Vm;
   return F_Vm;
}

double __fastcall TCell::I() {
   return F_I;
}

bool __fastcall TCell::IsVoltageDependent() {
   return false;
}

// add and remove currents
// override pure virtual methods in TCurrentUser
const TCurrent * __fastcall TCell::AddCurrent(TCurrent * c, TCell * cell) {
   FCurrents.push_back(c);
   return c;
}

void __fastcall TCell::RemoveCurrent(TCurrent * c) {
   TCurrentsArrayIterator toErase = std::find(FCurrents.begin(),
      FCurrents.end(), c);
   if (toErase != FCurrents.end()) {
		FCurrents.erase(toErase);
	}
}

void __fastcall TCell::ClearCurrents()
{
	FCurrents.clear();
}

const TCurrentsArray __fastcall TCell::GetCurrents() const {
   return FCurrents;
}

// add remove synapses
const TSynapse * __fastcall TCell::AddSynapse(TSynapse * s) {
   FSynapses.push_back(s);
   return s;
}

void __fastcall TCell::RemoveSynapse(TSynapse * s) {
   TSynapsesArrayIterator toErase = std::find(FSynapses.begin(),
      FSynapses.end(), s);
   if (toErase != FSynapses.end()) {
      FSynapses.erase(toErase);
   }
}

const TSynapsesArray __fastcall TCell::GetSynapses()const {
   return FSynapses;
}

// add remove electrodes
const TElectrode * __fastcall TCell::AddElectrode(TElectrode * e) {
   FElectrodes.push_back(e);
   return e;
}

void __fastcall TCell::RemoveElectrode(TElectrode * e) {
   TElectrodesArrayIterator toErase = std::find(FElectrodes.begin(),
      FElectrodes.end(), e);
   if (toErase != FElectrodes.end()) {
      FElectrodes.erase(toErase);
   }
}

const TElectrodesArray __fastcall TCell::GetElectrodes()const {
   return FElectrodes;
}

double __fastcall TCell::CalcVm(double step) {
   return F_Vm;
} // placeholder

double __fastcall TCell::Update(double step) {
   if (!IsActive()) {
      return 0;
   }

   return DoUpdate(step);
}

double __fastcall TCell::DoUpdate(double step) {
   F_I = 0.0;

   // loop over currents
   if (FCurrents.size() > 0) {
      TCurrentsArrayIterator currIter = FCurrents.begin(),
      currend = FCurrents.end();
      for (currIter; currIter != currend; currIter++) {
         F_I -= (*currIter)->Update(step, F_Vm, F_Vm); // subtract
      } /* Vkin */  /* Vdrv */
   }

   // loop over electrodes
   if (FElectrodes.size() > 0) {
      TElectrodesArrayIterator trodeIter = FElectrodes.begin(),
      trodeend = FElectrodes.end();
      for (trodeIter; trodeIter != trodeend; trodeIter++) {
         F_I += (*trodeIter)->Update(step); // add
      }
   }

   // loop over synapses
   if (FSynapses.size() > 0) {
      TSynapsesArrayIterator synpIter = FSynapses.begin(),
      synpend = FSynapses.end();
      for (synpIter; synpIter != synpend; synpIter++) {
         F_I -= (*synpIter)->Update(this, step); // subtract
      }
   }
   return F_I * FAOGain; // nA  * V / nA = V .
}

// ---------------------------------------------------------------------------

#pragma package(smart_init)
