//---------------------------------------------------------------------------

#ifndef comp2strH
#define comp2strH
//---------------------------------------------------------------------------

#include <Classes.hpp>

class PACKAGE CompressedData
{
public:
   unsigned long OriginalSize; //bytes
   unsigned long CompressedSize; //bytes
   unsigned long EncodedSize; //bytes
	AnsiString    CompressedData;
};

extern PACKAGE CompressedData DataToString(PBYTE data, unsigned long numbytes);

// data must be sized compdata.OriginalSize bytes.
extern PACKAGE void StringToData(CompressedData compdata, PBYTE data);
//---------------------------------------------------------------------------
#endif
