// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_filters.pas' rev: 21.00

#ifndef Gr32_filtersHPP
#define Gr32_filtersHPP

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
#include <Gr32_blend.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_bindings.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_filters
{
//-- type declarations -------------------------------------------------------
typedef StaticArray<System::Byte, 256> TLUT8;

#pragma option push -b-
enum TLogicalOperator { loXOR, loAND, loOR };
#pragma option pop

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall CheckParams(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, bool ResizeDst = true);
extern PACKAGE void __fastcall CopyComponents(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, Gr32::TColor32Components Components)/* overload */;
extern PACKAGE void __fastcall CopyComponents(Gr32::TCustomBitmap32* Dst, int DstX, int DstY, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TColor32Components Components)/* overload */;
extern PACKAGE void __fastcall AlphaToGrayscale(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src);
extern PACKAGE void __fastcall IntensityToAlpha(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src);
extern PACKAGE void __fastcall Invert(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, Gr32::TColor32Components Components = (Gr32::TColor32Components() << ccBlue << ccGreen << ccRed << ccAlpha ));
extern PACKAGE void __fastcall InvertRGB(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src);
extern PACKAGE void __fastcall ColorToGrayscale(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, bool PreserveAlpha = false);
extern PACKAGE void __fastcall ApplyLUT(Gr32::TCustomBitmap32* Dst, Gr32::TCustomBitmap32* Src, System::Byte const *LUT, bool PreserveAlpha = false);
extern PACKAGE void __fastcall ChromaKey(Gr32::TCustomBitmap32* ABitmap, Gr32::TColor32 TrColor);
extern PACKAGE Gr32::TColor32 __fastcall CreateBitmask(Gr32::TColor32Components Components);
extern PACKAGE void __fastcall ApplyBitmask(Gr32::TCustomBitmap32* Dst, int DstX, int DstY, Gr32::TCustomBitmap32* Src, const Types::TRect &SrcRect, Gr32::TColor32 Bitmask, TLogicalOperator LogicalOperator)/* overload */;
extern PACKAGE void __fastcall ApplyBitmask(Gr32::TCustomBitmap32* ABitmap, const Types::TRect &ARect, Gr32::TColor32 Bitmask, TLogicalOperator LogicalOperator)/* overload */;

}	/* namespace Gr32_filters */
using namespace Gr32_filters;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_filtersHPP
