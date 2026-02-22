//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("GUI_PeriodicityEditor.cpp", PeriodicityForm);
USEFORM("GUI_TextFileImportOptions.cpp", TextFileImportOptionsForm);
USEFORM("GUI_VoltageClampFitForm.cpp", VoltageClampFitForm);
USEFORM("GUI_RT_Edit_HHCurrent.cpp", HHCurrentForm);
USEFORM("GUI_PlaybackWaveformForm.cpp", PlaybackWaveformForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "NetFit: Fit Voltage Clamped Currents to Kinetic Model";
		TStyleManager::TrySetStyle("Charcoal Dark Slate");
		Application->CreateForm(__classid(TVoltageClampFitForm), &VoltageClampFitForm);
		Application->CreateForm(__classid(TPlaybackWaveformForm), &PlaybackWaveformForm);
		Application->CreateForm(__classid(TTextFileImportOptionsForm), &TextFileImportOptionsForm);
		Application->CreateForm(__classid(THHCurrentForm), &HHCurrentForm);
		Application->CreateForm(__classid(TPeriodicityForm), &PeriodicityForm);
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
