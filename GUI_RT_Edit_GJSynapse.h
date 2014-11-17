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

#ifndef GUI_RT_Edit_GJSynapseH
#define GUI_RT_Edit_GJSynapseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "GUI_RT_Edit_GJCurrent.h" //embedded forms

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGapJunctionSynapse   ***************************
//
//
/// GUI editor for TGapJunctionSynapse
/*!
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TGapJunctionSynapseForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TPanel *Panel1;
   TGroupBox *GroupBox2;
   TPanel *Panel2;
private:	// User declarations
public:		// User declarations
   TGJCurrentForm *PreToPostForm;
   TGJCurrentForm *PostToPreForm;
   __fastcall TGapJunctionSynapseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGapJunctionSynapseForm *GapJunctionSynapseForm;
//---------------------------------------------------------------------------
#endif
