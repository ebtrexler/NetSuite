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

#ifndef Fit_Thread_iGenH
#define Fit_Thread_iGenH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include "RT_HHCurrent.h"

//---------------------------------------------------------------------------
class TThread_iGen : public TThread
{
private:
	std::vector<double>::const_iterator voltiter;
	std::vector<double>::iterator toFilliter;
	double SampleRate;
	int NumEpisodes;
	int NumPointsPerEpisode;
	boost::shared_ptr<THHCurrent> Current;
protected:
	void __fastcall Execute();
public:
	__fastcall TThread_iGen(std::vector<double>::const_iterator viter,
									std::vector<double>::iterator filliter,
									double samprate, int numepisodes,
									int numptsperepisode,
									boost::shared_ptr<THHCurrent> curr);
};
//---------------------------------------------------------------------------
#endif
