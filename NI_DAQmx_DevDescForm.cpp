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

#include "NI_DAQmx_DevDescForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDeviceDesc *DeviceDesc;
//---------------------------------------------------------------------------
__fastcall TDeviceDesc::TDeviceDesc(TComponent* Owner)
	: TForm(Owner)
{
#ifndef SIM_ONLY_NO_NIDAQ

   for (int i=0; i<GetNIDAQmxSetup().NumDevices; i++){
		lbxInstalledDevices->Items->Add(GetNIDAQmxSetup().Devices[i]->DeviceName);
	}
	lbxInstalledDevices->Selected[0] = true;
	lbxInstalledDevicesClick(NULL);

#endif
}

//---------------------------------------------------------------------------

void __fastcall TDeviceDesc::lbxInstalledDevicesClick(TObject *Sender)
{
#ifndef SIM_ONLY_NO_NIDAQ

	TNIDAQmxDevice *dev = GetNIDAQmxSetup().Devices[lbxInstalledDevices->ItemIndex];
	lbxAIChannels->Clear();
	lbxAOChannels->Clear();
	lbxAIVoltRngs->Clear();
	lbxAOVoltRngs->Clear();
	lbxAIChannels->Items->AddStrings(dev->AIChannels);
	lbxAOChannels->Items->AddStrings(dev->AOChannels);
	lblMaxMultAISampRate->Caption = FloatToStr(dev->MaxMultiplexedAIRate);
	lblMaxMultAOSampRate->Caption = FloatToStr(dev->MaxMultiplexedAORate);
	int i;
	UnicodeString rngtxt;
	for (i = 0; i < dev->NumADCRangePairs; i++) {
		rngtxt.sprintf(L"%5.1f V to %5.1f V", dev->ADCRangePairs[i].Low,  dev->ADCRangePairs[i].High);
		lbxAIVoltRngs->Items->Add(rngtxt);
	}
	for (i = 0; i < dev->NumDACRangePairs; i++) {
		rngtxt.sprintf(L"%5.1f V to %5.1f V", dev->DACRangePairs[i].Low,  dev->DACRangePairs[i].High);
		lbxAOVoltRngs->Items->Add(rngtxt);
	}

#endif
}
//---------------------------------------------------------------------------

