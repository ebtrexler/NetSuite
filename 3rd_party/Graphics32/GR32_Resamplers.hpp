// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_resamplers.pas' rev: 21.00

#ifndef Gr32_resamplersHPP
#define Gr32_resamplersHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_transforms.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_ordinalmaps.hpp>	// Pascal unit
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_bindings.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_resamplers
{
//-- type declarations -------------------------------------------------------
typedef StaticArray<int, 33> TKernelEntry;

typedef TKernelEntry *PKernelEntry;

typedef DynamicArray<DynamicArray<int> > TArrayOfKernelEntry;

typedef StaticArray<Gr32::TArrayOfInteger, 1> TKernelEntryArray;

typedef TKernelEntryArray *PKernelEntryArray;

typedef float __fastcall (__closure *TFilterMethod)(float Value);

class DELPHICLASS EBitmapException;
class PASCALIMPLEMENTATION EBitmapException : public Sysutils::Exception
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall EBitmapException(const System::UnicodeString Msg) : Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EBitmapException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EBitmapException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	/* Exception.CreateResFmt */ inline __fastcall EBitmapException(int Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EBitmapException(const System::UnicodeString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EBitmapException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBitmapException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBitmapException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EBitmapException(void) { }
	
};


class DELPHICLASS ESrcInvalidException;
class PASCALIMPLEMENTATION ESrcInvalidException : public Sysutils::Exception
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall ESrcInvalidException(const System::UnicodeString Msg) : Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ESrcInvalidException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall ESrcInvalidException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	/* Exception.CreateResFmt */ inline __fastcall ESrcInvalidException(int Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall ESrcInvalidException(const System::UnicodeString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ESrcInvalidException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ESrcInvalidException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ESrcInvalidException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ESrcInvalidException(void) { }
	
};


class DELPHICLASS ENestedException;
class PASCALIMPLEMENTATION ENestedException : public Sysutils::Exception
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall ENestedException(const System::UnicodeString Msg) : Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ENestedException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall ENestedException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	/* Exception.CreateResFmt */ inline __fastcall ENestedException(int Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall ENestedException(const System::UnicodeString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ENestedException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ENestedException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ENestedException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ENestedException(void) { }
	
};


typedef Gr32::TColor32 __fastcall (__closure *TGetSampleInt)(int X, int Y);

typedef Gr32::TColor32 __fastcall (__closure *TGetSampleFloat)(float X, float Y);

typedef Gr32::TColor32 __fastcall (__closure *TGetSampleFixed)(Gr32::TFixed X, Gr32::TFixed Y);

class DELPHICLASS TCustomKernel;
class PASCALIMPLEMENTATION TCustomKernel : public Classes::TPersistent
{
	typedef Classes::TPersistent inherited;
	
protected:
	Gr32::TNotifiablePersistent* FObserver;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TCustomKernel(void);
	void __fastcall Changed(void);
	virtual float __fastcall Filter(float Value) = 0 ;
	virtual float __fastcall GetWidth(void) = 0 ;
	__property Gr32::TNotifiablePersistent* Observer = {read=FObserver};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCustomKernel(void) { }
	
};


typedef TMetaClass* TCustomKernelClass;

class DELPHICLASS TBoxKernel;
class PASCALIMPLEMENTATION TBoxKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	/* TCustomKernel.Create */ inline __fastcall virtual TBoxKernel(void) : TCustomKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBoxKernel(void) { }
	
};


class DELPHICLASS TLinearKernel;
class PASCALIMPLEMENTATION TLinearKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	/* TCustomKernel.Create */ inline __fastcall virtual TLinearKernel(void) : TCustomKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TLinearKernel(void) { }
	
};


class DELPHICLASS TCosineKernel;
class PASCALIMPLEMENTATION TCosineKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	/* TCustomKernel.Create */ inline __fastcall virtual TCosineKernel(void) : TCustomKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCosineKernel(void) { }
	
};


class DELPHICLASS TSplineKernel;
class PASCALIMPLEMENTATION TSplineKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	/* TCustomKernel.Create */ inline __fastcall virtual TSplineKernel(void) : TCustomKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSplineKernel(void) { }
	
};


