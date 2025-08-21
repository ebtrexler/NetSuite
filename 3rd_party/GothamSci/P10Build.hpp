// CodeGear C++Builder
// Copyright (c) 1995, 2015 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'P10Build.pas' rev: 30.00 (Windows)

#ifndef P10buildHPP
#define P10buildHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Parser10.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>

//-- user supplied -----------------------------------------------------------

namespace P10build
{
//-- forward type declarations -----------------------------------------------
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall ParseFunction(System::UnicodeString FunctionString, System::Classes::TStringList* Variables, System::Classes::TStringList* FunctionOne, System::Classes::TStringList* FunctionTwo, bool UsePascalNumbers, Parser10::POperation &FirstOP, bool &Error);
}	/* namespace P10build */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_P10BUILD)
using namespace P10build;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// P10buildHPP
