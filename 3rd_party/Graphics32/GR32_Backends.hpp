// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_backends.pas' rev: 21.00

#ifndef Gr32_backendsHPP
#define Gr32_backendsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_image.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_backends
{
//-- type declarations -------------------------------------------------------
__interface ITextSupport;
typedef System::DelphiInterface<ITextSupport> _di_ITextSupport;
__interface  INTERFACE_UUID("{225997CC-958A-423E-8B60-9EDE0D3B53B5}") ITextSupport  : public System::IInterface 
{
	
public:
	virtual void __fastcall Textout(int X, int Y, const System::UnicodeString Text) = 0 /* overload */;
	virtual void __fastcall Textout(int X, int Y, const Types::TRect &ClipRect, const System::UnicodeString Text) = 0 /* overload */;
	virtual void __fastcall Textout(Types::TRect &DstRect, const unsigned Flags, const System::UnicodeString Text) = 0 /* overload */;
	virtual tagSIZE __fastcall TextExtent(const System::UnicodeString Text) = 0 ;
	virtual void __fastcall TextoutW(int X, int Y, const System::WideString Text) = 0 /* overload */;
	virtual void __fastcall TextoutW(int X, int Y, const Types::TRect &ClipRect, const System::WideString Text) = 0 /* overload */;
	virtual void __fastcall TextoutW(Types::TRect &DstRect, const unsigned Flags, const System::WideString Text) = 0 /* overload */;
	virtual tagSIZE __fastcall TextExtentW(const System::WideString Text) = 0 ;
};

__interface IFontSupport;
typedef System::DelphiInterface<IFontSupport> _di_IFontSupport;
__interface  INTERFACE_UUID("{67C73044-1EFF-4FDE-AEA2-56BFADA50A48}") IFontSupport  : public System::IInterface 
{
	
public:
	virtual Classes::TNotifyEvent __fastcall GetOnFontChange(void) = 0 ;
	virtual void __fastcall SetOnFontChange(Classes::TNotifyEvent Handler) = 0 ;
	virtual Graphics::TFont* __fastcall GetFont(void) = 0 ;
	virtual void __fastcall SetFont(const Graphics::TFont* Font) = 0 ;
	virtual void __fastcall UpdateFont(void) = 0 ;
	__property Graphics::TFont* Font = {read=GetFont, write=SetFont};
	__property Classes::TNotifyEvent OnFontChange = {read=GetOnFontChange, write=SetOnFontChange};
};

__interface ICanvasSupport;
typedef System::DelphiInterface<ICanvasSupport> _di_ICanvasSupport;
__interface  INTERFACE_UUID("{5ACFEEC7-0123-4AD8-8AE6-145718438E01}") ICanvasSupport  : public System::IInterface 
{
	
public:
	virtual Classes::TNotifyEvent __fastcall GetCanvasChange(void) = 0 ;
	virtual void __fastcall SetCanvasChange(Classes::TNotifyEvent Handler) = 0 ;
	virtual Graphics::TCanvas* __fastcall GetCanvas(void) = 0 ;
	virtual void __fastcall DeleteCanvas(void) = 0 ;
	virtual bool __fastcall CanvasAllocated(void) = 0 ;
	__property Graphics::TCanvas* Canvas = {read=GetCanvas};
	__property Classes::TNotifyEvent OnCanvasChange = {read=GetCanvasChange, write=SetCanvasChange};
};

__interface IDeviceContextSupport;
typedef System::DelphiInterface<IDeviceContextSupport> _di_IDeviceContextSupport;
__interface  INTERFACE_UUID("{DD1109DA-4019-4A5C-A450-3631A73CF288}") IDeviceContextSupport  : public System::IInterface 
{
	
public:
	virtual HDC __fastcall GetHandle(void) = 0 ;
	virtual void __fastcall Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, HDC hSrc) = 0 ;
	virtual void __fastcall DrawTo(HDC hDst, int DstX, int DstY) = 0 /* overload */;
	virtual void __fastcall DrawTo(HDC hDst, const Types::TRect &DstRect, const Types::TRect &SrcRect) = 0 /* overload */;
	__property HDC Handle = {read=GetHandle};
};

__interface IBitmapContextSupport;
typedef System::DelphiInterface<IBitmapContextSupport> _di_IBitmapContextSupport;
__interface  INTERFACE_UUID("{DF0F9475-BA13-4C6B-81C3-D138624C4D08}") IBitmapContextSupport  : public System::IInterface 
{
	
public:
	virtual tagBITMAPINFO __fastcall GetBitmapInfo(void) = 0 ;
	virtual unsigned __fastcall GetBitmapHandle(void) = 0 ;
	__property tagBITMAPINFO BitmapInfo = {read=GetBitmapInfo};
	__property unsigned BitmapHandle = {read=GetBitmapHandle};
};

__interface IPaintSupport;
typedef System::DelphiInterface<IPaintSupport> _di_IPaintSupport;
__interface  INTERFACE_UUID("{CE64DBEE-C4A9-4E8E-ABCA-1B1FD6F45924}") IPaintSupport  : public System::IInterface 
{
	
public:
	virtual void __fastcall ImageNeeded(void) = 0 ;
	virtual void __fastcall CheckPixmap(void) = 0 ;
	virtual void __fastcall DoPaint(Gr32::TBitmap32* ABuffer, Gr32_containers::TRectList* AInvalidRects, Graphics::TCanvas* ACanvas, Gr32_image::TCustomPaintBox32* APaintBox) = 0 ;
};

#pragma option push -b-
enum TRequireOperatorMode { romAnd, romOr };
#pragma option pop

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE System::ResourceString _RCStrCannotAllocateDIBHandle;
#define Gr32_backends_RCStrCannotAllocateDIBHandle System::LoadResourceString(&Gr32_backends::_RCStrCannotAllocateDIBHandle)
extern PACKAGE System::ResourceString _RCStrCannotCreateCompatibleDC;
#define Gr32_backends_RCStrCannotCreateCompatibleDC System::LoadResourceString(&Gr32_backends::_RCStrCannotCreateCompatibleDC)
extern PACKAGE System::ResourceString _RCStrCannotSelectAnObjectIntoDC;
#define Gr32_backends_RCStrCannotSelectAnObjectIntoDC System::LoadResourceString(&Gr32_backends::_RCStrCannotSelectAnObjectIntoDC)
extern PACKAGE void __fastcall RequireBackendSupport(Gr32::TCustomBitmap32* TargetBitmap, GUID *RequiredInterfaces, const int RequiredInterfaces_Size, TRequireOperatorMode Mode, bool UseOptimizedDestructiveSwitchMethod, /* out */ Gr32::TCustomBackend* &ReleasedBackend);
extern PACKAGE void __fastcall RestoreBackend(Gr32::TCustomBitmap32* TargetBitmap, const Gr32::TCustomBackend* SavedBackend);

}	/* namespace Gr32_backends */
using namespace Gr32_backends;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_backendsHPP
