// CodeGear C++Builder
// Copyright (c) 1995, 2015 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'UAFDefs.pas' rev: 30.00 (Windows)

#ifndef UafdefsHPP
#define UafdefsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>

//-- user supplied -----------------------------------------------------------

namespace Uafdefs
{
//-- forward type declarations -----------------------------------------------
struct UAF_File;
//-- type declarations -------------------------------------------------------
enum DECLSPEC_DENUM UAF_FILE_TYPE : unsigned char { UAF_TYPE_UNKNOWN, UAF_TYPE_ADF, UAF_TYPE_WAV, UAF_TYPE_AU, UAF_TYPE_AIFF, UAF_TYPE_RAW };

struct DECLSPEC_DRECORD UAF_File
{
public:
	double FrameRate;
	System::Word Channels;
	System::Word Quantization;
	int Frames;
	System::Word FrameSize;
	void *FileRecord;
	UAF_FILE_TYPE FileType;
	int SubType;
	System::Word FrameSizeIn;
};


//-- var, const, procedure ---------------------------------------------------
extern "C" bool __fastcall UAF_open(UAF_File &uaf, System::UnicodeString fname, System::WideChar mode, UAF_FILE_TYPE ForceType);
extern "C" bool __fastcall UAF_create(UAF_File &uaf, System::UnicodeString fname, UAF_FILE_TYPE ftype, System::Word subformat, double srate, System::Word nchannels, System::Word bits);
extern "C" bool __fastcall UAF_close(UAF_File &uaf);
extern "C" int __fastcall UAF_read(UAF_File &uaf, void * buffer, int nFrames, int lpos);
extern "C" int __fastcall UAF_write(UAF_File &uaf, void * buffer, int nFrames, int lpos);
extern "C" bool __fastcall UAF_CreateFromUAF(UAF_File &uafin, UAF_File &uafout, System::UnicodeString fname);
extern "C" bool __fastcall UAF_SaveSection(UAF_File &uafin, UAF_File &uafout, int lstart, int lend);
extern "C" void __fastcall UAF_Copy_Marks(UAF_File &uafin, UAF_File &uafout);
extern "C" bool __fastcall FindMinMaxUAF(System::UnicodeString FileIn, double &Min, double &Max, UAF_FILE_TYPE OverrideType);
extern "C" UAF_FILE_TYPE __fastcall UAFTypeFromExtension(System::UnicodeString fname);
extern "C" System::WideChar * __fastcall UAF_ErrorMessage(void);
extern "C" System::WideChar * __fastcall UAF_Identity(const UAF_File &UAFIn);
extern "C" System::WideChar * __fastcall UAF_Description(const UAF_File &UAFIn);
}	/* namespace Uafdefs */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_UAFDEFS)
using namespace Uafdefs;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// UafdefsHPP
