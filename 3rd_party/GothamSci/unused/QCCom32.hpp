// CodeGear C++Builder
// Copyright (c) 1995, 2013 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QCCom32.pas' rev: 25.00 (Windows)

#ifndef Qccom32HPP
#define Qccom32HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <Vcl.ExtCtrls.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <Vcl.Forms.hpp>	// Pascal unit
#include <Vcl.Dialogs.hpp>	// Pascal unit
#include <Vcl.Graphics.hpp>	// Pascal unit
#include <Vcl.Controls.hpp>	// Pascal unit
#include <Vcl.Buttons.hpp>	// Pascal unit
#include <Vcl.StdCtrls.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qccom32
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TformPickCom;
class PASCALIMPLEMENTATION TformPickCom : public Vcl::Forms::TForm
{
	typedef Vcl::Forms::TForm inherited;
	
__published:
	Vcl::Buttons::TBitBtn* OKBtn;
	Vcl::Buttons::TBitBtn* CancelBtn;
	Vcl::Extctrls::TRadioGroup* radioCom;
public:
	/* TCustomForm.Create */ inline __fastcall virtual TformPickCom(System::Classes::TComponent* AOwner) : Vcl::Forms::TForm(AOwner) { }
	/* TCustomForm.CreateNew */ inline __fastcall virtual TformPickCom(System::Classes::TComponent* AOwner, int Dummy) : Vcl::Forms::TForm(AOwner, Dummy) { }
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TformPickCom(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TformPickCom(HWND ParentWindow) : Vcl::Forms::TForm(ParentWindow) { }
	
};


class DELPHICLASS TQCCom32;
class PASCALIMPLEMENTATION TQCCom32 : public System::Classes::TComponent
{
	typedef System::Classes::TComponent inherited;
	
private:
	int hPort;
	int nPort;
	int lBaud;
	System::UnicodeString sInTerminator;
	bool boolExpired;
	Vcl::Extctrls::TTimer* Timer;
	TformPickCom* formPickCom;
	bool boolShowErrors;
	bool boolInUse;
	System::Classes::TNotifyEvent pmOnTimeout;
	void __fastcall SetTimeout(int lTimeout);
	int __fastcall GetTimeout(void);
	void __fastcall SetBaud(int lBaudToSet);
	void __fastcall SetPort(int nPortToSet);
	int __fastcall GetInCount(void);
	void __fastcall TimesUp(System::TObject* Sender);
	bool __fastcall IsOpen(void);
	
protected:
	DYNAMIC void __fastcall Timeout(void);
	
public:
	__fastcall virtual TQCCom32(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TQCCom32(void);
	void __fastcall Pick(void);
	bool __fastcall Open(void);
	bool __fastcall Write(const System::UnicodeString sData);
	System::UnicodeString __fastcall Read(void);
	void __fastcall Flush(void);
	void __fastcall Close(void);
	void __fastcall SetComPort(int nPortToSet);
	
__published:
	__property int ResponseTime = {read=GetTimeout, write=SetTimeout, nodefault};
	__property System::Classes::TNotifyEvent OnTimeout = {read=pmOnTimeout, write=pmOnTimeout};
	__property bool TimedOut = {read=boolExpired, nodefault};
	__property int Baud = {read=lBaud, write=SetBaud, nodefault};
	__property int Port = {read=nPort, write=SetPort, nodefault};
	__property int InCount = {read=GetInCount, nodefault};
	__property System::UnicodeString EndOfResponse = {read=sInTerminator, write=sInTerminator};
	__property bool ShowErrors = {read=boolShowErrors, write=boolShowErrors, nodefault};
	__property bool Opened = {read=IsOpen, nodefault};
	__property bool InUse = {read=boolInUse, nodefault};
};


//-- var, const, procedure ---------------------------------------------------
}	/* namespace Qccom32 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_QCCOM32)
using namespace Qccom32;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Qccom32HPP
