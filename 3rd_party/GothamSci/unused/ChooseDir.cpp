//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChooseDir.h"
//--------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChooseDirForm *ChooseDirForm;
//---------------------------------------------------------------------------
__fastcall TChooseDirForm::TChooseDirForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChooseDirForm::Button1Click(TObject *Sender)
{
	ChosenDirectory = DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
