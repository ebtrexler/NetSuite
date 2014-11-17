// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_transforms.pas' rev: 21.00

#ifndef Gr32_transformsHPP
#define Gr32_transformsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_vectormaps.hpp>	// Pascal unit
#include <Gr32_rasterizers.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_transforms
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS ETransformError;
class PASCALIMPLEMENTATION ETransformError : public Sysutils::Exception
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall ETransformError(const System::UnicodeString Msg) : Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ETransformError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall ETransformError(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	/* Exception.CreateResFmt */ inline __fastcall ETransformError(int Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall ETransformError(const System::UnicodeString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ETransformError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ETransformError(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ETransformError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ETransformError(void) { }
	
};


class DELPHICLASS ETransformNotImplemented;
class PASCALIMPLEMENTATION ETransformNotImplemented : public Sysutils::Exception
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall ETransformNotImplemented(const System::UnicodeString Msg) : Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ETransformNotImplemented(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall ETransformNotImplemented(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	/* Exception.CreateResFmt */ inline __fastcall ETransformNotImplemented(int Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall ETransformNotImplemented(const System::UnicodeString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ETransformNotImplemented(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ETransformNotImplemented(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ETransformNotImplemented(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ETransformNotImplemented(void) { }
	
};


typedef StaticArray<StaticArray<float, 3>, 3> TFloatMatrix;

typedef StaticArray<StaticArray<Gr32::TFixed, 3>, 3> TFixedMatrix;

typedef StaticArray<float, 3> TVector3f;

typedef StaticArray<int, 3> TVector3i;

class DELPHICLASS TTransformation;
class PASCALIMPLEMENTATION TTransformation : public Gr32::TNotifiablePersistent
{
	typedef Gr32::TNotifiablePersistent inherited;
	
private:
	Gr32::TFloatRect FSrcRect;
	void __fastcall SetSrcRect(const Gr32::TFloatRect &Value);
	
protected:
	bool TransformValid;
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformInt(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall TransformInt(int SrcX, int SrcY, /* out */ int &DstX, /* out */ int &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	
public:
	virtual void __fastcall Changed(void);
	virtual bool __fastcall HasTransformedBounds(void);
	Gr32::TFloatRect __fastcall GetTransformedBounds(void)/* overload */;
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	virtual Types::TPoint __fastcall ReverseTransform(const Types::TPoint &P)/* overload */;
	virtual Gr32::TFixedPoint __fastcall ReverseTransform(const Gr32::TFixedPoint &P)/* overload */;
	virtual Gr32::TFloatPoint __fastcall ReverseTransform(const Gr32::TFloatPoint &P)/* overload */;
	virtual Types::TPoint __fastcall Transform(const Types::TPoint &P)/* overload */;
	virtual Gr32::TFixedPoint __fastcall Transform(const Gr32::TFixedPoint &P)/* overload */;
	virtual Gr32::TFloatPoint __fastcall Transform(const Gr32::TFloatPoint &P)/* overload */;
	__property Gr32::TFloatRect SrcRect = {read=FSrcRect, write=SetSrcRect};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTransformation(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TTransformation(void) : Gr32::TNotifiablePersistent() { }
	
};


class DELPHICLASS TAffineTransformation;
class PASCALIMPLEMENTATION TAffineTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
protected:
	TFloatMatrix FInverseMatrix;
	TFixedMatrix FFixedMatrix;
	TFixedMatrix FInverseFixedMatrix;
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	
public:
	TFloatMatrix Matrix;
	__fastcall virtual TAffineTransformation(void);
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	void __fastcall Clear(void);
	void __fastcall Rotate(float Alpha)/* overload */;
	void __fastcall Rotate(float Cx, float Cy, float Alpha)/* overload */;
	void __fastcall Skew(float Fx, float Fy);
	void __fastcall Scale(float Sx, float Sy)/* overload */;
	void __fastcall Scale(float Value)/* overload */;
	void __fastcall Translate(float Dx, float Dy);
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TAffineTransformation(void) { }
	
	
/* Hoisted overloads: */
	
public:
	inline Gr32::TFloatRect __fastcall  GetTransformedBounds(void){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TProjectiveTransformation;
class PASCALIMPLEMENTATION TProjectiveTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	float Wx0;
	float Wx1;
	float Wx2;
	float Wx3;
	float Wy0;
	float Wy1;
	float Wy2;
	float Wy3;
	void __fastcall SetX0(float Value);
	void __fastcall SetX1(float Value);
	void __fastcall SetX2(float Value);
	void __fastcall SetX3(float Value);
	void __fastcall SetY0(float Value);
	void __fastcall SetY1(float Value);
	void __fastcall SetY2(float Value);
	void __fastcall SetY3(float Value);
	
protected:
	TFloatMatrix FMatrix;
	TFloatMatrix FInverseMatrix;
	TFixedMatrix FFixedMatrix;
	TFixedMatrix FInverseFixedMatrix;
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall TransformFixed(Gr32::TFixed SrcX, Gr32::TFixed SrcY, /* out */ Gr32::TFixed &DstX, /* out */ Gr32::TFixed &DstY);
	
public:
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float X0 = {read=Wx0, write=SetX0};
	__property float X1 = {read=Wx1, write=SetX1};
	__property float X2 = {read=Wx2, write=SetX2};
	__property float X3 = {read=Wx3, write=SetX3};
	__property float Y0 = {read=Wy0, write=SetY0};
	__property float Y1 = {read=Wy1, write=SetY1};
	__property float Y2 = {read=Wy2, write=SetY2};
	__property float Y3 = {read=Wy3, write=SetY3};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TProjectiveTransformation(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TProjectiveTransformation(void) : TTransformation() { }
	
	
/* Hoisted overloads: */
	
public:
	inline Gr32::TFloatRect __fastcall  GetTransformedBounds(void){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TTwirlTransformation;
class PASCALIMPLEMENTATION TTwirlTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	float Frx;
	float Fry;
	float FTwirl;
	void __fastcall SetTwirl(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__fastcall virtual TTwirlTransformation(void);
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float Twirl = {read=FTwirl, write=SetTwirl};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTwirlTransformation(void) { }
	
	
/* Hoisted overloads: */
	
public:
	inline Gr32::TFloatRect __fastcall  GetTransformedBounds(void){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TBloatTransformation;
class PASCALIMPLEMENTATION TBloatTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	float FBloatPower;
	float FBP;
	float FPiW;
	float FPiH;
	void __fastcall SetBloatPower(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__fastcall virtual TBloatTransformation(void);
	
__published:
	__property float BloatPower = {read=FBloatPower, write=SetBloatPower};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBloatTransformation(void) { }
	
};


class DELPHICLASS TDisturbanceTransformation;
class PASCALIMPLEMENTATION TDisturbanceTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	float FDisturbance;
	void __fastcall SetDisturbance(const float Value);
	
protected:
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	
__published:
	__property float Disturbance = {read=FDisturbance, write=SetDisturbance};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TDisturbanceTransformation(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TDisturbanceTransformation(void) : TTransformation() { }
	
	
/* Hoisted overloads: */
	
public:
	inline Gr32::TFloatRect __fastcall  GetTransformedBounds(void){ return TTransformation::GetTransformedBounds(); }
	
};


class DELPHICLASS TFishEyeTransformation;
class PASCALIMPLEMENTATION TFishEyeTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	float Frx;
	float Fry;
	float Faw;
	float Fsr;
	float Sx;
	float Sy;
	float FMinR;
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TFishEyeTransformation(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TFishEyeTransformation(void) : TTransformation() { }
	
};


class DELPHICLASS TPolarTransformation;
class PASCALIMPLEMENTATION TPolarTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	Gr32::TFloatRect FDstRect;
	float FPhase;
	float Sx;
	float Sy;
	float Cx;
	float Cy;
	float Dx;
	float Dy;
	float Rt;
	float Rt2;
	float Rr;
	float Rcx;
	float Rcy;
	void __fastcall SetDstRect(const Gr32::TFloatRect &Value);
	void __fastcall SetPhase(const float Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	
public:
	__property Gr32::TFloatRect DstRect = {read=FDstRect, write=SetDstRect};
	__property float Phase = {read=FPhase, write=SetPhase};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TPolarTransformation(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TPolarTransformation(void) : TTransformation() { }
	
};


class DELPHICLASS TPathTransformation;
class PASCALIMPLEMENTATION TPathTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	struct _TPathTransformation__1
	{
		
public:
		float Dist;
		float RecDist;
	};
	
	
	typedef DynamicArray<_TPathTransformation__1> _TPathTransformation__2;
	
	
private:
	float FTopLength;
	float FBottomLength;
	Gr32::TArrayOfFloatPoint FBottomCurve;
	Gr32::TArrayOfFloatPoint FTopCurve;
	_TPathTransformation__2 FTopHypot;
	_TPathTransformation__2 FBottomHypot;
	void __fastcall SetBottomCurve(const Gr32::TArrayOfFloatPoint Value);
	void __fastcall SetTopCurve(const Gr32::TArrayOfFloatPoint Value);
	
protected:
	float rdx;
	float rdy;
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall TransformFloat(float SrcX, float SrcY, /* out */ float &DstX, /* out */ float &DstY);
	
public:
	__fastcall virtual ~TPathTransformation(void);
	__property Gr32::TArrayOfFloatPoint TopCurve = {read=FTopCurve, write=SetTopCurve};
	__property Gr32::TArrayOfFloatPoint BottomCurve = {read=FBottomCurve, write=SetBottomCurve};
public:
	/* TObject.Create */ inline __fastcall TPathTransformation(void) : TTransformation() { }
	
};


class DELPHICLASS TRemapTransformation;
class PASCALIMPLEMENTATION TRemapTransformation : public TTransformation
{
	typedef TTransformation inherited;
	
private:
	Gr32_vectormaps::TVectorMap* FVectorMap;
	Gr32::TFixedPoint FScalingFixed;
	Gr32::TFloatPoint FScalingFloat;
	Gr32::TFixedPoint FCombinedScalingFixed;
	Gr32::TFloatPoint FCombinedScalingFloat;
	Gr32::TFixedPoint FSrcTranslationFixed;
	Gr32::TFixedPoint FSrcScaleFixed;
	Gr32::TFixedPoint FDstTranslationFixed;
	Gr32::TFixedPoint FDstScaleFixed;
	Gr32::TFloatPoint FSrcTranslationFloat;
	Gr32::TFloatPoint FSrcScaleFloat;
	Gr32::TFloatPoint FDstTranslationFloat;
	Gr32::TFloatPoint FDstScaleFloat;
	Gr32::TFixedPoint FOffsetFixed;
	Types::TPoint FOffsetInt;
	Gr32::TFloatRect FMappingRect;
	Gr32::TFloatPoint FOffset;
	void __fastcall SetMappingRect(const Gr32::TFloatRect &Rect);
	void __fastcall SetOffset(const Gr32::TFloatPoint &Value);
	
protected:
	virtual void __fastcall PrepareTransform(void);
	virtual void __fastcall ReverseTransformInt(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);
	virtual void __fastcall ReverseTransformFloat(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);
	virtual void __fastcall ReverseTransformFixed(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);
	
public:
	__fastcall virtual TRemapTransformation(void);
	__fastcall virtual ~TRemapTransformation(void);
	virtual bool __fastcall HasTransformedBounds(void);
	virtual Gr32::TFloatRect __fastcall GetTransformedBounds(const Gr32::TFloatRect &ASrcRect)/* overload */;
	void __fastcall Scale(float Sx, float Sy);
	__property Gr32::TFloatRect MappingRect = {read=FMappingRect, write=SetMappingRect};
	__property Gr32::TFloatPoint Offset = {read=FOffset, write=SetOffset};
	__property Gr32_vectormaps::TVectorMap* VectorMap = {read=FVectorMap, write=FVectorMap};
	
/* Hoisted overloads: */
	
public:
	inline Gr32::TFloatRect __fastcall  GetTransformedBounds(void){ return TTransformation::GetTransformedBounds(); }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TFloatMatrix IdentityMatrix;
extern PACKAGE bool FullEdge;
extern PACKAGE System::ResourceString _RCStrReverseTransformationNotImplemented;
#define Gr32_transforms_RCStrReverseTransformationNotImplemented System::LoadResourceString(&Gr32_transforms::_RCStrReverseTransformationNotImplemented)
extern PACKAGE System::ResourceString _RCStrForwardTransformationNotImplemented;
#define Gr32_transforms_RCStrForwardTransformationNotImplemented System::LoadResourceString(&Gr32_transforms::_RCStrForwardTransformationNotImplemented)
extern PACKAGE System::ResourceString _RCStrTopBottomCurveNil;
#define Gr32_transforms_RCStrTopBottomCurveNil System::LoadResourceString(&Gr32_transforms::_RCStrTopBottomCurveNil)
extern PACKAGE void __fastcall Adjoint(StaticArray<float, 3> *M);
extern PACKAGE float __fastcall Determinant(StaticArray<float, 3> const *M);
extern PACKAGE void __fastcall Scale(StaticArray<float, 3> *M, float Factor);
extern PACKAGE void __fastcall Invert(StaticArray<float, 3> *M);
extern PACKAGE TFloatMatrix __fastcall Mult(StaticArray<float, 3> const *M1, StaticArray<float, 3> const *M2);
extern PACKAGE TVector3f __fastcall VectorTransform(StaticArray<float, 3> const *M, float const *V);
extern PACKAGE Gr32::TArrayOfArrayOfFixedPoint __fastcall TransformPoints(Gr32::TArrayOfArrayOfFixedPoint Points, TTransformation* Transformation);
extern PACKAGE void __fastcall Transform(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, TTransformation* Transformation)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, TTransformation* Transformation, const Types::TRect &DstClip)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, TTransformation* Transformation, Gr32_rasterizers::TRasterizer* Rasterizer)/* overload */;
extern PACKAGE void __fastcall Transform(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, TTransformation* Transformation, Gr32_rasterizers::TRasterizer* Rasterizer, const Types::TRect &DstClip)/* overload */;
extern PACKAGE void __fastcall SetBorderTransparent(Gr32::TCustomBitmap32* ABitmap, const Types::TRect &ARect);
extern PACKAGE void __fastcall RasterizeTransformation(Gr32_vectormaps::TVectorMap* Vectormap, TTransformation* Transformation, const Types::TRect &DstRect, Gr32_vectormaps::TVectorCombineMode CombineMode = (Gr32_vectormaps::TVectorCombineMode)(0x0), Gr32_vectormaps::TVectorCombineEvent CombineCallback = 0x0);
extern PACKAGE TFixedMatrix __fastcall FixedMatrix(StaticArray<float, 3> const *FloatMatrix)/* overload */;
extern PACKAGE TFloatMatrix __fastcall FloatMatrix(StaticArray<Gr32::TFixed, 3> const *FixedMatrix)/* overload */;

}	/* namespace Gr32_transforms */
using namespace Gr32_transforms;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_transformsHPP
