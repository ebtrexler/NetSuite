//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <clipbrd.hpp>

#include "InputBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TInputBox *)
{
	new TInputBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TInputBox::TInputBox(TComponent* Owner)
	: TCustomControl(Owner)
{
	IsChanged = false;
   UseSelectionColor = false;
   RestartInput = true;
   FAlignment = taRightJustify;
   FAutoHeight = true;
   FMaxChars = 10;
   FChangedAlert = true;
   FText = Name;
   FDigitsOnly = true;
   Height = 17;
   Width = 50;
   TabStop = true;

   ParentColor = false;
   
   //setup PopupMenu
   PopupMenu = new TPopupMenu(this);
   TMenuItem *item = new TMenuItem(PopupMenu);
   item->Caption = "Copy";
   item->OnClick = Copy;
   PopupMenu->Items->Add(item);
   item = new TMenuItem(PopupMenu);
   item->Caption = "Cut";
   item->OnClick = Cut;
   PopupMenu->Items->Add(item);
   item = new TMenuItem(PopupMenu);
   item->Caption = "Paste";
   item->OnClick = Paste;
   PopupMenu->Items->Add(item);
   PopupMenu->AutoPopup = true;
}
//---------------------------------------------------------------------------
namespace Inputbox
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TInputBox)};
		RegisterComponents("Gotham Scientific", classes, 0);
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::KeyDown(WORD &Key, Classes::TShiftState Shift)
{
	if (Shift.Contains(ssCtrl)){
   	if (Key == 'c' || Key == 'C') Copy(0);
      if (Key == 'x' || Key == 'X') Cut(0);
      if (Key == 'v' || Key == 'V') Paste(0);
//      Key = 0;
   }
   else if (Key == VK_DELETE){
   	FText = "";
	   Invalidate();
   }
  	TWinControl::KeyDown(Key, Shift);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::KeyPress(wchar_t &Key)
{
   if (Key == VK_RETURN || Key == VK_TAB){
      Input(Key);
   }
   else {
   	if (RestartInput){
         FText = "";
         RestartInput = false;
      }
      if (Key == 8){ //backspace
         FText.Delete(FText.Length(), FMaxChars);
         AlertChange();
         Invalidate();
      }
      else if (FText.Length() > FMaxChars);		//
      else if (FDigitsOnly && !IsDigit(Key));	// trap errors
      else if (!IsAlphaNumeric(Key));				//
      else {
      	FText += Key;
         if (FChangedAlert) AlertChange();
      	Invalidate();
      }
   }
   TWinControl::KeyPress(Key);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::MouseDown(TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y)
{
   TControl::MouseDown(Button, Shift, X, Y);
   UseSelectionColor = true;
   Invalidate();
   SetFocus();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::DoEnter()
{
   TWinControl::DoEnter();
   UseSelectionColor = true;
   Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::DoExit()
{
   TWinControl::DoExit();
   UseSelectionColor = false;
   Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::Paint()
{
   Canvas->Font->Assign(Font);
   int textwidth = Canvas->TextWidth(FText);
   int textheight = FText != "" ? Canvas->TextHeight(FText) : 10;
   if (FAutoHeight){
      if (Height != textheight + 3){
         Height = textheight + 3;
         return; // changing the height means this function is called again
                  //because the window is invalidated
      }
	}
   int left, top = 2;
   switch (FAlignment) {
   	case taLeftJustify: left = 2; break;
      case taCenter:	left = Width/2 - textwidth/2; break;
      case taRightJustify: left = Width - textwidth - 1; break;
   };
   TColor backcolor, textcolor;
   backcolor = UseSelectionColor ? clHighlight : Color;
   textcolor = UseSelectionColor ? clHighlightText : Font->Color;
	Canvas->Font->Color = textcolor;
   Canvas->Brush->Color = backcolor;
   Canvas->Pen->Color = clBtnShadow;
   Canvas->Rectangle(ClientRect);
   Canvas->TextOut(left, top, FText);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::Input(wchar_t &Key)
{
   if (FOnInput){
      FOnInput(this);
   }
   Key = 0;
   Font->Style = TFontStyles();
   IsChanged = false;
   RestartInput = true;
   Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// called from SetText to notify of outside changes
void __fastcall TInputBox::AlertChange()
{
   if (ComponentState.Contains(csLoading) ||
   	ComponentState.Contains(csDesigning)) return;
	if (!IsChanged){  //enter block on first change
      Font->Style = TFontStyles() << fsBold;
      Invalidate();
   }
   IsChanged = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::SetChangedAlert(bool set)
{
   FChangedAlert = set;
   Font->Style = TFontStyles();
   Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall TInputBox::IsDigit(wchar_t &c)
{
	if (c == 45 ||							// -
     	 c == 46 ||							// .
     	(c >= 48 && c <= 57)){			// 0..9
      return true;
   }
   return false;
}
bool __fastcall TInputBox::IsAlphaNumeric(wchar_t &c)
{
	if (IsDigit(c) ||
   	(c >= 65 && c <= 90) ||  		// A..Z
      (c >= 97 && c <= 122)){			// a..z
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::Flash(int cycles)
{
   if (cycles < 1) return;
   TFont *normal = new TFont;
   normal->Assign(Font);
   normal->Style = TFontStyles();
   TFont *bold = new TFont;
   bold->Assign(Font);
   bold->Style = TFontStyles() << fsBold;
	for (int i=0; i<cycles; i++) {
      Font->Assign(bold);
      Invalidate();
      Application->ProcessMessages();
      ::Sleep(70);
		Font->Assign(normal);
      Invalidate();
      Application->ProcessMessages();
      ::Sleep(70);
   }
   RestartInput = true;
   delete bold;
   delete normal;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall TInputBox::GetValue(double &number)
{
   double temp;
	try {
   	temp = StrToFloat(FText);
   }
   catch (...) {
   	return false;  // could not covert, squash exceptions
   }

   number = temp;
   return true;
}

bool __fastcall TInputBox::GetValue(int &number)
{
   int temp;
	try {
   	temp = StrToInt(FText);
   }
   catch (...) {
   	return false;  // could not covert, squash exceptions
   }

   number = temp;
   return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::SetAlignment(TAlignment alignment)
{
	if (alignment != FAlignment){
   	FAlignment = alignment;
      Invalidate();
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::SetText(const String text)
{
	if (text != FText){
      if (text == "") {
         FText = "";
         return;
      }
   	if (text.Length() > FMaxChars) return;
      if (FDigitsOnly){
         try {
            StrToFloat(text);
         }
         catch (...) {
            return;  // could not covert, squash exceptions
         }
      }
   	FText = text;
      if (FChangedAlert) AlertChange();
      Invalidate();
   }
   RestartInput = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::SetAutoHeight(bool set)
{
  	FAutoHeight = set;
	Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TInputBox::SetDigitsOnly(bool set)
{
	FDigitsOnly = set;
   if (FDigitsOnly) Text = "";
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PopupMenu handlers

void __fastcall TInputBox::Copy(TObject *Sender)
{
	Clipboard()->AsText = FText;
}
void __fastcall TInputBox::Cut(TObject *Sender)
{
	Clipboard()->AsText = FText;
   FText = "";
   Invalidate();
}
void __fastcall TInputBox::Paste(TObject *Sender)
{
	String temp;
   try {
   	temp = Clipboard()->AsText;
   }
   catch(...) {
   	return; // no text in clipboard
   }
   Text = temp;
}
