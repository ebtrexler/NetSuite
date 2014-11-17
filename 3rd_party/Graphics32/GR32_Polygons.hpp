// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_polygons.pas' rev: 21.00

#ifndef Gr32_polygonsHPP
#define Gr32_polygonsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_lowlevel.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_transforms.hpp>	// Pascal unit
#include <Gr32_resamplers.hpp>	// Pascal unit
#include <Gr32_math.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_polygons
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TPolyFillMode { pfAlternate, pfWinding };
#pragma option pop

#pragma option push -b-
enum TAntialiasMode { am32times, am16times, am8times, am4times, am2times, amNone };
#pragma option pop

typedef void __fastcall (__closure *TFillLineEvent)(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);

class DELPHICLASS TCustomPolygonFiller;
class PASCALIMPLEMENTATION TCustomPolygonFiller : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	virtual TFillLineEvent __fastcall GetFillLine(void) = 0 ;
	
public:
	__property TFillLineEvent FillLine = {read=GetFillLine};
public:
	/* TObject.Create */ inline __fastcall TCustomPolygonFiller(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TCustomPolygonFiller(void) { }
	
};


class DELPHICLASS TPolygon32;
class PASCALIMPLEMENTATION TPolygon32 : public Gr32::TThreadPersistent
{
	typedef Gr32::TThreadPersistent inherited;
	
private:
	bool FAntialiased;
	bool FClosed;
	TPolyFillMode FFillMode;
	Gr32::TArrayOfArrayOfFixedPoint FNormals;
	Gr32::TArrayOfArrayOfFixedPoint FPoints;
	TAntialiasMode FAntialiasMode;
	
protected:
	void __fastcall BuildNormals(void);
	virtual void __fastcall CopyPropertiesTo(TPolygon32* Dst);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	
public:
	__fastcall virtual TPolygon32(void);
	__fastcall virtual ~TPolygon32(void);
	void __fastcall Add(const Gr32::TFixedPoint &P);
	void __fastcall AddPoints(Gr32::TFixedPoint &First, int Count);
	bool __fastcall ContainsPoint(const Gr32::TFixedPoint &P);
	void __fastcall Clear(void);
	TPolygon32* __fastcall Grow(const Gr32::TFixed Delta, float EdgeSharpness = 0.000000E+00);
	void __fastcall Draw(Gr32::TCustomBitmap32* Bitmap, Gr32::TColor32 OutlineColor, Gr32::TColor32 FillColor, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall Draw(Gr32::TCustomBitmap32* Bitmap, Gr32::TColor32 OutlineColor, TFillLineEvent FillCallback, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall Draw(Gr32::TCustomBitmap32* Bitmap, Gr32::TColor32 OutlineColor, TCustomPolygonFiller* Filler, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall DrawEdge(Gr32::TCustomBitmap32* Bitmap, Gr32::TColor32 Color, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
	void __fastcall DrawFill(Gr32::TCustomBitmap32* Bitmap, Gr32::TColor32 Color, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall DrawFill(Gr32::TCustomBitmap32* Bitmap, TFillLineEvent FillCallback, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall DrawFill(Gr32::TCustomBitmap32* Bitmap, TCustomPolygonFiller* Filler, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
	void __fastcall NewLine(void);
	void __fastcall Offset(const Gr32::TFixed Dx, const Gr32::TFixed Dy);
	TPolygon32* __fastcall Outline(void);
	void __fastcall Transform(Gr32_transforms::TTransformation* Transformation);
	Gr32::TFixedRect __fastcall GetBoundingRect(void);
	__property bool Antialiased = {read=FAntialiased, write=FAntialiased, nodefault};
	__property TAntialiasMode AntialiasMode = {read=FAntialiasMode, write=FAntialiasMode, nodefault};
	__property bool Closed = {read=FClosed, write=FClosed, nodefault};
	__property TPolyFillMode FillMode = {read=FFillMode, write=FFillMode, nodefault};
	__property Gr32::TArrayOfArrayOfFixedPoint Normals = {read=FNormals, write=FNormals};
	__property Gr32::TArrayOfArrayOfFixedPoint Points = {read=FPoints, write=FPoints};
};


class DELPHICLASS TBitmapPolygonFiller;
class PASCALIMPLEMENTATION TBitmapPolygonFiller : public TCustomPolygonFiller
{
	typedef TCustomPolygonFiller inherited;
	
private:
	Gr32::TCustomBitmap32* FPattern;
	int FOffsetY;
	int FOffsetX;
	
protected:
	virtual TFillLineEvent __fastcall GetFillLine(void);
	void __fastcall FillLineOpaque(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);
	void __fastcall FillLineBlend(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);
	void __fastcall FillLineBlendMasterAlpha(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);
	void __fastcall FillLineCustomCombine(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);
	
public:
	__property Gr32::TCustomBitmap32* Pattern = {read=FPattern, write=FPattern};
	__property int OffsetX = {read=FOffsetX, write=FOffsetX, nodefault};
	__property int OffsetY = {read=FOffsetY, write=FOffsetY, nodefault};
public:
	/* TObject.Create */ inline __fastcall TBitmapPolygonFiller(void) : TCustomPolygonFiller() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TBitmapPolygonFiller(void) { }
	
};


class DELPHICLASS TSamplerFiller;
class PASCALIMPLEMENTATION TSamplerFiller : public TCustomPolygonFiller
{
	typedef TCustomPolygonFiller inherited;
	
private:
	Gr32::TCustomSampler* FSampler;
	Gr32_resamplers::TGetSampleInt FGetSample;
	void __fastcall SetSampler(const Gr32::TCustomSampler* Value);
	
protected:
	virtual TFillLineEvent __fastcall GetFillLine(void);
	void __fastcall SampleLineOpaque(Gr32::PColor32 Dst, int DstX, int DstY, int Length, Gr32::PColor32 AlphaValues);
	__property Gr32::TCustomSampler* Sampler = {read=FSampler, write=SetSampler};
public:
	/* TObject.Create */ inline __fastcall TSamplerFiller(void) : TCustomPolygonFiller() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TSamplerFiller(void) { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const TAntialiasMode DefaultAAMode = (TAntialiasMode)(2);
extern PACKAGE void __fastcall PolylineTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolylineAS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolylineXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolylineXSP(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolyPolylineTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolyPolylineAS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolyPolylineXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32::TColor32 Color, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolyPolylineXSP(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, bool Closed = false, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE void __fastcall PolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, Gr32::TColor32 Color, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, TFillLineEvent FillLineCallback, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, TCustomPolygonFiller* Filler, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, Gr32::TColor32 Color, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, TFillLineEvent FillLineCallback, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points, TCustomPolygonFiller* Filler, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32::TColor32 Color, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, TFillLineEvent FillLineCallback, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonTS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, TCustomPolygonFiller* Filler, TPolyFillMode Mode = (TPolyFillMode)(0x0), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32::TColor32 Color, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, TFillLineEvent FillLineCallback, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE void __fastcall PolyPolygonXS(Gr32::TCustomBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint Points, TCustomPolygonFiller* Filler, TPolyFillMode Mode = (TPolyFillMode)(0x0), const TAntialiasMode AAMode = (TAntialiasMode)(0x2), Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0))/* overload */;
extern PACKAGE Gr32::TFixedRect __fastcall PolygonBounds(const Gr32::TArrayOfFixedPoint Points, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE Gr32::TFixedRect __fastcall PolyPolygonBounds(const Gr32::TArrayOfArrayOfFixedPoint Points, Gr32_transforms::TTransformation* Transformation = (Gr32_transforms::TTransformation*)(0x0));
extern PACKAGE bool __fastcall PtInPolygon(const Gr32::TFixedPoint &Pt, const Gr32::TArrayOfFixedPoint Points);

}	/* namespace Gr32_polygons */
using namespace Gr32_polygons;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_polygonsHPP
