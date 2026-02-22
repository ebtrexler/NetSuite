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

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("NI_DAQmx_DevDescForm.cpp", DeviceDesc);
USEFORM("GUI_CopyCurrentsForm.cpp", CopyCurrentsForm);
USEFORM("GUI_NetworkForm.cpp", NetworkGUI);
USEFORM("GUI_CirclePerimeterEditor.cpp", GUI_CirclePerimeterForm);
USEFORM("GUI_PlaybackWaveformForm.cpp", PlaybackWaveformForm);
USEFORM("GUI_RT_Edit_BiologicalCell.cpp", BiologicalCellForm);
USEFORM("GUI_PeriodicityEditor.cpp", PeriodicityForm);
USEFORM("ChooseTypeAndName.cpp", TypeAndNameDlg);
USEFORM("GUI_RT_Edit_GenBiDirSynapse.cpp", GenBiDirSynapseForm);
USEFORM("GUI_SquareLatticeEditor.cpp", GUI_SquareLatticeForm);
USEFORM("GUI_TextFileImportOptions.cpp", TextFileImportOptionsForm);
USEFORM("GUI_RT_Edit_VoltageClampPIDCurrent.cpp", VoltageClamp_PID_CurrentForm);
USEFORM("GUI_RunModelForm.cpp", RunDynamicClampForm);
USEFORM("GUI_RT_Edit_PlaybackElectrode.cpp", PlaybackElectrodeForm);
USEFORM("GUI_RT_Edit_HH2Current.cpp", HH2CurrentForm);
USEFORM("GUI_RT_Edit_HHCurrent.cpp", HHCurrentForm);
USEFORM("GUI_RT_Edit_GJCurrent.cpp", GJCurrentForm);
USEFORM("GUI_RT_Edit_GJSynapse.cpp", GapJunctionSynapseForm);
USEFORM("GUI_RT_Edit_PlaybackCell.cpp", PlaybackCellForm);
USEFORM("GUI_RT_Edit_PlaybackCurrent.cpp", PlaybackCurrentForm);
USEFORM("GUI_RT_Edit_InjElectrode.cpp", InjectionElectrodeForm);
USEFORM("GUI_RT_Edit_ModelCell.cpp", ModelCellForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		TStyleManager::TrySetStyle("Cyan Dusk");
		Application->CreateForm(__classid(TNetworkGUI), &NetworkGUI);
		Application->CreateForm(__classid(TRunDynamicClampForm), &RunDynamicClampForm);
		Application->CreateForm(__classid(TTypeAndNameDlg), &TypeAndNameDlg);
		Application->CreateForm(__classid(TPeriodicityForm), &PeriodicityForm);
		Application->CreateForm(__classid(TCopyCurrentsForm), &CopyCurrentsForm);
		Application->CreateForm(__classid(TTextFileImportOptionsForm), &TextFileImportOptionsForm);
		Application->CreateForm(__classid(TDeviceDesc), &DeviceDesc);
		Application->CreateForm(__classid(TGJCurrentForm), &GJCurrentForm);
		Application->CreateForm(__classid(TPlaybackCurrentForm), &PlaybackCurrentForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
