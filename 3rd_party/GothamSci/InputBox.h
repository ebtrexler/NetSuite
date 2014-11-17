//---------------------------------------------------------------------------

#ifndef InputBoxH
#define InputBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TInputBox : public TCustomControl
{
private:
	// Catch this event and gobble it
//   void __fastcall WMEraseBkgnd(TWMEraseBkgnd &Message) {}

	// Property storage and handlers
   TNotifyEvent 				FOnInput;
   TKeyPressEvent          FOnKeyPress;
   TAlignment					FAlignment;
   void __fastcall			SetAlignment(TAlignment alignment);
   UnicodeString				FText;
   void __fastcall			SetText(const String text);
   bool 							FAutoHeight;
   void __fastcall			SetAutoHeight(bool set);
   int							FMaxChars;
   bool							FDigitsOnly;
   void __fastcall			SetDigitsOnly(bool set);
   bool                    FChangedAlert;
   void __fastcall         SetChangedAlert(bool set);

   // User interface
   bool 							IsChanged;
   bool							UseSelectionColor;
   bool							RestartInput;

   // PopupMenu handlers
   void __fastcall			Copy(TObject *Sender);
   void __fastcall			Cut(TObject *Sender);
   void __fastcall			Paste(TObject *Sender);

   // Other funcs
   bool __fastcall			IsDigit(wchar_t &c);
   bool __fastcall			IsAlphaNumeric(wchar_t &c);

protected:
	// Event handlers from TWinControl
   DYNAMIC void __fastcall KeyDown(WORD &Key, Classes::TShiftState Shift);
//   DYNAMIC void __fastcall KeyUp(WORD &Key, Classes::TShiftState Shift);
   DYNAMIC void __fastcall	KeyPress(wchar_t &Key);
   DYNAMIC void __fastcall DoEnter();
   DYNAMIC void __fastcall DoExit();
   DYNAMIC void __fastcall MouseDown(TMouseButton Button,
   											Classes::TShiftState Shift,
                                    int X, int Y);
   virtual void __fastcall Paint();

   // New event handlers
   virtual void __fastcall Input(wchar_t &Key);
   virtual void __fastcall AlertChange();

public:
	void __fastcall         Flash(int cycles);
   bool __fastcall         GetValue(double &number);
   bool __fastcall         GetValue(int &number);

	__fastcall              TInputBox(TComponent* Owner);

__published:
	// Hidden now seen
   __property ParentColor = {default = false};
   __property Color ={stored = true, nodefault};
	__property ParentFont;
	__property Font;
   __property Constraints;
   __property Anchors;
   __property Align;
   __property TabOrder;
   __property TabStop;

   __property OnEnter;
   __property OnExit;
   __property OnKeyPress;
   __property OnKeyDown;
   __property OnKeyUp;

   // Changed from TCustomControl
   __property Height = {default = 17};
   __property Width = {default = 50};

   // New
   __property TNotifyEvent OnInput = {read = FOnInput, write = FOnInput};

   __property bool			DigitsOnly = {read = FDigitsOnly,
   												write = SetDigitsOnly,
                                       default = true};
   __property bool 			AutoHeight = {read = FAutoHeight,
   												write = SetAutoHeight,
                                       default = true};
   __property int				MaxChars = {read = FMaxChars,
   												write = FMaxChars,
                                       default = 10};
	__property TAlignment	Alignment = {read = FAlignment,
   												write = SetAlignment,
                                       default = taRightJustify};
   __property String       Text = 		{read = FText, write = SetText};

   __property bool         ChangedAlert = {read = FChangedAlert,
                                          write = SetChangedAlert,
                                          default = true};

/*
BEGIN_MESSAGE_MAP
   MESSAGE_HANDLER(WM_ERASEBKGND, TWMEraseBkgnd, WMEraseBkgnd);
END_MESSAGE_MAP(TCustomControl)
//*/
};
//---------------------------------------------------------------------------
#endif


