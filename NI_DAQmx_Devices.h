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

#ifndef NI_DAQmx_DevicesH
#define NI_DAQmx_DevicesH

#include "nidaqmx.h"
//
const int32 PROPBUFFSIZE = 65536;
//const int32 MAXCHANNELS  = 1024;
//const int32 MAXDEVICES   = 8;
//const int32 MAXPROPCHAR  = 256;

//!

///
class EDAQmxException: public Exception
{
public:
	__fastcall EDAQmxException(int errorCode): Exception(errorCode)
	{
		char *errBuff = new char[2048];
		DAQmxGetExtendedErrorInfo(errBuff,2048);
//		DAQmxGetErrorString (errorCode, errBuff, 2048);
		Message = UnicodeString(errBuff);
//		Exception (errBuff);
		delete[] errBuff;
	}
};

struct TRangePair
{
	float64						Low;
	float64						High;
};

class TNIDAQmxDevice
{
private:
	AnsiString					FDeviceName;
	TStringList* 				FAIChannels;
	TStringList* 				FAOChannels;
	int							FNumAIChannels;
	int							FNumAOChannels;
	int							FNumADCRangePairs;
	int							FNumDACRangePairs;
	TRangePair*					FADCRangePairs;
	TRangePair*					FDACRangePairs;
	float64						FMaxMultiplexedAIRate;
	float64						FMaxMultiplexedAORate;

protected:

public:
	// Constructor
	__fastcall TNIDAQmxDevice(UnicodeString name);

	// Destructor
	__fastcall ~TNIDAQmxDevice();

	// properties -- variables
	__property AnsiString			DeviceName =   {read = FDeviceName};
	__property TStringList*			AIChannels =	{read = FAIChannels};
	__property TStringList*			AOChannels =	{read = FAOChannels};
	__property int						NumAIChannels =
				{read = FNumAIChannels};
	__property int						NumAOChannels =
				{read = FNumAOChannels};
	__property int						NumADCRangePairs =
				{read = FNumADCRangePairs};
	__property int						NumDACRangePairs =
				{read = FNumDACRangePairs};
	__property TRangePair*			ADCRangePairs =
				{read = FADCRangePairs};
	__property TRangePair*			DACRangePairs =
				{read = FDACRangePairs};
	__property float64				MaxMultiplexedAIRate =
				{read = FMaxMultiplexedAIRate};
	__property float64				MaxMultiplexedAORate =
				{read = FMaxMultiplexedAORate};
};


class TNIDAQmxSetup
{

private:
	int32						FNumDevices;
	TNIDAQmxDevice**			FDevices;

public:

	// properties -- variables
	__property int32					NumDevices =
				{read = FNumDevices, write = FNumDevices};
	__property TNIDAQmxDevice**			Devices =
				{read = FDevices};

	__fastcall TNIDAQmxSetup();
	__fastcall ~TNIDAQmxSetup();

	static void __fastcall			DAQmxErrChk(int32 functionCall);
};

//---------------------------------------------------------------------------
TNIDAQmxSetup & GetNIDAQmxSetup();
//---------------------------------------------------------------------------
#endif
