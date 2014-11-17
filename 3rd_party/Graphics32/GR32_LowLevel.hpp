// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_lowlevel.pas' rev: 21.00

#ifndef Gr32_lowlevelHPP
#define Gr32_lowlevelHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_math.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_bindings.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_lowlevel
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall (*FillLongword)(void *X, unsigned Count, unsigned Value);
extern PACKAGE StaticArray<Gr32::TWrapProc, 3> WRAP_PROCS;
extern PACKAGE StaticArray<Gr32::TWrapProcEx, 3> WRAP_PROCS_EX;
extern PACKAGE int __fastcall Clamp(const int Value)/* overload */;
extern PACKAGE void __fastcall FillWord(void *X, unsigned Count, unsigned Value);
extern PACKAGE void __fastcall MoveLongword(const void *Source, void *Dest, int Count);
extern PACKAGE void __fastcall MoveWord(const void *Source, void *Dest, int Count);
extern PACKAGE void __fastcall Swap(void * &A, void * &B)/* overload */;
extern PACKAGE void __fastcall Swap(int &A, int &B)/* overload */;
extern PACKAGE void __fastcall Swap(Gr32::TFixed &A, Gr32::TFixed &B)/* overload */;
extern PACKAGE void __fastcall Swap(Gr32::TColor32 &A, Gr32::TColor32 &B)/* overload */;
extern PACKAGE void __fastcall TestSwap(int &A, int &B)/* overload */;
extern PACKAGE void __fastcall TestSwap(Gr32::TFixed &A, Gr32::TFixed &B)/* overload */;
extern PACKAGE bool __fastcall TestClip(int &A, int &B, const int Size)/* overload */;
extern PACKAGE bool __fastcall TestClip(int &A, int &B, const int Start, const int Stop)/* overload */;
extern PACKAGE int __fastcall Constrain(const int Value, const int Lo, const int Hi)/* overload */;
extern PACKAGE float __fastcall Constrain(const float Value, const float Lo, const float Hi)/* overload */;
extern PACKAGE int __fastcall SwapConstrain(const int Value, int Constrain1, int Constrain2);
extern PACKAGE int __fastcall Max(const int A, const int B, const int C)/* overload */;
extern PACKAGE int __fastcall Min(const int A, const int B, const int C)/* overload */;
extern PACKAGE int __fastcall Clamp(int Value, int Max)/* overload */;
extern PACKAGE int __fastcall Clamp(int Value, int Min, int Max)/* overload */;
extern PACKAGE int __fastcall Wrap(int Value, int Max)/* overload */;
extern PACKAGE int __fastcall Wrap(int Value, int Min, int Max)/* overload */;
extern PACKAGE float __fastcall Wrap(float Value, float Max)/* overload */;
extern PACKAGE int __fastcall Mirror(int Value, int Max)/* overload */;
extern PACKAGE int __fastcall Mirror(int Value, int Min, int Max)/* overload */;
extern PACKAGE int __fastcall WrapPow2(int Value, int Max)/* overload */;
extern PACKAGE int __fastcall WrapPow2(int Value, int Min, int Max)/* overload */;
extern PACKAGE int __fastcall MirrorPow2(int Value, int Max)/* overload */;
extern PACKAGE int __fastcall MirrorPow2(int Value, int Min, int Max)/* overload */;
extern PACKAGE Gr32::TWrapProc __fastcall GetOptimalWrap(int Max)/* overload */;
extern PACKAGE Gr32::TWrapProcEx __fastcall GetOptimalWrap(int Min, int Max)/* overload */;
extern PACKAGE Gr32::TWrapProc __fastcall GetOptimalMirror(int Max)/* overload */;
extern PACKAGE Gr32::TWrapProcEx __fastcall GetOptimalMirror(int Min, int Max)/* overload */;
extern PACKAGE Gr32::TWrapProc __fastcall GetWrapProc(Gr32::TWrapMode WrapMode)/* overload */;
extern PACKAGE Gr32::TWrapProc __fastcall GetWrapProc(Gr32::TWrapMode WrapMode, int Max)/* overload */;
extern PACKAGE Gr32::TWrapProcEx __fastcall GetWrapProcEx(Gr32::TWrapMode WrapMode)/* overload */;
extern PACKAGE Gr32::TWrapProcEx __fastcall GetWrapProcEx(Gr32::TWrapMode WrapMode, int Min, int Max)/* overload */;
extern PACKAGE unsigned __fastcall Div255(unsigned Value);
extern PACKAGE int __fastcall SAR_4(int Value);
extern PACKAGE int __fastcall SAR_8(int Value);
extern PACKAGE int __fastcall SAR_9(int Value);
extern PACKAGE int __fastcall SAR_11(int Value);
extern PACKAGE int __fastcall SAR_12(int Value);
extern PACKAGE int __fastcall SAR_13(int Value);
extern PACKAGE int __fastcall SAR_14(int Value);
extern PACKAGE int __fastcall SAR_15(int Value);
extern PACKAGE int __fastcall SAR_16(int Value);
extern PACKAGE Gr32::TColor32 __fastcall ColorSwap(Graphics::TColor WinColor);
extern PACKAGE void * __fastcall StackAlloc(int Size);
extern PACKAGE void __fastcall StackFree(void * P);

}	/* namespace Gr32_lowlevel */
using namespace Gr32_lowlevel;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_lowlevelHPP
