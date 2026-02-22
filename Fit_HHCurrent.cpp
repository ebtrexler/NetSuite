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

#pragma hdrstop

#include "Fit_HHCurrent.h"
#include "GUI_VoltageClampFitForm.h"
#include "Fit_Thread_iGen.h"
#include "tools.h"
#include <values.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)


__fastcall 			THHCurrentModel::THHCurrentModel()
{
	Description = "Hodgkin-Huxley type current";
	NumParams = 0;
	ParamDesc.push_back("Erev"), NumParams++;
	ParamDesc.push_back("Gmax"), NumParams++;
	ParamDesc.push_back("m-V_1/2 (mV)"), NumParams++;
	ParamDesc.push_back("m-k"), NumParams++;
	ParamDesc.push_back("m-Tau low (ms)"), NumParams++;
	ParamDesc.push_back("m-Tau hi (ms)"), NumParams++;
	ParamDesc.push_back("m-SSmin"), NumParams++;
	ParamDesc.push_back("m-w (den. expt)"), NumParams++;
	ParamDesc.push_back("m-p (exponent)"), NumParams++;
	ParamDesc.push_back("h-V_1/2 (mV)"), NumParams++;
	ParamDesc.push_back("h-k"), NumParams++;
	ParamDesc.push_back("h-Tau low (ms)"), NumParams++;
	ParamDesc.push_back("h-Tau hi (ms)"), NumParams++;
	ParamDesc.push_back("h-SSmin"), NumParams++;
	ParamDesc.push_back("h-w (den. expt)"), NumParams++;
	ParamDesc.push_back("h-q (exponent)"), NumParams++;
	ParamDesc.push_back("n-V_1/2 (mV)"), NumParams++;
	ParamDesc.push_back("n-k"), NumParams++;
	ParamDesc.push_back("n-Tau low (ms)"), NumParams++;
	ParamDesc.push_back("n-Tau hi (ms)"), NumParams++;
	ParamDesc.push_back("n-SSmin"), NumParams++;
	ParamDesc.push_back("n-w (den. expt)"), NumParams++;
	ParamDesc.push_back("n-r (exponent)"), NumParams++;

	DoFit.reserve(NumParams);
	SeedParams.reserve(NumParams);
	FitParams.reserve(NumParams);
	for (int i=0; i<NumParams; i++) {
		DoFit[i] = 0;
	}

	Waveform = new TPlaybackWaveform();
	Current = new THHCurrent();
}

THHCurrentModel::~THHCurrentModel()
{
	delete Waveform;
	delete Current;
}

int __fastcall 	THHCurrentModel::OutFit()
{
	for (int i=0; i < NumDataPoints; i++) {
		VoltageClampFitForm->genrdata[i] =
							constrain(fitdata[i], (double)-MAXFLOAT, (double)MAXFLOAT);
	}
	VoltageClampFitForm->PlotPanel1->ScalePlots(true);

	NumCalls++;

	VoltageClampFitForm->fitGenLabel->Caption = NumCalls;
	VoltageClampFitForm->ChiSqLabel->Caption = FloatToStr(GetChiSq());
	Application->ProcessMessages();
	return 0;
}

void __fastcall 	THHCurrentModel::StuffParms(double *parms)
{
	int i, j;
	for (i=0,j=0; i<NumParams; i++){
		if (DoFit[i] == 1){
			FitParams[i] = parms[j];
			++j;
		}
	}

	i = 0;

	Current->E = FitParams[i++];
	Current->Gmax = FitParams[i++];

	Current->get_m().V0 = FitParams[i++];
	Current->get_m().k = FitParams[i++];
	Current->get_m().t_lo = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_m().t_hi = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_m().infMin = constrain(FitParams[i++], 0.0, 1.0);
	Current->get_m().w = constrain(FitParams[i++], 1e-2, 6.0);
	Current->p = constrain(FitParams[i++], 0.0, 6.0);

	Current->get_h().V0 = FitParams[i++];
	Current->get_h().k = FitParams[i++];
	Current->get_h().t_lo = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_h().t_hi = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_h().infMin = constrain(FitParams[i++], 0.0, 1.0);
	Current->get_h().w = constrain(FitParams[i++], 1e-2, 6.0);
	Current->q = constrain(FitParams[i++], 0.0, 6.0);

	Current->get_n().V0 = FitParams[i++];
	Current->get_n().k = FitParams[i++];
	Current->get_n().t_lo = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_n().t_hi = constrain(FitParams[i++], 1e-3, 1e6);
	Current->get_n().infMin = constrain(FitParams[i++], 0.0, 1.0);
	Current->get_n().w = constrain(FitParams[i++], 1e-2, 6.0);
	Current->r = constrain(FitParams[i++], 0.0, 6.0);
}

