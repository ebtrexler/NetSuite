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

#include "GUI_RT_Edit_GJCurrent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGJCurrentForm *GJCurrentForm;


//===========================================================================
//===========================================================================
//===========================================================================
//
// RT -- TGapJunctionCurrent methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region RT classes with methods that define TGapJunctionCurrent

void* const __fastcall       TGapJunctionCurrent::GetEditForm()
{
   static TGJCurrentForm *form = new TGJCurrentForm(NULL);
   return form;
}

void const __fastcall TGapJunctionCurrent::WriteToStream(ostream &stream) const
{
	stream.write((char *)&F_Gmax, sizeof(double));
	TCurrent::WriteToStream(stream); //Write base class members
}

void const __fastcall TGapJunctionCurrent::ReadFromStream(istream &stream)
{
	stream.read((char *)&F_Gmax, sizeof(double));
	TCurrent::ReadFromStream(stream); //Read base class members
}

const double __fastcall			TGapJunctionCurrent::Gmax()
{
   return F_Gmax;
}

void __fastcall               TGapJunctionCurrent:: SetGmax(double set)
{
   F_Gmax = set;
}

// implement pure virtual
bool __fastcall               TGapJunctionCurrent::Initialize(bool Reset)
{
   return true;
}

// implement pure virtual
#define TGAPJUNCTIONCURRENT_KEY L"Gap Junction Current"
const std::wstring & __fastcall TGapJunctionCurrent::ClassKey() const
{
   static std::wstring Key(TGAPJUNCTIONCURRENT_KEY);
	return Key;
}

// implement pure virtual
const void __fastcall			TGapJunctionCurrent::GetParamLogHeader(
												std::vector<std::string>& params) const
{
	params.clear();  // do nothing, set size to zero
}


// override TCurrent::DoUpdate
double __fastcall TGapJunctionCurrent::DoUpdate(double step, double Vpre,
																double Vpost,
																std::vector<double> & params)
{
	params.clear();  // do nothing, set size to zero
	Ielect = F_Gmax*(Vpost-Vpre);  // uS * mV = nA
   return Ielect;
}

void __fastcall   TGapJunctionCurrent::PopulateEditForm()
{
   TGJCurrentForm *form = (TGJCurrentForm *)GetEditForm();
   form->GJCurrent = this;
   form->Edit1->Text = FloatToStr(Gmax());
}

bool __fastcall       TGapJunctionCurrent::ValidateEditForm()
{
   TGJCurrentForm *form = (TGJCurrentForm *)GetEditForm();
   bool ok = false;
   try {
      double gmax = StrToFloat(form->Edit1->Text);
      if (gmax < 0.0){
         Application->MessageBox(L"Conductance must be greater than or equal to zero",
                                 L"Please correct input");
      } else {
         ok = true;
         SetGmax(gmax);
      }

   } catch (EConvertError & e) {
      Application->MessageBox(e.ToString().c_str(), L"Please correct input");
   }
   return ok;
}

// Default Constructor
__fastcall TGapJunctionCurrent::TGapJunctionCurrent():
         TCurrent(NULL, L""), F_Gmax(0) {}

// Specialized Constructor 2 param
__fastcall TGapJunctionCurrent::TGapJunctionCurrent(
            TCurrentUser *owner, const std::wstring & name):
         TCurrent(owner, name), F_Gmax(0) {}

// Specialized Constructor 1 param
__fastcall TGapJunctionCurrent::TGapJunctionCurrent(
            const std::wstring & name):
         TCurrent(NULL, name), F_Gmax(0) {}

// copy constructor
__fastcall TGapJunctionCurrent::TGapJunctionCurrent( const TGapJunctionCurrent & source ) :
      TCurrent(source.Owner(), source.Name()),
      F_Gmax(source.F_Gmax)
{
}

// overloaded assignment operator
TGapJunctionCurrent & TGapJunctionCurrent::operator = (const TGapJunctionCurrent & source)
{
   if (this != &source) {  // make sure not same object
      this->TCurrent::operator=(source); //copy base class members
      // now copy THHCurrent fields
      F_Gmax = source.F_Gmax;
   }
   return *this;
}

void __fastcall TGapJunctionCurrent::CopyParamsFrom(const TCurrent * const source )
{
   if (this != source) {  // make sure not same object
      if (this->ClassType() == source->ClassType()) { // make sure same type
         TGapJunctionCurrent *cast_source = (TGapJunctionCurrent *)source;
         F_Gmax = cast_source->F_Gmax;
      }
   }
}

TGapJunctionCurrent::~TGapJunctionCurrent(){;}

#ifdef SERIALIZE
BOOST_CLASS_EXPORT(TGapJunctionCurrent)
#endif

////////////////////////////////////////////////////////////////////////////////
//    Register classes with factories
//

namespace FNadim
{
   static struct regGJCurrent {
      regGJCurrent(){
         GetCurrentFactory().registerBuilder(
                                 TGAPJUNCTIONCURRENT_KEY, //L"Gap Junction Current",
                                 TypeID<TGapJunctionCurrent>(),
                                 TypeID<TCurrentUser*const>(),
                                 TypeID<const std::wstring>());
      }
   } RegGJCurrent;
}

//---------------------------------------------------------------------------

#pragma end_region

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TGJCurrentForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================

#pragma region GUI methods of TGJCurrentForm


__fastcall TGJCurrentForm::TGJCurrentForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
#pragma end_region
void __fastcall TGJCurrentForm::Edit1KeyPress(TObject *Sender, wchar_t &Key)
{
   if (Key == VK_RETURN) {
      GJCurrent->ValidateEditForm();
      Key = 0;
   }
}
//---------------------------------------------------------------------------

