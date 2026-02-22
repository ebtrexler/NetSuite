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

#include <vcl.h>
#pragma hdrstop

#include "GUI_RT_Edit_HHCurrent.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MultiPLOTPanel"
#pragma resource "*.dfm"
THHCurrentForm *HHCurrentForm;



//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- THHCurrentForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================


#pragma region GUI methods of THHCurrentForm

//---------------------------------------------------------------------------

/// Constructor that builds the form and plots to show params
__fastcall THHCurrentForm::THHCurrentForm(TComponent* Owner)
   : TForm(Owner), HHCurrent(NULL)
{
   double start_mV = -150.0;
   double mVperPt = 1.0;

   // IV curve
	PlotIV = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[0], 0, "",
                                 clBlack, start_mV, mVperPt,
                                 NumPoints,
                                 &dataIV[0]);
	MultiPlotPanel1->PLOTPanel[0]->AddPlot(PlotIV, false);



   // inf values
	Plotminf = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[1], 0, "",
                                 clRed, start_mV, mVperPt,
                                 NumPoints,
											&dataminf[0]);
	MultiPlotPanel1->PLOTPanel[1]->AddPlot(Plotminf, false);

	Plothinf = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[1], 0, "",
                                 clBlue, start_mV, mVperPt,
                                 NumPoints,
											&datahinf[0]);
	MultiPlotPanel1->PLOTPanel[1]->AddPlot(Plothinf, false);

	Plotninf = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[1], 0, "",
                                 clGreen, start_mV, mVperPt,
                                 NumPoints,
											&dataninf[0]);
	MultiPlotPanel1->PLOTPanel[1]->AddPlot(Plotninf, false);



   // Tau values
	Plottaum = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[2], 0, "",
                                 clRed, start_mV, mVperPt,
                                 NumPoints,
                                 &datataum[0]);
	MultiPlotPanel1->PLOTPanel[2]->AddPlot(Plottaum, false);

	Plottauh = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[2], 0, "",
                                 clBlue, start_mV, mVperPt,
                                 NumPoints,
                                 &datatauh[0]);
	MultiPlotPanel1->PLOTPanel[2]->AddPlot(Plottauh, false);

	Plottaun = new TStaticFunctionPlot(MultiPlotPanel1->PLOTPanel[2], 0, "",
                                 clGreen, start_mV, mVperPt,
                                 NumPoints,
                                 &datataun[0]);
	MultiPlotPanel1->PLOTPanel[2]->AddPlot(Plottaun, false);


   //Periodicity form
   CurrentPeriodForm = new TPeriodicityForm(this);

}
//---------------------------------------------------------------------------

/// Called when parameter changes need to be reflected in GUI
void __fastcall THHCurrentForm::UpdatePlots()
{
   double volt = -150.0;
	double mVperPt = 1.0;

   for (int v = 0; v < NumPoints; v++)
	{
      // make taus
		datataum[v] = HHCurrent->get_m().tau(volt);
		datatauh[v] = HHCurrent->get_h().tau(volt);
		datataun[v] = HHCurrent->get_n().tau(volt);

		// Make infs
		dataminf[v] = HHCurrent->get_m().inf(volt);
//		if (dataminf[v] < 0.0) {
//			dataminf[v] = 0.0;
//		}
		datahinf[v] = HHCurrent->get_h().inf(volt);
//		if (datahinf[v] < 0.0) {
//			datahinf[v] = 0.0;
//		}
		dataninf[v] = HHCurrent->get_n().inf(volt);
//		if (dataninf[v] < 0.0) {
//			dataninf[v] = 0.0;
//		}

      // Make IV
		double mp = HHCurrent->p <= 0 ? 1 : pow(dataminf[v],HHCurrent->p);
		double hq = HHCurrent->q <= 0 ? 1 : pow(datahinf[v],HHCurrent->q);
		double nr = HHCurrent->r <= 0 ? 1 : pow(dataninf[v],HHCurrent->r);
		dataIV[v] = HHCurrent->Gmax*mp*hq*nr;
		//added 2014-07-09 to accomodate piecewise linear current
		if (UseVdrvComboBox->Checked){
			dataIV[v] = dataIV[v]*(volt - HHCurrent->E);
		}

      volt += mVperPt;
	}

	MultiPlotPanel1->PLOTPanel[0]->ScalePlots(true);
	MultiPlotPanel1->PLOTPanel[1]->ScalePlots(true);
	MultiPlotPanel1->PLOTPanel[2]->ScalePlots(true);
}
//---------------------------------------------------------------------------

/// Calls UpdatePlots to reflect changes if valid, otherwise no changes accepted
void __fastcall THHCurrentForm::ParamsEditors_KeyPress(TObject *Sender, wchar_t &Key)
{
   if (Key == VK_RETURN) {
      HHCurrent->ValidateEditForm();
      UpdatePlots();
      Key = 0;
   }
}
//---------------------------------------------------------------------------

/// Opens saved parameters from dialog-chosen file
void __fastcall THHCurrentForm::LoadButtonClick(TObject *Sender)
{
	OpenDialog1->InitialDir = ExtractFilePath(Application->ExeName)  + L"currents";
	if (OpenDialog1->Execute()){
      std::ifstream ifs(OpenDialog1->FileName.c_str());
      if (ifs.good()){
         ifs >> *HHCurrent;
         HHCurrent->PopulateEditForm();
      }
      else Application->MessageBox(L"Unable to open file for reading", L"I-O Error");
   }
}
//---------------------------------------------------------------------------

/// Saves parameters to dialog-chosen file
void __fastcall THHCurrentForm::SaveButtonClick(TObject *Sender)
{
	SaveDialog1->InitialDir = ExtractFilePath(Application->ExeName) + L"currents";
	if (SaveDialog1->Execute()){
		// make an archive
      std::ofstream ofs(SaveDialog1->FileName.c_str());
      if (ofs.good()){
         ofs << *HHCurrent;
      }
      else Application->MessageBox(L"Unable to open file for writing", L"I-O Error");
   }
}
//---------------------------------------------------------------------------


void __fastcall THHCurrentForm::PeriodEditButtonClick(TObject *Sender)
{
//   HHCurrent->PopulateEditForm();
   if (CurrentPeriodForm->ShowModal() == mrOk){
      HHCurrent->ValidateEditForm();
   }
}
//---------------------------------------------------------------------------


#pragma end_region

void __fastcall THHCurrentForm::UseVdrvComboBoxClick(TObject *Sender)
{
	UpdatePlots();
}
//---------------------------------------------------------------------------

