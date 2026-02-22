// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_backends_vcl.pas' rev: 21.00

#ifndef Gr32_backends_vclHPP
#define Gr32_backends_vclHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_backends.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_image.hpp>	// Pascal unit
#include <Gr32_backends_generic.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_backends_vcl
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TGDIBackend;
class PASCALIMPLEMENTATION TGDIBackend : public Gr32::TCustomBackend
{
	typedef Gr32::TCustomBackend inherited;
	
private:
	void __fastcall FontChangedHandler(System::TObject* Sender);
	void __fastcall CanvasChangedHandler(System::TObject* Sender);
	void __fastcall CanvasChanged(void);
	void __fastcall FontChanged(void);
	
protected:
	#pragma pack(push,1)
	tagBITMAPINFO FBitmapInfo;
	#pragma pack(pop)
	HBITMAP FBitmapHandle;
	HDC FHDC;
	Graphics::TFont* FFont;
	Graphics::TCanvas* FCanvas;
	HFONT FFontHandle;
	unsigned FMapHandle;
	Classes::TNotifyEvent FOnFontChange;
	Classes::TNotifyEvent FOnCanvasChange;
	virtual void __fastcall InitializeSurface(int NewWidth, int NewHeight, bool ClearBuffer);
	virtual void __fastcall FinalizeSurface(void);
	virtual void __fastcall PrepareFileMapping(int NewWidth, int NewHeight);
	
public:
	__fastcall virtual TGDIBackend(void)/* overload */;
	__fastcall virtual ~TGDIBackend(void);
	virtual void __fastcall Changed(void);
	virtual bool __fastcall Empty(void);
	void __fastcall ImageNeeded(void);
	void __fastcall CheckPixmap(void);
	void __fastcall DoPaint(Gr32::TBitmap32* ABuffer, Gr32_containers::TRectList* AInvalidRects, Graphics::TCanvas* ACanvas, Gr32_image::TCustomPaintBox32* APaintBox);
	tagBITMAPINFO __fastcall GetBitmapInfo(void);
	unsigned __fastcall GetBitmapHandle(void);
	__property tagBITMAPINFO BitmapInfo = {read=GetBitmapInfo};
	__property unsigned BitmapHandle = {read=GetBitmapHandle, nodefault};
	HDC __fastcall GetHandle(void);
	void __fastcall Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, HDC hSrc)/* overload */;
	void __fastcall DrawTo(HDC hDst, int DstX, int DstY)/* overload */;
	void __fastcall DrawTo(HDC hDst, const Types::TRect &DstRect, const Types::TRect &SrcRect)/* overload */;
	__property HDC Handle = {read=GetHandle, nodefault};
	void __fastcall Textout(int X, int Y, const System::UnicodeString Text)/* overload */;
	void __fastcall Textout(int X, int Y, const Types::TRect &ClipRect, const System::UnicodeString Text)/* overload */;
	void __fastcall Textout(Types::TRect &DstRect, const unsigned Flags, const System::UnicodeString Text)/* overload */;
	tagSIZE __fastcall TextExtent(const System::UnicodeString Text);
	void __fastcall TextoutW(int X, int Y, const System::WideString Text)/* overload */;
	void __fastcall TextoutW(int X, int Y, const Types::TRect &ClipRect, const System::WideString Text)/* overload */;
	void __fastcall TextoutW(Types::TRect &DstRect, const unsigned Flags, const System::WideString Text)/* overload */;
	tagSIZE __fastcall TextExtentW(const System::WideString Text);
	Classes::TNotifyEvent __fastcall GetOnFontChange(void);
	void __fastcall SetOnFontChange(Classes::TNotifyEvent Handler);
	Graphics::TFont* __fastcall GetFont(void);
	void __fastcall SetFont(const Graphics::TFont* Font);
	void __fastcall UpdateFont(void);
	__property Graphics::TFont* Font = {read=GetFont, write=SetFont};
	__property Classes::TNotifyEvent OnFontChange = {read=FOnFontChange, write=FOnFontChange};
	Classes::TNotifyEvent __fastcall GetCanvasChange(void);
	void __fastcall SetCanvasChange(Classes::TNotifyEvent Handler);
	Graphics::TCanvas* __fastcall GetCanvas(void);
	void __fastcall DeleteCanvas(void);
	bool __fastcall CanvasAllocated(void);
	__property Graphics::TCanvas* Canvas = {read=GetCanvas};
	__property Classes::TNotifyEvent OnCanvasChange = {read=GetCanvasChange, write=SetCanvasChange};
