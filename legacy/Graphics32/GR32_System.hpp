// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_system.pas' rev: 21.00

#ifndef Gr32_systemHPP
#define Gr32_systemHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_system
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TPerfTimer;
class PASCALIMPLEMENTATION TPerfTimer : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	__int64 FFrequency;
	__int64 FPerformanceCountStart;
	__int64 FPerformanceCountStop;
	
public:
	void __fastcall Start(void);
	System::UnicodeString __fastcall ReadNanoseconds(void);
	System::UnicodeString __fastcall ReadMilliseconds(void);
	System::UnicodeString __fastcall ReadSeconds(void);
	__int64 __fastcall ReadValue(void);
public:
	/* TObject.Create */ inline __fastcall TPerfTimer(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TPerfTimer(void) { }
	
};


#pragma option push -b-
enum TCPUInstructionSet { ciMMX, ciEMMX, ciSSE, ciSSE2, ci3DNow, ci3DNowExt };
#pragma option pop

typedef Set<TCPUInstructionSet, ciMMX, ci3DNowExt>  TCPUFeatures;

typedef TCPUFeatures *PCPUFeatures;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TPerfTimer* GlobalPerfTimer;
extern PACKAGE unsigned __fastcall GetTickCount(void);
extern PACKAGE unsigned __fastcall GetProcessorCount(void);
extern PACKAGE bool __fastcall HasInstructionSet(const TCPUInstructionSet InstructionSet);
extern PACKAGE TCPUFeatures __fastcall CPUFeatures(void);

}	/* namespace Gr32_system */
using namespace Gr32_system;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_systemHPP
