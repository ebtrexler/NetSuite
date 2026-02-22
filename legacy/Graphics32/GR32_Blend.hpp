// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_blend.pas' rev: 21.00

#ifndef Gr32_blendHPP
#define Gr32_blendHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_bindings.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_blend
{
//-- type declarations -------------------------------------------------------
typedef Gr32::TColor32 __fastcall (*TBlendReg)(Gr32::TColor32 F, Gr32::TColor32 B);

typedef void __fastcall (*TBlendMem)(Gr32::TColor32 F, Gr32::TColor32 &B);

typedef Gr32::TColor32 __fastcall (*TBlendRegEx)(Gr32::TColor32 F, Gr32::TColor32 B, Gr32::TColor32 M);

typedef void __fastcall (*TBlendMemEx)(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);

typedef void __fastcall (*TBlendLine)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count);

typedef void __fastcall (*TBlendLineEx)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count, Gr32::TColor32 M);

typedef Gr32::TColor32 __fastcall (*TCombineReg)(Gr32::TColor32 X, Gr32::TColor32 Y, Gr32::TColor32 W);

typedef void __fastcall (*TCombineMem)(Gr32::TColor32 X, Gr32::TColor32 &Y, Gr32::TColor32 W);

typedef void __fastcall (*TCombineLine)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count, Gr32::TColor32 W);

typedef Gr32::TColor32 __fastcall (*TLightenReg)(Gr32::TColor32 C, int Amount);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool MMX_ACTIVE;
extern PACKAGE void __fastcall (*EMMS)(void);
extern PACKAGE TBlendReg BlendReg;
extern PACKAGE TBlendMem BlendMem;
extern PACKAGE TBlendRegEx BlendRegEx;
extern PACKAGE TBlendMemEx BlendMemEx;
extern PACKAGE TBlendLine BlendLine;
extern PACKAGE TBlendLineEx BlendLineEx;
extern PACKAGE TCombineReg CombineReg;
extern PACKAGE TCombineMem CombineMem;
extern PACKAGE TCombineLine CombineLine;
extern PACKAGE TBlendReg MergeReg;
extern PACKAGE TBlendMem MergeMem;
extern PACKAGE TBlendRegEx MergeRegEx;
extern PACKAGE TBlendMemEx MergeMemEx;
extern PACKAGE TBlendLine MergeLine;
extern PACKAGE TBlendLineEx MergeLineEx;
extern PACKAGE TBlendReg ColorAdd;
extern PACKAGE TBlendReg ColorSub;
extern PACKAGE TBlendReg ColorDiv;
extern PACKAGE TBlendReg ColorModulate;
extern PACKAGE TBlendReg ColorMax;
extern PACKAGE TBlendReg ColorMin;
extern PACKAGE TBlendReg ColorDifference;
extern PACKAGE TBlendReg ColorAverage;
extern PACKAGE TBlendReg ColorExclusion;
extern PACKAGE TBlendReg ColorScale;
extern PACKAGE void *AlphaTable;
extern PACKAGE void *bias_ptr;
extern PACKAGE void *alpha_ptr;
extern PACKAGE TLightenReg LightenReg;
extern PACKAGE StaticArray<void *, 2> BLEND_REG;
extern PACKAGE StaticArray<void *, 2> BLEND_MEM;
extern PACKAGE StaticArray<void *, 2> BLEND_REG_EX;
extern PACKAGE StaticArray<void *, 2> BLEND_MEM_EX;
extern PACKAGE StaticArray<void *, 2> BLEND_LINE;
extern PACKAGE StaticArray<void *, 2> BLEND_LINE_EX;
extern PACKAGE Gr32_bindings::TFunctionRegistry* BlendRegistry;
extern PACKAGE Gr32::TColor32 __fastcall Lighten(Gr32::TColor32 C, int Amount);

}	/* namespace Gr32_blend */
using namespace Gr32_blend;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_blendHPP
