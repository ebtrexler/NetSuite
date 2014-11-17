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

#pragma hdrstop

#include "RT_Base.h"



// Default constructor
__fastcall TRTBase::TRTBase():
   FName(L"UnNamed"), FActive(true), F_X(0), F_Y(0)
{
}
// Specialized constructor
__fastcall TRTBase::TRTBase(const std::wstring & name, const bool active) :
   FName(name), FActive(active), F_X(0), F_Y(0)
{
}

// copy constructor
__fastcall TRTBase::TRTBase(const TRTBase &source) :
   FName(source.FName), FActive(source.FActive), F_X(source.F_X), F_Y(source.F_Y)
{
}

// Overloaded assignment operator
TRTBase& TRTBase::operator = (const TRTBase & source)
{
   if (this != &source) {  // make sure not same object
     // this->Parent::operator=(source); //copy base class members
      // now copy TRTBase fields
      FName = source.FName;
      FActive = source.FActive;
      F_X = source.F_X;
      F_Y = source.F_Y;
   }
   return *this;
}

TRTBase::~TRTBase() {}

// friend insertion << operator
ostream &operator<<(ostream &stream, const TRTBase &rtbase)
{
//	int len = rtbase.FName.length();
//	const wchar_t* data = rtbase.FName.data();
//	stream.write((char *)&len, sizeof(int));
//	stream.write((char *)&data, len * sizeof(wchar_t));
	stream.write((char *)&rtbase.F_X, sizeof(int));
	stream.write((char *)&rtbase.F_Y, sizeof(int));
   stream.write((char *)&rtbase.FActive, sizeof(bool));

   rtbase.WriteToStream(stream); // call most derived WriteToStream (polymorphism)

   return stream;
}

// friend extraction >> operator
istream &operator>>(istream &stream, TRTBase &rtbase)
{
//   int len;
//   stream.read((char *)&len, sizeof(int));
////   boost::shared_ptr<wchar_t> data(new wchar_t[len]);
//	wchar_t * data = new wchar_t[len];
//	stream.read((char *)&(data), len * sizeof(wchar_t));
//   rtbase.FName.assign(data);
   stream.read((char *)&rtbase.F_X, sizeof(int));
   stream.read((char *)&rtbase.F_Y, sizeof(int));
   stream.read((char *)&rtbase.FActive, sizeof(bool));

   rtbase.ReadFromStream(stream); // call most derived ReadToStream (polymorphism)

   return stream;
}

const std::wstring __fastcall TRTBase::ClassType() const
{
   UnicodeString str(typeid(*this).name());
   std::wstring wstr(str.c_str());
   return wstr;
}

const std::wstring __fastcall TRTBase::Name() const
{
   return FName;
}

void __fastcall               TRTBase::SetName(const std::wstring & name)
{
   FName = name;
}

// Determines if object is included in model
const bool __fastcall         TRTBase::IsActive() const
{
   return FActive;
}

void __fastcall               TRTBase::SetActive(bool active)
{
   FActive = active;
}

const int __fastcall          TRTBase::GetX() const { return F_X; }
void __fastcall               TRTBase::SetX(int x) { F_X = x; }
const int __fastcall          TRTBase::GetY() const { return F_Y; }
void __fastcall               TRTBase::SetY(int y) { F_Y = y; }

bool __fastcall               TRTBase::HitTest(int X, int Y, int tol) const
{
   bool val = false;
   if ((F_X <= X+tol) && (F_X >= X-tol) && (F_Y <= Y+tol) && (F_Y >= Y-tol)) {
      val = true;
   }
   return val;
}

void* const __fastcall       TRTBase::GetEditForm() { return NULL;}

const bool __fastcall         TRTBase::AttachEditFormTo(TComponent *Owner, TWinControl *Parent)
{
   if (GetEditForm() == NULL) return false;

   ((TForm *)GetEditForm())->BorderStyle= bsNone;
   ((TForm *)GetEditForm())->Parent = Parent;

   PopulateEditForm();

   ((TForm *)GetEditForm())->Visible = true;
   return true;

}

void __fastcall       TRTBase::PopulateEditForm() {;}
bool __fastcall       TRTBase::ValidateEditForm() {return true;}
//---------------------------------------------------------------------------

#pragma package(smart_init)