class DELPHICLASS TMitchellKernel;
class PASCALIMPLEMENTATION TMitchellKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
public:
	/* TCustomKernel.Create */ inline __fastcall virtual TMitchellKernel(void) : TCustomKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TMitchellKernel(void) { }
	
};


class DELPHICLASS TCubicKernel;
class PASCALIMPLEMENTATION TCubicKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
private:
	float FCoeff;
	void __fastcall SetCoeff(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TCubicKernel(void);
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
	
__published:
	__property float Coeff = {read=FCoeff, write=SetCoeff};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TCubicKernel(void) { }
	
};


class DELPHICLASS THermiteKernel;
class PASCALIMPLEMENTATION THermiteKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
private:
	float FBias;
	float FTension;
	void __fastcall SetBias(const float Value);
	void __fastcall SetTension(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual THermiteKernel(void);
	virtual float __fastcall Filter(float Value);
	virtual float __fastcall GetWidth(void);
	
__published:
	__property float Bias = {read=FBias, write=SetBias};
	__property float Tension = {read=FTension, write=SetTension};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~THermiteKernel(void) { }
	
};


class DELPHICLASS TWindowedSincKernel;
class PASCALIMPLEMENTATION TWindowedSincKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
private:
	float FWidth;
	float FWidthReciprocal;
	
protected:
	virtual bool __fastcall RangeCheck(void);
	virtual float __fastcall Window(float Value) = 0 ;
	
public:
	__fastcall virtual TWindowedSincKernel(void);
	virtual float __fastcall Filter(float Value);
	void __fastcall SetWidth(float Value);
	virtual float __fastcall GetWidth(void);
	__property float WidthReciprocal = {read=FWidthReciprocal};
	
__published:
	__property float Width = {read=FWidth, write=SetWidth};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TWindowedSincKernel(void) { }
	
};


class DELPHICLASS TAlbrechtKernel;
class PASCALIMPLEMENTATION TAlbrechtKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
private:
	int FTerms;
	StaticArray<double, 12> FCoefPointer;
	void __fastcall SetTerms(int Value);
	
protected:
	virtual float __fastcall Window(float Value);
	
public:
	__fastcall virtual TAlbrechtKernel(void);
	
__published:
	__property int Terms = {read=FTerms, write=SetTerms, nodefault};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TAlbrechtKernel(void) { }
	
};


class DELPHICLASS TLanczosKernel;
class PASCALIMPLEMENTATION TLanczosKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	/* TWindowedSincKernel.Create */ inline __fastcall virtual TLanczosKernel(void) : TWindowedSincKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TLanczosKernel(void) { }
	
};


class DELPHICLASS TGaussianKernel;
class PASCALIMPLEMENTATION TGaussianKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
private:
	float FSigma;
	float FSigmaReciprocalLn2;
	void __fastcall SetSigma(const float Value);
	
protected:
	virtual float __fastcall Window(float Value);
	
public:
	__fastcall virtual TGaussianKernel(void);
	
__published:
	__property float Sigma = {read=FSigma, write=SetSigma};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TGaussianKernel(void) { }
	
};


class DELPHICLASS TBlackmanKernel;
class PASCALIMPLEMENTATION TBlackmanKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	/* TWindowedSincKernel.Create */ inline __fastcall virtual TBlackmanKernel(void) : TWindowedSincKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TBlackmanKernel(void) { }
	
};


class DELPHICLASS THannKernel;
class PASCALIMPLEMENTATION THannKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	/* TWindowedSincKernel.Create */ inline __fastcall virtual THannKernel(void) : TWindowedSincKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~THannKernel(void) { }
	
};


