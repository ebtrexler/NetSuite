// CodeGear C++Builder
// Copyright (c) 1995, 2013 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'VersionInfo.pas' rev: 25.00 (Windows)

#ifndef VersioninfoHPP
#define VersioninfoHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Versioninfo
{
//-- type declarations -------------------------------------------------------
enum DECLSPEC_DENUM Versioninfo__1 : unsigned char { maDebugBuild, maPreRelease, maPatched, maPrivateBuild, maInfoInferred, maSpecialBuild };

typedef System::Set<Versioninfo__1, Versioninfo__1::maDebugBuild, Versioninfo__1::maSpecialBuild>  TModuleAttributes;

class DELPHICLASS TVersionInfo;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TVersionInfo : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	unsigned FLocaleIndex;
	unsigned FLangID;
	void *FVerInfo;
	unsigned FVerInfoSize;
	tagVS_FIXEDFILEINFO *FVerValue;
	System::UnicodeString __fastcall GetPropertyString(int Index);
	System::Word __fastcall GetVersionNumber(int Index);
	TModuleAttributes __fastcall GetModuleAttributes(void);
	System::UnicodeString __fastcall GetLangString(unsigned LangID);
	unsigned __fastcall GetLocaleInfoCount(void);
	void __fastcall SetLocalIndex(const unsigned Value);
	
public:
	__fastcall TVersionInfo(System::UnicodeString FileName);
	__fastcall virtual ~TVersionInfo(void);
	System::UnicodeString __fastcall GetVersionString(System::UnicodeString Value);
	unsigned __fastcall GetLocaleInfo(unsigned Index);
	__property unsigned LocaleInfoCount = {read=GetLocaleInfoCount, nodefault};
	__property unsigned LocaleIndex = {read=FLocaleIndex, write=SetLocalIndex, nodefault};
	__property System::UnicodeString Language = {read=GetPropertyString, index=0};
	__property System::UnicodeString CompanyName = {read=GetPropertyString, index=1};
	__property System::UnicodeString FileDescription = {read=GetPropertyString, index=2};
	__property System::UnicodeString FileVersion = {read=GetPropertyString, index=3};
	__property System::UnicodeString InternalName = {read=GetPropertyString, index=4};
	__property System::UnicodeString LegalCopyright = {read=GetPropertyString, index=5};
	__property System::UnicodeString LegalTrademarks = {read=GetPropertyString, index=6};
	__property System::UnicodeString OriginalFileName = {read=GetPropertyString, index=7};
	__property System::UnicodeString ProductName = {read=GetPropertyString, index=8};
	__property System::UnicodeString ProductVersion = {read=GetPropertyString, index=9};
	__property System::UnicodeString Comments = {read=GetPropertyString, index=10};
	__property System::UnicodeString PrivateBuild = {read=GetPropertyString, index=11};
	__property System::UnicodeString SpecialBuild = {read=GetPropertyString, index=12};
	__property System::Word MajorVersion = {read=GetVersionNumber, index=0, nodefault};
	__property System::Word MinorVersion = {read=GetVersionNumber, index=1, nodefault};
	__property System::Word ReleaseVersion = {read=GetVersionNumber, index=2, nodefault};
	__property System::Word Build = {read=GetVersionNumber, index=3, nodefault};
	__property TModuleAttributes ModuleAttributes = {read=GetModuleAttributes, nodefault};
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
}	/* namespace Versioninfo */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_VERSIONINFO)
using namespace Versioninfo;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// VersioninfoHPP
