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

#include <fstream>
#include <math.h>
#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include "GUI_RunModelForm.h"

#include "MonotCubicInterpolator.h"
#include "abffiles.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MultiPLOTPanel"
#pragma link "VCLTee.Chart"
#pragma link "VCLTee.TeEngine"
#pragma link "VCLTee.TeeProcs"
#pragma link "VCLTee.Series"
#pragma link "CGMComponent"
#pragma resource "*.dfm"
TRunDynamicClampForm *RunDynamicClampForm;
//---------------------------------------------------------------------------
__fastcall TRunDynamicClampForm::TRunDynamicClampForm(TComponent* Owner)
   : TForm(Owner)
{
   SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
   SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

   SampleRate = 50000;
   SecondsBefore = 0;
   SecondsDuring = 0;
   SecondsAfter = 0;
   NumRepeats = 1;
   InterpolationRate = 5000;
   DoInterpolate = true;
   for (int i=0; i < MAXPLOTS; i++) {
      PlotValues[i] = NULL;
   }
   Terminated = true;

   FileLocation = L"";
   LogLocation = L"";

   DataLoggingListBox->Clear();

   ReadBins = NULL;
   mVCells = NULL;
   ADCVolts = NULL;
   I_nA_DACVolts = NULL;
   V_datalogs = NULL;

#ifndef SIM_ONLY_NO_NIDAQ
   for (int i=0; i<GetNIDAQmxSetup().NumDevices; i++){
      TNIDAQmxDevice *dev = GetNIDAQmxSetup().Devices[i];
      DataLoggingListBox->Items->AddStrings(dev->AIChannels);
	}
#else  // no need to interpolate in NetSim
   InterpolateCheckBox->Visible = false;
   InterpolateRateEdit->Visible = false;
   DoInterpolate = false;
#endif
}
//---------------------------------------------------------------------------
__fastcall TRunDynamicClampForm::~TRunDynamicClampForm()
{
   FreeArrayMemory();
   for (int i=0; i < MAXPLOTS; i++) {
      if (PlotValues[i] != NULL) delete[] PlotValues[i];
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::FreeArrayMemory()
{
   if (ReadBins) delete[] ReadBins;
   if (mVCells) delete[] mVCells;
   if (ADCVolts) delete[] ADCVolts;
   if (I_nA_DACVolts) delete[] I_nA_DACVolts;

   if (NumDataLogChans > 0) {
      if (V_datalogs) delete[] V_datalogs;
   }

   ReadBins = NULL;
   mVCells = NULL;
   ADCVolts = NULL;
   I_nA_DACVolts = NULL;
   V_datalogs = NULL;
}
//---------------------------------------------------------------------------

// called by LetErRip to populate PlotsDisplayedListBox
void __fastcall TRunDynamicClampForm::NewNetworkPlots(int numcells, int numelectrodes)
{
   // add as many cells as possible
   int count = std::min(numcells, MAXPANELS);
   for (int i = 0; i < count; i++) {
      PlotsDisplayedListBox->Items->Add(CellsPlotsListBox->Items->Strings[i]);
   }

   // add electrodes to limit set by MAXPANELS
   if (count < MAXPANELS) {
      int ecount = std::min(numelectrodes, MAXPANELS - count);
      for (int i = 0; i < ecount; i++) {
         PlotsDisplayedListBox->Items->Add(ElectrodesPlotsListBox->Items->Strings[i]);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::LetErRip()
{
   MaxRK4TimestepEdit->Text = FloatToStr(GetNet()->GetMaxRK4Timestep());
   TimeBeforeEdit->Text = FloatToStr(SecondsBefore);
   DurationEdit->Text = FloatToStr(SecondsDuring);
   TimeAfterEdit->Text = FloatToStr(SecondsAfter);
   SampleRateEdit->Text = FloatToStr(SampleRate);
   InterpolateRateEdit->Text = FloatToStr(InterpolationRate);
   InterpolateCheckBox->Checked = DoInterpolate;
   CoercedSampleRateLabel->Caption = "We'll see...";
   RepeatEdit->Text = NumRepeats;

   int numcells = 0;
   int numelectrodes = 0;

   CellsPlotsListBox->Clear();
   ElectrodesPlotsListBox->Clear();

   // first add cells (voltages)
   TCellsMapConstIterator citer = GetNet()->GetCells().begin();
   while (citer != GetNet()->GetCells().end()) {
		if (citer->second->IsActive()){
      	CellsPlotsListBox->Items->Add(citer->first.c_str());
			numcells++;
		}
		citer++;
	}  ;

   // second, add electrodes (currents)
   TElectrodesMapConstIterator eiter = GetNet()->GetElectrodes().begin();
	while (eiter != GetNet()->GetElectrodes().end()) {
		if (eiter->second->IsActive()) {
      	ElectrodesPlotsListBox->Items->Add(eiter->first.c_str());
			numelectrodes++;
		}
		eiter++;
	}  ;

//   PlotsDisplayedListBox->Clear();
//   NewNetworkPlots(numcells, numelectrodes);

   // now worry about which plots were chosen before, if previous runs
   if (PlotsDisplayedListBox->Items->Count == 0) { // this means no previous runs
      // populate with network plots
      NewNetworkPlots(numcells, numelectrodes);
   }
   else {  // previous run, but perhaps cells or electrodes removed from network
      int count = PlotsDisplayedListBox->Items->Count;
      std::vector<int> plotstodelete;
      for (int i=0; i < count; i++) {
         if ((CellsPlotsListBox->Items->IndexOf(PlotsDisplayedListBox->Items->Strings[i]) == -1) &&
             (ElectrodesPlotsListBox->Items->IndexOf(PlotsDisplayedListBox->Items->Strings[i]) == -1)) {
            plotstodelete.push_back(i);
         }
      }
      std::vector<int>::reverse_iterator to_del_Iter();
      for (std::vector<int>::reverse_iterator to_del_Iter = plotstodelete.rbegin();
            to_del_Iter != plotstodelete.rend(); to_del_Iter++) {
         PlotsDisplayedListBox->Items->Delete(*to_del_Iter);
      }

      // now that old plots removed, repopulate with new network plots
//      NewNetworkPlots(numcells, numelectrodes);
   }

   if (!GetNet()->Initialize(true)) return; // at start of run everything should be reset

   Show();
}

void __fastcall TRunDynamicClampForm::RunTheModel(float64 requestedSampleRate)
{
   SampleRate = requestedSampleRate;
   CoercedSampleRateLabel->Caption = FloatToStr(SampleRate);

   // Calculate Sample periods based on requested values
	unsigned long NumSamplesDuring = ceil(SecondsDuring * SampleRate);

   AvgSampsLabel->Caption = L"N/A ... Model network";
   TotalSampsReqLabel->Caption = NumSamplesDuring;
   TotalSampsReadLabel->Caption = NumSamplesDuring;
   TotalReadsLabel->Caption = NumSamplesDuring;
   AvgusDuringLabel->Caption = L"N/A ... Model network";
   MaxusDuringLabel->Caption = L"N/A ... Model network";

   FreeArrayMemory();

   mVCells = new double[NumSamplesDuring*NetDescription.NumTimeCells];

   double step = 1000.0 / SampleRate;  // ms per step

   // Run nested loops for network of entirely time dependent cells
   int numReps = NumRepeats == 0 ? 1 : NumRepeats;
   int decrement = NumRepeats == 0 ? -1 : 1;
   do {
      double * mVptr = mVCells;

      Application->ProcessMessages();

      for (unsigned long i=0; i<NumSamplesDuring; i++) {
         GetNet()->Update(step, NULL, mVptr, NULL);
         mVptr += NetDescription.NumTimeCells;
         Application->ProcessMessages();
         if (Terminated) {
            break;
         }
      }

      Application->ProcessMessages();

      AvgSampleTime = 1.0 / SampleRate;
      SamplesPerPlot = NumSamplesDuring;
      NumCells = NetDescription.NumTimeCells;
      NumDataLogChans = 0;

      PackageTheData();
   	PlotTheData();
      WriteTheData();

      RunNumberLabel->Caption = IntToStr(numReps) + " of " + IntToStr(NumRepeats);
      numReps -= decrement;

      if (Terminated) {
         break;
      }

      bool doReset = ResetAfterRunCheckBox->Checked;
      GetNet()->Initialize(doReset); // for next rep

   } while (numReps > 0);

}

int __fastcall TRunDynamicClampForm::RunDynamicClamp(float64 requestedSampleRate)
{
#ifndef SIM_ONLY_NO_NIDAQ
   try {
      // at start of run everything should be reset
//      if (!TheNet->Initialize()) return -1;

      /*********************************************/
      // DAQmx Configure Code    -- Analog Input
      /*********************************************/

      TNIDAQmxSetup::DAQmxErrChk (DAQmxCreateTask ("FiniteSamplesInput", &AI_Task));
      TNIDAQmxSetup::DAQmxErrChk (DAQmxCreateAIVoltageChan(AI_Task, AI_ChanString.c_str(),"",DAQmx_Val_RSE ,-10.0,10.0,DAQmx_Val_Volts,NULL));
      TNIDAQmxSetup::DAQmxErrChk (DAQmxCfgSampClkTiming (AI_Task, "", requestedSampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, floor(SampleRate * 10)));
      TNIDAQmxSetup::DAQmxErrChk (DAQmxSetReadReadAllAvailSamp(AI_Task, true));

      //Get real (coerced) sample rate
      TNIDAQmxSetup::DAQmxErrChk (DAQmxGetSampClkRate(AI_Task, &SampleRate));
      CoercedSampleRateLabel->Caption = FloatToStr(SampleRate);

      //Get the number of AI channels  - from network and from dataloggers combined
      TNIDAQmxSetup::DAQmxErrChk (DAQmxGetReadNumChans(AI_Task, &NumAIChans));

      /*********************************************/
      // DAQmx Configure Code    -- Analog Output
      /*********************************************/

      TNIDAQmxSetup::DAQmxErrChk (DAQmxCreateTask("SinglePointOutput",&AO_Task));
#ifdef USB6009
      TNatInstSetup::DAQmxErrChk (DAQmxCreateAOVoltageChan (AO_Task, AO_ChanString, "", 0, 5.0, DAQmx_Val_Volts, ""));
#else
      int AOerr = DAQmxCreateAOVoltageChan (AO_Task, AO_ChanString.c_str(), "", -10.0, 10.0, DAQmx_Val_Volts, "");
      if (AOerr == -200219) {  // ao channels in wrong order
         Application->MessageBox(L"The Analog Out (ao) channels need to be in physical order. \
Try renaming the cell that uses ao0 with an underscore at the beginning.  \
This will ensure proper order for devices with 2 ao channels.",
                                 L"AO Channels in wrong order...");
      }
		else TNIDAQmxSetup::DAQmxErrChk (AOerr);
#endif


/* DONE -oebt : Do we need input/output synched with the same clock?  I doubt it */
//#define USE_AI_CLOCK_FOR_AO

#ifdef USE_AI_CLOCK_FOR_AO
		/* DONE : Warning -- only this routing is possible, for now,
			for a single device. Must alter to accomodate multiple devices */

		//get device name
      UnicodeString temp = GetNIDAQmxSetup().Devices[0]->DeviceName;
      temp = "/"+temp+"/ai/SampleClock";
      TNIDAQmxSetup::DAQmxErrChk (DAQmxCfgSampClkTiming (AO_Task, temp.t_str(), SampleRate, DAQmx_Val_Rising, DAQmx_Val_HWTimedSinglePoint , 5000));

#else
      double MaxDACRate;
      TNIDAQmxSetup::DAQmxErrChk (DAQmxGetSampClkMaxRate(AO_Task, &MaxDACRate));
      TNIDAQmxSetup::DAQmxErrChk (DAQmxCfgSampClkTiming (AO_Task, "", MaxDACRate, DAQmx_Val_Rising, DAQmx_Val_HWTimedSinglePoint , 5000));
#endif

      // Call the signal acquisition and processing function
      int numReps = NumRepeats == 0 ? 1 : NumRepeats;
      int decrement = NumRepeats == 0 ? 0 : 1;
      do {
         SignalProcess();
         RunNumberLabel->Caption = IntToStr(numReps) + " of " + IntToStr(NumRepeats);
         numReps -= decrement;
         bool doReset = ResetAfterRunCheckBox->Checked;
         GetNet()->Initialize(doReset); // for next rep
      } while (numReps > 0);


      DAQmxStopTask(AI_Task);
      DAQmxClearTask(AI_Task);
      DAQmxStopTask(AO_Task);
      DAQmxClearTask(AO_Task);

   }
   catch (const EDAQmxException &e)
   {
      Application->ShowException(&e);
         //cleanup)
      if( AI_Task!=0 ) {
         /*********************************************/
         // DAQmx Stop Code
         /*********************************************/
         DAQmxStopTask(AI_Task);
         DAQmxClearTask(AI_Task);
      }
      if( AO_Task!=0 ) {
         /*********************************************/
         // DAQmx Stop Code
         /*********************************************/
         DAQmxStopTask(AO_Task);
         DAQmxClearTask(AO_Task);
      }
   }

#endif //#ifndef SIM_ONLY_NO_NIDAQ

	return 0;
}

void __fastcall TRunDynamicClampForm::TurnOffDACs ()
{
#ifndef SIM_ONLY_NO_NIDAQ
   double *DACVolts = new double[NetDescription.NumVDepCells];
   int32 written;

// turn "OFF" DAC outputs
   memset(DACVolts, 0, sizeof(double)*NetDescription.NumVDepCells);
   TNIDAQmxSetup::DAQmxErrChk (DAQmxWriteAnalogF64 (AO_Task, 1, false, 10.0, DAQmx_Val_GroupByScanNumber, DACVolts, &written, NULL));
   delete[] DACVolts;
#endif //#ifndef SIM_ONLY_NO_NIDAQ
}


int32 __fastcall TRunDynamicClampForm::SignalProcess ()
{
#ifndef SIM_ONLY_NO_NIDAQ

   ProgressBar1->Position = 0;
   Application->ProcessMessages();

	int32  		totalRead=0;
	int32  		count = 0, readCount = 0;
	int32       read=0, written = 0;

   float64     avgpresamps, avgdursamps, avgaftsamps;

   // need the total number of cells in network
   NumCells = NetDescription.NumVDepCells + NetDescription.NumTimeCells;

	// Calculate Sample periods based on requested values
	NumSamplesBefore = ceil(SecondsBefore * SampleRate);
	NumSamplesBeforeAndDuring = NumSamplesBefore + ceil(SecondsDuring * SampleRate);
	NumSamplesBeforeAndDuringAndAfter = NumSamplesBeforeAndDuring + ceil(SecondsAfter *SampleRate);
	TotalSeconds = SecondsBefore + SecondsDuring +SecondsAfter;

   double mspersample = 1000.0 / SampleRate;

   try {
      FreeArrayMemory();

      ReadBins = new int32[NumSamplesBeforeAndDuringAndAfter];

      // give 10 ms buffer for read
      int ReadBufferSize = ceil(0.01*SampleRate*NumAIChans);
      ADCVolts = new double[ReadBufferSize];

      mVCells = new double[NumSamplesBeforeAndDuringAndAfter*NumCells];

      if (NumDataLogChans > 0) {
         V_datalogs = new double[NumSamplesBeforeAndDuringAndAfter*NumDataLogChans];
      }

      I_nA_DACVolts = new double[NetDescription.NumVDepCells];

      memset(I_nA_DACVolts, 0, sizeof(double)*NetDescription.NumVDepCells);
      memset(ADCVolts, 0, sizeof(double)*ReadBufferSize);


      double * mVptr = mVCells;
      double * Vdataptr = V_datalogs;

      /*********************************************/
      // DAQmx Start Code
      /*********************************************/
      TNIDAQmxSetup::DAQmxErrChk (DAQmxStartTask(AO_Task));

      TNIDAQmxSetup::DAQmxErrChk (DAQmxWriteAnalogF64 (AO_Task, 1, false, 10.0, DAQmx_Val_GroupByScanNumber, I_nA_DACVolts, &written, NULL));

      TNIDAQmxSetup::DAQmxErrChk (DAQmxStartTask(AI_Task));

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//make a loop for the samples preceding the dynamic clamp period
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
      count = 0;
      do {
         /*********************************************/
         // DAQmx Read Code
         /*********************************************/
         TNIDAQmxSetup::DAQmxErrChk (DAQmxReadAnalogF64 (AI_Task, -1, 10.0, DAQmx_Val_GroupByScanNumber, ADCVolts, ReadBufferSize, &read, NULL));
         count++;
         if( read>0 ) {

            // for tracking read efficiency and interrupts
            ReadBins[readCount++] = read;

            totalRead += read;
            Application->ProcessMessages();
            if (Terminated) {
               TurnOffDACs();
               return -1;
            }
         /*********************************************/
         // Update the network
         /*********************************************/
            // step is in ms
            double step = read * mspersample;

            // send the last sample from the ADCs, disregard previous samples
            double * VoltsIn = &ADCVolts[(read-1)*NumAIChans];//NetDescription.NumVDepCells];

            // Call TheNet->Update to SetVm() and CalcVm() appropriately
            // I_nA_DACVolts is ignored, and will not be written in this period
            GetNet()->Update(step, VoltsIn, mVptr, I_nA_DACVolts);

            // move to next segment of mVptr (or mVCells).
            mVptr += NumCells;

            // Copy data log samples to storage
            if (NumDataLogChans > 0) {
               // need pointer to correct sample
               memcpy(Vdataptr, &VoltsIn[NetDescription.NumVDepCells], sizeof(double) * NumDataLogChans);
               Vdataptr += NumDataLogChans;
            }

         }
      } while (totalRead < NumSamplesBefore);
      avgpresamps = NumSamplesBefore;
      avgpresamps /= count;
      ProgressBar1->Position = 30;
      Application->ProcessMessages();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//make a loop for the samples during the dynamic clamp period
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//      readCount = 0;
      count = 0;
      double mspersample = 1000.0 / SampleRate;
      do {

         /*********************************************/
         // DAQmx Read Code
         /*********************************************/
         TNIDAQmxSetup::DAQmxErrChk (DAQmxReadAnalogF64 (AI_Task, -1, 10.0, DAQmx_Val_GroupByScanNumber, ADCVolts, ReadBufferSize, &read, NULL));
         count++;
         if( read>0 ) {

            // for tracking read efficiency and interrupts
            ReadBins[readCount++] = read;

            totalRead += read;
            Application->ProcessMessages();


         /*********************************************/
         // Update the network
         /*********************************************/
            // step is in ms
            double step = read * mspersample;

            // send the last sample from the ADCs, disregard previous samples
            double * VoltsIn = &ADCVolts[(read-1)*NumAIChans];//NetDescription.NumVDepCells];

            // Get the Currents to apply (in volts) from Update
            GetNet()->Update(step, VoltsIn, mVptr, I_nA_DACVolts);

            // move to next segment of mVptr (or mVCells).
            mVptr += NumCells;

            // Copy data log samples to storage
            if (NumDataLogChans > 0) {
               // need pointer to correct sample
               memcpy(Vdataptr, &VoltsIn[NetDescription.NumVDepCells], sizeof(double) * NumDataLogChans);
               Vdataptr += NumDataLogChans;
            }


         /*********************************************/
         // DAQmx Write Code
         /*********************************************/

         // check for exceeding DAC limits
            double *Iptr = I_nA_DACVolts;
            for (int i=0; i<NetDescription.NumVDepCells; i++){
               if (*Iptr > 10.0000) {
                  *Iptr = 10.0000;
               }
               if (*Iptr < -10.0000) {
                  *Iptr = -10.0000;
               }
               Iptr++;
            }

            TNIDAQmxSetup::DAQmxErrChk (DAQmxWriteAnalogF64 (AO_Task, 1, false, 10.0, DAQmx_Val_GroupByScanNumber, I_nA_DACVolts, &written, NULL));
            if (Terminated) {
               TurnOffDACs();
               return -1;
            }
         }
      } while (totalRead < NumSamplesBeforeAndDuring);
      avgdursamps = NumSamplesBeforeAndDuring - NumSamplesBefore;
      avgdursamps /= count;
      ProgressBar1->Position = 70;
      Application->ProcessMessages();

      TurnOffDACs();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//make a loop for the samples after the dynamic clamp period
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
      count = 0;
      do {
         /*********************************************/
         // DAQmx Read Code
         /*********************************************/
         TNIDAQmxSetup::DAQmxErrChk (DAQmxReadAnalogF64 (AI_Task, -1, 10.0, DAQmx_Val_GroupByScanNumber, ADCVolts, ReadBufferSize, &read, NULL));
         count++;
         if( read>0 ) {

         // for tracking read efficiency and interrupts
            ReadBins[readCount++] = read;

            totalRead += read;
            Application->ProcessMessages();
            if (Terminated) {
               TurnOffDACs();
               return -1;
            }

         /*********************************************/
         // Update the network
         /*********************************************/
            // step is in ms
            double step = read * mspersample;

            // send the last sample from the ADCs, disregard previous samples
            double * VoltsIn = &ADCVolts[(read-1)*NumAIChans];//NetDescription.NumVDepCells];

            // Call TheNet->Update to SetVm() and CalcVm() appropriately
            // I_nA_DACVolts is ignored, and will not be written in this period
            GetNet()->Update(step, VoltsIn, mVptr, I_nA_DACVolts);

            // move to next segment of mVptr (or mVCells).
            mVptr += NumCells;

            // Copy data log samples to storage
            if (NumDataLogChans > 0) {
               // need pointer to correct sample
               memcpy(Vdataptr, &VoltsIn[NetDescription.NumVDepCells], sizeof(double) * NumDataLogChans);
               Vdataptr += NumDataLogChans;
            }

         }
      } while (totalRead < NumSamplesBeforeAndDuringAndAfter);
      avgaftsamps = NumSamplesBeforeAndDuringAndAfter - NumSamplesBeforeAndDuring;
      avgaftsamps /= count;

      ProgressBar1->Position = 100;
      Application->ProcessMessages();

      TNIDAQmxSetup::DAQmxErrChk (DAQmxStopTask(AI_Task));
      TNIDAQmxSetup::DAQmxErrChk (DAQmxStopTask(AO_Task));
   }
   catch (const Exception &e)
   {
      FreeArrayMemory();
      throw(e);
   }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Fill in run performance parameters
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
   UnicodeString avgsamps = "Pre: " + FloatToStrF(avgpresamps,ffFixed,4,4) +
                              ", During: " + FloatToStrF(avgdursamps,ffFixed,4,4) +
                              ", After: " + FloatToStrF(avgaftsamps,ffFixed,4,4);
   AvgSampsLabel->Caption = avgsamps;
   TotalSampsReqLabel->Caption = FloatToStr(TotalSeconds*SampleRate);
   TotalSampsReadLabel->Caption = FloatToStr(totalRead);
   TotalReadsLabel->Caption = FloatToStr(readCount);

   double temp = 0.0;
   double testbin ;
   double max = 0;
   double uspersample = (1.0e6/SampleRate);

   //Assign bin edges for histogram
   double bin_edges[14] = {0.0, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0, 200.0,
                                       500.0, 1000.0, 2000.0, 5000.0, 10000.0};
//   double bin_centers[13];
   unsigned long bins[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

   for (int line = 0; line < readCount; line++) {
      testbin = ReadBins[line] * uspersample;
      temp += testbin;            // store for average samp time later
      if (max < testbin) {
         max = testbin;           // store for max samp time
      }
      // make histogram
      for (int j=0; j < 13; j++) {
         if (testbin > bin_edges[j] && testbin <= bin_edges[j+1]) {
            bins[j]++;
         }
      }
   }

   Series1->Clear();
   for (int i=0; i<13; i++) {
//      bin_centers[i] = (bin_edges[i] + bin_edges[i+1]) / 2.0;
//      Series1->AddXY(bin_centers[i], bins[i], FloatToStr(bin_centers[i]));
      Series1->AddXY(bin_edges[i+1], bins[i], FloatToStr(bin_edges[i+1]));
   }

   temp /= readCount;
   AvgusDuringLabel->Caption = FloatToStrF(temp,ffFixed,4,4);
   AvgSampleTime = temp * 1e-6;   // convert to seconds for plotting

   temp = max;
   temp *= uspersample;
   MaxusDuringLabel->Caption = FloatToStrF(temp,ffFixed,6,6);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Plot the data
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
   SamplesPerPlot = readCount;

   PackageTheData();
	PlotTheData();
   WriteTheData();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Cleanup
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//   delete[] ReadBins;
//   delete[] mVCells;
//   delete[] ADCVolts;
//   delete[] I_nA_DACVolts;

#endif //#ifndef SIM_ONLY_NO_NIDAQ
   return 0;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::PackageTheData()
{
// this method places all available traces --
// mV from all cells, Volts from all data loggers, nA from all electrodes
// into PlotsMap that can be reused if user changes plotted channels or wishes
// to write to disk after an acquisition has already occurred.

   TCellsMapConstIterator celliter = GetNet()->GetCells().begin();
   TCellsMapConstIterator cells_end = GetNet()->GetCells().end();
   TElectrodesMapConstIterator trodeiter = GetNet()->GetElectrodes().begin();

   PlotsMap.clear();

   NumAvailablePlots = 0;

   MonotCubicInterpolator *terp = NULL;
   double *demux = NULL;
   double *time = NULL;
   unsigned long InterpSamps = 0;
   double interptime = 1000.0 / InterpolationRate;
   NumPlotVals = SamplesPerPlot;

   if (DoInterpolate) {
      InterpSamps =  ceil((SecondsBefore + SecondsDuring + SecondsAfter) * InterpolationRate);
      NumPlotVals =  InterpSamps;
      demux = new double[SamplesPerPlot];
      time = new double[SamplesPerPlot];
      double mspersamp = 1000.0 / SampleRate;
      time[0] = 0.0;
      for (unsigned long i = 1; i < SamplesPerPlot; i++) {
         time[i] = (ReadBins[i] * mspersamp) + time[i - 1];
      }
      // SampleTime is the interpolation rate
      SampleTime = 1.0 / InterpolationRate;
   }
   else SampleTime = AvgSampleTime;

   // de-multiplex voltage data from cells
   // voltage dependent cells first, then rest of cells
   while (celliter != cells_end) {
      if (celliter->second->IsActive()) {
         if (celliter->second->IsVoltageDependent()) {
            // handle memory for PlotValues
            if (PlotValues[NumAvailablePlots] != NULL) {
               delete[] PlotValues[NumAvailablePlots];
            }
            PlotValues[NumAvailablePlots] = new double[NumPlotVals];

            // Demux mVCells into PlotValues
            double * plotPtr = &mVCells[NumAvailablePlots];
            if (!DoInterpolate) {
               demux = PlotValues[NumAvailablePlots];
            }
            for (unsigned long j = 0; j < SamplesPerPlot; j++) {
               demux[j] = *plotPtr;
               plotPtr += NumCells;
            }

            if (DoInterpolate) {
               terp = new MonotCubicInterpolator(time, demux, SamplesPerPlot);
               for (unsigned long j = 0; j < InterpSamps; j++) {
                  PlotValues[NumAvailablePlots][j] = terp->evaluate(j*interptime);
               }
               delete terp;
            }

            UnitsDataPair udpair(L"mV",PlotValues[NumAvailablePlots]);
            PlotsMap[celliter->first] = udpair; // assign each channel's data to map
            NumAvailablePlots++;
         }
		}
		celliter++;
	}

   // demux rest of cells
   celliter = GetNet()->GetCells().begin(); //start over from beginning
   while (celliter != cells_end) {
      if (celliter->second->IsActive()) {
         if (!celliter->second->IsVoltageDependent()) { //not voltage dependent
            // handle memory for PlotValues
            if (PlotValues[NumAvailablePlots] != NULL) {
               delete[] PlotValues[NumAvailablePlots];
            }
            PlotValues[NumAvailablePlots] = new double[NumPlotVals];

            // Demux mVCells into PlotValues
            double * plotPtr = &mVCells[NumAvailablePlots];
            if (!DoInterpolate) {
               demux = PlotValues[NumAvailablePlots];
            }
            for (unsigned long j = 0; j < SamplesPerPlot; j++) {
               demux[j] = *plotPtr;
               plotPtr += NumCells;
            }

            if (DoInterpolate) {
               terp = new MonotCubicInterpolator(time, demux, SamplesPerPlot);
               for (unsigned long j = 0; j < InterpSamps; j++) {
                  PlotValues[NumAvailablePlots][j] = terp->evaluate(j*interptime);
               }
               delete terp;
            }

            UnitsDataPair udpair(L"mV",PlotValues[NumAvailablePlots]);
            PlotsMap[celliter->first] = udpair; // assign each channel's data to map
            NumAvailablePlots++;
         }
		}
		celliter++;
	}

   // de-multiplex voltage data from data loggers
   for (uInt32 i=0; i < NumDataLogChans; i++) {
      if (PlotValues[NumAvailablePlots] != NULL) {
         delete[] PlotValues[NumAvailablePlots];
      }
      PlotValues[NumAvailablePlots] = new double[NumPlotVals];
      double * plotPtr = &V_datalogs[i];
      if (!DoInterpolate) {
            demux = PlotValues[NumAvailablePlots];
         }
      for (unsigned long j = 0; j < SamplesPerPlot; j++) {
         demux[j] = *plotPtr;
         plotPtr += NumDataLogChans;
      }

      if (DoInterpolate) {
         terp = new MonotCubicInterpolator(time, demux, SamplesPerPlot);
         for (unsigned long j = 0; j < InterpSamps; j++) {
            PlotValues[NumAvailablePlots][j] = terp->evaluate(j*interptime);
         }
         delete terp;
      }

      UnitsDataPair udpair(L"V",PlotValues[NumAvailablePlots]);
      std::wstring name(DataLogsPlotsListBox->Items->Strings[i].c_str());
      PlotsMap[name] = udpair; // assign each channel's data to map
      NumAvailablePlots++;
	}

   // Now add electrodes to PlotsMap
   int numElectrodes = GetNet()->GetElectrodes().size();
   NumElectrodes = 0;
   for (int i=0; i < numElectrodes; i++) {
		if (trodeiter->second->IsActive()) {
			if (PlotValues[NumAvailablePlots] != NULL) {
				delete[] PlotValues[NumAvailablePlots];
			}
			PlotValues[NumAvailablePlots] = new double[NumPlotVals];
			trodeiter->second->Initialize(true);
			double trode_time = SampleTime*1000.0;
			for (unsigned long j = 0; j < NumPlotVals; j++) {
				PlotValues[NumAvailablePlots][j] = trodeiter->second->Update(trode_time);
			}
			UnitsDataPair udpair(L"nA",PlotValues[NumAvailablePlots]);
			PlotsMap[trodeiter->first] = udpair; // assign each electrode's data to map

         NumElectrodes++;
			NumAvailablePlots++;
		}

		trodeiter++;
	}

   if (DoInterpolate) {
      delete[] demux;
      delete[] time;
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::PlotTheData()
{
   TColor colors[12] = {clRed,  TColor(0x000080FF) , clYellow, clGreen, clBlue, clPurple,
							 clMaroon, clBlack, clNavy, clTeal, clFuchsia, clDkGray};


   // only draw chosen plots
   int count = PlotsDisplayedListBox->Items->Count;
   MultiPLOTPanel1->NumVisiblePlots = count;
   MultiPLOTPanel1->IsRealTimePlot = false;

	TStrings *plots = PlotsDisplayedListBox->Items;
	for (int i = 0; i < count; i++) {
		MultiPLOTPanel1->PLOTPanel[i]->ClearPlots();
		MultiPLOTPanel1->PLOTPanel[i]->XUnits = "seconds";
		std::wstring label = (PlotsMap[plots->Strings[i].c_str()].first + L" (");
		label += plots->Strings[i].c_str();
		label += L")";

		MultiPLOTPanel1->PLOTPanel[i]->YUnits = label.c_str();

		MultiPLOTPanel1->PLOTPanel[i]->AddPlot(
			new TStaticFunctionPlot(MultiPLOTPanel1->PLOTPanel[i], 0, "",
											colors[8], 0.0, SampleTime,
											NumPlotVals,
											PlotsMap[plots->Strings[i].c_str()].second),
											false);
		MultiPLOTPanel1->PLOTPanel[i]->ScalePlots(true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::WriteTheData()
{
   if (NumAvailablePlots == 0 || NumPlotVals == 0) {
      return;
   }

   // if requested, write all channels to disk
   if (SaveDataCheckBox->Checked) {
      if (FileLocation.Length() != 0) {

		 FormatSettings.DateSeparator = L'-';
		 FormatSettings.ShortDateFormat = L"yyyy/mm/dd";
		 FormatSettings.TimeSeparator = L'_';
         double *plotdata = NULL;
         double factor = 327.7;

         switch (SaveDialog1->FilterIndex) {
            case 1: {  ////// text format
               UnicodeString filename = ChangeFileExt(FileLocation, L"") + L"__" +
                                       DateToStr(Date()) + L"__" +
                                       TimeToStr(Time()) + L".txt";
               std::wofstream datafile_fs(filename.c_str());
               if (datafile_fs) {
                  datafile_fs.precision(6);

                  PlotsMapType::iterator pmIter = PlotsMap.begin();

                  // write header row  -- # hashtag for gnuplot
                  datafile_fs << L"#Time (s)" << L"\t";

                  for (unsigned long n=0; n < NumAvailablePlots; n++) {
                     datafile_fs << pmIter->first  << L"\t";
                     pmIter++;
                  }

                  datafile_fs << L"\n";

                  for (unsigned long j = 0; j < NumPlotVals ; j++) {
                     // reset PlotsMap iterator
                     pmIter = PlotsMap.begin();

                     datafile_fs << j * SampleTime << L"\t" ;

                     for (unsigned long i=0; i < NumAvailablePlots; i++) {
                        plotdata = pmIter->second.second;
                        datafile_fs << plotdata[j] << L"\t";
                        pmIter++;
                     }
                     datafile_fs << L"\n";
                  }
               }
            }; break;
            case 2: {   //////// abf format
			   AnsiString filename = ChangeFileExt(FileLocation, L"") + L"__" +
                                       DateToStr(Date()) + L"__" +
                                       TimeToStr(Time()) + L".abf";
               int hFile;
               HANDLE hHandle;
               int nError = 0;
               DWORD dwEpisodes, dwSamples;
               uInt32 numElectrodes = GetNet()->GetElectrodes().size();
               short *abfdata = new short[NumAvailablePlots*NumPlotVals];
               short *pabfdata = abfdata;

               ABFFileHeader *pFH = new ABFFileHeader;
               ABFH_Initialize( pFH );
               pFH->nADCNumChannels = NumAvailablePlots > ABF_ADCCOUNT ?
                                       ABF_ADCCOUNT : NumAvailablePlots;
               pFH->fADCSequenceInterval = SampleTime * 1000000.0;
               pFH->lNumSamplesPerEpisode = NumPlotVals;
               pFH->fADCRange = 20.0;
               pFH->lADCResolution = 65536;
					pFH->lActualAcqLength = SecondsBefore + SecondsDuring + SecondsAfter;
               pFH->lActualEpisodes = 1;
					pFH->fSecondsPerRun = SecondsBefore + SecondsDuring + SecondsAfter;
               pFH->lEpisodesPerRun = 1;
               pFH->lRunsPerTrial = 1;
               pFH->lNumberOfTrials = 1;

			   if (!ABF_WriteOpen( filename.c_str(), &hFile, ABF_DATAFILE, pFH, &nError ) ){
                  return;
               }

// from abfheadr.h
// char     sADCChannelName[ABF_ADCCOUNT][ABF_ADCNAMELEN];   // extra chars so name can be modified for P/N
// char     sADCUnits[ABF_ADCCOUNT][ABF_ADCUNITLEN];
// float    fSignalGain[ABF_ADCCOUNT];
// float    fInstrumentScaleFactor[ABF_ADCCOUNT];
               PlotsMapType::iterator pmIter = PlotsMap.begin();
               int numADCs = 0;
               for (unsigned long i=0; i < NumAvailablePlots; i++) {
                  if (numADCs < ABF_ADCCOUNT){
					 AnsiString name(pmIter->first.c_str());
					 AnsiString units(pmIter->second.first.c_str());
					 strncpy(pFH->sADCChannelName[i], name.c_str(), ABF_ADCNAMELEN);
					 strncpy(pFH->sADCUnits[i], units.c_str(), ABF_ADCUNITLEN);
                     plotdata = pmIter->second.second;
                     abfdata = &pabfdata[i]; //set mux correctly
                     factor = 327.7;
                     if (units == L"V") {
                        factor = 3277;
                        pFH->fInstrumentScaleFactor[i] = 1.0;
                     }
                     for (unsigned long j = 0; j < NumPlotVals ; j++) {
                        *abfdata = *plotdata * factor;
                        plotdata++;
                        abfdata += NumAvailablePlots;
                     }
                     pmIter++;
                     numADCs++;
                  }
               }
               for (int i=0; i<16; i++) {
                  pFH->nADCSamplingSeq[i] = i;
               }

               if (!ABF_WriteRawData( hFile, pabfdata, NumAvailablePlots*NumPlotVals*2, &nError )){
                  return;
               }

               if (!ABF_UpdateHeader( hFile, pFH, &nError )){
                  return;
               }
               if (!ABF_Close( hFile, &nError )){
                  return;
               }
            }; break;
            default: break;
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::StartActionUpdate(TObject *Sender)
{
   if (!Terminated) {
      StartAction->Enabled = false;
   }
   else StartAction->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::StopActionUpdate(TObject *Sender)
{
   if (Terminated) {
      StopAction->Enabled = false;
   }
   else StopAction->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::StartActionExecute(TObject *Sender)
{
   if (!Terminated) { //acquisition in progress
      return;
   }

   float64 reqsamprate;

	bool proceed = true;

	if (PlotsDisplayedListBox->Count == 0){
		Application->MessageBox(L"Please choose plots to display", L"Please correct input");
		return;
	}

   try {
      NetDescription = GetNet()->DescribeNetwork();

   // get requested sample rate
      reqsamprate = StrToFloat(SampleRateEdit->Text);

   // get max integration time step
      GetNet()->SetMaxRK4Timestep(StrToFloat(MaxRK4TimestepEdit->Text));

   // get duration
      SecondsBefore = StrToFloat(TimeBeforeEdit->Text);
      SecondsDuring = StrToFloat(DurationEdit->Text);
      SecondsAfter  = StrToFloat(TimeAfterEdit->Text);

   // get interpolation
      InterpolationRate = StrToFloat(InterpolateRateEdit->Text);
      DoInterpolate = InterpolateCheckBox->Checked && (NetDescription.NumVDepCells > 0);

      if (SecondsDuring <= 0) {
         proceed = false;
      }
      if (SecondsBefore < 0 || SecondsAfter < 0) {
         proceed = false;
      }

   // get number of repeats
      NumRepeats= StrToFloat(RepeatEdit->Text);

   } catch (EConvertError & e) {
      Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      return;
   }

   if (proceed == false) {
      return;
   }

   if (reqsamprate < 1) {
      Application->MessageBox(L"Requested Sample Rate Too Low", L"Please correct input");
      return;
   }

   CoercedSampleRateLabel->Caption = L"We'll see....";

#ifndef SIM_ONLY_NO_NIDAQ
   if (reqsamprate > GetNIDAQmxSetup().Devices[0]->MaxMultiplexedAIRate) {
      Application->MessageBox(L"Requested Sample Rate Too High", L"Please correct input");
      return;
   }
#endif

   // clear the statistics
   AvgSampsLabel->Caption = L"";
   TotalSampsReqLabel->Caption = L"";
   TotalSampsReadLabel->Caption = L"";
   TotalReadsLabel->Caption = L"";
   AvgusDuringLabel->Caption = L"";
   MaxusDuringLabel->Caption = L"";

   Terminated = false;

   StartActionUpdate(NULL);
   StopActionUpdate(NULL);

   TCursor oldCursor = Screen->Cursor;
   Screen->Cursor = crHourGlass;
   Application->ProcessMessages();

   NumCells = 0;
   NumDataLogChans = 0;

   UnicodeString filename;
   try {

      // added 2/21/2013 -- parameter logging in currents.
      GetNet()->SetParamLoggingEnabled(LogKineticParamsCheckBox->Checked);
      if (GetNet()->IsParamLoggingEnabled()) {
		 FormatSettings.DateSeparator = L'-';
		 FormatSettings.ShortDateFormat = L"yyyy/mm/dd";
		 FormatSettings.TimeSeparator = L'_';
         filename = ChangeFileExt(LogLocation, L"") + L"__" +
                     DateToStr(Date()) + L"__" +
                     TimeToStr(Time()) + L"_kinetic_params.txt";

         GetNet()->WriteParamLoggingHeader();
      }

      if (NetDescription.NumVDepCells > 0){ //

#ifndef SIM_ONLY_NO_NIDAQ
		 AnsiString aistr(NetDescription.AIChans.c_str());
		 AnsiString aostr(NetDescription.AOChans.c_str());

         // add data logging channels to end of ai channel string
         NumDataLogChans = DataLogsPlotsListBox->GetCount();
         if (NumDataLogChans > 0) {
            DataLogsPlotsListBox->Items->Delimiter = L',';
            UnicodeString aiLOGstr(DataLogsPlotsListBox->Items->DelimitedText);
            aistr = aistr + L"," + aiLOGstr;
         }

		 AI_ChanString.assign(aistr.c_str());
		 AO_ChanString.assign(aostr.c_str());

         RunDynamicClamp(reqsamprate);
#endif

      }
      else RunTheModel(reqsamprate);
   } __finally {
      Terminated = true;
      Screen->Cursor = oldCursor;
      if (GetNet()->IsParamLoggingEnabled()) {
		 GetNet()->CloseLoggingStream(filename.c_str());
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::StopActionExecute(TObject *Sender)
{
   // terminate run, if active
   Terminated = true;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::CloseActionExecute(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::AddToDisplayButtonClick(TObject *Sender)
{
   // get AllPlotsListBox selected and add to PlotsDisplayedListBox
   typedef boost::shared_ptr<TStringList>TSharedStringList;
   TSharedStringList list(new TStringList);
   list->Duplicates = System::Classes::dupIgnore;

   // add displayed plots first
   list->AddStrings(PlotsDisplayedListBox->Items);

   // add selected plots from cell choices second
   for (int i = 0; i < CellsPlotsListBox->Items->Count; i++) {
      if (CellsPlotsListBox->Selected[i]) {
         list->Add(CellsPlotsListBox->Items->Strings[i]);
      }
   }
   // add selected plots from electrode choices third
   for (int i = 0; i < ElectrodesPlotsListBox->Items->Count; i++) {
      if (ElectrodesPlotsListBox->Selected[i]) {
         list->Add(ElectrodesPlotsListBox->Items->Strings[i]);
      }
   }
   // add selected plots from data loggers choices fourth
   for (int i = 0; i < DataLogsPlotsListBox->Items->Count; i++) {
      if (DataLogsPlotsListBox->Selected[i]) {
         list->Add(DataLogsPlotsListBox->Items->Strings[i]);
      }
   }

   //check if more than maximum
   if (list->Count > MAXPANELS) {
      UnicodeString msg = L"Maximum of ";
      msg = msg + MAXPANELS + L" plots exceeded";
      Application->MessageBox(msg.c_str(), L"Error");
   }
   // if not, then add to list
   else {
      PlotsDisplayedListBox->Clear();
      PlotsDisplayedListBox->Items->AddStrings(list.get());
      PlotTheData();
   }

}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::RemoveFromDisplayButtonClick(TObject *Sender)

{
   // get PlotsDisplayedListBox selected and remove from list
   typedef boost::shared_ptr<TStringList>TSharedStringList;
   TSharedStringList list(new TStringList);
   list->Duplicates = System::Classes::dupIgnore;

   for (int i = 0; i < PlotsDisplayedListBox->Count; i++) {
      if (!PlotsDisplayedListBox->Selected[i]) {
         list->Add(PlotsDisplayedListBox->Items->Strings[i]);
      }
   }
   PlotsDisplayedListBox->Clear();
   PlotsDisplayedListBox->Items->AddStrings(list.get());
   PlotTheData();
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::ClearDisplayListButtonClick(TObject *Sender)

{
   // remove all from list PlotsDisplayedListBox
   PlotsDisplayedListBox->Clear();
   PlotTheData();
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::CGM_ButtonClick(TObject *Sender)
{
   if (Sender == PS_Button) {
      SaveDialog2->FilterIndex = 2;
   }
   else SaveDialog2->FilterIndex = 1;

   UnicodeString FileLocation = L"";

   if (SaveDialog2->Execute()) {
      FileLocation = SaveDialog2->FileName;//ExtractFileName(SaveDialog2->FileName);
   }
   else return;  // user changed mind

   TCellsMapConstIterator celliter = GetNet()->GetCells().begin();
   TElectrodesMapConstIterator trodeiter = GetNet()->GetElectrodes().begin();
   std::map< std::wstring, double * > PlotsMap;
   int numCells = CellsPlotsListBox->Items->Count;
   int numElectrodes = ElectrodesPlotsListBox->Items->Count;
   int increment = CGMIncrementEdit->Text.ToInt();

   // Make plots name map with PlotValues
   // use cells first
   for (int i=0; i < numCells; i++) {
		if (celliter->second->IsActive())
			PlotsMap[celliter->first] = PlotValues[i]; // assign each channel's data to map
      celliter++;
   }
   // use electrodes second
   for (int i=numCells; i < numCells + numElectrodes; i++) {
		if (trodeiter->second->IsActive())
			PlotsMap[trodeiter->first] = PlotValues[i]; // assign each channel's data to map
      trodeiter++;
   }

   // only draw chosen plots
   int count = PlotsDisplayedListBox->Items->Count;
   TStrings *plots = PlotsDisplayedListBox->Items;

   FILE *outf;

   // set current directory to application directory, so files aren't dropped randomly
   ChDir(ExtractFilePath(Application->ExeName));


   // ps or cgm -- 1st step is cgm
   outf = fopen("temp.cgm", "wt");
   if (outf){
      CGMComponent1->BeginCGM(PlotsMap[plots->Strings[0].c_str()], SamplesPerPlot, 150,-150,
                              L"mV", 1.0/SampleRate, increment, outf);
      for (int i = 1; i < count; i++) {
         CGMComponent1->AddPlot(PlotsMap[plots->Strings[i].c_str()], SamplesPerPlot,
                              increment, L"0 0 0", 100*i);
      }
      CGMComponent1->WriteFile();
      fclose(outf);
   }

   if (Sender == CGM_Button) {
      CopyFileW(L"temp.cgm", FileLocation.c_str(), false);  // copy temp file to user chosen file
   }

   else if (Sender == PS_Button) {
      // ralcgm -d ps ___.cgm ___.ps
      // environment variables must be set for windows 64 bit
      // CGMDATA = C:\Program Files (x86)\ralcgm\data\  <-- need trailing slash
      // path = path; C:\Program Files (x86)\ralcgm
      UnicodeString command;
      command.sprintf(L"ralcgm -v -d ps \"%s\" \"%s\"", L"temp.cgm", L"temp.ps");

      _wsystem(command.c_str());
      CopyFileW(L"temp.ps", FileLocation.c_str(), false);  // copy temp file to user chosen file
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::SaveDataCheckBoxClick(TObject *Sender)
{
	if (SaveDataCheckBox->Checked) {  // user changed state to checked
		if (FileLocation.Length() == 0) {  // forget last base filneame choice
			SaveDataCheckBox->Checked = SetFileLocation(); // if fails, uncheck box
      }
   }
   if (SaveDataCheckBox->Checked) {  // user changed state to checked and still checked
      WriteTheData();
   }
}
//---------------------------------------------------------------------------

bool __fastcall TRunDynamicClampForm::SetFileLocation()
{
	bool isOK = true;
	if (SaveDialog1->Execute()) {
      FileLocation = SaveDialog1->FileName;//ExtractFileName(SaveDialog1->FileName);
      // this branch never called -- but keep for now (ebt 2-21-2012)
		if (FileLocation == L"") {
			SaveDataCheckBox->Checked = false;
			Application->MessageBoxA(L"Files not saved.  Must choose base file name",
											L"Notification");
			isOK = false;
		}
	}
	else isOK = false;
	return isOK;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::FileParamsButtonClick(TObject *Sender)
{
   SetFileLocation();
}
//---------------------------------------------------------------------------

// this function stores changes to run parameters even if Start was not clicked
void __fastcall TRunDynamicClampForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	 try {
   // get requested sample rate
      SampleRate = StrToFloat(SampleRateEdit->Text);

   // get max integration time step
      GetNet()->SetMaxRK4Timestep(StrToFloat(MaxRK4TimestepEdit->Text));

   // get duration
      SecondsBefore = StrToFloat(TimeBeforeEdit->Text);
      SecondsDuring = StrToFloat(DurationEdit->Text);
      SecondsAfter  = StrToFloat(TimeAfterEdit->Text);

	// get number of repeats
		NumRepeats= StrToFloat(RepeatEdit->Text);

   } catch (EConvertError & e) {
      Application->MessageBox(e.ToString().c_str(), L"Please correct input");
      return;
	}
}
//---------------------------------------------------------------------------


void __fastcall TRunDynamicClampForm::EditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
      StartActionExecute(Sender);
      Key = 0;
   }
}
//---------------------------------------------------------------------------


void __fastcall TRunDynamicClampForm::DataLoggingListBoxClick(TObject *Sender)
{
   if (DataLoggingListBox->SelCount > 0 ) {
      DataLogsPlotsListBox->Clear();
      DataLoggingListBox->CopySelection(DataLogsPlotsListBox);
   }
   PlotsDisplayedListBox->Clear(); // adding new choices so must pick again
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::InterpolateCheckBoxClick(TObject *Sender)
{
   if (NetDescription.NumVDepCells > 0) {
      DoInterpolate = InterpolateCheckBox->Checked && (NetDescription.NumVDepCells > 0);
      PackageTheData();
      PlotTheData();
      WriteTheData();
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::InterpolateRateEditKeyPress(TObject *Sender,
          wchar_t &Key)
{
   double temp = 0.0;
   if (Key == VK_RETURN) {
      if (NetDescription.NumVDepCells > 0) {
         try {
            temp = StrToFloat(InterpolateRateEdit->Text);
            if (temp < 0.0) {
               Application->MessageBox(L"Interpolation rate must be greater than zero",
                                       L"Please correct input");
               return;
            }
         } catch (EConvertError & e) {
            Application->MessageBox(e.ToString().c_str(), L"Please correct input");
            return;
         }
         InterpolationRate = temp;
         DoInterpolate = InterpolateCheckBox->Checked && (NetDescription.NumVDepCells > 0);
         PackageTheData();
         PlotTheData();
         WriteTheData();
      }
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::LogKineticParamsButtonClick(TObject *Sender)

{
   SetKineticLogLocation();
}
//---------------------------------------------------------------------------
bool __fastcall TRunDynamicClampForm::SetKineticLogLocation()
{
	bool isOK = true;
	if (SaveDialog3->Execute()) {
      LogLocation = SaveDialog3->FileName;
      // this branch never called -- but keep for now (ebt 2-21-2012)
		if (LogLocation == L"") {
			LogKineticParamsCheckBox->Checked = false;
			Application->MessageBoxA(L"Kinetic Parameters not saved.  Must choose base file name",
											L"Notification");
			isOK = false;
		}
	}
	else isOK = false;
	return isOK;
}
//---------------------------------------------------------------------------

void __fastcall TRunDynamicClampForm::LogKineticParamsCheckBoxClick(TObject *Sender)

{
   if (LogKineticParamsCheckBox->Checked) {  // user changed state to checked
		if (LogLocation.Length() == 0) {  // forget last base filneame choice
			LogKineticParamsCheckBox->Checked = SetKineticLogLocation(); // if fails, uncheck box
      }
   }
}
//---------------------------------------------------------------------------

