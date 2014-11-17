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

#include <vcl.h>
#pragma hdrstop

#include "GUI_VoltageClampFitForm.h"
#include "GUI_RT_Edit_HHCurrent.h" 		// so we can hide some controls not used
#include "GUI_PlaybackWaveformForm.h"  // so we can hide some controls not used

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVoltageClampFitForm *VoltageClampFitForm;
//---------------------------------------------------------------------------
__fastcall TVoltageClampFitForm::TVoltageClampFitForm(TComponent* Owner)
	: TForm(Owner), origdata(NULL), genrdata(NULL), mf(NULL)
{
	HHModel.Current->AttachEditFormTo(this, ScrollBox2);
	HHModel.Waveform->AttachEditFormTo(this, ScrollBox1);

	// hide some controls that might be confusing -- not used in this app
	THHCurrentForm *curform =
		(THHCurrentForm *)HHModel.Current->GetEditForm();
	TPlaybackWaveformForm *pbwaveform =
		(TPlaybackWaveformForm *)HHModel.Waveform->GetEditForm();

	curform->ParamLoggingCheckBox->Visible = false;
	curform->PeriodEditButton->Visible = false;
	curform->PeriodicCheckBox->Visible = false;
	pbwaveform->RepeatsRadioGroup->Visible = false;
	pbwaveform->FixedRepeatsEdit->Visible = false;
	pbwaveform->Label8->Visible = false;
	pbwaveform->SamplingSettingsGroupBox->Visible = false;

	PageControl1->ActivePage = TabSheet1;
}

__fastcall TVoltageClampFitForm::~TVoltageClampFitForm()
{
	if (origdata) {
		delete[] origdata;
	}
	if (genrdata) {
		delete[] genrdata;
	}
	if (mf) {
		delete mf;
	}
}

