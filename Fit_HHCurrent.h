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

#ifndef Fit_HHCurrentH
#define Fit_HHCurrentH
//---------------------------------------------------------------------------
#include "RT_HHCurrent.h"
#include "Fit_LevenbergMaquardt.h"
#include "GUI_PlaybackWaveform.h"


class THHCurrentModel: public TModelBase
{
protected:
public:
	__fastcall THHCurrentModel();
	virtual ~THHCurrentModel();

	TPlaybackWaveform *Waveform;
	THHCurrent	*Current;
	virtual int __fastcall OutFit();

	virtual double __fastcall GetChiSq(); // calculates/returns ChiSq   --measure of goodness of fit

	//this function will be called by ModelFittingModule to generate data
	virtual int __fastcall fitGen(int *numdatapoints, int *numparms,
						double *parms, double *fvec, int *iflag);

	double SampleRate; // ms

	int NumEpisodes;
	int NumPointsPerEpisode;

	std::vector<double> voltdata;
	std::vector<double> currdata;
	std::vector<double> fitdata;

	void __fastcall	StuffParms(double *parms);
	void __fastcall 	PopToFitParms();
	void __fastcall	Setup(int currchan, int voltchan);
	int __fastcall 	iGen(std::vector<double> & toFill);
};

#endif
