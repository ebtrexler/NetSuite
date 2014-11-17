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

#include "Fit_Thread_iGen.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThread_iGen::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThread_iGen::TThread_iGen(std::vector<double>::const_iterator viter,
													std::vector<double>::iterator filliter,
													double samprate, int numepisodes,
													int numptsperepisode,
													boost::shared_ptr<THHCurrent> curr):
							TThread(true), voltiter(viter), toFilliter(filliter),
							SampleRate(samprate), NumEpisodes(numepisodes),
							NumPointsPerEpisode(numptsperepisode), Current(curr)
{
}
//---------------------------------------------------------------------------
void __fastcall TThread_iGen::Execute()
{
	//---- Place thread code here ----
//	Current->Initialize(true);
	for (int i=0; i < NumEpisodes; i++) {
		Current->Restart(*voltiter);
		for (int j = 0; j < NumPointsPerEpisode; j++) {
			*toFilliter = Current->Update(SampleRate, *voltiter, *voltiter);
			toFilliter++;
			voltiter++;
		}
	}
}
//---------------------------------------------------------------------------
