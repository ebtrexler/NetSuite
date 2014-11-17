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

#include <iostream>

#include "NI_DAQmx_Devices.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

TNIDAQmxSetup &GetNIDAQmxSetup()
{
//   static TNIDAQmxSetup setup;
   static TNIDAQmxSetup *setup = NULL;
#ifndef SIM_ONLY_NO_NIDAQ
   if (!setup) setup = new TNIDAQmxSetup;
#endif
   return *setup;
}


void __fastcall TNIDAQmxSetup::DAQmxErrChk(int32 functionCall)
{
	if (functionCall < 0) {  //error state
		throw EDAQmxException(functionCall);
	}
}

__fastcall TNIDAQmxSetup::TNIDAQmxSetup()
{
#ifndef SIM_ONLY_NO_NIDAQ

	//local variables
	char *devicetext;
	TStringList *devicenames;

	//property storage initialization
	FNumDevices = 0;
	FDevices = NULL;

	try {
		devicetext = new char[PROPBUFFSIZE];
      memset(devicetext, 'a', PROPBUFFSIZE);
		//get list of device names
		DAQmxErrChk (DAQmxGetSysDevNames(devicetext, PROPBUFFSIZE));

		//load text into stringlist for parsing
		devicenames = new TStringList;
		devicenames->Delimiter = L',';
		devicenames->DelimitedText = UnicodeString(devicetext);

		FNumDevices = devicenames->Count;

		//fill in devices
		FDevices = new TNIDAQmxDevice*[FNumDevices];
		for (int i = 0; i < FNumDevices; i++) {
			FDevices[i] = new TNIDAQmxDevice(devicenames->Strings[i]);
		}

		//cleanup
		delete[] devicetext;
		delete devicenames;
	}
	catch (const EDAQmxException &e) {
		Application->ShowException(&e);
		//cleanup
		delete[] devicetext;
		delete devicenames;
//		Application->Terminate();
	}

#endif //#ifndef SIM_ONLY_NO_NIDAQ
}

__fastcall TNIDAQmxSetup::~TNIDAQmxSetup()
{
#ifndef SIM_ONLY_NO_NIDAQ
	for (int i = 0; i < FNumDevices; i++) {
		delete FDevices[i];
	}
	delete[] FDevices;
#endif
}


__fastcall TNIDAQmxDevice::TNIDAQmxDevice(UnicodeString name)
{
#ifndef SIM_ONLY_NO_NIDAQ

	// local variables
	char *channeltext;
	bool32 AOSampClock;

	//property storage initialization
	FDeviceName = name;

	FNumAIChannels = 0;
	FNumAOChannels = 0;
	FAIChannels = NULL;
	FAOChannels = NULL;
	FNumADCRangePairs = 0;
	FNumDACRangePairs = 0;
	FADCRangePairs = NULL;
	FDACRangePairs = NULL;
	FMaxMultiplexedAIRate = 0;
	FMaxMultiplexedAORate = 0;

	try {
		channeltext = new char[PROPBUFFSIZE];
		FAIChannels = new TStringList;
		FAOChannels = new TStringList;
		FAIChannels->Delimiter = L',';
		FAOChannels->Delimiter = L',';

		//get list of channel names
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAIPhysicalChans(FDeviceName.c_str(), channeltext, PROPBUFFSIZE));
		//load text into stringlist for parsing
		FAIChannels->DelimitedText = UnicodeString(channeltext);
		FNumAIChannels = FAIChannels->Count;

		//get list of channel names
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAOPhysicalChans(FDeviceName.c_str(), channeltext, PROPBUFFSIZE));
		//load text into stringlist for parsing
		FAOChannels->DelimitedText = UnicodeString(channeltext);
		FNumAOChannels = FAOChannels->Count;

		//get analog in voltage range pairs --> DAQmx version of ADC gains
		float64 data[100];
		memset(data, 0, 100*sizeof(float64));
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAIVoltageRngs(FDeviceName.c_str(), data, 100));
		int j=0;
		while ((data[j] != 0) || (data[j+1] != 0)) {
			j+=2;
		}
		FNumADCRangePairs = j/2;
		FADCRangePairs = new TRangePair[FNumADCRangePairs];
		j=0;
		while ((data[j] != 0) || (data[j+1] != 0)) {
			FADCRangePairs[j/2].Low = data[j];
			FADCRangePairs[j/2].High = data[j+1];
			j+=2;
		}

		//get analog out voltage range pairs --> DAQmx version of DAC gains
		memset(data, 0, 100*sizeof(float64));
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAOVoltageRngs(FDeviceName.c_str(), data, 100));
		j=0;
		while ((data[j] != 0) || (data[j+1] != 0)) {
			j+=2;
		}
		FNumDACRangePairs = j/2;
		FDACRangePairs = new TRangePair[FNumDACRangePairs];
		j=0;
		while ((data[j] != 0) || (data[j+1] != 0)) {
			FDACRangePairs[j/2].Low = data[j];
			FDACRangePairs[j/2].High = data[j+1];
			j+=2;
		}

		//get max ai rate with multiple channels
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAIMaxMultiChanRate(FDeviceName.c_str(), &FMaxMultiplexedAIRate));

		//get max ao rate with multiple channels
		TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAOSampClkSupported(FDeviceName.c_str(), &AOSampClock));
		if (AOSampClock){
			TNIDAQmxSetup::DAQmxErrChk (DAQmxGetDevAOMaxRate(FDeviceName.c_str(), &FMaxMultiplexedAORate));
		}

		//cleanup
		delete[] channeltext;
	}
	catch (const EDAQmxException &e) {
		Application->ShowException(&e);
		//cleanup
		delete[] channeltext;
	}

#endif //#ifndef SIM_ONLY_NO_NIDAQ
}

__fastcall TNIDAQmxDevice::~TNIDAQmxDevice()
{
#ifndef SIM_ONLY_NO_NIDAQ
	delete FAIChannels;
	delete FAOChannels;
	delete[] FADCRangePairs;
	delete[] FDACRangePairs;
#endif
}
