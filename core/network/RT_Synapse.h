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

#ifndef RT_SynapseH
#define RT_SynapseH

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8

#include "RT_Base.h"
#include "RT_CurrentUser.h"
#include "RT_Current.h"

//---------------------------------------------------------------------------
//
/// Base class for current containers that facilitate communication between cells
/*!
TSynapse is bidirectional by design and has two arrays of TCurrents,
   accessed by PreToPostCurrents() and PostToPreCurrents().
Arrays were chosen rather than single TCurrents, because
   it is possible that a single neurotransmitter activates multiple
   postsynaptic receptors or a synapse might co-release transmitters and
   peptides or some other effector.
TSynpase owns pointers to the pre- and
   postsynaptic TCells, accessed through Pre() and Post().

@author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TSynapse : public TCurrentUser
{
#ifdef SERIALIZE
///  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
///  Required for serialization and saving networks to disk
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TCurrentUser>(*this);
    	ar & BOOST_SERIALIZATION_NVP(FPre);
     	ar & BOOST_SERIALIZATION_NVP(FPost);
      ar & BOOST_SERIALIZATION_NVP(FPreToPostCurrents);
      ar & BOOST_SERIALIZATION_NVP(FPostToPreCurrents);
	}
#endif //SERIALIZE
private:

   /// Presynaptic cell
	TCell *        					FPre;
   /// Postsynaptic cell
	TCell *   	      				FPost;

   // multiple possible currents per synapse direction...
	TCurrentsArray		            FPreToPostCurrents;
   TCurrentsArray                FPostToPreCurrents;

protected:
    /// Writes data members to a stream
   virtual void WriteToStream(std::ostream &stream) const;
   /// Reads data members from a stream
   virtual void ReadFromStream(std::istream &stream);

public:
   /// Returns a pointer to the presynaptic cell
	virtual TCell * const  __fastcall   Pre();
   /// Returns a pointer to the postsynaptic cell
	virtual TCell * const  __fastcall   Post();

   /// const method, Returns a pointer to the presynaptic cell
	virtual TCell * const  __fastcall   Pre() const;
   /// const method, Returns a pointer to the postsynaptic cell
	virtual TCell * const  __fastcall   Post() const;

   /// Returns the array of currents affecting the presynaptic cell
   virtual TCurrentsArray __fastcall PreToPostCurrents() const;
   /// Returns the array of currents affecting the postsynaptic cell
   virtual TCurrentsArray __fastcall PostToPreCurrents() const;

   /// Returns true if cell is postsynaptic, false otherwise
   bool                          IsPostSynaptic(TCell * const cell);

   /// Checks if active and if so, calls DoUpdate()
  	double __fastcall 		      Update(TCell * const cell, double step);

   /// Returns the current contribution to cell of this synapse
   /*!
   This method queries the cell parameter to see if pre-
   or postsynaptic and calls, in a loop, either
       -FPreToPostCurrents[i]->Update(step, FPre->Vm, FPost->Vm), or
       -FPostToPreCurrents[i]->Update(step, FPost->Vm, FPre->Vm).

   It returns the summed current contribution to the cell from this synapse.

   <STRONG>
   --Derived classes only need override DoUpdate if they do not use the TCurrent
   members to define current changes.
   </STRONG>

   \param cell is equal to either Pre() or Post(), and determines direction for
      current calculation
   \param step is in ms, the time since the last call
  */
   virtual double __fastcall     DoUpdate(TCell * const cell, double step);

   /// Overrides pure virtual but does nothing
   virtual bool __fastcall       Initialize(bool Reset);

   // add and remove currents
   // override pure virtual methods in TCurrentUser
   virtual const TCurrent * __fastcall AddCurrent( TCurrent * c,  TCell* const toCell);

   virtual void __fastcall       RemoveCurrent(TCurrent * c);

   /// Default constructor
   __fastcall TSynapse();

   /// Specialized constructor
	__fastcall TSynapse(const std::wstring & name, TCell * const pre, TCell * const post);

   /// Copy constructor
   __fastcall TSynapse(const TSynapse &source);

   /// "Duplicate properties with new cells" constructor
   TSynapse(const TSynapse &source, TCell * const newPre, TCell * const newPost);

   /// Overloaded assignment operator
   TSynapse& operator = (const TSynapse & source);

	virtual ~TSynapse();
};


//---------------------------------------------------------------------------
#endif
