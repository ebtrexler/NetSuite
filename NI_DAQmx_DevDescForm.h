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

#ifndef NI_DAQmx_DevDescFormH
#define NI_DAQmx_DevDescFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "NI_DAQmx_Devices.h"

//---------------------------------------------------------------------------
class TDeviceDesc : public TForm
{
__published:	// IDE-managed Components
	TListBox *lbxInstalledDevices;
	TListBox *lbxAIChannels;
	TListBox *lbxAOChannels;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *lblMaxMultAISampRate;
	TLabel *Label5;
	TLabel *lblMaxMultAOSampRate;
	TListBox *lbxAIVoltRngs;
	TLabel *Label6;
	TLabel *Label7;
	TListBox *lbxAOVoltRngs;
	void __fastcall lbxInstalledDevicesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDeviceDesc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDeviceDesc *DeviceDesc;
//---------------------------------------------------------------------------
#endif
