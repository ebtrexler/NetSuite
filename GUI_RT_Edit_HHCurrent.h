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

#ifndef GUI_RT_Edit_HHCurrentH
#define GUI_RT_Edit_HHCurrentH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
#include "MultiPLOTPanel.h"
#include <Dialogs.hpp>
#include "GUI_PeriodicityEditor.h"
#include "RT_HHCurrent.h"

const int NumPoints = 301;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   THHCurrentForm   ***************************
//
//
/**
   @brief GUI editor for THHCurrent
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class THHCurrentForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TPanel *Panel1;
   TValueListEditor *ValueListEditor_m;
   TGroupBox *GroupBox2;
   TPanel *Panel2;
   TValueListEditor *ValueListEditor_h;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TEdit *GmaxEdit;
   TEdit *ErevEdit;
   TButton *LoadButton;
   TButton *SaveButton;
   TMultiPLOTPanel *MultiPlotPanel1;
   TGroupBox *GroupBox3;
   TPanel *Panel3;
   TValueListEditor *ValueListEditor_n;
   TLabel *Label5;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
   TEdit *GnoiseEdit;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *Label8;
   TPanel *Panel4;
   TPanel *Panel5;
   TCheckBox *PeriodicCheckBox;
   TButton *PeriodEditButton;
   TCheckBox *ParamLoggingCheckBox;
   TPanel *Panel6;
	TCheckBox *UseVdrvComboBox;
   void __fastcall ParamsEditors_KeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall LoadButtonClick(TObject *Sender);
   void __fastcall SaveButtonClick(TObject *Sender);
   void __fastcall PeriodEditButtonClick(TObject *Sender);
	void __fastcall UseVdrvComboBoxClick(TObject *Sender);

private:	// User declarations
   TStaticFunctionPlot
      *PlotIV,
      *Plotminf, *Plothinf, *Plotninf,
      *Plottaum, *Plottauh, *Plottaun;

   double
      dataIV[NumPoints],
      dataminf[NumPoints], datahinf[NumPoints], dataninf[NumPoints],
      datataum[NumPoints], datatauh[NumPoints], datataun[NumPoints];

public:		// User declarations
   void __fastcall UpdatePlots();
   THHCurrent * HHCurrent;
   TPeriodicityForm *CurrentPeriodForm;
   __fastcall THHCurrentForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE THHCurrentForm *HHCurrentForm;

//---------------------------------------------------------------------------
#endif
