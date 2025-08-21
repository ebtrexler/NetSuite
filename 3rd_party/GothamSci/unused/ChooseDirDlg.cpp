//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "choosedir.h"
#include "ChooseDirDlg.h"
#pragma package(smart_init)

#pragma link "choosedir"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TChooseDirDialog *)
{
	new TChooseDirDialog(NULL);
}
//---------------------------------------------------------------------------
__fastcall TChooseDirDialog::TChooseDirDialog(TComponent* Owner)
   : TComponent(Owner)
{
}
//---------------------------------------------------------------------------
//namespace Choosedirdialog
//{
//   void __fastcall PACKAGE Register()
//   {
//		 TComponentClass classes[1] = {__classid(TChooseDirDialog)};
//		 RegisterComponents("Components", classes, 0);
//   }
//}
//---------------------------------------------------------------------------
bool __fastcall TChooseDirDialog::Execute()
{
	ChooseDirForm = new TChooseDirForm(Application);
   ChooseDirForm->ChosenDirectory = Directory;
   bool Result;
   try
   {
      Result = (ChooseDirForm->ShowModal() == IDOK);
      if (Result){
         Directory = ChooseDirForm->ChosenDirectory;
      }
   }
   catch(...)
   {
      Result = false;
   }

   ChooseDirForm->Release();
   return Result;
}
//---------------------------------------------------------------------------
void __fastcall TChooseDirDialog::SetDirLabel(TLabel *label)
{
   FDirLabel = label;
   FDirLabel->Caption = Directory;
}
//---------------------------------------------------------------------------
void __fastcall TChooseDirDialog::SetDirectory(AnsiString dir)
{
   FDirectory = dir;
   FDirLabel->Caption = Directory;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TChooseDirDialog::GetDirectory()
{
//  FDirectory = ChooseDirForm->ChosenDirectory;
   return FDirectory;
}
//---------------------------------------------------------------------------