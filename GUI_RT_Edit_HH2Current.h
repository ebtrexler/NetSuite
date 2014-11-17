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

#ifndef GUI_RT_Edit_HH2CurrentH
#define GUI_RT_Edit_HH2CurrentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MultiPLOTPanel.h"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Tabs.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
const int NumPoints = 301;

class THH2Current; // forward declaration

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   THH2CurrentForm   ***************************
//
//
/**
   @brief GUI editor for THH2Current
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/

class THH2CurrentForm : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label8;
   TGroupBox *GroupBox1;
   TPanel *Panel1;
   TValueListEditor *ValueListEditor_m;
   TGroupBox *GroupBox2;
   TPanel *Panel2;
   TValueListEditor *ValueListEditor_h;
   TMultiPLOTPanel *MultiPLOTPanel1;
   TLabel *Label7;
   TGroupBox *GroupBox3;
   TPanel *Panel3;
   TValueListEditor *ValueListEditor_n;
   TPanel *Panel4;
   TPanel *Panel5;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
   TTabControl *TabControl1;
   TPanel *Panel6;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label6;
   TEdit *GmaxEdit;
   TEdit *ErevEdit;
   TButton *LoadButton;
   TButton *SaveButton;
   TEdit *GnoiseEdit;
   TPanel *Panel7;
   TCheckBox *ParamLoggingCheckBox;
	TRadioGroup *GCalcRadioGroup;
   void __fastcall ParamsEditors_KeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall LoadButtonClick(TObject *Sender);
   void __fastcall SaveButtonClick(TObject *Sender);
   void __fastcall TabControl1Changing(TObject *Sender, bool &AllowChange);
   void __fastcall TabControl1Change(TObject *Sender);
	void __fastcall GCalcRadioGroupClick(TObject *Sender);


private:	// User declarations

   TStaticFunctionPlot
		*PlotIV, *PlotIVCombined,
      *Plotminf, *Plothinf, *Plotninf,
      *Plottaum, *Plottauh, *Plottaun;

	double
		dataIV[NumPoints], dataIVCombined[NumPoints],
      dataminf[NumPoints], datahinf[NumPoints], dataninf[NumPoints],
      datataum[NumPoints], datatauh[NumPoints], datataun[NumPoints];

public:		// User declarations
   void __fastcall UpdatePlots();
   THH2Current * HH2Current;
public:		// User declarations
   __fastcall THH2CurrentForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THH2CurrentForm *HH2CurrentForm;
//---------------------------------------------------------------------------
#endif
