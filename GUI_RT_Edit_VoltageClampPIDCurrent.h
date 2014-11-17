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

#ifndef GUI_RT_Edit_VoltageClampPIDCurrentH
#define GUI_RT_Edit_VoltageClampPIDCurrentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TVoltageClamp_PID_CurrentForm   ***************************
//
//
/**
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
   @brief GUI editor for TVoltageClamp_PID_Current
*/
class TVoltageClamp_PID_CurrentForm : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TEdit *PGainEdit;
   TEdit *IGainEdit;
   TEdit *DGainEdit;
   TPanel *Panel1;
   TEdit *TauEdit;
   TLabel *Label5;
   TLabel *Label6;
   TEdit *VCommandEdit;
   TLabel *Label7;
   TLabel *Label8;
   TEdit *ImaxEdit;
   TEdit *IminEdit;
private:	// User declarations
public:		// User declarations
   __fastcall TVoltageClamp_PID_CurrentForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVoltageClamp_PID_CurrentForm *VoltageClamp_PID_CurrentForm;
//---------------------------------------------------------------------------
#endif
