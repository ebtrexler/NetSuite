// CodeGear C++Builder
// Copyright (c) 1995, 2015 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Knob.pas' rev: 30.00 (Windows)

#ifndef KnobHPP
#define KnobHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Winapi.Messages.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include <System.SysUtils.hpp>
#include <System.UITypes.hpp>
#include <System.Types.hpp>

//-- user supplied -----------------------------------------------------------

namespace Knob
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TKnob;
//-- type declarations -------------------------------------------------------
class PASCALIMPLEMENTATION TKnob : public Vcl::Controls::TCustomControl
{
	typedef Vcl::Controls::TCustomControl inherited;
	
private:
	System::Uitypes::TColor fLineColor;
	int fMin;
	int fMax;
	int fPosition;
	Vcl::Stdctrls::TLabel* fPositionLabel;
	bool fSpringLoaded;
	System::Word fPageSize;
	int fDiameter;
	int fRadius;
	int fSteps;
	float fAngleInterval;
	int fAngle;
	int fMouseAngle;
	bool fDragging;
	System::Types::TPoint fIndicatorInnerPosition;
	System::Types::TPoint fIndicatorOuterPosition;
	System::Types::TPoint fLastIndicatorInnerPosition;
	System::Types::TPoint fLastIndicatorOuterPosition;
	System::Classes::TNotifyEvent fOnChange;
	void __fastcall SetMin(const int NewMinValue);
	void __fastcall SetMax(const int NewMaxValue);
	void __fastcall SetPosition(const int NewPosition);
	void __fastcall SetParams(int APosition, int AMin, int AMax);
	void __fastcall SetSteps(void);
	void __fastcall CalcAngle(void);
	int __fastcall CalcPosition(const int TheAngle);
	void __fastcall SetPositionLabel(Vcl::Stdctrls::TLabel* const NewLabel);
	void __fastcall ShowPosition(const int ThePosition);
	void __fastcall SetSpringLoaded(const bool Sprung);
	void __fastcall SetLineColor(System::Uitypes::TColor NewColor);
	MESSAGE void __fastcall WMCreate(Winapi::Messages::TWMCreate &Message);
	HIDESBASE MESSAGE void __fastcall WMSize(Winapi::Messages::TWMSize &Message);
	HIDESBASE MESSAGE void __fastcall WMSetFocus(Winapi::Messages::TWMSetFocus &Message);
	HIDESBASE MESSAGE void __fastcall WMKillFocus(Winapi::Messages::TWMKillFocus &Message);
	MESSAGE void __fastcall WMGetDlgCode(Winapi::Messages::TWMNoParams &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Winapi::Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMEnabledChanged(Winapi::Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMVisibleChanged(Winapi::Messages::TMessage &Message);
	MESSAGE void __fastcall CM_ParentColorChanged(Winapi::Messages::TMessage &Msg);
	MESSAGE void __fastcall CM_TextChanged(Winapi::Messages::TMessage &Msg);
	
protected:
	virtual void __fastcall Paint(void);
	void __fastcall PaintKnob(void);
	void __fastcall PaintState(void);
	void __fastcall PaintIndicator(void);
	DYNAMIC void __fastcall MouseUp(System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseDown(System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(System::Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall KeyDown(System::Word &Key, System::Classes::TShiftState Shift);
	virtual void __fastcall Notification(System::Classes::TComponent* AComponent, System::Classes::TOperation Operation);
	
public:
	__fastcall virtual TKnob(System::Classes::TComponent* AOwner);
	
__published:
	__property ParentShowHint = {default=1};
	__property ShowHint;
	__property int Max = {read=fMax, write=SetMax, default=127};
	__property int Min = {read=fMin, write=SetMin, default=0};
	__property Caption = {default=0};
	__property System::Uitypes::TColor LineColor = {read=fLineColor, write=SetLineColor, default=-16777214};
	__property int Position = {read=fPosition, write=SetPosition, default=0};
	__property System::Word PageSize = {read=fPageSize, write=fPageSize, default=10};
	__property Vcl::Stdctrls::TLabel* PositionLabel = {read=fPositionLabel, write=SetPositionLabel};
	__property bool SpringLoaded = {read=fSpringLoaded, write=SetSpringLoaded, default=0};
	__property TabStop = {default=1};
	__property TabOrder = {default=-1};
	__property PopupMenu;
	__property Visible = {default=1};
	__property Enabled = {default=1};
	__property System::Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
	__property OnClick;
	__property OnEnter;
	__property OnExit;
	__property OnKeyDown;
	__property OnKeyPress;
	__property OnKeyUp;
	__property OnMouseDown;
	__property OnMouseMove;
	__property OnMouseUp;
public:
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TKnob(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TKnob(HWND ParentWindow) : Vcl::Controls::TCustomControl(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall Register(void);
}	/* namespace Knob */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_KNOB)
using namespace Knob;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// KnobHPP
