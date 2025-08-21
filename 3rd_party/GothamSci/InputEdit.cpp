//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "InputEdit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TInputEdit *)
{
   new TInputEdit(NULL);
}
//---------------------------------------------------------------------------
__fastcall TInputEdit::TInputEdit(TComponent* Owner)
   : TEdit(Owner)
{
   IsChanged = false;
}
//---------------------------------------------------------------------------
namespace Inputedit
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TInputEdit)};
       RegisterComponents("Gotham Scientific", classes, 0);
   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::Input(wchar_t &Key)
{
   Key = 0;
   if (FOnInput){
      OnInput(this);
   }
   Font->Style = TFontStyles();
   IsChanged = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::KeyPress(wchar_t &Key)
{
   if (Key == VK_RETURN){
	  Input(Key);
   }
   TWinControl::KeyPress(Key);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::MouseDown(TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y)
{
   TControl::MouseDown(Button, Shift, X, Y);
   SelectAll();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::DoEnter()
{
   TWinControl::DoEnter();
   SelectAll();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::DoExit()
{
   TWinControl::DoExit();
   SelLength = 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::Change()
{
   TCustomEdit::Change();
   if (ComponentState.Contains(csLoading) ||
   	ComponentState.Contains(csDesigning)) return;
	if (!IsChanged){  //enter block on first change
      Font->Style = TFontStyles() << fsBold;
   }
   IsChanged = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputEdit::Flash(int cycles)
{
   if (cycles < 1) return;
   TFontStyles oldFontStyles = Font->Style;
	for (int i=0; i<cycles; i++) {
      Font->Style = TFontStyles() << fsBold;
      Application->ProcessMessages();
      ::Sleep(70);
      Font->Style = TFontStyles();
      Application->ProcessMessages();
      ::Sleep(70);
   }
   Font->Style = oldFontStyles;
}
//---------------------------------------------------------------------------


