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

#ifndef GUI_VoltageClampFitFormH
#define GUI_VoltageClampFitFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "Fit_HHCurrent.h"
#include "PLOTPanel.h"
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
class TVoltageClampFitForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TLabel *Label1;
	TComboBox *VChanComboBox;
	TButton *GenerateButton;
	TLabel *Label2;
	TComboBox *CChanComboBox;
	TLabel *Label3;
	TLabel *FilenameLabel;
	TButton *SaveButton;
	TSaveDialog *SaveDialog1;
	TPanel *Panel3;
	TPanel *ParamsPanel;
	TPLOTPanel *PlotPanel1;
	TLabel *Label4;
	TCheckBox *ConcatEpisCheckBox;
	TButton *OptimizeButton;
	TValueListEditor *ValueListEditor1;
	TLabel *Label5;
	TLabel *fitGenLabel;
	TLabel *Label6;
	TLabel *ChiSqLabel;
	TScrollBox *ScrollBox1;
	TScrollBox *ScrollBox2;
	TCheckBox *MultiThreadCheckBox;
	TLabel *Label7;
	TEdit *NumThreadsEdit;
	TButton *StopButton;
	TButton *SaveParamsButton;
	TSaveDialog *SaveDialog2;
	void __fastcall GenerateButtonClick(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall PageControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall OptimizeButtonClick(TObject *Sender);
	void __fastcall ConcatEpisCheckBoxClick(TObject *Sender);
	void __fastcall StopButtonClick(TObject *Sender);
	void __fastcall SaveParamsButtonClick(TObject *Sender);
private:	// User declarations
   ModelFittingModule *mf;
public:		// User declarations
	double *origdata;
	double *genrdata;
	THHCurrentModel HHModel;

	__fastcall TVoltageClampFitForm(TComponent* Owner);
	__fastcall ~TVoltageClampFitForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TVoltageClampFitForm *VoltageClampFitForm;
//---------------------------------------------------------------------------
#endif