void __fastcall 	THHCurrentModel::PopToFitParms()
{
	int i = 0;

	FitParams[i++] = Current->E;
	FitParams[i++] = Current->Gmax;

	FitParams[i++] = 	Current->get_m().V0;
	FitParams[i++] = 	Current->get_m().k;
	FitParams[i++] = 	Current->get_m().t_lo;
	FitParams[i++] = 	Current->get_m().t_hi;
	FitParams[i++] = 	Current->get_m().infMin;
	FitParams[i++] = 	Current->get_m().w;
	FitParams[i++] = 	Current->p;

	FitParams[i++] = 	Current->get_h().V0;
	FitParams[i++] = 	Current->get_h().k;
	FitParams[i++] = 	Current->get_h().t_lo;
	FitParams[i++] = 	Current->get_h().t_hi;
	FitParams[i++] = 	Current->get_h().infMin;
	FitParams[i++] = 	Current->get_h().w;
	FitParams[i++] = 	Current->q;

	FitParams[i++] = 	Current->get_n().V0;
	FitParams[i++] = 	Current->get_n().k;
	FitParams[i++] = 	Current->get_n().t_lo;
	FitParams[i++] = 	Current->get_n().t_hi;
	FitParams[i++] = 	Current->get_n().infMin;
	FitParams[i++] = 	Current->get_n().w;
	FitParams[i++] = 	Current->r;
}

double __fastcall	THHCurrentModel::GetChiSq()
{
	double ChiSq = 0;
	double temp = 0;
	double var = 0;
	double mean = 0;
	const int noise_samples = 10;
	for (int i=0; i < noise_samples; i++) {    // take first points as baseline noise
		mean += currdata[i];
	}
	mean /= noise_samples;
	for (int i=0; i < noise_samples; i++) {
		temp = currdata[i]-mean;
		var += temp*temp;
	}
	var /= noise_samples;
	if (var == 0.0){
   	var = 1;
	}
	for (int i=0; i<NumDataPoints; i++){
		temp = (fitdata[i] - currdata[i]);
		ChiSq += temp*temp / var;
	}
	ChiSq /= (NumDataPoints-NumParams-1); //divide by deg. of freedom
	return ChiSq;
}

void __fastcall	THHCurrentModel::Setup(int currchan, int voltchan)
{
	NumDataPoints =	Waveform->GetNumPoints();
	SampleRate =		Waveform->GetPlaybackRate();
	NumEpisodes =		Waveform->GetNumEpisodes();
	NumPointsPerEpisode = NumDataPoints/NumEpisodes;

	Waveform->GetChannelData(currchan, currdata);
	Waveform->GetChannelData(voltchan, voltdata);

	fitdata.reserve(voltdata.size());

	PopToFitParms(); // load HHCurrent params into FitParams

	NumCalls = 0;
}

int __fastcall THHCurrentModel::fitGen(int *numdatapoints, int *numparms,
						double *parms, double *fvec, int *iflag)
{
	int i = 0;
	StuffParms(parms);

	if (iGen(fitdata)!= 0){
		*iflag = STOP_FIT;
		return -1;
	}

	for (i=0; i<(*numdatapoints); i++){
		fvec[i++] = fitdata[i] - currdata[i];
	}

	OutFit();

 	return 0; //success
}

int __fastcall THHCurrentModel::iGen(std::vector<double> & toFill)
{
	bool mt = VoltageClampFitForm->MultiThreadCheckBox->Checked;

	if (!mt) {
		std::vector<double>::const_iterator voltiter = voltdata.begin();
		std::vector<double>::iterator toFilliter = toFill.begin();

		for (int i=0; i < NumEpisodes; i++) {
			Current->Restart(*voltiter);
			for (int j = 0; j < NumPointsPerEpisode; j++) {
				*toFilliter = Current->Update(SampleRate, *voltiter, *voltiter);
				toFilliter++;
				voltiter++;
			}
		}
	} else {
		//try multithreaded version
		std::vector<double>::const_iterator voltiter = voltdata.begin();
		std::vector<double>::iterator toFilliter = toFill.begin();

		int NumThreads = VoltageClampFitForm->NumThreadsEdit->Text.ToInt();
		int NumEpisPerThread = NumEpisodes / NumThreads;
		if ((NumEpisPerThread * NumThreads) < NumEpisodes){
			NumEpisPerThread += 1;
		}
		int LastThreadEpis = NumEpisodes - (NumEpisPerThread * (NumThreads-1));
		int iteroffset = NumEpisPerThread*NumPointsPerEpisode;
		typedef boost::shared_ptr<TThread_iGen> threadptr;
		std::vector<threadptr> threads;
		for (int i = 0; i < NumThreads; i++) {
			boost::shared_ptr<THHCurrent> currcopy(new THHCurrent(*Current));
			threadptr thread(new TThread_iGen(voltiter, toFilliter,
														SampleRate,
														(i==NumThreads-1) ? LastThreadEpis : NumEpisPerThread,
														NumPointsPerEpisode, currcopy));
			threads.push_back(thread);
			voltiter += iteroffset;
			toFilliter += iteroffset;
		}
		for (int i = 0; i < NumThreads; i++) {
			threads[i]->Start();
		}
		for (int i = 0; i < NumThreads; i++) {
			threads[i]->WaitFor();
		}
	}
	return 0; //success
}
