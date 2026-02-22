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

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_GenBiDirSynapse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGenBiDirSynapseForm *GenBiDirSynapseForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TGenBiDirSynapse methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region RT classes with methods that define TGenBiDirSynapse

#include "RT_Network.h"

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGenBiDirSynapse   ***************************
//
//
/// Implements a bidirectional synapse between two cells
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TGenBiDirSynapse : public TSynapse
{
#ifdef SERIALIZE
//  Required for serialization and saving networks to disk
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
      ar & boost::serialization::base_object<TSynapse>(*this);
	}
#endif

private:

protected:

public:
	// implement pure virtual
	bool __fastcall               Initialize(bool Reset)
	{
		return true;
	}

	virtual void __fastcall       PopulateEditForm()
	{
	}

	virtual bool __fastcall       ValidateEditForm()
	{
		return true;
	}

   virtual void* const __fastcall
                                 GetEditForm()
   {
      static TGenBiDirSynapseForm *form = new TGenBiDirSynapseForm(NULL);
      form->GenBiDirSynapse = this;
      return form;
	}

	// implement pure virtual
	#define TGenBiDirSynapse_KEY L"Generic Bi-Directional Synapse"
	const std::wstring & __fastcall ClassKey() const
	{
		static std::wstring Key(TGenBiDirSynapse_KEY);
		return Key;
	}

	/// Default constructor
	__fastcall TGenBiDirSynapse()
	{

	}

	/// Specialized constructor
   __fastcall TGenBiDirSynapse(const std::wstring & name, TCell * const pre, TCell * const post)
                     : TSynapse(name, pre, post)
   {
   }

   /// Copy constructor
   __fastcall TGenBiDirSynapse(const TGenBiDirSynapse &source) :
                        TSynapse(source.Name(), source.Pre(), source.Post())
   {
   }

   /// "Duplicate properties with new cells" constructor
   __fastcall TGenBiDirSynapse::TGenBiDirSynapse(const TGenBiDirSynapse &source,
                                                TCell * const newPre,
                                                TCell * const newPost) :
                        TSynapse(source.Name(), newPre, newPost)
   {
   }

   /// Overloaded assignment operator
   TGenBiDirSynapse& operator = (const TGenBiDirSynapse & source)
   {
      if (this != &source) {  // make sure not same object
         this->TSynapse::operator=(source); //copy base class members
         // now copy TGenBiDirSynapse fields
      }
      return *this;
   }

	virtual ~TGenBiDirSynapse(){;}

};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TGenBiDirSynapse)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace TGenBiDirSynapsespace
{
   static struct regGenBiDirSynapse {
      regGenBiDirSynapse(){
         GetSynapseFactory().registerBuilder(
                                 TGenBiDirSynapse_KEY, // L"Generic Bi-Directional Synapse",
                                 TypeID<TGenBiDirSynapse>(),
                                 TypeID<const std::wstring>(),
                                 TypeID<TCell * const>(),
                                 TypeID<TCell * const>());
      }
   } RegGenBiDirSynapse;
}

//---------------------------------------------------------------------------

#pragma end_region

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TGenBiDirSynapseForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================
//---------------------------------------------------------------------------
__fastcall TGenBiDirSynapseForm::TGenBiDirSynapseForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
