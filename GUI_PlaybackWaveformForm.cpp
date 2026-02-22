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

#include "GUI_PlaybackWaveformForm.h"
#include "GUI_TextFileImportOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PLOTPanel"
#pragma resource "*.dfm"
TPlaybackWaveformForm *PlaybackWaveformForm;

//===========================================================================
//===========================================================================
//===========================================================================
//
// GUI -- TPlaybackWaveformForm methods
//
//===========================================================================
//===========================================================================
//===========================================================================
//---------------------------------------------------------------------------
__fastcall TPlaybackWaveformForm::TPlaybackWaveformForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPlaybackWaveformForm::BrowseBitBtnClick(TObject *Sender)
{
	if (OpenDialog1->Execute()) {
		if (OpenDialog1->FilterIndex == 3){ // *.txt file
			if (TextFileImportOptionsForm->ShowModal() == mrOk){
				bool timecolumn = TextFileImportOptionsForm->GetFirstColumnTime();
				Waveform->SetTXTHeaderRow(TextFileImportOptionsForm->GetFirstRowHeader());
				Waveform->SetTXTTimeColumn(timecolumn);
				if (!timecolumn){
					Waveform->SetSampleRate(StrToFloat(TextFileImportOptionsForm->SampleRateEdit->Text));
				}
				Waveform->SetFileName(OpenDialog1->FileName.c_str());
			}
		}
		else Waveform->SetFileName(OpenDialog1->FileName.c_str());
   }
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackWaveformForm::ChannelComboBoxChange(TObject *Sender)
{
	Waveform->SetChannel(ChannelComboBox->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackWaveformForm::ConvertGainEditKeyPress(TObject *Sender, wchar_t &Key)

{
   if (Key == VK_RETURN) {
		Waveform->SetScaleFactor(StrToFloat(ConvertGainEdit->Text));
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackWaveformForm::OffsetEditKeyPress(TObject *Sender, wchar_t &Key)

{
	if (Key == VK_RETURN) {
		Waveform->SetOffset(StrToFloat(OffsetEdit->Text));
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackWaveformForm::RepeatsRadioGroupClick(TObject *Sender)
{
	if (RepeatsRadioGroup->ItemIndex == 0){ // Fixed Number
		FixedRepeatsEdit->Enabled = true;
	}
	else {
      FixedRepeatsEdit->Text = L"-1";
		FixedRepeatsEdit->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TPlaybackWaveformForm::WaveformDurationEditKeyPress(TObject *Sender,
          wchar_t &Key)
{
   if (Key == VK_RETURN) {
		Waveform->SetWfmDuration(StrToFloat(WaveformDurationEdit->Text));
      Key = 0;
   }
}
//---------------------------------------------------------------------------


void __fastcall TPlaybackWaveformForm::ConcatenateEpisodesCheckBoxClick(TObject *Sender)

{
	Waveform->SetConcatenateEpisodes(ConcatenateEpisodesCheckBox->Checked);
}
//---------------------------------------------------------------------------