private:
	void *__ICanvasSupport;	/* Gr32_backends::ICanvasSupport */
	void *__IFontSupport;	/* Gr32_backends::IFontSupport */
	void *__ITextSupport;	/* Gr32_backends::ITextSupport */
	void *__IDeviceContextSupport;	/* Gr32_backends::IDeviceContextSupport */
	void *__IBitmapContextSupport;	/* Gr32_backends::IBitmapContextSupport */
	void *__IPaintSupport;	/* Gr32_backends::IPaintSupport */
	
public:
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_ICanvasSupport()
	{
		Gr32_backends::_di_ICanvasSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator ICanvasSupport*(void) { return (ICanvasSupport*)&__ICanvasSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IFontSupport()
	{
		Gr32_backends::_di_IFontSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IFontSupport*(void) { return (IFontSupport*)&__IFontSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_ITextSupport()
	{
		Gr32_backends::_di_ITextSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator ITextSupport*(void) { return (ITextSupport*)&__ITextSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IDeviceContextSupport()
	{
		Gr32_backends::_di_IDeviceContextSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IDeviceContextSupport*(void) { return (IDeviceContextSupport*)&__IDeviceContextSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IBitmapContextSupport()
	{
		Gr32_backends::_di_IBitmapContextSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IBitmapContextSupport*(void) { return (IBitmapContextSupport*)&__IBitmapContextSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IPaintSupport()
	{
		Gr32_backends::_di_IPaintSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IPaintSupport*(void) { return (IPaintSupport*)&__IPaintSupport; }
	#endif
	
};


class DELPHICLASS TGDIMMFBackend;
class PASCALIMPLEMENTATION TGDIMMFBackend : public TGDIBackend
{
	typedef TGDIBackend inherited;
	
private:
	unsigned FMapFileHandle;
	bool FMapIsTemporary;
	System::UnicodeString FMapFileName;
	
protected:
	virtual void __fastcall PrepareFileMapping(int NewWidth, int NewHeight);
	
public:
	__fastcall virtual TGDIMMFBackend(Gr32::TBitmap32* Owner, bool IsTemporary, const System::UnicodeString MapFileName);
	__fastcall virtual ~TGDIMMFBackend(void);
};


class DELPHICLASS TGDIMemoryBackend;
class PASCALIMPLEMENTATION TGDIMemoryBackend : public Gr32_backends_generic::TMemoryBackend
{
	typedef Gr32_backends_generic::TMemoryBackend inherited;
	
private:
	void __fastcall DoPaintRect(Gr32::TBitmap32* ABuffer, const Types::TRect &ARect, Graphics::TCanvas* ACanvas);
	HDC __fastcall GetHandle(void);
	
protected:
	#pragma pack(push,1)
	tagBITMAPINFO FBitmapInfo;
	#pragma pack(pop)
	virtual void __fastcall InitializeSurface(int NewWidth, int NewHeight, bool ClearBuffer);
	
public:
	__fastcall virtual TGDIMemoryBackend(void)/* overload */;
	void __fastcall ImageNeeded(void);
	void __fastcall CheckPixmap(void);
	void __fastcall DoPaint(Gr32::TBitmap32* ABuffer, Gr32_containers::TRectList* AInvalidRects, Graphics::TCanvas* ACanvas, Gr32_image::TCustomPaintBox32* APaintBox);
	void __fastcall Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, HDC hSrc)/* overload */;
	void __fastcall DrawTo(HDC hDst, int DstX, int DstY)/* overload */;
	void __fastcall DrawTo(HDC hDst, const Types::TRect &DstRect, const Types::TRect &SrcRect)/* overload */;
public:
	/* TCustomBackend.Destroy */ inline __fastcall virtual ~TGDIMemoryBackend(void) { }
	
private:
	void *__IDeviceContextSupport;	/* Gr32_backends::IDeviceContextSupport */
	void *__IPaintSupport;	/* Gr32_backends::IPaintSupport */
	
public:
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IDeviceContextSupport()
	{
		Gr32_backends::_di_IDeviceContextSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IDeviceContextSupport*(void) { return (IDeviceContextSupport*)&__IDeviceContextSupport; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator Gr32_backends::_di_IPaintSupport()
	{
		Gr32_backends::_di_IPaintSupport intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IPaintSupport*(void) { return (IPaintSupport*)&__IPaintSupport; }
	#endif
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_backends_vcl */
using namespace Gr32_backends_vcl;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_backends_vclHPP
