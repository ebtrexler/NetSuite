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

#ifndef GUI_RT_Edit_ModelCellH
#define GUI_RT_Edit_ModelCellH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

class TModelCell;  // forward declaration


////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TModelCellForm   ***************************
//
//
/// GUI editor for TModelCell
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TModelCellForm : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TLabel *Label2;
   TEdit *CapacitanceEdit;
   TLabel *Label3;
   TEdit *InitialVmEdit;
   TLabel *Label4;
   void __fastcall EditKeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
public:		// User declarations
   TModelCell *ModelCell;
   __fastcall TModelCellForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TModelCellForm *ModelCellForm;
//---------------------------------------------------------------------------
#endif
