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


#pragma hdrstop

#include "RT_CurrentUser.h"

__fastcall TCurrentUser::TCurrentUser() : TRTBase()
{
}

// Specialized constructor
__fastcall TCurrentUser::TCurrentUser(const std::wstring & name, const bool active) :
                              TRTBase(name, active)
{
}

// copy constructor
__fastcall TCurrentUser::TCurrentUser(const TCurrentUser & source) :
   TRTBase(source.Name(), source.IsActive())
{
}

// overloaded assignment operator
TCurrentUser & TCurrentUser::operator = (const TCurrentUser & source)
{
   if (this != &source) {  // make sure not same object
      this->TRTBase::operator=(source); //copy base class members
      // now copy TCurrentUser fields
   }
   return *this;
}

// destructor
TCurrentUser::~TCurrentUser()
{
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
