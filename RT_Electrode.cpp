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

#include "RT_Electrode.h"
#include "RT_Cell.h"

double __fastcall       TElectrode::ElapsedTime() const
{
   return FElapsedTime;
}

bool __fastcall			      TElectrode::Initialize(bool Reset)
{
   // regardless of Reset, FElapsedTime must be set to zero
   FElapsedTime = 0;
   return true;
}

TCell * const __fastcall      TElectrode::Owner() const
{
   return FOwner;
}

double __fastcall 		TElectrode::Update(double step)
{
   if (!IsActive()) {
       return 0;
   }
   double val = DoUpdate(step);  // overridden in derived classes
   FElapsedTime += step;
   return val;
}

/// Default constructor
__fastcall TElectrode::TElectrode() : TRTBase(L"UnNamed", true)
{
}

/// Specialized constructor
__fastcall TElectrode::TElectrode(TCell * const owner, const std::wstring & name) :
            TRTBase(name, true), FElapsedTime(0), FOwner(owner)
{
   if (FOwner) FOwner->AddElectrode(this);
}

/// Copy constructor
__fastcall TElectrode::TElectrode(const TElectrode &source) :
   TRTBase(source.Name(), source.IsActive()),
   FElapsedTime(source.FElapsedTime),
   FOwner(source.FOwner)
{
}

/// Overloaded assignment operator
TElectrode& TElectrode::operator = (const TElectrode & source)
{
   if (this != &source) {  // make sure not same object
      this->TRTBase::operator=(source); //copy base class members
      // now copy TElectrode fields
      FElapsedTime = source.FElapsedTime;
      FOwner = source.FOwner;
   }
   return *this;
}

TElectrode::~TElectrode()
{
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
