// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_backends_generic.pas' rev: 21.00

#ifndef Gr32_backends_genericHPP
#define Gr32_backends_genericHPP

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
#include <Gr32_backends.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_backends_generic
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TMemoryBackend;
class PASCALIMPLEMENTATION TMemoryBackend : public Gr32::TCustomBackend
{
	typedef Gr32::TCustomBackend inherited;
	
protected:
	virtual void __fastcall InitializeSurface(int NewWidth, int NewHeight, bool ClearBuffer);
	virtual void __fastcall FinalizeSurface(void);
public:
	/* TCustomBackend.Create */ inline __fastcall virtual TMemoryBackend(void)/* overload */ : Gr32::TCustomBackend() { }
	/* TCustomBackend.Destroy */ inline __fastcall virtual ~TMemoryBackend(void) { }
	
};


class DELPHICLASS TMMFBackend;
class PASCALIMPLEMENTATION TMMFBackend : public TMemoryBackend
{
	typedef TMemoryBackend inherited;
	
private:
	unsigned FMapHandle;
	bool FMapIsTemporary;
	unsigned FMapFileHandle;
	System::UnicodeString FMapFileName;
	
protected:
	virtual void __fastcall InitializeSurface(int NewWidth, int NewHeight, bool ClearBuffer);
	virtual void __fastcall FinalizeSurface(void);
	
public:
	__fastcall virtual TMMFBackend(Gr32::TCustomBitmap32* Owner, bool IsTemporary, const System::UnicodeString MapFileName);
	__fastcall virtual ~TMMFBackend(void);
	__classmethod void __fastcall InitializeFileMapping(unsigned &MapHandle, unsigned &MapFileHandle, System::UnicodeString &MapFileName);
	__classmethod void __fastcall DeinitializeFileMapping(unsigned MapHandle, unsigned MapFileHandle, const System::UnicodeString MapFileName);
	__classmethod void __fastcall CreateFileMapping(unsigned &MapHandle, unsigned &MapFileHandle, System::UnicodeString &MapFileName, bool IsTemporary, int NewWidth, int NewHeight);
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_backends_generic */
using namespace Gr32_backends_generic;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_backends_genericHPP
