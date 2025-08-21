//---------------------------------------------------------------------------


#pragma hdrstop

#include "comp2str.h"

#include "Base64Coder.h"
#include "huffman.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CompressedData DataToString(PBYTE data, unsigned long numbytes)
{
   Base64Coder		coder;
   CompressedData compdata;

   unsigned char *compbuf = new unsigned char[numbytes*2 + 384];
   compdata.OriginalSize = numbytes;
   compdata.CompressedSize = Huffman_Compress(data, compbuf, numbytes);

   compdata.EncodedSize = coder.Encode(compbuf, compdata.CompressedSize);
   compdata.CompressedData = coder.EncodedMessage();

   return compdata;
}

void StringToData(CompressedData compdata, PBYTE data)
{
   Base64Coder		decoder;
   decoder.Decode(compdata.CompressedData.c_str());
   Huffman_Uncompress((PBYTE)decoder.DecodedMessage(), data, compdata.CompressedSize, compdata.OriginalSize);
}
