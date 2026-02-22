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

#ifndef RT_CurrentUserH
#define RT_CurrentUserH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "RT_Base.h"


//---------------------------------------------------------------------------
//   **********************  TCurrentUser  **************************
//

/// Base class for derived classes such as TSynapse and TCell that have TCurrent-derived children.
/*!
  @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TCurrentUser : public TRTBase
{

private:
protected:
public:

   /// Adds a TCurrent * to the array of currents in derived class
   /*! Pure virtual --> class is abstract and can only serve as base class
        derived classes must override and implement

        \param c is pointer to TCurrent derived class
        \param toCell is a pointer to the cell affected by this current
   */
   virtual const TCurrent * __fastcall AddCurrent(TCurrent * const c, TCell * const toCell = NULL) = 0;

   /// Removes current from class
   /*!
      Pure virtual --> class is abstract and can only serve as base class
     derived classes must override and implement
   */
	virtual void __fastcall       RemoveCurrent(TCurrent * c) = 0;

	/// Default constructor
   __fastcall TCurrentUser();
   /// Specialized constructor
   __fastcall TCurrentUser(const std::wstring & name, const bool active);
   /// copy constructor
   __fastcall TCurrentUser(const TCurrentUser & source);
   /// overloaded assignment operator
   TCurrentUser & operator = (const TCurrentUser & source);

   virtual ~TCurrentUser();
};


//---------------------------------------------------------------------------
#endif
