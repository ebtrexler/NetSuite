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

#ifndef GUI_CopyCurrentsFormH
#define GUI_CopyCurrentsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TCopyCurrentsForm   ***************************
//
//
/**
   @brief GUI class for copying currents from one cell to one or many others.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TCopyCurrentsForm : public TForm
{
__published:	// IDE-managed Components
   TComboBox *CopyFromComboBox;
   TListBox *CopyToListBox;
   TLabel *Label1;
   TLabel *Label2;
   TButton *Button1;
   TButton *Button2;
   TListBox *ToNamesListBox;
	TCheckBox *ClearThenCopyCheckBox;
private:	// User declarations
public:		// User declarations
   __fastcall TCopyCurrentsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCopyCurrentsForm *CopyCurrentsForm;
//---------------------------------------------------------------------------
#endif
