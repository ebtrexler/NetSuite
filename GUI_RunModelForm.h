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

#ifndef GUI_RunModelFormH
#define GUI_RunModelFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MultiPLOTPanel.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "nidaqmx.h"
#include "NI_DAQmx_Devices.h"
#include "VCLTee.Chart.hpp"
#include "VCLTee.TeEngine.hpp"
#include "VCLTee.TeeProcs.hpp"
#include <Grids.hpp>
#include <ValEdit.hpp>

#include "RT_Network.h"
#include "VCLTee.Series.hpp"
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ImgList.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include "CGMComponent.h"
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>

//#include <boost/scoped_array.hpp>

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TRunDynamicClampForm   ***************************
//
//
/// Graphical interface for running simulation or dynamic clamp or hybrid
/*!
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TRunDynamicClampForm : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TEdit *SampleRateEdit;
   TEdit *TimeBeforeEdit;
   TEdit *DurationEdit;
   TEdit *TimeAfterEdit;
   TLabel *CoercedSampleRateLabel;
   TPanel *Panel2;
   TProgressBar *ProgressBar1;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *RunNumberLabel;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TChart *Chart1;
   TGroupBox *GroupBox1;
   TLabel *Label11;
   TLabel *AvgSampsLabel;
   TLabel *Label13;
   TLabel *Label14;
   TLabel *AvgusDuringLabel;
   TLabel *MaxusDuringLabel;
   TLabel *Label17;
   TLabel *Label18;
   TLabel *TotalSampsReqLabel;
   TLabel *Label12;
   TLabel *TotalSampsReadLabel;
   TLabel *TotalReadsLabel;
   TMultiPLOTPanel *MultiPLOTPanel1;
   TBarSeries *Series1;
   TActionToolBar *ActionToolBar1;
   TActionManager *ActionManager1;
   TAction *StartAction;
   TAction *StopAction;
   TAction *CloseAction;
   TImageList *ImageList1;
   TAction *Action1;
   TPanel *Panel3;
   TLabel *Label16;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TListBox *PlotsDisplayedListBox;
   TLabel *Label19;
   TImageList *ImageList2;
   TCGMComponent *CGMComponent1;
   TSaveDialog *SaveDialog1;
   TSaveDialog *SaveDialog2;
   TPanel *Panel7;
   TCheckBox *ResetAfterRunCheckBox;
   TLabel *Label15;
   TEdit *RepeatEdit;
   TPanel *Panel8;
   TCheckBox *SaveDataCheckBox;
   TButton *FileParamsButton;
   TPanel *Panel9;
   TButton *CGM_Button;
   TEdit *CGMIncrementEdit;
   TButton *PS_Button;
   TLabel *Label20;
   TLabel *Label21;
   TLabel *Label22;
   TListBox *CellsPlotsListBox;
   TSplitter *Splitter3;
	TSpeedButton *AddToDisplayButton;
	TSpeedButton *RemoveFromDisplayButton;
	TSpeedButton *ClearDisplayListButton;
	TLabel *Label24;
   TLabel *Label25;
   TPanel *Panel10;
   TPanel *Panel11;
   TEdit *MaxRK4TimestepEdit;
   TPanel *Panel12;
   TPanel *Panel13;
   TLabel *Label26;
   TListBox *DataLoggingListBox;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   TPanel *Panel14;
   TPanel *Panel15;
   TLabel *Label27;
   TListBox *DataLogsPlotsListBox;
   TLabel *Label23;
   TListBox *ElectrodesPlotsListBox;
   TCheckBox *InterpolateCheckBox;
   TEdit *InterpolateRateEdit;
   TPanel *Panel16;
   TCheckBox *LogKineticParamsCheckBox;
   TButton *LogKineticParamsButton;
   TSaveDialog *SaveDialog3;
   void __fastcall StartActionUpdate(TObject *Sender);
   void __fastcall StopActionUpdate(TObject *Sender);
   void __fastcall StartActionExecute(TObject *Sender);
   void __fastcall StopActionExecute(TObject *Sender);
   void __fastcall CloseActionExecute(TObject *Sender);
   void __fastcall AddToDisplayButtonClick(TObject *Sender);
   void __fastcall RemoveFromDisplayButtonClick(TObject *Sender);
   void __fastcall ClearDisplayListButtonClick(TObject *Sender);
   void __fastcall CGM_ButtonClick(TObject *Sender);
   void __fastcall SaveDataCheckBoxClick(TObject *Sender);
   void __fastcall FileParamsButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall EditKeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall DataLoggingListBoxClick(TObject *Sender);
   void __fastcall InterpolateCheckBoxClick(TObject *Sender);
   void __fastcall InterpolateRateEditKeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall LogKineticParamsButtonClick(TObject *Sender);
   void __fastcall LogKineticParamsCheckBoxClick(TObject *Sender);


private:	// User declarations
   unsigned long NumRuns;
   unsigned long SamplesPerPlot;
   unsigned long NumPlotVals;
   double SampleTime, AvgSampleTime;
   double InterpolationRate;
   bool  DoInterpolate;
   UnicodeString FileLocation;
   UnicodeString LogLocation;
   TaskHandle  AI_Task, AO_Task;
   float64 SecondsBefore, SecondsDuring, SecondsAfter, TotalSeconds;
   std::string AI_ChanString;
   std::string AO_ChanString;
   uInt32 NumCells;
   uInt32 NumAIChans;
   uInt32 NumDataLogChans;
   uInt32 NumElectrodes;
   uInt32 NumAvailablePlots;
   int32 NumSamplesBefore,  NumSamplesBeforeAndDuring, NumSamplesBeforeAndDuringAndAfter;
   int NumRepeats;

   double *    ADCVolts;         // DAQmx read buffer
   double *    I_nA_DACVolts;    // DAQmx write buffer
   double *    mVCells;          // holds the converted Vm's from each cell
   double *    V_datalogs;       // holds the acquired voltages from arbitrary AI channels
   int32  *    ReadBins;         // how many samples occured between reads

   typedef std::pair< std::wstring, double * > UnitsDataPair;
	typedef std::map< std::wstring,  UnitsDataPair > PlotsMapType;
	PlotsMapType PlotsMap;

   void __fastcall FreeArrayMemory();
   int __fastcall RunDynamicClamp(float64 requestedSampleRate);
   void __fastcall RunTheModel(float64 requestedSampleRate);
   int32 __fastcall SignalProcess ();
   void __fastcall PackageTheData();
	void __fastcall PlotTheData();
   void __fastcall WriteTheData();
   bool __fastcall SetFileLocation();
   bool __fastcall SetKineticLogLocation();

   NetDescriptionStruct NetDescription;
   void __fastcall NewNetworkPlots(int numcells, int numelectrodes);
#define MAXPLOTS 256
   double *PlotValues[MAXPLOTS];
   bool Terminated; // controls loops and start and stop button appearance
public:		// User declarations
   float64 SampleRate; // public because needed by TPlaybackCell
   void __fastcall LetErRip();
   void __fastcall TurnOffDACs();
   __fastcall TRunDynamicClampForm(TComponent* Owner);
   __fastcall ~TRunDynamicClampForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TRunDynamicClampForm *RunDynamicClampForm;
//---------------------------------------------------------------------------
#endif