class DELPHICLASS THammingKernel;
class PASCALIMPLEMENTATION THammingKernel : public TWindowedSincKernel
{
	typedef TWindowedSincKernel inherited;
	
protected:
	virtual float __fastcall Window(float Value);
public:
	/* TWindowedSincKernel.Create */ inline __fastcall virtual THammingKernel(void) : TWindowedSincKernel() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~THammingKernel(void) { }
	
};


class DELPHICLASS TSinshKernel;
class PASCALIMPLEMENTATION TSinshKernel : public TCustomKernel
{
	typedef TCustomKernel inherited;
	
private:
	float FWidth;
	float FCoeff;
	void __fastcall SetCoeff(const float Value);
	
protected:
	virtual bool __fastcall RangeCheck(void);
	
public:
	__fastcall virtual TSinshKernel(void);
	void __fastcall SetWidth(float Value);
	virtual float __fastcall GetWidth(void);
	virtual float __fastcall Filter(float Value);
	
__published:
	__property float Coeff = {read=FCoeff, write=SetCoeff};
	__property float Width = {read=GetWidth, write=SetWidth};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSinshKernel(void) { }
	
};


class DELPHICLASS TNearestResampler;
class PASCALIMPLEMENTATION TNearestResampler : public Gr32::TCustomResampler
{
	typedef Gr32::TCustomResampler inherited;
	
private:
	TGetSampleInt FGetSampleInt;
	
protected:
	Gr32::TColor32 __fastcall GetPixelTransparentEdge(int X, int Y);
	virtual float __fastcall GetWidth(void);
	virtual void __fastcall Resample(Gr32::TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	
public:
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall PrepareSampling(void);
public:
	/* TCustomResampler.Create */ inline __fastcall virtual TNearestResampler(void)/* overload */ : Gr32::TCustomResampler() { }
	
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNearestResampler(void) { }
	
};


class DELPHICLASS TLinearResampler;
class PASCALIMPLEMENTATION TLinearResampler : public Gr32::TCustomResampler
{
	typedef Gr32::TCustomResampler inherited;
	
private:
	TLinearKernel* FLinearKernel;
	TGetSampleFixed FGetSampleFixed;
	
protected:
	virtual float __fastcall GetWidth(void);
	Gr32::TColor32 __fastcall GetPixelTransparentEdge(Gr32::TFixed X, Gr32::TFixed Y);
	virtual void __fastcall Resample(Gr32::TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	
public:
	__fastcall virtual TLinearResampler(void)/* overload */;
	__fastcall virtual ~TLinearResampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall PrepareSampling(void);
};


class DELPHICLASS TDraftResampler;
class PASCALIMPLEMENTATION TDraftResampler : public TLinearResampler
{
	typedef TLinearResampler inherited;
	
protected:
	virtual void __fastcall Resample(Gr32::TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
public:
	/* TLinearResampler.Create */ inline __fastcall virtual TDraftResampler(void)/* overload */ : TLinearResampler() { }
	/* TLinearResampler.Destroy */ inline __fastcall virtual ~TDraftResampler(void) { }
	
};


#pragma option push -b-
enum TKernelMode { kmDynamic, kmTableNearest, kmTableLinear };
#pragma option pop

class DELPHICLASS TKernelResampler;
class PASCALIMPLEMENTATION TKernelResampler : public Gr32::TCustomResampler
{
	typedef Gr32::TCustomResampler inherited;
	
private:
	TCustomKernel* FKernel;
	TKernelMode FKernelMode;
	Gr32_ordinalmaps::TIntegerMap* FWeightTable;
	int FTableSize;
	Gr32::TColor32 FOuterColor;
	void __fastcall SetKernel(const TCustomKernel* Value);
	System::UnicodeString __fastcall GetKernelClassName(void);
	void __fastcall SetKernelClassName(System::UnicodeString Value);
	void __fastcall SetKernelMode(const TKernelMode Value);
	void __fastcall SetTableSize(int Value);
	
protected:
	virtual float __fastcall GetWidth(void);
	
public:
	__fastcall virtual TKernelResampler(void)/* overload */;
	__fastcall virtual ~TKernelResampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual void __fastcall Resample(Gr32::TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack);
	virtual void __fastcall PrepareSampling(void);
	virtual void __fastcall FinalizeSampling(void);
	
__published:
	__property System::UnicodeString KernelClassName = {read=GetKernelClassName, write=SetKernelClassName};
	__property TCustomKernel* Kernel = {read=FKernel, write=SetKernel};
	__property TKernelMode KernelMode = {read=FKernelMode, write=SetKernelMode, nodefault};
	__property int TableSize = {read=FTableSize, write=SetTableSize, nodefault};
};


class DELPHICLASS TNestedSampler;
class PASCALIMPLEMENTATION TNestedSampler : public Gr32::TCustomSampler
{
	typedef Gr32::TCustomSampler inherited;
	
private:
	Gr32::TCustomSampler* FSampler;
	TGetSampleInt FGetSampleInt;
	TGetSampleFixed FGetSampleFixed;
	TGetSampleFloat FGetSampleFloat;
	void __fastcall SetSampler(const Gr32::TCustomSampler* Value);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	
public:
	__fastcall virtual TNestedSampler(Gr32::TCustomSampler* ASampler);
	virtual void __fastcall PrepareSampling(void);
	virtual void __fastcall FinalizeSampling(void);
	virtual bool __fastcall HasBounds(void);
	virtual Gr32::TFloatRect __fastcall GetSampleBounds(void);
	
__published:
	__property Gr32::TCustomSampler* Sampler = {read=FSampler, write=SetSampler};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TNestedSampler(void) { }
	
};


typedef void __fastcall (__closure *TReverseTransformInt)(int DstX, int DstY, /* out */ int &SrcX, /* out */ int &SrcY);

typedef void __fastcall (__closure *TReverseTransformFixed)(Gr32::TFixed DstX, Gr32::TFixed DstY, /* out */ Gr32::TFixed &SrcX, /* out */ Gr32::TFixed &SrcY);

typedef void __fastcall (__closure *TReverseTransformFloat)(float DstX, float DstY, /* out */ float &SrcX, /* out */ float &SrcY);

class DELPHICLASS TTransformer;
class PASCALIMPLEMENTATION TTransformer : public TNestedSampler
{
	typedef TNestedSampler inherited;
	
private:
	Gr32_transforms::TTransformation* FTransformation;
	TReverseTransformInt FTransformationReverseTransformInt;
	TReverseTransformFixed FTransformationReverseTransformFixed;
	TReverseTransformFloat FTransformationReverseTransformFloat;
	void __fastcall SetTransformation(const Gr32_transforms::TTransformation* Value);
	
public:
	__fastcall TTransformer(Gr32::TCustomSampler* ASampler, Gr32_transforms::TTransformation* ATransformation);
	virtual void __fastcall PrepareSampling(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	virtual Gr32::TColor32 __fastcall GetSampleFloat(float X, float Y);
	virtual bool __fastcall HasBounds(void);
	virtual Gr32::TFloatRect __fastcall GetSampleBounds(void);
	
__published:
	__property Gr32_transforms::TTransformation* Transformation = {read=FTransformation, write=SetTransformation};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TTransformer(void) { }
	
};


typedef int TSamplingRange;

class DELPHICLASS TSuperSampler;
class PASCALIMPLEMENTATION TSuperSampler : public TNestedSampler
{
	typedef TNestedSampler inherited;
	
private:
	TSamplingRange FSamplingY;
	TSamplingRange FSamplingX;
	Gr32::TFixed FDistanceX;
	Gr32::TFixed FDistanceY;
	Gr32::TFixed FOffsetX;
	Gr32::TFixed FOffsetY;
	Gr32::TFixed FScale;
	void __fastcall SetSamplingX(const TSamplingRange Value);
	void __fastcall SetSamplingY(const TSamplingRange Value);
	
public:
	__fastcall virtual TSuperSampler(Gr32::TCustomSampler* Sampler);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property TSamplingRange SamplingX = {read=FSamplingX, write=SetSamplingX, nodefault};
	__property TSamplingRange SamplingY = {read=FSamplingY, write=SetSamplingY, nodefault};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSuperSampler(void) { }
	
};


typedef Gr32::TColor32 __fastcall (__closure *TRecurseProc)(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed W, const Gr32::TColor32 C1, const Gr32::TColor32 C2);

class DELPHICLASS TAdaptiveSuperSampler;
class PASCALIMPLEMENTATION TAdaptiveSuperSampler : public TNestedSampler
{
	typedef TNestedSampler inherited;
	
private:
	Gr32::TFixed FMinOffset;
	int FLevel;
	int FTolerance;
	void __fastcall SetLevel(const int Value);
	Gr32::TColor32 __fastcall DoRecurse(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 A, const Gr32::TColor32 B, const Gr32::TColor32 C, const Gr32::TColor32 D, const Gr32::TColor32 E);
	Gr32::TColor32 __fastcall QuadrantColor(const Gr32::TColor32 C1, const Gr32::TColor32 C2, Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, TRecurseProc Proc);
	Gr32::TColor32 __fastcall RecurseAC(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 A, const Gr32::TColor32 C);
	Gr32::TColor32 __fastcall RecurseBD(Gr32::TFixed X, Gr32::TFixed Y, Gr32::TFixed Offset, const Gr32::TColor32 B, const Gr32::TColor32 D);
	
protected:
	virtual bool __fastcall CompareColors(Gr32::TColor32 C1, Gr32::TColor32 C2);
	
public:
	__fastcall virtual TAdaptiveSuperSampler(Gr32::TCustomSampler* Sampler);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property int Level = {read=FLevel, write=SetLevel, nodefault};
	__property int Tolerance = {read=FTolerance, write=FTolerance, nodefault};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TAdaptiveSuperSampler(void) { }
	
};


typedef DynamicArray<DynamicArray<Gr32::TFloatPoint> > Gr32_resamplers__82;

typedef DynamicArray<DynamicArray<DynamicArray<Gr32::TFloatPoint> > > TFloatSamplePattern;

typedef DynamicArray<DynamicArray<Gr32::TFixedPoint> > Gr32_resamplers__92;

typedef DynamicArray<DynamicArray<DynamicArray<Gr32::TFixedPoint> > > TFixedSamplePattern;

class DELPHICLASS TPatternSampler;
class PASCALIMPLEMENTATION TPatternSampler : public TNestedSampler
{
	typedef TNestedSampler inherited;
	
private:
	TFixedSamplePattern FPattern;
	void __fastcall SetPattern(const TFixedSamplePattern Value);
	
protected:
	Gr32::TWrapProc WrapProcVert;
	
public:
	__fastcall virtual ~TPatternSampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	__property TFixedSamplePattern Pattern = {read=FPattern, write=SetPattern};
public:
	/* TNestedSampler.Create */ inline __fastcall virtual TPatternSampler(Gr32::TCustomSampler* ASampler) : TNestedSampler(ASampler) { }
	
};


struct TBufferEntry;
typedef TBufferEntry *PBufferEntry;

struct TBufferEntry
{
	
public:
	int B;
	int G;
	int R;
	int A;
};


class DELPHICLASS TKernelSampler;
class PASCALIMPLEMENTATION TKernelSampler : public TNestedSampler
{
	typedef TNestedSampler inherited;
	
private:
	Gr32_ordinalmaps::TIntegerMap* FKernel;
	TBufferEntry FStartEntry;
	int FCenterX;
	int FCenterY;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight) = 0 ;
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
	
public:
	__fastcall virtual TKernelSampler(Gr32::TCustomSampler* ASampler);
	__fastcall virtual ~TKernelSampler(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property Gr32_ordinalmaps::TIntegerMap* Kernel = {read=FKernel, write=FKernel};
	__property int CenterX = {read=FCenterX, write=FCenterX, nodefault};
	__property int CenterY = {read=FCenterY, write=FCenterY, nodefault};
};


class DELPHICLASS TConvolver;
class PASCALIMPLEMENTATION TConvolver : public TKernelSampler
{
	typedef TKernelSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	/* TKernelSampler.Create */ inline __fastcall virtual TConvolver(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TConvolver(void) { }
	
};


class DELPHICLASS TSelectiveConvolver;
class PASCALIMPLEMENTATION TSelectiveConvolver : public TConvolver
{
	typedef TConvolver inherited;
	
private:
	Gr32::TColor32 FRefColor;
	int FDelta;
	TBufferEntry FWeightSum;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
	
public:
	__fastcall virtual TSelectiveConvolver(Gr32::TCustomSampler* ASampler);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
	
__published:
	__property int Delta = {read=FDelta, write=FDelta, nodefault};
public:
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TSelectiveConvolver(void) { }
	
};


class DELPHICLASS TMorphologicalSampler;
class PASCALIMPLEMENTATION TMorphologicalSampler : public TKernelSampler
{
	typedef TKernelSampler inherited;
	
protected:
	virtual Gr32::TColor32 __fastcall ConvertBuffer(TBufferEntry &Buffer);
public:
	/* TKernelSampler.Create */ inline __fastcall virtual TMorphologicalSampler(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TMorphologicalSampler(void) { }
	
};


class DELPHICLASS TDilater;
class PASCALIMPLEMENTATION TDilater : public TMorphologicalSampler
{
	typedef TMorphologicalSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	/* TKernelSampler.Create */ inline __fastcall virtual TDilater(Gr32::TCustomSampler* ASampler) : TMorphologicalSampler(ASampler) { }
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TDilater(void) { }
	
};


class DELPHICLASS TEroder;
class PASCALIMPLEMENTATION TEroder : public TMorphologicalSampler
{
	typedef TMorphologicalSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	
public:
	__fastcall virtual TEroder(Gr32::TCustomSampler* ASampler);
public:
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TEroder(void) { }
	
};


class DELPHICLASS TExpander;
class PASCALIMPLEMENTATION TExpander : public TKernelSampler
{
	typedef TKernelSampler inherited;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
public:
	/* TKernelSampler.Create */ inline __fastcall virtual TExpander(Gr32::TCustomSampler* ASampler) : TKernelSampler(ASampler) { }
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TExpander(void) { }
	
};


class DELPHICLASS TContracter;
class PASCALIMPLEMENTATION TContracter : public TExpander
{
	typedef TExpander inherited;
	
private:
	Gr32::TColor32 FMaxWeight;
	
protected:
	virtual void __fastcall UpdateBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color, int Weight);
	
public:
	virtual void __fastcall PrepareSampling(void);
	virtual Gr32::TColor32 __fastcall GetSampleInt(int X, int Y);
	virtual Gr32::TColor32 __fastcall GetSampleFixed(Gr32::TFixed X, Gr32::TFixed Y);
public:
	/* TKernelSampler.Create */ inline __fastcall virtual TContracter(Gr32::TCustomSampler* ASampler) : TExpander(ASampler) { }
	/* TKernelSampler.Destroy */ inline __fastcall virtual ~TContracter(void) { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const ShortInt MAX_KERNEL_WIDTH = 0x10;
extern PACKAGE Gr32_containers::TClassList* KernelList;
extern PACKAGE Gr32_containers::TClassList* ResamplerList;
extern PACKAGE TBufferEntry EMPTY_ENTRY;
extern PACKAGE Gr32::TColor32 __fastcall (*BlockAverage)(unsigned Dlx, unsigned Dly, Gr32::PColor32 RowSrc, unsigned OffSrc);
extern PACKAGE Gr32::TColor32 __fastcall (*Interpolator)(unsigned WX_256, unsigned WY_256, Gr32::PColor32 C11, Gr32::PColor32 C21);
extern PACKAGE System::ResourceString _SDstNil;
#define Gr32_resamplers_SDstNil System::LoadResourceString(&Gr32_resamplers::_SDstNil)
extern PACKAGE System::ResourceString _SSrcNil;
#define Gr32_resamplers_SSrcNil System::LoadResourceString(&Gr32_resamplers::_SSrcNil)
extern PACKAGE System::ResourceString _SSrcInvalid;
#define Gr32_resamplers_SSrcInvalid System::LoadResourceString(&Gr32_resamplers::_SSrcInvalid)
extern PACKAGE System::ResourceString _SSamplerNil;
#define Gr32_resamplers_SSamplerNil System::LoadResourceString(&Gr32_resamplers::_SSamplerNil)
extern PACKAGE void __fastcall Convolve(Gr32::TCustomBitmap32* Src, Gr32::TCustomBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Dilate(Gr32::TCustomBitmap32* Src, Gr32::TCustomBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Erode(Gr32::TCustomBitmap32* Src, Gr32::TCustomBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Expand(Gr32::TCustomBitmap32* Src, Gr32::TCustomBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall Contract(Gr32::TCustomBitmap32* Src, Gr32::TCustomBitmap32* Dst, Gr32_ordinalmaps::TIntegerMap* Kernel, int CenterX, int CenterY);
extern PACKAGE void __fastcall IncBuffer(TBufferEntry &Buffer, Gr32::TColor32 Color);
extern PACKAGE void __fastcall MultiplyBuffer(TBufferEntry &Buffer, int W);
extern PACKAGE void __fastcall ShrBuffer(TBufferEntry &Buffer, int Shift);
extern PACKAGE Gr32::TColor32 __fastcall BufferToColor32(const TBufferEntry &Buffer, int Shift);
extern PACKAGE void __fastcall BlockTransfer(Gr32::TCustomBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE void __fastcall BlockTransferX(Gr32::TCustomBitmap32* Dst, Gr32::TFixed DstX, Gr32::TFixed DstY, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE void __fastcall BlendTransfer(Gr32::TCustomBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TCustomBitmap32* SrcF, const Types::TRect &SrcRectF, Gr32::TCustomBitmap32* SrcB, const Types::TRect &SrcRectB, Gr32_blend::TBlendReg BlendCallback)/* overload */;
extern PACKAGE void __fastcall BlendTransfer(Gr32::TCustomBitmap32* Dst, int DstX, int DstY, const Types::TRect &DstClip, Gr32::TCustomBitmap32* SrcF, const Types::TRect &SrcRectF, Gr32::TCustomBitmap32* SrcB, const Types::TRect &SrcRectB, Gr32_blend::TBlendRegEx BlendCallback, int MasterAlpha)/* overload */;
extern PACKAGE void __fastcall StretchTransfer(Gr32::TCustomBitmap32* Dst, const Types::TRect &DstRect, const Types::TRect &DstClip, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TCustomResampler* Resampler, Gr32::TDrawMode CombineOp, Gr32::TPixelCombineEvent CombineCallBack = 0x0);
extern PACKAGE TFixedSamplePattern __fastcall CreateJitteredPattern(int TileWidth, int TileHeight, int SamplesX, int SamplesY);
extern PACKAGE void __fastcall RegisterResampler(Gr32::TCustomResamplerClass ResamplerClass);
extern PACKAGE void __fastcall RegisterKernel(TCustomKernelClass KernelClass);

}	/* namespace Gr32_resamplers */
using namespace Gr32_resamplers;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_resamplersHPP
