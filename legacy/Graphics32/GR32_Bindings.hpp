// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_bindings.pas' rev: 21.00

#ifndef Gr32_bindingsHPP
#define Gr32_bindingsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_bindings
{
//-- type declarations -------------------------------------------------------
typedef System::UnicodeString TFunctionName;

typedef int TFunctionID;

struct TFunctionInfo;
typedef TFunctionInfo *PFunctionInfo;

struct TFunctionInfo
{
	
public:
	int FunctionID;
	void *Proc;
	Gr32_system::TCPUFeatures CPUFeatures;
	int Flags;
};


typedef int __fastcall (*TFunctionPriority)(PFunctionInfo Info);

struct TFunctionBinding;
typedef TFunctionBinding *PFunctionBinding;

struct TFunctionBinding
{
	
public:
	int FunctionID;
	void * *BindVariable;
};


class DELPHICLASS TFunctionRegistry;
class PASCALIMPLEMENTATION TFunctionRegistry : public Classes::TPersistent
{
	typedef Classes::TPersistent inherited;
	
private:
	Classes::TList* FItems;
	Classes::TList* FBindings;
	System::UnicodeString FName;
	void __fastcall SetName(const System::UnicodeString Value);
	PFunctionInfo __fastcall GetItems(int Index);
	void __fastcall SetItems(int Index, const PFunctionInfo Value);
	
public:
	__fastcall virtual TFunctionRegistry(void);
	__fastcall virtual ~TFunctionRegistry(void);
	void __fastcall Clear(void);
	void __fastcall Add(int FunctionID, void * Proc, Gr32_system::TCPUFeatures CPUFeatures = Gr32_system::TCPUFeatures() , int Flags = 0x0);
	void __fastcall RegisterBinding(int FunctionID, System::PPointer BindVariable);
	void __fastcall RebindAll(TFunctionPriority PriorityCallback = 0x0);
	void __fastcall Rebind(int FunctionID, TFunctionPriority PriorityCallback = 0x0);
	void * __fastcall FindFunction(int FunctionID, TFunctionPriority PriorityCallback = 0x0);
	__property PFunctionInfo Items[int Index] = {read=GetItems, write=SetItems};
	
__published:
	__property System::UnicodeString Name = {read=FName, write=SetName};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TFunctionPriority DefaultPriority;
extern PACKAGE int INVALID_PRIORITY;
extern PACKAGE TFunctionRegistry* __fastcall NewRegistry(const System::UnicodeString Name = L"");
extern PACKAGE int __fastcall DefaultPriorityProc(PFunctionInfo Info);

}	/* namespace Gr32_bindings */
using namespace Gr32_bindings;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_bindingsHPP
