// CodeGear C++Builder
// Copyright (c) 1995, 2015 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AudioIO.pas' rev: 30.00 (Windows)

#ifndef AudioioHPP
#define AudioioHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <Winapi.Messages.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Winapi.MMSystem.hpp>
#include <UAFDefs.hpp>

//-- user supplied -----------------------------------------------------------

namespace Audioio
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TCallBackWinOut;
class DELPHICLASS TCallBackWinIn;
class DELPHICLASS TAudioIO;
class DELPHICLASS TAudioOut;
class DELPHICLASS TAudioIn;
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TBufferEvent)(System::TObject* Sender, char * Buffer, int &Size, bool &Continue);

typedef TAudioIO* *PAudioIO;

typedef TAudioOut* *PAudioOut;

typedef TAudioIn* *PAudioIn;

class PASCALIMPLEMENTATION TCallBackWinOut : public Vcl::Controls::TWinControl
{
	typedef Vcl::Controls::TWinControl inherited;
	
private:
	TAudioOut* *AudioComponent;
	MESSAGE void __fastcall BufferDone(Winapi::Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveOpen(Winapi::Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveClose(Winapi::Messages::TMessage &Msg);
public:
	/* TWinControl.Create */ inline __fastcall virtual TCallBackWinOut(System::Classes::TComponent* AOwner) : Vcl::Controls::TWinControl(AOwner) { }
	/* TWinControl.CreateParented */ inline __fastcall TCallBackWinOut(HWND ParentWindow) : Vcl::Controls::TWinControl(ParentWindow) { }
	/* TWinControl.Destroy */ inline __fastcall virtual ~TCallBackWinOut(void) { }
	
};


class PASCALIMPLEMENTATION TCallBackWinIn : public Vcl::Controls::TWinControl
{
	typedef Vcl::Controls::TWinControl inherited;
	
private:
	TAudioIn* *AudioComponent;
	MESSAGE void __fastcall BufferFinished(Winapi::Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveOpenIn(Winapi::Messages::TMessage &Msg);
	MESSAGE void __fastcall WaveCloseIn(Winapi::Messages::TMessage &Msg);
public:
	/* TWinControl.Create */ inline __fastcall virtual TCallBackWinIn(System::Classes::TComponent* AOwner) : Vcl::Controls::TWinControl(AOwner) { }
	/* TWinControl.CreateParented */ inline __fastcall TCallBackWinIn(HWND ParentWindow) : Vcl::Controls::TWinControl(ParentWindow) { }
	/* TWinControl.Destroy */ inline __fastcall virtual ~TCallBackWinIn(void) { }
	
};


class PASCALIMPLEMENTATION TAudioIO : public System::Classes::TComponent
{
	typedef System::Classes::TComponent inherited;
	
protected:
	int FBufferSize;
	int FRequestedBufferSize;
	int FNumBuffers;
	bool FPaused;
	tWAVEFORMATEX FWaveFmtEx;
	System::Classes::TNotifyEvent FonOpen;
	System::Classes::TNotifyEvent FonClose;
	unsigned FWaveDevice;
	System::StaticArray<NativeUInt, 4> hWaveHeader;
	System::StaticArray<Winapi::Mmsystem::PWaveHdr, 4> WaveHdr;
	System::StaticArray<char *, 4> WaveBuffer;
	System::StaticArray<NativeUInt, 4> hWaveBuffer;
	int BufIndex;
	bool ContinueProcessing;
	void __fastcall SetNumBuffers(int Value);
	void __fastcall SetBufferSize(int Value);
	void __fastcall SetFrameRate(int Value);
	void __fastcall SetStereo(bool Value);
	void __fastcall SetBits(System::Word Value);
	int __fastcall GetFrameRate(void);
	bool __fastcall GetStereo(void);
	void __fastcall MakeWaveFmtConsistent(void);
	bool __fastcall InitWaveHeaders(void);
	bool __fastcall AllocPCMBuffers(void);
	bool __fastcall FreePCMBuffers(void);
	bool __fastcall AllocWaveHeaders(void);
	void __fastcall FreeWaveHeaders(void);
	
public:
	System::UnicodeString ErrorMessage;
	bool Active;
	int FilledBuffers;
	int QueuedBuffers;
	int ProcessedBuffers;
	__fastcall virtual TAudioIO(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TAudioIO(void);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	
__published:
	__property int BufferSize = {read=FBufferSize, write=SetBufferSize, default=8192};
	__property int NumBuffers = {read=FNumBuffers, write=SetNumBuffers, default=4};
	__property int FrameRate = {read=GetFrameRate, write=SetFrameRate, default=22055};
	__property bool Stereo = {read=GetStereo, write=SetStereo, default=0};
	__property System::Word Quantization = {read=FWaveFmtEx.wBitsPerSample, write=SetBits, default=16};
	__property unsigned WaveDevice = {read=FWaveDevice, write=FWaveDevice, default=0};
	__property System::Classes::TNotifyEvent OnStart = {read=FonOpen, write=FonOpen};
	__property System::Classes::TNotifyEvent OnStop = {read=FonClose, write=FonClose};
};


class PASCALIMPLEMENTATION TAudioOut : public TAudioIO
{
	typedef TAudioIO inherited;
	
private:
	bool WaveDeviceOpen;
	TCallBackWinOut* CallBackWin;
	TBufferEvent FOnFillBuffer;
	bool __fastcall QueueBuffer(void);
	virtual bool __fastcall ReadBuffer(int Idx, int N);
	void __fastcall SetPaused(bool Value);
	virtual void __fastcall CloseWaveDevice(void);
	virtual bool __fastcall Setup(TAudioOut* &TS);
	bool __fastcall StartIt(void);
	
public:
	NativeInt WaveHandle;
	bool __fastcall Start(TAudioOut* &TS);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	double __fastcall ElapsedTime(void);
	
__published:
	__property bool Paused = {read=FPaused, write=SetPaused, default=0};
	__property TBufferEvent OnFillBuffer = {read=FOnFillBuffer, write=FOnFillBuffer};
public:
	/* TAudioIO.Create */ inline __fastcall virtual TAudioOut(System::Classes::TComponent* AOwner) : TAudioIO(AOwner) { }
	/* TAudioIO.Destroy */ inline __fastcall virtual ~TAudioOut(void) { }
	
};


class PASCALIMPLEMENTATION TAudioIn : public TAudioIO
{
	typedef TAudioIO inherited;
	
private:
	bool WaveDeviceOpen;
	TCallBackWinIn* CallBackWin;
	TBufferEvent FOnBufferFilled;
	bool __fastcall QueueBuffer(void);
	virtual bool __fastcall ProcessBuffer(char * B, int N);
	virtual void __fastcall CloseWaveDevice(void);
	virtual bool __fastcall Setup(TAudioIn* &TS);
	bool __fastcall StartIt(void);
	
public:
	NativeInt WaveHandle;
	bool __fastcall Start(TAudioIn* &TS);
	virtual void __fastcall StopAtOnce(void);
	virtual void __fastcall StopGracefully(void);
	double __fastcall ElapsedTime(void);
	
__published:
	__property TBufferEvent OnBufferFilled = {read=FOnBufferFilled, write=FOnBufferFilled};
public:
	/* TAudioIO.Create */ inline __fastcall virtual TAudioIn(System::Classes::TComponent* AOwner) : TAudioIO(AOwner) { }
	/* TAudioIO.Destroy */ inline __fastcall virtual ~TAudioIn(void) { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const System::Int8 MAXBUFFERS = System::Int8(0x4);
extern DELPHI_PACKAGE void __fastcall Register(void);
}	/* namespace Audioio */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_AUDIOIO)
using namespace Audioio;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AudioioHPP
