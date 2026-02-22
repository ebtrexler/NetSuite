// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32.pas' rev: 21.00

#ifndef Gr32HPP
#define Gr32HPP

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
#include <Gr32_system.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32
{
//-- type declarations -------------------------------------------------------
typedef unsigned TColor32;

typedef TColor32 *PColor32;

typedef StaticArray<TColor32, 1> TColor32Array;

typedef TColor32Array *PColor32Array;

typedef DynamicArray<TColor32> TArrayOfColor32;

#pragma option push -b-
enum TColor32Component { ccBlue, ccGreen, ccRed, ccAlpha };
#pragma option pop

typedef Set<TColor32Component, ccBlue, ccAlpha>  TColor32Components;

struct TColor32Entry;
typedef TColor32Entry *PColor32Entry;

#pragma pack(push,1)
struct TColor32Entry
{
	
	union
	{
		struct 
		{
			StaticArray<System::Byte, 4> Components;
			
		};
		struct 
		{
			StaticArray<System::Byte, 4> Planes;
			
		};
		struct 
		{
			TColor32 ARGB;
			
		};
		struct 
		{
			System::Byte B;
			System::Byte G;
			System::Byte R;
			System::Byte A;
			
		};
		
	};
};
#pragma pack(pop)


typedef StaticArray<TColor32Entry, 1> TColor32EntryArray;

typedef TColor32EntryArray *PColor32EntryArray;

typedef DynamicArray<TColor32Entry> TArrayOfColor32Entry;

typedef StaticArray<TColor32, 256> TPalette32;

typedef TPalette32 *PPalette32;

typedef int TFixed;

typedef TFixed *PFixed;

struct TFixedRec;
typedef TFixedRec *PFixedRec;

#pragma pack(push,1)
struct TFixedRec
{
	
	union
	{
		struct 
		{
			System::Word Frac;
			short Int;
			
		};
		struct 
		{
			TFixed Fixed;
			
		};
		
	};
};
#pragma pack(pop)


typedef StaticArray<TFixed, 1> TFixedArray;

typedef TFixedArray *PFixedArray;

typedef DynamicArray<TFixed> TArrayOfFixed;

typedef TArrayOfFixed *PArrayOfFixed;

typedef DynamicArray<DynamicArray<TFixed> > TArrayOfArrayOfFixed;

typedef TArrayOfArrayOfFixed *PArrayOfArrayOfFixed;

typedef float *PFloat;

typedef float TFloat;

typedef StaticArray<System::Byte, 1> TByteArray;

typedef TByteArray *PByteArray;

typedef DynamicArray<System::Byte> TArrayOfByte;

typedef TArrayOfByte *PArrayOfByte;

typedef StaticArray<System::Word, 1> TWordArray;

typedef TWordArray *PWordArray;

typedef DynamicArray<System::Word> TArrayOfWord;

typedef TArrayOfWord *PArrayOfWord;

typedef StaticArray<int, 1> TIntegerArray;

typedef TIntegerArray *PIntegerArray;

typedef DynamicArray<int> TArrayOfInteger;

typedef TArrayOfInteger *PArrayOfInteger;

typedef DynamicArray<DynamicArray<int> > TArrayOfArrayOfInteger;

typedef TArrayOfArrayOfInteger *PArrayOfArrayOfInteger;

typedef StaticArray<float, 1> TSingleArray;

typedef TSingleArray *PSingleArray;

typedef DynamicArray<float> TArrayOfSingle;

typedef TArrayOfSingle *PArrayOfSingle;

typedef StaticArray<float, 1> TFloatArray;

typedef TFloatArray *PFloatArray;

typedef DynamicArray<float> TArrayOfFloat;

typedef TArrayOfFloat *PArrayOfFloat;

typedef Types::TPoint *PPoint;

typedef Types::TPoint TPoint;

typedef StaticArray<Types::TPoint, 1> TPointArray;

typedef TPointArray *PPointArray;

typedef DynamicArray<Types::TPoint> TArrayOfPoint;

typedef TArrayOfPoint *PArrayOfPoint;

typedef DynamicArray<DynamicArray<Types::TPoint> > TArrayOfArrayOfPoint;

typedef TArrayOfArrayOfPoint *PArrayOfArrayOfPoint;

struct TFloatPoint;
typedef TFloatPoint *PFloatPoint;

struct TFloatPoint
{
	
public:
	float X;
	float Y;
};


typedef StaticArray<TFloatPoint, 1> TFloatPointArray;

typedef TFloatPointArray *PFloatPointArray;

typedef DynamicArray<TFloatPoint> TArrayOfFloatPoint;

typedef TArrayOfFloatPoint *PArrayOfFloatPoint;

typedef DynamicArray<DynamicArray<TFloatPoint> > TArrayOfArrayOfFloatPoint;

typedef TArrayOfArrayOfFloatPoint *PArrayOfArrayOfFloatPoint;

struct TFixedPoint
{
	
public:
	TFixed X;
	TFixed Y;
};


typedef TFixedPoint *PFixedPoint;

typedef StaticArray<TFixedPoint, 1> TFixedPointArray;

typedef TFixedPointArray *PFixedPointArray;

typedef DynamicArray<TFixedPoint> TArrayOfFixedPoint;

typedef TArrayOfFixedPoint *PArrayOfFixedPoint;

typedef DynamicArray<DynamicArray<TFixedPoint> > TArrayOfArrayOfFixedPoint;

typedef TArrayOfArrayOfFixedPoint *PArrayOfArrayOfFixedPoint;

typedef Types::PRect PRect;

typedef Types::TRect TRect;

struct TFloatRect;
typedef TFloatRect *PFloatRect;

#pragma pack(push,1)
struct TFloatRect
{
	
	union
	{
		struct 
		{
			TFloatPoint TopLeft;
			TFloatPoint BottomRight;
			
		};
		struct 
		{
			float Left;
			float Top;
			float Right;
			float Bottom;
			
		};
		
	};
};
#pragma pack(pop)


#pragma pack(push,1)
struct TFixedRect
{
	
	union
	{
		struct 
		{
			TFixedPoint TopLeft;
			TFixedPoint BottomRight;
			
		};
		struct 
		{
			TFixed Left;
			TFixed Top;
			TFixed Right;
			TFixed Bottom;
			
		};
		
	};
};
#pragma pack(pop)


typedef TFixedRect *PFixedRect;

#pragma option push -b-
enum TRectRounding { rrClosest, rrOutside, rrInside };
#pragma option pop

#pragma option push -b-
enum TDrawMode { dmOpaque, dmBlend, dmCustom, dmTransparent };
#pragma option pop

#pragma option push -b-
enum TCombineMode { cmBlend, cmMerge };
#pragma option pop

#pragma option push -b-
enum TWrapMode { wmClamp, wmRepeat, wmMirror };
#pragma option pop

typedef int __fastcall (*TWrapProc)(int Value, int Max);

typedef int __fastcall (*TWrapProcEx)(int Value, int Min, int Max);

#pragma option push -b-
enum TStretchFilter { sfNearest, sfDraft, sfLinear, sfCosine, sfSpline, sfLanczos, sfMitchell };
#pragma option pop

class DELPHICLASS TPlainInterfacedPersistent;
class PASCALIMPLEMENTATION TPlainInterfacedPersistent : public Classes::TPersistent
{
	typedef Classes::TPersistent inherited;
	
private:
	bool FRefCounted;
	int FRefCount;
	
protected:
	int __stdcall _AddRef(void);
	int __stdcall _Release(void);
	virtual HRESULT __stdcall QueryInterface(const GUID &IID, /* out */ void *Obj);
	__property bool RefCounted = {read=FRefCounted, write=FRefCounted, nodefault};
	
public:
	virtual void __fastcall AfterConstruction(void);
	virtual void __fastcall BeforeDestruction(void);
	__classmethod virtual System::TObject* __fastcall NewInstance();
	__property int RefCount = {read=FRefCount, nodefault};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TPlainInterfacedPersistent(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TPlainInterfacedPersistent(void) : Classes::TPersistent() { }
	
private:
	void *__IInterface;	/* System::IInterface */
	
public:
	#if defined(MANAGED_INTERFACE_OPERATORS)
	operator System::_di_IInterface()
	{
		System::_di_IInterface intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IInterface*(void) { return (IInterface*)&__IInterface; }
	#endif
	
};


class DELPHICLASS TNotifiablePersistent;
class PASCALIMPLEMENTATION TNotifiablePersistent : public TPlainInterfacedPersistent
{
	typedef TPlainInterfacedPersistent inherited;
	
private:
	int FUpdateCount;
	Classes::TNotifyEvent FOnChange;
	
protected:
	__property int UpdateCount = {read=FUpdateCount, nodefault};
	
public:
	virtual void __fastcall Changed(void);
	virtual void __fastcall BeginUpdate(void);
	virtual void __fastcall EndUpdate(void);
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNotifiablePersistent(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TNotifiablePersistent(void) : TPlainInterfacedPersistent() { }
	
};


class DELPHICLASS TThreadPersistent;
class PASCALIMPLEMENTATION TThreadPersistent : public TNotifiablePersistent
{
	typedef TNotifiablePersistent inherited;
	
private:
	int FLockCount;
	
protected:
	_RTL_CRITICAL_SECTION FLock;
	__property int LockCount = {read=FLockCount, nodefault};
	
public:
	__fastcall virtual TThreadPersistent(void);
	__fastcall virtual ~TThreadPersistent(void);
	void __fastcall Lock(void);
	void __fastcall Unlock(void);
};


class DELPHICLASS TCustomMap;
class PASCALIMPLEMENTATION TCustomMap : public TThreadPersistent
{
	typedef TThreadPersistent inherited;
	
protected:
	int FHeight;
	int FWidth;
	Classes::TNotifyEvent FOnResize;
	virtual void __fastcall SetHeight(int NewHeight);
	virtual void __fastcall SetWidth(int NewWidth);
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	
public:
	virtual void __fastcall Delete(void);
	virtual bool __fastcall Empty(void);
	virtual void __fastcall Resized(void);
	bool __fastcall SetSizeFrom(Classes::TPersistent* Source);
	virtual bool __fastcall SetSize(int NewWidth, int NewHeight);
	__property int Height = {read=FHeight, write=SetHeight, nodefault};
	__property int Width = {read=FWidth, write=SetWidth, nodefault};
	__property Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};
public:
	/* TThreadPersistent.Create */ inline __fastcall virtual TCustomMap(void) : TThreadPersistent() { }
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TCustomMap(void) { }
	
};


typedef void __fastcall (__closure *TPixelCombineEvent)(TColor32 F, TColor32 &B, TColor32 M);

typedef void __fastcall (__closure *TAreaChangedEvent)(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);

typedef TMetaClass* TCustomBackendClass;

class DELPHICLASS TCustomBitmap32;
class DELPHICLASS TCustomBackend;
class DELPHICLASS TCustomResampler;
class PASCALIMPLEMENTATION TCustomBitmap32 : public TCustomMap
{
	typedef TCustomMap inherited;
	
private:
	TCustomBackend* FBackend;
	TColor32Array *FBits;
	Types::TRect FClipRect;
	TFixedRect FFixedClipRect;
	Types::TRect F256ClipRect;
	bool FClipping;
	TDrawMode FDrawMode;
	TCombineMode FCombineMode;
	TWrapMode FWrapMode;
	unsigned FMasterAlpha;
	TColor32 FOuterColor;
	TColor32 FPenColor;
	float FStippleCounter;
	TArrayOfColor32 FStipplePattern;
	float FStippleStep;
	TStretchFilter FStretchFilter;
	TPixelCombineEvent FOnPixelCombine;
	TAreaChangedEvent FOnAreaChanged;
	TAreaChangedEvent FOldOnAreaChanged;
	bool FMeasuringMode;
	TCustomResampler* FResampler;
	virtual void __fastcall BackendChangedHandler(System::TObject* Sender);
	virtual void __fastcall BackendChangingHandler(System::TObject* Sender);
	PColor32 __fastcall GetPixelPtr(int X, int Y);
	PColor32Array __fastcall GetScanLine(int Y);
	void __fastcall SetCombineMode(const TCombineMode Value);
	void __fastcall SetDrawMode(TDrawMode Value);
	void __fastcall SetWrapMode(TWrapMode Value);
	void __fastcall SetMasterAlpha(unsigned Value);
	void __fastcall SetStretchFilter(TStretchFilter Value);
	void __fastcall SetClipRect(const Types::TRect &Value);
	void __fastcall SetResampler(TCustomResampler* Resampler);
	System::UnicodeString __fastcall GetResamplerClassName(void);
	void __fastcall SetResamplerClassName(System::UnicodeString Value);
	
protected:
	TWrapProcEx WrapProcHorz;
	TWrapProcEx WrapProcVert;
	void *BlendProc;
	int RasterX;
	int RasterY;
	TFixed RasterXF;
	TFixed RasterYF;
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight);
	virtual void __fastcall CopyMapTo(TCustomBitmap32* Dst);
	virtual void __fastcall CopyPropertiesTo(TCustomBitmap32* Dst);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	bool __fastcall Equal(TCustomBitmap32* B);
	void __fastcall SET_T256(int X, int Y, TColor32 C);
	void __fastcall SET_TS256(int X, int Y, TColor32 C);
	TColor32 __fastcall GET_T256(int X, int Y);
	TColor32 __fastcall GET_TS256(int X, int Y);
	virtual void __fastcall ReadData(Classes::TStream* Stream);
	virtual void __fastcall WriteData(Classes::TStream* Stream);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	virtual void __fastcall InitializeBackend(void);
	virtual void __fastcall FinalizeBackend(void);
	virtual void __fastcall SetBackend(const TCustomBackend* Backend);
	virtual HRESULT __stdcall QueryInterface(const GUID &IID, /* out */ void *Obj);
	TColor32 __fastcall GetPixel(int X, int Y);
	TColor32 __fastcall GetPixelS(int X, int Y);
	TColor32 __fastcall GetPixelW(int X, int Y);
	TColor32 __fastcall GetPixelF(float X, float Y);
	TColor32 __fastcall GetPixelFS(float X, float Y);
	TColor32 __fastcall GetPixelFW(float X, float Y);
	TColor32 __fastcall GetPixelX(TFixed X, TFixed Y);
	TColor32 __fastcall GetPixelXS(TFixed X, TFixed Y);
	TColor32 __fastcall GetPixelXW(TFixed X, TFixed Y);
	TColor32 __fastcall GetPixelFR(float X, float Y);
	TColor32 __fastcall GetPixelXR(TFixed X, TFixed Y);
	TColor32 __fastcall GetPixelB(int X, int Y);
	void __fastcall SetPixel(int X, int Y, TColor32 Value);
	void __fastcall SetPixelS(int X, int Y, TColor32 Value);
	void __fastcall SetPixelW(int X, int Y, TColor32 Value);
	void __fastcall SetPixelF(float X, float Y, TColor32 Value);
	void __fastcall SetPixelFS(float X, float Y, TColor32 Value);
	void __fastcall SetPixelFW(float X, float Y, TColor32 Value);
	void __fastcall SetPixelX(TFixed X, TFixed Y, TColor32 Value);
	void __fastcall SetPixelXS(TFixed X, TFixed Y, TColor32 Value);
	void __fastcall SetPixelXW(TFixed X, TFixed Y, TColor32 Value);
	
public:
	__fastcall virtual TCustomBitmap32(void);
	__fastcall virtual ~TCustomBitmap32(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	Types::TRect __fastcall BoundsRect(void);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(void)/* overload */;
	void __fastcall Clear(TColor32 FillColor)/* overload */;
	virtual void __fastcall Delete(void);
	void __fastcall BeginMeasuring(const TAreaChangedEvent Callback);
	void __fastcall EndMeasuring(void);
	TCustomBackend* __fastcall ReleaseBackend(void);
	virtual void __fastcall PropertyChanged(void);
	virtual void __fastcall Changed(void)/* overload */;
	HIDESBASE virtual void __fastcall Changed(const Types::TRect &Area, const unsigned Info = (unsigned)(0x80000000))/* overload */;
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream);
	virtual void __fastcall SaveToStream(Classes::TStream* Stream, bool SaveTopDown = false);
	virtual void __fastcall LoadFromFile(const System::UnicodeString FileName);
	virtual void __fastcall SaveToFile(const System::UnicodeString FileName, bool SaveTopDown = false);
	void __fastcall LoadFromResourceID(unsigned Instance, int ResID);
	void __fastcall LoadFromResourceName(unsigned Instance, const System::UnicodeString ResName);
	void __fastcall ResetAlpha(void)/* overload */;
	void __fastcall ResetAlpha(const System::Byte AlphaValue)/* overload */;
	void __fastcall Draw(int DstX, int DstY, TCustomBitmap32* Src)/* overload */;
	void __fastcall Draw(int DstX, int DstY, const Types::TRect &SrcRect, TCustomBitmap32* Src)/* overload */;
	void __fastcall Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, TCustomBitmap32* Src)/* overload */;
	void __fastcall SetPixelT(int X, int Y, TColor32 Value)/* overload */;
	void __fastcall SetPixelT(PColor32 &Ptr, TColor32 Value)/* overload */;
	void __fastcall SetPixelTS(int X, int Y, TColor32 Value);
	void __fastcall DrawTo(TCustomBitmap32* Dst)/* overload */;
	void __fastcall DrawTo(TCustomBitmap32* Dst, int DstX, int DstY, const Types::TRect &SrcRect)/* overload */;
	void __fastcall DrawTo(TCustomBitmap32* Dst, int DstX, int DstY)/* overload */;
	void __fastcall DrawTo(TCustomBitmap32* Dst, const Types::TRect &DstRect)/* overload */;
	void __fastcall DrawTo(TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &SrcRect)/* overload */;
	void __fastcall SetStipple(TArrayOfColor32 NewStipple)/* overload */;
	void __fastcall SetStipple(TColor32 *NewStipple, const int NewStipple_Size)/* overload */;
	void __fastcall AdvanceStippleCounter(float LengthPixels);
	TColor32 __fastcall GetStippleColor(void);
	void __fastcall HorzLine(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineS(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineT(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineTS(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineTSP(int X1, int Y, int X2);
	void __fastcall HorzLineX(TFixed X1, TFixed Y, TFixed X2, TColor32 Value);
	void __fastcall HorzLineXS(TFixed X1, TFixed Y, TFixed X2, TColor32 Value);
	void __fastcall VertLine(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineS(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineT(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineTS(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineTSP(int X, int Y1, int Y2);
	void __fastcall VertLineX(TFixed X, TFixed Y1, TFixed Y2, TColor32 Value);
	void __fastcall VertLineXS(TFixed X, TFixed Y1, TFixed Y2, TColor32 Value);
	void __fastcall Line(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineT(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineTS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineA(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineAS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L = false);
	void __fastcall LineX(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, TColor32 Value, bool L = false)/* overload */;
	void __fastcall LineF(float X1, float Y1, float X2, float Y2, TColor32 Value, bool L = false)/* overload */;
	void __fastcall LineXS(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, TColor32 Value, bool L = false)/* overload */;
	void __fastcall LineFS(float X1, float Y1, float X2, float Y2, TColor32 Value, bool L = false)/* overload */;
	void __fastcall LineXP(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, bool L = false)/* overload */;
	void __fastcall LineFP(float X1, float Y1, float X2, float Y2, bool L = false)/* overload */;
	void __fastcall LineXSP(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, bool L = false)/* overload */;
	void __fastcall LineFSP(float X1, float Y1, float X2, float Y2, bool L = false)/* overload */;
	__property TColor32 PenColor = {read=FPenColor, write=FPenColor, nodefault};
	void __fastcall MoveTo(int X, int Y);
	void __fastcall LineToS(int X, int Y);
	void __fastcall LineToTS(int X, int Y);
	void __fastcall LineToAS(int X, int Y);
	void __fastcall MoveToX(TFixed X, TFixed Y);
	void __fastcall MoveToF(float X, float Y);
	void __fastcall LineToXS(TFixed X, TFixed Y);
	void __fastcall LineToFS(float X, float Y);
	void __fastcall LineToXSP(TFixed X, TFixed Y);
	void __fastcall LineToFSP(float X, float Y);
	void __fastcall FillRect(int X1, int Y1, int X2, int Y2, TColor32 Value);
	void __fastcall FillRectS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FillRectT(int X1, int Y1, int X2, int Y2, TColor32 Value);
	void __fastcall FillRectTS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FillRectS(const Types::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FillRectTS(const Types::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FrameRectS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FrameRectTS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FrameRectTSP(int X1, int Y1, int X2, int Y2);
	void __fastcall FrameRectS(const Types::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FrameRectTS(const Types::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall RaiseRectTS(int X1, int Y1, int X2, int Y2, int Contrast)/* overload */;
	void __fastcall RaiseRectTS(const Types::TRect &ARect, int Contrast)/* overload */;
	void __fastcall Roll(int Dx, int Dy, bool FillBack, TColor32 FillColor);
	void __fastcall FlipHorz(TCustomBitmap32* Dst = (TCustomBitmap32*)(0x0));
	void __fastcall FlipVert(TCustomBitmap32* Dst = (TCustomBitmap32*)(0x0));
	void __fastcall Rotate90(TCustomBitmap32* Dst = (TCustomBitmap32*)(0x0));
	void __fastcall Rotate180(TCustomBitmap32* Dst = (TCustomBitmap32*)(0x0));
	void __fastcall Rotate270(TCustomBitmap32* Dst = (TCustomBitmap32*)(0x0));
	void __fastcall ResetClipRect(void);
	__property TColor32 Pixel[int X][int Y] = {read=GetPixel, write=SetPixel/*, default*/};
	__property TColor32 PixelS[int X][int Y] = {read=GetPixelS, write=SetPixelS};
	__property TColor32 PixelW[int X][int Y] = {read=GetPixelW, write=SetPixelW};
	__property TColor32 PixelX[TFixed X][TFixed Y] = {read=GetPixelX, write=SetPixelX};
	__property TColor32 PixelXS[TFixed X][TFixed Y] = {read=GetPixelXS, write=SetPixelXS};
	__property TColor32 PixelXW[TFixed X][TFixed Y] = {read=GetPixelXW, write=SetPixelXW};
	__property TColor32 PixelF[float X][float Y] = {read=GetPixelF, write=SetPixelF};
	__property TColor32 PixelFS[float X][float Y] = {read=GetPixelFS, write=SetPixelFS};
	__property TColor32 PixelFW[float X][float Y] = {read=GetPixelFW, write=SetPixelFW};
	__property TColor32 PixelFR[float X][float Y] = {read=GetPixelFR};
	__property TColor32 PixelXR[TFixed X][TFixed Y] = {read=GetPixelXR};
	__property TCustomBackend* Backend = {read=FBackend, write=SetBackend};
	__property PColor32Array Bits = {read=FBits};
	__property Types::TRect ClipRect = {read=FClipRect, write=SetClipRect};
	__property bool Clipping = {read=FClipping, nodefault};
	__property PColor32 PixelPtr[int X][int Y] = {read=GetPixelPtr};
	__property PColor32Array ScanLine[int Y] = {read=GetScanLine};
	__property float StippleCounter = {read=FStippleCounter, write=FStippleCounter};
	__property float StippleStep = {read=FStippleStep, write=FStippleStep};
	__property bool MeasuringMode = {read=FMeasuringMode, nodefault};
	
__published:
	__property TDrawMode DrawMode = {read=FDrawMode, write=SetDrawMode, default=0};
	__property TCombineMode CombineMode = {read=FCombineMode, write=SetCombineMode, default=0};
	__property TWrapMode WrapMode = {read=FWrapMode, write=SetWrapMode, default=0};
	__property unsigned MasterAlpha = {read=FMasterAlpha, write=SetMasterAlpha, default=255};
	__property TColor32 OuterColor = {read=FOuterColor, write=FOuterColor, default=0};
	__property TStretchFilter StretchFilter = {read=FStretchFilter, write=SetStretchFilter, default=0};
	__property System::UnicodeString ResamplerClassName = {read=GetResamplerClassName, write=SetResamplerClassName};
	__property TCustomResampler* Resampler = {read=FResampler, write=SetResampler};
	__property OnChange;
	__property TPixelCombineEvent OnPixelCombine = {read=FOnPixelCombine, write=FOnPixelCombine};
	__property TAreaChangedEvent OnAreaChanged = {read=FOnAreaChanged, write=FOnAreaChanged};
	__property OnResize;
};


class DELPHICLASS TBitmap32;
class PASCALIMPLEMENTATION TBitmap32 : public TCustomBitmap32
{
	typedef TCustomBitmap32 inherited;
	
private:
	Classes::TNotifyEvent FOnHandleChanged;
	virtual void __fastcall BackendChangedHandler(System::TObject* Sender);
	virtual void __fastcall BackendChangingHandler(System::TObject* Sender);
	void __fastcall FontChanged(System::TObject* Sender);
	void __fastcall CanvasChanged(System::TObject* Sender);
	Graphics::TCanvas* __fastcall GetCanvas(void);
	tagBITMAPINFO __fastcall GetBitmapInfo(void);
	HBITMAP __fastcall GetHandle(void);
	HDC __fastcall GetHDC(void);
	Graphics::TFont* __fastcall GetFont(void);
	void __fastcall SetFont(Graphics::TFont* Value);
	
protected:
	virtual void __fastcall InitializeBackend(void);
	virtual void __fastcall FinalizeBackend(void);
	virtual void __fastcall SetBackend(const TCustomBackend* Backend);
	virtual void __fastcall HandleChanged(void);
	virtual void __fastcall CopyPropertiesTo(TCustomBitmap32* Dst);
	
public:
	HIDESBASE void __fastcall Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, HDC hSrc)/* overload */;
	HIDESBASE void __fastcall DrawTo(HDC hDst, int DstX, int DstY)/* overload */;
	HIDESBASE void __fastcall DrawTo(HDC hDst, const Types::TRect &DstRect, const Types::TRect &SrcRect)/* overload */;
	void __fastcall TileTo(HDC hDst, const Types::TRect &DstRect, const Types::TRect &SrcRect);
	void __fastcall UpdateFont(void);
	void __fastcall Textout(int X, int Y, const System::UnicodeString Text)/* overload */;
	void __fastcall Textout(int X, int Y, const Types::TRect &ClipRect, const System::UnicodeString Text)/* overload */;
	void __fastcall Textout(const Types::TRect &DstRect, const unsigned Flags, const System::UnicodeString Text)/* overload */;
	tagSIZE __fastcall TextExtent(const System::UnicodeString Text);
	int __fastcall TextHeight(const System::UnicodeString Text);
	int __fastcall TextWidth(const System::UnicodeString Text);
	void __fastcall RenderText(int X, int Y, const System::UnicodeString Text, int AALevel, TColor32 Color);
	void __fastcall TextoutW(int X, int Y, const System::WideString Text)/* overload */;
	void __fastcall TextoutW(int X, int Y, const Types::TRect &ClipRect, const System::WideString Text)/* overload */;
	void __fastcall TextoutW(const Types::TRect &DstRect, const unsigned Flags, const System::WideString Text)/* overload */;
	tagSIZE __fastcall TextExtentW(const System::WideString Text);
	int __fastcall TextHeightW(const System::WideString Text);
	int __fastcall TextWidthW(const System::WideString Text);
	void __fastcall RenderTextW(int X, int Y, const System::WideString Text, int AALevel, TColor32 Color);
	__property Graphics::TCanvas* Canvas = {read=GetCanvas};
	bool __fastcall CanvasAllocated(void);
	void __fastcall DeleteCanvas(void);
	__property Graphics::TFont* Font = {read=GetFont, write=SetFont};
	__property HBITMAP BitmapHandle = {read=GetHandle, nodefault};
	__property tagBITMAPINFO BitmapInfo = {read=GetBitmapInfo};
	__property HDC Handle = {read=GetHDC, nodefault};
	
__published:
	__property Classes::TNotifyEvent OnHandleChanged = {read=FOnHandleChanged, write=FOnHandleChanged};
public:
	/* TCustomBitmap32.Create */ inline __fastcall virtual TBitmap32(void) : TCustomBitmap32() { }
	/* TCustomBitmap32.Destroy */ inline __fastcall virtual ~TBitmap32(void) { }
	
	
/* Hoisted overloads: */
	
public:
	inline void __fastcall  Draw(int DstX, int DstY, TCustomBitmap32* Src){ TCustomBitmap32::Draw(DstX, DstY, Src); }
	inline void __fastcall  Draw(int DstX, int DstY, const Types::TRect &SrcRect, TCustomBitmap32* Src){ TCustomBitmap32::Draw(DstX, DstY, SrcRect, Src); }
	inline void __fastcall  Draw(const Types::TRect &DstRect, const Types::TRect &SrcRect, TCustomBitmap32* Src){ TCustomBitmap32::Draw(DstRect, SrcRect, Src); }
	inline void __fastcall  DrawTo(TCustomBitmap32* Dst){ TCustomBitmap32::DrawTo(Dst); }
	inline void __fastcall  DrawTo(TCustomBitmap32* Dst, int DstX, int DstY, const Types::TRect &SrcRect){ TCustomBitmap32::DrawTo(Dst, DstX, DstY, SrcRect); }
	inline void __fastcall  DrawTo(TCustomBitmap32* Dst, int DstX, int DstY){ TCustomBitmap32::DrawTo(Dst, DstX, DstY); }
	inline void __fastcall  DrawTo(TCustomBitmap32* Dst, const Types::TRect &DstRect){ TCustomBitmap32::DrawTo(Dst, DstRect); }
	inline void __fastcall  DrawTo(TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &SrcRect){ TCustomBitmap32::DrawTo(Dst, DstRect, SrcRect); }
	
};


class PASCALIMPLEMENTATION TCustomBackend : public TThreadPersistent
{
	typedef TThreadPersistent inherited;
	
protected:
	TColor32Array *FBits;
	TCustomBitmap32* FOwner;
	Classes::TNotifyEvent FOnChanging;
	virtual void __fastcall Changing(void);
	virtual void __fastcall InitializeSurface(int NewWidth, int NewHeight, bool ClearBuffer);
	virtual void __fastcall FinalizeSurface(void);
	
public:
	__fastcall virtual TCustomBackend(void)/* overload */;
	__fastcall virtual TCustomBackend(TCustomBitmap32* Owner)/* overload */;
	__fastcall virtual ~TCustomBackend(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	virtual void __fastcall Clear(void);
	virtual bool __fastcall Empty(void);
	virtual void __fastcall ChangeSize(int &Width, int &Height, int NewWidth, int NewHeight, bool ClearBuffer = true);
	__property PColor32Array Bits = {read=FBits};
	__property Classes::TNotifyEvent OnChanging = {read=FOnChanging, write=FOnChanging};
};


class DELPHICLASS TCustomSampler;
class PASCALIMPLEMENTATION TCustomSampler : public TNotifiablePersistent
{
	typedef TNotifiablePersistent inherited;
	
public:
	virtual TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual TColor32 __fastcall GetSampleFixed(TFixed X, TFixed Y);
	virtual TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall PrepareSampling(void);
	virtual void __fastcall FinalizeSampling(void);
	virtual bool __fastcall HasBounds(void);
	virtual TFloatRect __fastcall GetSampleBounds(void);
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomSampler(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TCustomSampler(void) : TNotifiablePersistent() { }
	
};


#pragma option push -b-
enum TPixelAccessMode { pamUnsafe, pamSafe, pamWrap, pamTransparentEdge };
#pragma option pop

class PASCALIMPLEMENTATION TCustomResampler : public TCustomSampler
{
	typedef TCustomSampler inherited;
	
private:
	TCustomBitmap32* FBitmap;
	Types::TRect FClipRect;
	TPixelAccessMode FPixelAccessMode;
	void __fastcall SetPixelAccessMode(const TPixelAccessMode Value);
	
protected:
	virtual float __fastcall GetWidth(void);
	virtual void __fastcall Resample(TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, TCustomBitmap32* Src, const Types::TRect &SrcRect, TDrawMode CombineOp, TPixelCombineEvent CombineCallBack) = 0 ;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	__property Types::TRect ClipRect = {read=FClipRect};
	
public:
	__fastcall virtual TCustomResampler(void)/* overload */;
	__fastcall virtual TCustomResampler(TCustomBitmap32* ABitmap)/* overload */;
	virtual void __fastcall Changed(void);
	virtual void __fastcall PrepareSampling(void);
	virtual bool __fastcall HasBounds(void);
	virtual TFloatRect __fastcall GetSampleBounds(void);
	__property TCustomBitmap32* Bitmap = {read=FBitmap, write=FBitmap};
	__property float Width = {read=GetWidth};
	
__published:
	__property TPixelAccessMode PixelAccessMode = {read=FPixelAccessMode, write=SetPixelAccessMode, default=1};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomResampler(void) { }
	
};


typedef TMetaClass* TCustomResamplerClass;

//-- var, const, procedure ---------------------------------------------------
#define Graphics32Version L"1.9.1"
static const TColor32 clBlack32 = 0xff000000;
static const TColor32 clDimGray32 = 0xff3f3f3f;
static const TColor32 clGray32 = 0xff7f7f7f;
static const TColor32 clLightGray32 = 0xffbfbfbf;
static const TColor32 clWhite32 = 0xffffffff;
static const TColor32 clMaroon32 = 0xff7f0000;
static const TColor32 clGreen32 = 0xff007f00;
static const TColor32 clOlive32 = 0xff7f7f00;
static const TColor32 clNavy32 = 0xff00007f;
static const TColor32 clPurple32 = 0xff7f007f;
static const TColor32 clTeal32 = 0xff007f7f;
static const TColor32 clRed32 = 0xffff0000;
static const TColor32 clLime32 = 0xff00ff00;
static const TColor32 clYellow32 = 0xffffff00;
static const TColor32 clBlue32 = 0xff0000ff;
static const TColor32 clFuchsia32 = 0xffff00ff;
static const TColor32 clAqua32 = 0xff00ffff;
static const TColor32 clAliceBlue32 = 0xfff0f8ff;
static const TColor32 clAntiqueWhite32 = 0xfffaebd7;
static const TColor32 clAquamarine32 = 0xff7fffd4;
static const TColor32 clAzure32 = 0xfff0ffff;
static const TColor32 clBeige32 = 0xfff5f5dc;
static const TColor32 clBisque32 = 0xffffe4c4;
static const TColor32 clBlancheDalmond32 = 0xffffebcd;
static const TColor32 clBlueViolet32 = 0xff8a2be2;
static const TColor32 clBrown32 = 0xffa52a2a;
static const TColor32 clBurlyWood32 = 0xffdeb887;
static const TColor32 clCadetblue32 = 0xff5f9ea0;
static const TColor32 clChartReuse32 = 0xff7fff00;
static const TColor32 clChocolate32 = 0xffd2691e;
static const TColor32 clCoral32 = 0xffff7f50;
static const TColor32 clCornFlowerBlue32 = 0xff6495ed;
static const TColor32 clCornSilk32 = 0xfffff8dc;
static const TColor32 clCrimson32 = 0xffdc143c;
static const TColor32 clDarkBlue32 = 0xff00008b;
static const TColor32 clDarkCyan32 = 0xff008b8b;
static const TColor32 clDarkGoldenRod32 = 0xffb8860b;
static const TColor32 clDarkGray32 = 0xffa9a9a9;
static const TColor32 clDarkGreen32 = 0xff006400;
static const TColor32 clDarkGrey32 = 0xffa9a9a9;
static const TColor32 clDarkKhaki32 = 0xffbdb76b;
static const TColor32 clDarkMagenta32 = 0xff8b008b;
static const TColor32 clDarkOliveGreen32 = 0xff556b2f;
static const TColor32 clDarkOrange32 = 0xffff8c00;
static const TColor32 clDarkOrchid32 = 0xff9932cc;
static const TColor32 clDarkRed32 = 0xff8b0000;
static const TColor32 clDarkSalmon32 = 0xffe9967a;
static const TColor32 clDarkSeaGreen32 = 0xff8fbc8f;
static const TColor32 clDarkSlateBlue32 = 0xff483d8b;
static const TColor32 clDarkSlateGray32 = 0xff2f4f4f;
static const TColor32 clDarkSlateGrey32 = 0xff2f4f4f;
static const TColor32 clDarkTurquoise32 = 0xff00ced1;
static const TColor32 clDarkViolet32 = 0xff9400d3;
static const TColor32 clDeepPink32 = 0xffff1493;
static const TColor32 clDeepSkyBlue32 = 0xff00bfff;
static const TColor32 clDodgerBlue32 = 0xff1e90ff;
static const TColor32 clFireBrick32 = 0xffb22222;
static const TColor32 clFloralWhite32 = 0xfffffaf0;
static const TColor32 clGainsBoro32 = 0xffdcdcdc;
static const TColor32 clGhostWhite32 = 0xfff8f8ff;
static const TColor32 clGold32 = 0xffffd700;
static const TColor32 clGoldenRod32 = 0xffdaa520;
static const TColor32 clGreenYellow32 = 0xffadff2f;
static const TColor32 clGrey32 = 0xff808080;
static const TColor32 clHoneyDew32 = 0xfff0fff0;
static const TColor32 clHotPink32 = 0xffff69b4;
static const TColor32 clIndianRed32 = 0xffcd5c5c;
static const TColor32 clIndigo32 = 0xff4b0082;
static const TColor32 clIvory32 = 0xfffffff0;
static const TColor32 clKhaki32 = 0xfff0e68c;
static const TColor32 clLavender32 = 0xffe6e6fa;
static const TColor32 clLavenderBlush32 = 0xfffff0f5;
static const TColor32 clLawnGreen32 = 0xff7cfc00;
static const TColor32 clLemonChiffon32 = 0xfffffacd;
static const TColor32 clLightBlue32 = 0xffadd8e6;
static const TColor32 clLightCoral32 = 0xfff08080;
static const TColor32 clLightCyan32 = 0xffe0ffff;
static const TColor32 clLightGoldenRodYellow32 = 0xfffafad2;
static const TColor32 clLightGreen32 = 0xff90ee90;
static const TColor32 clLightGrey32 = 0xffd3d3d3;
static const TColor32 clLightPink32 = 0xffffb6c1;
static const TColor32 clLightSalmon32 = 0xffffa07a;
static const TColor32 clLightSeagreen32 = 0xff20b2aa;
static const TColor32 clLightSkyblue32 = 0xff87cefa;
static const TColor32 clLightSlategray32 = 0xff778899;
static const TColor32 clLightSlategrey32 = 0xff778899;
static const TColor32 clLightSteelblue32 = 0xffb0c4de;
static const TColor32 clLightYellow32 = 0xffffffe0;
static const TColor32 clLtGray32 = 0xffc0c0c0;
static const TColor32 clMedGray32 = 0xffa0a0a4;
static const TColor32 clDkGray32 = 0xff808080;
static const TColor32 clMoneyGreen32 = 0xffc0dcc0;
static const TColor32 clLegacySkyBlue32 = 0xffa6caf0;
static const TColor32 clCream32 = 0xfffffbf0;
static const TColor32 clLimeGreen32 = 0xff32cd32;
static const TColor32 clLinen32 = 0xfffaf0e6;
static const TColor32 clMediumAquamarine32 = 0xff66cdaa;
static const TColor32 clMediumBlue32 = 0xff0000cd;
static const TColor32 clMediumOrchid32 = 0xffba55d3;
static const TColor32 clMediumPurple32 = 0xff9370db;
static const TColor32 clMediumSeaGreen32 = 0xff3cb371;
static const TColor32 clMediumSlateBlue32 = 0xff7b68ee;
static const TColor32 clMediumSpringGreen32 = 0xff00fa9a;
static const TColor32 clMediumTurquoise32 = 0xff48d1cc;
static const TColor32 clMediumVioletRed32 = 0xffc71585;
static const TColor32 clMidnightBlue32 = 0xff191970;
static const TColor32 clMintCream32 = 0xfff5fffa;
static const TColor32 clMistyRose32 = 0xffffe4e1;
static const TColor32 clMoccasin32 = 0xffffe4b5;
static const TColor32 clNavajoWhite32 = 0xffffdead;
static const TColor32 clOldLace32 = 0xfffdf5e6;
static const TColor32 clOliveDrab32 = 0xff6b8e23;
static const TColor32 clOrange32 = 0xffffa500;
static const TColor32 clOrangeRed32 = 0xffff4500;
static const TColor32 clOrchid32 = 0xffda70d6;
static const TColor32 clPaleGoldenRod32 = 0xffeee8aa;
static const TColor32 clPaleGreen32 = 0xff98fb98;
static const TColor32 clPaleTurquoise32 = 0xffafeeee;
static const TColor32 clPaleVioletred32 = 0xffdb7093;
static const TColor32 clPapayaWhip32 = 0xffffefd5;
static const TColor32 clPeachPuff32 = 0xffffdab9;
static const TColor32 clPeru32 = 0xffcd853f;
static const TColor32 clPlum32 = 0xffdda0dd;
static const TColor32 clPowderBlue32 = 0xffb0e0e6;
static const TColor32 clRosyBrown32 = 0xffbc8f8f;
static const TColor32 clRoyalBlue32 = 0xff4169e1;
static const TColor32 clSaddleBrown32 = 0xff8b4513;
static const TColor32 clSalmon32 = 0xfffa8072;
static const TColor32 clSandyBrown32 = 0xfff4a460;
static const TColor32 clSeaGreen32 = 0xff2e8b57;
static const TColor32 clSeaShell32 = 0xfffff5ee;
static const TColor32 clSienna32 = 0xffa0522d;
static const TColor32 clSilver32 = 0xffc0c0c0;
static const TColor32 clSkyblue32 = 0xff87ceeb;
static const TColor32 clSlateBlue32 = 0xff6a5acd;
static const TColor32 clSlateGray32 = 0xff708090;
static const TColor32 clSlateGrey32 = 0xff708090;
static const TColor32 clSnow32 = 0xfffffafa;
static const TColor32 clSpringgreen32 = 0xff00ff7f;
static const TColor32 clSteelblue32 = 0xff4682b4;
static const TColor32 clTan32 = 0xffd2b48c;
static const TColor32 clThistle32 = 0xffd8bfd8;
static const TColor32 clTomato32 = 0xffff6347;
static const TColor32 clTurquoise32 = 0xff40e0d0;
static const TColor32 clViolet32 = 0xffee82ee;
static const TColor32 clWheat32 = 0xfff5deb3;
static const TColor32 clWhitesmoke32 = 0xfff5f5f5;
static const TColor32 clYellowgreen32 = 0xff9acd32;
static const TColor32 clTrWhite32 = 0x7fffffff;
static const TColor32 clTrBlack32 = 0x7f000000;
static const TColor32 clTrRed32 = 0x7fff0000;
static const TColor32 clTrGreen32 = 0x7f00ff00;
static const TColor32 clTrBlue32 = 0x7f0000ff;
static const int FixedOne = 0x10000;
static const Word FixedHalf = 0x7fff;
static const __int64 FixedPI = 0x000000000003243fLL;
static const Extended FixedToFloat = 1.525879E-05;
extern PACKAGE StaticArray<System::Byte, 256> GAMMA_TABLE;
static const unsigned AREAINFO_RECT = 0x80000000;
static const int AREAINFO_LINE = 0x40000000;
static const int AREAINFO_ELLIPSE = 0x20000000;
static const int AREAINFO_ABSOLUTE = 0x10000000;
static const unsigned AREAINFO_MASK = 0xff000000;
extern PACKAGE Graphics::TBitmap* StockBitmap;
extern PACKAGE TColor32 __fastcall Color32(Graphics::TColor WinColor)/* overload */;
extern PACKAGE TColor32 __fastcall Color32(System::Byte R, System::Byte G, System::Byte B, System::Byte A = (System::Byte)(0xff))/* overload */;
extern PACKAGE TColor32 __fastcall Color32(System::Byte Index, TColor32 *Palette)/* overload */;
extern PACKAGE TColor32 __fastcall Gray32(System::Byte Intensity, System::Byte Alpha = (System::Byte)(0xff));
extern PACKAGE Graphics::TColor __fastcall WinColor(TColor32 Color32);
extern PACKAGE TArrayOfColor32 __fastcall ArrayOfColor32(TColor32 *Colors, const int Colors_Size);
extern PACKAGE void __fastcall Color32ToRGB(TColor32 Color32, System::Byte &R, System::Byte &G, System::Byte &B);
extern PACKAGE void __fastcall Color32ToRGBA(TColor32 Color32, System::Byte &R, System::Byte &G, System::Byte &B, System::Byte &A);
extern PACKAGE TColor32Components __fastcall Color32Components(bool R, bool G, bool B, bool A);
extern PACKAGE int __fastcall RedComponent(TColor32 Color32);
extern PACKAGE int __fastcall GreenComponent(TColor32 Color32);
extern PACKAGE int __fastcall BlueComponent(TColor32 Color32);
extern PACKAGE int __fastcall AlphaComponent(TColor32 Color32);
extern PACKAGE int __fastcall Intensity(TColor32 Color32);
extern PACKAGE TColor32 __fastcall SetAlpha(TColor32 Color32, int NewAlpha);
extern PACKAGE TColor32 __fastcall HSLtoRGB(float H, float S, float L)/* overload */;
extern PACKAGE void __fastcall RGBtoHSL(TColor32 RGB, /* out */ float &H, /* out */ float &S, /* out */ float &L)/* overload */;
extern PACKAGE TColor32 __fastcall HSLtoRGB(int H, int S, int L)/* overload */;
extern PACKAGE void __fastcall RGBtoHSL(TColor32 RGB, /* out */ System::Byte &H, /* out */ System::Byte &S, /* out */ System::Byte &L)/* overload */;
extern PACKAGE HPALETTE __fastcall WinPalette(TColor32 const *P);
extern PACKAGE TFixed __fastcall Fixed(float S)/* overload */;
extern PACKAGE TFixed __fastcall Fixed(int I)/* overload */;
extern PACKAGE Types::TPoint __fastcall Point(int X, int Y)/* overload */;
extern PACKAGE Types::TPoint __fastcall Point(const TFloatPoint &FP)/* overload */;
extern PACKAGE Types::TPoint __fastcall Point(const TFixedPoint &FXP)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(float X, float Y)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(const Types::TPoint &P)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(const TFixedPoint &FXP)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(int X, int Y)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(float X, float Y)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(const Types::TPoint &P)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(const TFloatPoint &FP)/* overload */;
extern PACKAGE Types::TRect __fastcall MakeRect(const int L, const int T, const int R, const int B)/* overload */;
extern PACKAGE Types::TRect __fastcall MakeRect(const TFloatRect &FR, TRectRounding Rounding = (TRectRounding)(0x0))/* overload */;
extern PACKAGE Types::TRect __fastcall MakeRect(const TFixedRect &FXR, TRectRounding Rounding = (TRectRounding)(0x0))/* overload */;
extern PACKAGE TFixedRect __fastcall FixedRect(const TFixed L, const TFixed T, const TFixed R, const TFixed B)/* overload */;
extern PACKAGE TFixedRect __fastcall FixedRect(const Types::TRect &ARect)/* overload */;
extern PACKAGE TFixedRect __fastcall FixedRect(const TFloatRect &FR)/* overload */;
extern PACKAGE TFloatRect __fastcall FloatRect(const float L, const float T, const float R, const float B)/* overload */;
extern PACKAGE TFloatRect __fastcall FloatRect(const Types::TRect &ARect)/* overload */;
extern PACKAGE TFloatRect __fastcall FloatRect(const TFixedRect &FXR)/* overload */;
extern PACKAGE bool __fastcall IntersectRect(/* out */ Types::TRect &Dst, const Types::TRect &R1, const Types::TRect &R2)/* overload */;
extern PACKAGE bool __fastcall IntersectRect(/* out */ TFloatRect &Dst, const TFloatRect &FR1, const TFloatRect &FR2)/* overload */;
extern PACKAGE bool __fastcall UnionRect(/* out */ Types::TRect &Rect, const Types::TRect &R1, const Types::TRect &R2)/* overload */;
extern PACKAGE bool __fastcall UnionRect(/* out */ TFloatRect &Rect, const TFloatRect &R1, const TFloatRect &R2)/* overload */;
extern PACKAGE bool __fastcall EqualRect(const Types::TRect &R1, const Types::TRect &R2)/* overload */;
extern PACKAGE bool __fastcall EqualRect(const TFloatRect &R1, const TFloatRect &R2)/* overload */;
extern PACKAGE bool __fastcall EqualRectSize(const Types::TRect &R1, const Types::TRect &R2)/* overload */;
extern PACKAGE bool __fastcall EqualRectSize(const TFloatRect &R1, const TFloatRect &R2)/* overload */;
extern PACKAGE void __fastcall InflateRect(Types::TRect &R, int Dx, int Dy)/* overload */;
extern PACKAGE void __fastcall InflateRect(TFloatRect &FR, float Dx, float Dy)/* overload */;
extern PACKAGE void __fastcall OffsetRect(Types::TRect &R, int Dx, int Dy)/* overload */;
extern PACKAGE void __fastcall OffsetRect(TFloatRect &FR, float Dx, float Dy)/* overload */;
extern PACKAGE bool __fastcall IsRectEmpty(const Types::TRect &R)/* overload */;
extern PACKAGE bool __fastcall IsRectEmpty(const TFloatRect &FR)/* overload */;
extern PACKAGE bool __fastcall PtInRect(const Types::TRect &R, const Types::TPoint &P)/* overload */;
extern PACKAGE bool __fastcall PtInRect(const TFloatRect &R, const Types::TPoint &P)/* overload */;
extern PACKAGE bool __fastcall PtInRect(const Types::TRect &R, const TFloatPoint &P)/* overload */;
extern PACKAGE bool __fastcall PtInRect(const TFloatRect &R, const TFloatPoint &P)/* overload */;
extern PACKAGE void __fastcall SetGamma(float Gamma = 7.000000E-01);
extern PACKAGE TCustomBackendClass __fastcall GetPlatformBackendClass(void);

}	/* namespace Gr32 */
using namespace Gr32;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32HPP
