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

#include "GUI_RT_Edit_GJSynapse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGapJunctionSynapseForm *GapJunctionSynapseForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TGapJunctionSynapse methods
//
//===========================================================================
//===========================================================================
//===========================================================================


////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGapJunctionSynapse   ***************************
//
//
/// Specialized synapse with two TGJCurrents, one for each direction
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TGapJunctionSynapse : public TSynapse
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
		TGapJunctionSynapseForm *form = (TGapJunctionSynapseForm * )GetEditForm();

		std::wstring caption = L"Direction: From " + Pre()->Name() + L" to " + Post()->Name();
		form->GroupBox1->Caption = caption.c_str();

		caption = L"Direction: From " + Post()->Name() + L" to " + Pre()->Name();
		form->GroupBox2->Caption = caption.c_str();

		TGapJunctionCurrent *pre2post = (TGapJunctionCurrent *)PreToPostCurrents()[0];
		TGapJunctionCurrent *post2pre = (TGapJunctionCurrent *)PostToPreCurrents()[0];
		form->PreToPostForm->Edit1->Text = FloatToStr(pre2post->Gmax());
		form->PostToPreForm->Edit1->Text = FloatToStr(post2pre->Gmax());
	}

	virtual bool __fastcall       ValidateEditForm()
	{
		TGapJunctionSynapseForm *form = (TGapJunctionSynapseForm * )GetEditForm();
		TGapJunctionCurrent *pre2post = (TGapJunctionCurrent *)PreToPostCurrents()[0];
		TGapJunctionCurrent *post2pre = (TGapJunctionCurrent *)PostToPreCurrents()[0];

		bool ok1 = false;
		bool ok2 = false;
		try {
			double gmax = StrToFloat(form->PreToPostForm->Edit1->Text);
			if (gmax < 0.0){
				Application->MessageBox(L"Conductance must be greater than or equal to zero",
												L"Please correct input");
			} else {
				pre2post->SetGmax(gmax);
				ok1 = true;
			}

			gmax = StrToFloat(form->PostToPreForm->Edit1->Text);
			if (gmax < 0.0){
				Application->MessageBox(L"Conductance must be greater than or equal to zero",
												L"Please correct input");
			} else {
				ok2 = true;
				post2pre->SetGmax(gmax);
			}

		} catch (EConvertError & e) {
			Application->MessageBox(e.ToString().c_str(), L"Please correct input");
		}
		return (ok1 && ok2);
	}

	virtual void* const __fastcall
											GetEditForm()
	{
		static TGapJunctionSynapseForm *form = new TGapJunctionSynapseForm(NULL);
		return form;
	}


	// implement pure virtual
	#define TGAPJUNCTIONSYNAPSE_KEY L"Gap Junction Synapse"
   const std::wstring & __fastcall ClassKey() const
   {
      static std::wstring Key(TGAPJUNCTIONSYNAPSE_KEY);
      return Key;
   }

   /// Default Constructor
   __fastcall TGapJunctionSynapse()
   {

   }

   /// Specialized Constructor
   __fastcall TGapJunctionSynapse(const std::wstring & name, TCell * const pre, TCell * const post)
                     : TSynapse(name, pre, post)
   {
   /* TODO : note -- the currents created here will not be owned by network. This
         might be beneficial because they can't be deleted by GUI */
      std::wstring dirname = pre->Name() + L" to " + post->Name();
      TCurrent * currpre(GetCurrentFactory().buildObj(
            L"Gap Junction Current",
            (TCurrentUser *)this,
            dirname));
      AddCurrent(currpre, post);

      dirname = post->Name() + L" to " + pre->Name();
      TCurrent * currpost(GetCurrentFactory().buildObj(
            L"Gap Junction Current",
            (TCurrentUser *)this,
            dirname));
      AddCurrent(currpost, pre);
   }

   /// Copy Constructor
   __fastcall TGapJunctionSynapse::TGapJunctionSynapse(const TGapJunctionSynapse &source) :
                                 TSynapse(source.Name(), source.Pre(), source.Post())
   {
   }

   /// "duplicate properties with new cells" constructor
   __fastcall TGapJunctionSynapse::TGapJunctionSynapse(const TGapJunctionSynapse &source,
                                                      TCell * const newPre,
                                                      TCell * const newPost) :
                                 TSynapse(source.Name(), newPre, newPost)
   {
   }

   /// Overloaded assignment operator
   TGapJunctionSynapse& TGapJunctionSynapse::operator = (const TGapJunctionSynapse & source)
   {
      if (this != &source) {  // make sure not same object
         this->TSynapse::operator=(source); //copy base class members
         // now copy TGapJunctionSynapse fields
      }
      return *this;
   }

	virtual ~TGapJunctionSynapse(){;}

};

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TGapJunctionSynapse)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
   static struct regGJSynapse {
      regGJSynapse(){
         GetSynapseFactory().registerBuilder(
                                 TGAPJUNCTIONSYNAPSE_KEY,// L"Gap Junction Synapse"
                                 TypeID<TGapJunctionSynapse>(),
                                 TypeID<const std::wstring>(),
                                 TypeID<TCell * const>(),
                                 TypeID<TCell * const>());
      }
   } RegGJSynapse;
}

//---------------------------------------------------------------------------

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TGapJunctionSynapseForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================

//---------------------------------------------------------------------------
__fastcall TGapJunctionSynapseForm::TGapJunctionSynapseForm(TComponent* Owner)
   : TForm(Owner)
{
   PreToPostForm = new TGJCurrentForm(Panel1);
   PreToPostForm->BorderStyle= bsNone;
   PreToPostForm->Parent = Panel1;
   PreToPostForm->Visible = true;

   PostToPreForm = new TGJCurrentForm(Panel2);
   PostToPreForm->BorderStyle= bsNone;
   PostToPreForm->Parent = Panel2;
   PostToPreForm->Visible = true;
}
//---------------------------------------------------------------------------
