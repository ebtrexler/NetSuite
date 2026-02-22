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

#include "GUI_TextFileImportOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTextFileImportOptionsForm *TextFileImportOptionsForm;
//---------------------------------------------------------------------------
__fastcall TTextFileImportOptionsForm::TTextFileImportOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool __fastcall TTextFileImportOptionsForm::GetFirstColumnTime()
{
	return FirstColumnTimeCheckBox->Checked;
}
//---------------------------------------------------------------------------

bool __fastcall TTextFileImportOptionsForm::GetFirstRowHeader()
{
	return FirstRowHeaderCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TTextFileImportOptionsForm::FirstColumnTimeCheckBoxClick(TObject *Sender)

{
	SampleRateEdit->Enabled = !FirstColumnTimeCheckBox->Checked;
}
//---------------------------------------------------------------------------