//---------------------------------------------------------------------------
void __fastcall TVoltageClampFitForm::GenerateButtonClick(TObject *Sender)
{
	long numpoints = HHModel.Waveform->GetNumPoints();
	double samprate = HHModel.Waveform->GetPlaybackRate();
	int numepis = HHModel.Waveform->GetNumEpisodes();
	int numptsperepis = numpoints/numepis;

	HHModel.Setup(CChanComboBox->ItemIndex, VChanComboBox->ItemIndex);

	PlotPanel1->ClearPlots();

	if (origdata) {
		delete[] origdata;
	}
	if (genrdata) {
		delete[] genrdata;
	}

	origdata = new double[numpoints];
	genrdata = new double[numpoints];

	// load array with current data
	double *dataptr = origdata;
	for (long i=0; i < numpoints; i++) {
		*dataptr++ = HHModel.currdata[i];
	}

	// make plots of currents (episodes) in gray for contrast
	PlotPanel1->BeginUpdate();

	for (int i=0; i < numepis; i++) {
		TStaticFunctionPlot *DataPlot;
		DataPlot = new TStaticFunctionPlot(PlotPanel1,
														i, "",
														clGray, i*HHModel.Waveform->GetEpisodeSpacing(),
														samprate,
														numptsperepis,
														&origdata[i*numptsperepis]);
		PlotPanel1->AddPlot(DataPlot, false);
	}

	PlotPanel1->ConcatenatePlots = ConcatEpisCheckBox->Checked;

	PlotPanel1->ScalePlots(true);

	PlotPanel1->EndUpdate();

	TCursor old_cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	Application->ProcessMessages();

	// generate current data
	HHModel.iGen(HHModel.fitdata);


	PlotPanel1->BeginUpdate();

	// copy to array
	//reset dataptr
	dataptr = genrdata;
	std::vector<double>::const_iterator fititer = HHModel.fitdata.begin();
	for (int i=0; i < numepis; i++) {
		for (int j = 0; j < numptsperepis; j++) {
			*dataptr++ = *fititer++;
		}

		TStaticFunctionPlot *DataPlot;
		DataPlot = new TStaticFunctionPlot(PlotPanel1,
														i+numepis, "",
														clRed, i*HHModel.Waveform->GetEpisodeSpacing(),
														samprate,
														numptsperepis,
														&genrdata[i*numptsperepis]);
		PlotPanel1->AddPlot(DataPlot, false);
	}
	PlotPanel1->ScalePlots(true);
	PlotPanel1->EndUpdate();

	Screen->Cursor = old_cursor;

	FilenameLabel->Caption = HHModel.Waveform->GetFileName().c_str();
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::PageControl1Change(TObject *Sender)
{
	if (PageControl1->ActivePage == TabSheet3) {
		if (FilenameLabel->Caption != HHModel.Waveform->GetFileName().c_str()) {
			VChanComboBox->Clear();
			CChanComboBox->Clear();
			for (int i=0; i<HHModel.Waveform->GetNumChans(); i++) {
				VChanComboBox->Items->Add(HHModel.Waveform->GetChannelNames()[i].c_str());
				CChanComboBox->Items->Add(HHModel.Waveform->GetChannelNames()[i].c_str());
			}
			for (int i = 0; i< HHModel.NumParams; i++) {
				ValueListEditor1->Values[HHModel.ParamDesc[i].c_str()] = HHModel.DoFit[i];
			}
		}
	}
	if (PageControl1->ActivePage == TabSheet2) {
      HHModel.Current->PopulateEditForm();
	}
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::PageControl1Changing(TObject *Sender, bool &AllowChange)

{
	if (PageControl1->ActivePage == TabSheet2) {
		AllowChange = HHModel.Current->ValidateEditForm();
	}
}
//---------------------------------------------------------------------------


void __fastcall TVoltageClampFitForm::SaveButtonClick(TObject *Sender)
{
	long numpoints = HHModel.Waveform->GetNumPoints();
	double samprate = HHModel.Waveform->GetPlaybackRate();
	int numepis = HHModel.Waveform->GetNumEpisodes();
	int numptsperepis = numpoints/numepis;

	if (SaveDialog1->Execute()) {
		AnsiString filename(SaveDialog1->FileName);
		int hFile;
		HANDLE hHandle;
		int nError = 0;
		DWORD dwEpisodes, dwSamples;

		short *abfdata = new short[numpoints * 3];
		short *pabfdata = abfdata;

		ABFFileHeader *pFH = new ABFFileHeader;
		ABFH_Initialize( pFH );

		if (!ABF_WriteOpen( filename.c_str(), &hFile, ABF_DATAFILE, pFH, &nError ) ){
			return;
		}

		pFH->nOperationMode = 5; //episodic
		pFH->nADCNumChannels = 3; // 1 voltage, 2 current (orig & generated)
		pFH->fADCSequenceInterval = samprate * 1000.0;
		pFH->lNumSamplesPerEpisode = 3*numptsperepis;
		pFH->fADCRange = 10.0;
		pFH->lADCResolution = 32768;
		pFH->lActualAcqLength = 3 * numpoints * samprate / 1000.0;
		pFH->lActualEpisodes = numepis;
		pFH->fSecondsPerRun = numpoints * samprate / 1000.0;
		pFH->lEpisodesPerRun = numepis;
		pFH->lRunsPerTrial = 1;
		pFH->lNumberOfTrials = 1;
		pFH->fEpisodeStartToStart = HHModel.Waveform->GetEpisodeSpacing() / 1000.0;

		for (int i=0; i<16; i++) {
			pFH->nADCSamplingSeq[i] = i;
			pFH->fInstrumentScaleFactor[i] = 0.01;
		}

		strncpy(pFH->sADCChannelName[0], "Voltage", ABF_ADCNAMELEN);
		strncpy(pFH->sADCUnits[0], "mV", ABF_ADCUNITLEN);
		strncpy(pFH->sADCChannelName[1], "Orig Current", ABF_ADCNAMELEN);
		strncpy(pFH->sADCUnits[1], "nA", ABF_ADCUNITLEN);
		strncpy(pFH->sADCChannelName[2], "Genr Current", ABF_ADCNAMELEN);
		strncpy(pFH->sADCUnits[2], "nA", ABF_ADCUNITLEN);

		std::vector<double> voltdata;
		HHModel.Waveform->GetChannelData(VChanComboBox->ItemIndex, voltdata);
		std::vector<double>::const_iterator voltiter = voltdata.begin();

		abfdata = &pabfdata[0]; //set mux correctly
		double factor = 32.768;

		// copy/convert mV to shorts and put in data array
		for (unsigned long j = 0; j < numpoints ; j++) {
			*abfdata = *voltiter * factor;
			voltiter++;
			abfdata += 3;
		}

		for (int i = 1; i < 3; i++) {
			double *plotdata;
			switch (i) {
			case 1: plotdata = origdata; break;
			case 2: plotdata = genrdata; break;
			default:
				;
			}
			abfdata = &pabfdata[i]; //set mux correctly
			for (unsigned long j = 0; j < numpoints ; j++) {
				*abfdata = *plotdata * factor;
				plotdata++;
				abfdata += 3;
			}
		}

		if (!ABF_WriteRawData( hFile, pabfdata, 3*numpoints*2, &nError )){
			return;
		}

		if (!ABF_UpdateHeader( hFile, pFH, &nError )){
			return;
		}
		if (!ABF_Close( hFile, &nError )){
			return;
		}

		delete[] pabfdata;
	}
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::OptimizeButtonClick(TObject *Sender)
{
	GenerateButtonClick(NULL); // need to call in case user forgot

	TCursor old_cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;
	Application->ProcessMessages();

	for (int i = 0; i< HHModel.NumParams; i++) {
		HHModel.DoFit[i] = StrToInt(ValueListEditor1->Values[HHModel.ParamDesc[i].c_str()]);
	}

	HHModel.Setup(CChanComboBox->ItemIndex, VChanComboBox->ItemIndex);

	if (mf) delete mf;
	mf = new ModelFittingModule();
	if (mf->Fit(HHModel) > 0){
		int numpoints = HHModel.NumEpisodes * HHModel.NumPointsPerEpisode;

		for (int i=0; i < numpoints; i++) {
			genrdata[i] = HHModel.fitdata[i];
		}
		PlotPanel1->ScalePlots(true);
	}
	else Application->MessageBox(L"Try limiting the number of varying parameters.", L"Math error");

	Screen->Cursor = old_cursor;
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::ConcatEpisCheckBoxClick(TObject *Sender)
{
	PlotPanel1->ConcatenatePlots = ConcatEpisCheckBox->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::StopButtonClick(TObject *Sender)
{
	mf->Terminated = true;
}
//---------------------------------------------------------------------------

void __fastcall TVoltageClampFitForm::SaveParamsButtonClick(TObject *Sender)
{

	if (SaveDialog2->Execute()) {
		FILE *outfile = fopen(AnsiString(SaveDialog2->FileName).c_str(),"wt");
      fprintf(outfile, "Param Desc.\tFitted = 1\tValue\n");
		for (int i = 0; i<HHModel.NumParams; i++) {
			fprintf(outfile, "%s\t%d\t%8.4f\n", HHModel.ParamDesc[i].c_str(), HHModel.DoFit[i],
																	HHModel.FitParams[i]);
		}
		fclose(outfile);
	}
}
//---------------------------------------------------------------------------

