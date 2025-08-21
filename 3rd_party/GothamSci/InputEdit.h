//---------------------------------------------------------------------------
#ifndef InputEditH
#define InputEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>                 
//---------------------------------------------------------------------------
class PACKAGE TInputEdit : public TEdit
{
private:
   //Property storage and handlers
   TNotifyEvent FOnInput;

   bool IsChanged;
protected:
   //Event Handlers
   void __fastcall Input(wchar_t &Key);
   DYNAMIC void __fastcall KeyPress(wchar_t &Key);
   DYNAMIC void __fastcall DoEnter();
   DYNAMIC void __fastcall DoExit();
   DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   DYNAMIC void __fastcall Change();

public:
	void __fastcall Flash(int cycles);
   __fastcall TInputEdit(TComponent* Owner);
__published:
   __property TNotifyEvent OnInput = {read = FOnInput, write = FOnInput};
};
//---------------------------------------------------------------------------
#endif                                        
