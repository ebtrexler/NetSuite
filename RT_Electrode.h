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

#ifndef RT_ElectrodeH
#define RT_ElectrodeH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "RT_Base.h"

//---------------------------------------------------------------------------
//

/// Base class designed to function as a simple current injecting electrode.
/*!
  It has virtual DoUpdate(step) that calculates a
   current based on arbitrary parameters set in derived classes.  DoUpdate(step)
   must be overridden.  Descendents of TElectrode can use its ElapsedTime to
   help define a time dependent waveform.

  <STRONG>
  --Derived classes must override DoUpdate(step) and return the
    new current value.
  </STRONG>

  @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TElectrode : public TRTBase
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
//  Required for serialization and saving networks to disk
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TRTBase>(*this);
      ar & BOOST_SERIALIZATION_NVP(FOwner);
      ar & BOOST_SERIALIZATION_NVP(FElapsedTime);
	}
#endif //SERIALIZE

private:
   // assumes Update in derived classes will output a function of time
   double                        FElapsedTime; // in ms
   TCell *                       FOwner;

protected:

public:
   /// Returns the cell "impaled" by this current injecting electrode
	TCell * const __fastcall      Owner() const;

	/// Returns the time since last call to Initialize
  	double __fastcall       ElapsedTime() const;

   /// Must override but can call default implementation TElectrode::Initialize();
	virtual bool __fastcall			Initialize(bool Reset) = 0;

   /// Calculates current to inject, must override
   virtual double __fastcall     DoUpdate(double step) = 0;

   /// Updates ElapsedTime and calls DoUpdate if Active()==true
	double __fastcall       Update(double step);


   /// Default constructor
   __fastcall TElectrode();

   /// Specialized constructor
   __fastcall TElectrode(TCell * const owner, const std::wstring & name);

   /// Copy constructor
   __fastcall TElectrode(const TElectrode &source);

   /// Overloaded assignment operator
   TElectrode& operator = (const TElectrode & source);

   virtual ~TElectrode();
};


//---------------------------------------------------------------------------
#endif
