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

#include "RT_Synapse.h"

#include "RT_Cell.h"

// default constructor
__fastcall TSynapse::TSynapse()
                     : TCurrentUser(L"UnNamed", true), FPre(NULL), FPost(NULL)
{
}

// specialized constructor
__fastcall TSynapse::TSynapse(const std::wstring & name, TCell * const pre, TCell * const post)
                     : TCurrentUser(name, true), FPre(pre), FPost(post)
{
   FPre->AddSynapse(this);
   FPost->AddSynapse(this);
}

// copy constructor
__fastcall TSynapse::TSynapse(const TSynapse &source) :
                              TCurrentUser(source.Name(), source.IsActive()),
                              FPre(source.FPre),
                              FPost(source.FPost)
{
}

// duplicate properties with new cells constructor
__fastcall TSynapse::TSynapse(const TSynapse &source, TCell * const newPre, TCell * const newPost) :
                              TCurrentUser(source.Name(), source.IsActive()),
                              FPre(newPre),
                              FPost(newPost)
{
   FPre->AddSynapse(this);
   FPost->AddSynapse(this);
}

// Overloaded assignment operator
TSynapse& TSynapse::operator = (const TSynapse & source)
{
   if (this != &source) {  // make sure not same object
      this->TCurrentUser::operator=(source); //copy base class members
      // now copy TSynapse fields
      FPre = source.FPre;
      FPost = source.FPost;
   }
   return *this;
}

TSynapse::~TSynapse()
{
}

bool TSynapse::IsPostSynaptic(TCell * const cell)
{
   return (cell == FPost);
}

// Writes members to a stream
void const __fastcall TSynapse::WriteToStream(ostream &stream) const
{
//   stream << FPre;
//   stream << FPost;
//   stream << FPreToPostCurrents;
//   stream << FPostToPreCurrents;
}

// Reads members from a stream
void const __fastcall TSynapse::ReadFromStream(istream &stream)
{
//   stream >> FPre;
//   stream >> FPost;
//   stream >> FPreToPostCurrents;
//   stream >> FPostToPreCurrents;
}


// add and remove currents
// override pure virtual methods in TCurrentUser
const TCurrent * __fastcall TSynapse::AddCurrent(TCurrent * c, TCell * const toCell)
{
	if (toCell->AcceptsCurrents()) {
		if (toCell == FPost) {
			FPreToPostCurrents.push_back(c);
		}
		else if (toCell == FPre) {
			FPostToPreCurrents.push_back(c);
		}
	}
	else {
		throw Exception(L"The cell that would receive this synaptic current does not accept currents");
   }
	return c;
}

void __fastcall       TSynapse::RemoveCurrent(TCurrent * c)
{
   // based on cell type (post or pre, remove currents from proper direction
   TCurrentsArrayIterator toErase =
      std::find(FPreToPostCurrents.begin(), FPreToPostCurrents.end(), c);
   if (toErase != FPreToPostCurrents.end()) {  // find succeeded
      FPreToPostCurrents.erase(toErase);
      return;
   }
   toErase =
      std::find(FPostToPreCurrents.begin(), FPostToPreCurrents.end(), c);
   if (toErase != FPreToPostCurrents.end()) {
      FPostToPreCurrents.erase(toErase);
   }
}

double __fastcall 		TSynapse::Update(TCell * const cell, double step)
{
   if (!IsActive()) {
       return 0;
   }
   return DoUpdate(cell, step);
}

bool __fastcall TSynapse::Initialize(bool Reset)
{
   return true;
}

double __fastcall TSynapse::DoUpdate(TCell * const cell, double step)
{
	double current = 0.0;

   // "normal" transfer from pre to postsynaptic cell
   if (IsPostSynaptic(cell))
   {
      TCurrentsArrayIterator currIter = FPreToPostCurrents.begin(),
                                 end = FPreToPostCurrents.end();
      // loop over currents
      for (currIter; currIter != end; currIter++) {
         current += (*currIter)->Update(step, FPre->Vm(), FPost->Vm());
      }
   }
   // reciprocal direction
   else {
      TCurrentsArrayIterator currIter = FPostToPreCurrents.begin(),
                                 end = FPostToPreCurrents.end();
      // loop over currents
      for (currIter; currIter != end; currIter++) {
         current += (*currIter)->Update(step, FPost->Vm(), FPre->Vm());
      }
   }
	return current;
}

TCell * const __fastcall               TSynapse::Pre()
{
   return FPre;
}
TCell * const 	__fastcall              TSynapse::Post()
{
   return FPost;
}

// const methods
TCell * const __fastcall               TSynapse::Pre() const
{
   return FPre;
}
TCell * const 	__fastcall              TSynapse::Post() const
{
   return FPost;
}

TCurrentsArray	__fastcall	            TSynapse::PreToPostCurrents() const
{
   return FPreToPostCurrents;
}
TCurrentsArray __fastcall              TSynapse::PostToPreCurrents() const
{
   return FPostToPreCurrents;
}



//---------------------------------------------------------------------------

#pragma package(smart_init)
