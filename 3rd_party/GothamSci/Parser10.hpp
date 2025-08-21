// CodeGear C++Builder
// Copyright (c) 1995, 2015 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Parser10.pas' rev: 30.00 (Windows)

#ifndef Parser10HPP
#define Parser10HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>

//-- user supplied -----------------------------------------------------------

namespace Parser10
{
//-- forward type declarations -----------------------------------------------
struct TOperation;
class DELPHICLASS EMathParserError;
class DELPHICLASS ESyntaxError;
class DELPHICLASS EExpressionHasBlanks;
class DELPHICLASS EExpressionTooComplex;
class DELPHICLASS ETooManyNestings;
class DELPHICLASS EMissMatchingBracket;
class DELPHICLASS EBadName;
class DELPHICLASS EParserInternalError;
class DELPHICLASS TCustomParser;
class DELPHICLASS TFParser;
class DELPHICLASS TFParserASM;
//-- type declarations -------------------------------------------------------
typedef double *PParserFloat;

typedef double ParserFloat;

enum DECLSPEC_DENUM TToken : unsigned char { variab, constant, minus, sum, diff, prod, divis, modulo, IntDiv, integerpower, realpower, square, third, fourth, FuncOneVar, FuncTwoVar };

typedef TOperation *POperation;

typedef void __fastcall (*TMathProcedure)(POperation AnOperation);

struct DECLSPEC_DRECORD TOperation
{
public:
	double *Arg1;
	double *Arg2;
	double *Dest;
	TOperation *NextOperation;
	TMathProcedure MathProc;
	TToken Token;
};


#pragma pack(push,4)
class PASCALIMPLEMENTATION EMathParserError : public System::Sysutils::Exception
{
	typedef System::Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall EMathParserError(const System::UnicodeString Msg) : System::Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EMathParserError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : System::Sysutils::Exception(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EMathParserError(NativeUInt Ident)/* overload */ : System::Sysutils::Exception(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EMathParserError(System::PResStringRec ResStringRec)/* overload */ : System::Sysutils::Exception(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EMathParserError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EMathParserError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EMathParserError(const System::UnicodeString Msg, int AHelpContext) : System::Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EMathParserError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : System::Sysutils::Exception(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EMathParserError(NativeUInt Ident, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EMathParserError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EMathParserError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EMathParserError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EMathParserError(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION ESyntaxError : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall ESyntaxError(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ESyntaxError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall ESyntaxError(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall ESyntaxError(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall ESyntaxError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall ESyntaxError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall ESyntaxError(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ESyntaxError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ESyntaxError(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ESyntaxError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ESyntaxError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ESyntaxError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ESyntaxError(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION EExpressionHasBlanks : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall EExpressionHasBlanks(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EExpressionHasBlanks(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EExpressionHasBlanks(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EExpressionHasBlanks(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EExpressionHasBlanks(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EExpressionHasBlanks(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EExpressionHasBlanks(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EExpressionHasBlanks(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EExpressionHasBlanks(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EExpressionHasBlanks(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EExpressionHasBlanks(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EExpressionHasBlanks(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EExpressionHasBlanks(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION EExpressionTooComplex : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall EExpressionTooComplex(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EExpressionTooComplex(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EExpressionTooComplex(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EExpressionTooComplex(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EExpressionTooComplex(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EExpressionTooComplex(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EExpressionTooComplex(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EExpressionTooComplex(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EExpressionTooComplex(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EExpressionTooComplex(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EExpressionTooComplex(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EExpressionTooComplex(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EExpressionTooComplex(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION ETooManyNestings : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall ETooManyNestings(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall ETooManyNestings(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall ETooManyNestings(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall ETooManyNestings(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall ETooManyNestings(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall ETooManyNestings(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall ETooManyNestings(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall ETooManyNestings(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ETooManyNestings(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall ETooManyNestings(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ETooManyNestings(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall ETooManyNestings(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~ETooManyNestings(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION EMissMatchingBracket : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall EMissMatchingBracket(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EMissMatchingBracket(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EMissMatchingBracket(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EMissMatchingBracket(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EMissMatchingBracket(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EMissMatchingBracket(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EMissMatchingBracket(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EMissMatchingBracket(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EMissMatchingBracket(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EMissMatchingBracket(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EMissMatchingBracket(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EMissMatchingBracket(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EMissMatchingBracket(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION EBadName : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall EBadName(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EBadName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EBadName(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EBadName(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EBadName(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EBadName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EBadName(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EBadName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBadName(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBadName(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBadName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBadName(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EBadName(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION EParserInternalError : public EMathParserError
{
	typedef EMathParserError inherited;
	
public:
	/* Exception.Create */ inline __fastcall EParserInternalError(const System::UnicodeString Msg) : EMathParserError(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EParserInternalError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High) : EMathParserError(Msg, Args, Args_High) { }
	/* Exception.CreateRes */ inline __fastcall EParserInternalError(NativeUInt Ident)/* overload */ : EMathParserError(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EParserInternalError(System::PResStringRec ResStringRec)/* overload */ : EMathParserError(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EParserInternalError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(Ident, Args, Args_High) { }
	/* Exception.CreateResFmt */ inline __fastcall EParserInternalError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High)/* overload */ : EMathParserError(ResStringRec, Args, Args_High) { }
	/* Exception.CreateHelp */ inline __fastcall EParserInternalError(const System::UnicodeString Msg, int AHelpContext) : EMathParserError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EParserInternalError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_High, int AHelpContext) : EMathParserError(Msg, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EParserInternalError(NativeUInt Ident, int AHelpContext)/* overload */ : EMathParserError(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EParserInternalError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EParserInternalError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(ResStringRec, Args, Args_High, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EParserInternalError(NativeUInt Ident, System::TVarRec const *Args, const int Args_High, int AHelpContext)/* overload */ : EMathParserError(Ident, Args, Args_High, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EParserInternalError(void) { }
	
};

#pragma pack(pop)

typedef void __fastcall (__closure *TParserExceptionEvent)(System::TObject* Sender, System::Sysutils::Exception* E);

class PASCALIMPLEMENTATION TCustomParser : public System::Classes::TComponent
{
	typedef System::Classes::TComponent inherited;
	
private:
	System::UnicodeString FExpression;
	bool FPascalNumberformat;
	bool FParserError;
	System::Classes::TStringList* FVariables;
	TOperation *FStartOperationList;
	TParserExceptionEvent FOnParserError;
	System::Classes::TStringList* FFunctionOne;
	System::Classes::TStringList* FFunctionTwo;
	virtual System::Extended __fastcall GetValue(void);
	void __fastcall SetExpression(const System::UnicodeString AnExpression);
	void __fastcall SetVar(const System::UnicodeString VarName, const System::Extended Value);
	void __fastcall SetValue(const System::Extended Value);
	bool __fastcall ParseExpression(void);
	virtual void __fastcall FreeExpression(void);
	System::Extended __fastcall GetVariable(const System::UnicodeString VarName);
	void __fastcall DisposeVariableFloatAt(int Index);
	int __fastcall ConnectMandatoryOperations(void);
	virtual void __fastcall GenerateDynamicCode(int OperationCount);
	
protected:
	void __fastcall DefineBasicFunctions(void);
	__property System::Classes::TStringList* FunctionOne = {read=FFunctionOne};
	__property System::Classes::TStringList* FunctionTwo = {read=FFunctionTwo};
	__property POperation LinkedOperationList = {read=FStartOperationList};
	
public:
	__fastcall virtual TCustomParser(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomParser(void);
	void __fastcall AddFunctionOneParam(const System::UnicodeString AFunctionName, const TMathProcedure Func);
	void __fastcall AddFunctionTwoParam(const System::UnicodeString AFunctionName, const TMathProcedure Func);
	void __fastcall ClearFunctions(void);
	void __fastcall ClearFunction(const System::UnicodeString AFunctionName);
	void __fastcall ClearVariables(void);
	void __fastcall ClearVariable(const System::UnicodeString AVarName);
	bool __fastcall VariableExists(const System::UnicodeString AVarName);
	PParserFloat __fastcall SetVariable(System::UnicodeString VarName, const System::Extended Value);
	__property System::Extended Variable[const System::UnicodeString VarName] = {read=GetVariable, write=SetVar};
	__property bool ParserError = {read=FParserError, nodefault};
	
__published:
	__property System::Extended Value = {read=GetValue, write=SetValue, stored=false};
	__property System::UnicodeString Expression = {read=FExpression, write=SetExpression};
	__property bool PascalNumberformat = {read=FPascalNumberformat, write=FPascalNumberformat, default=1};
	__property TParserExceptionEvent OnParserError = {read=FOnParserError, write=FOnParserError};
};


class PASCALIMPLEMENTATION TFParser : public TCustomParser
{
	typedef TCustomParser inherited;
	
private:
	double FA;
	double FB;
	double FC;
	double FD;
	double FE;
	double FX;
	double FY;
	double FT;
	
public:
	__fastcall virtual TFParser(System::Classes::TComponent* AOwner);
	
__published:
	__property double A = {read=FA, write=FA};
	__property double B = {read=FB, write=FB};
	__property double C = {read=FC, write=FC};
	__property double D = {read=FD, write=FD};
	__property double E = {read=FE, write=FE};
	__property double T = {read=FT, write=FT};
	__property double X = {read=FX, write=FX};
	__property double Y = {read=FY, write=FY};
public:
	/* TCustomParser.Destroy */ inline __fastcall virtual ~TFParser(void) { }
	
};


class PASCALIMPLEMENTATION TFParserASM : public TCustomParser
{
	typedef TCustomParser inherited;
	
private:
	double FA;
	double FB;
	double FC;
	double FD;
	double FE;
	double FX;
	double FY;
	double FT;
	void *DynamicCode;
	virtual System::Extended __fastcall GetValue(void);
	virtual void __fastcall FreeExpression(void);
	virtual void __fastcall GenerateDynamicCode(int OperationCount);
	
public:
	__fastcall virtual TFParserASM(System::Classes::TComponent* AOwner);
	
__published:
	__property double A = {read=FA, write=FA};
	__property double B = {read=FB, write=FB};
	__property double C = {read=FC, write=FC};
	__property double D = {read=FD, write=FD};
	__property double E = {read=FE, write=FE};
	__property double T = {read=FT, write=FT};
	__property double X = {read=FX, write=FX};
	__property double Y = {read=FY, write=FY};
public:
	/* TCustomParser.Destroy */ inline __fastcall virtual ~TFParserASM(void) { }
	
};


//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall Register(void);
}	/* namespace Parser10 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_PARSER10)
using namespace Parser10;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Parser10HPP
