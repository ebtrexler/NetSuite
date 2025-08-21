//---------------------------------------------------------------------------

#ifndef CGMComponentH
#define CGMComponentH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class PACKAGE TCGMComponent : public TComponent
{
private:
   unsigned long  FSegmentLength;
   bool           FAutoScale;
	double         XScaler, YScaler;
	double			FAbsoluteOffset;
protected:
   UnicodeString  CGMText;
   FILE           *OutputFile;
public:
   __fastcall TCGMComponent(TComponent* Owner);
   // overloaded BeginCGM functions --
   // require FILE* pointer -- must be opened and closed in calling function
   // *data pointer begins the first plot -- zero xoffset assumed for now
   bool __fastcall BeginCGM(short *data, unsigned long numpoints, unsigned long adcbits,
                        wchar_t *units, double samplerate, unsigned long increment,
                        double scalefactor, double offset, FILE *outf);
   bool __fastcall BeginCGM(double *data, unsigned long numpoints, double ymax,
                        double ymin, wchar_t *units, double samplerate,
                        unsigned long increment, FILE *outf);
   // overloaded AddPlot functions --
   // must be of the same y-scaling and samplerate as the BeginCGM plot
   // xoffset in terms of points/samples, not time
   bool __fastcall AddPlot(short *data, unsigned long numpoints,
                           unsigned long increment, double scalefactor,
                           double offset, String plotcolor, int plotwidth = 1,
                           unsigned long xoffset = 0);
   bool __fastcall AddPlot(double *data, unsigned long numpoints,
                           unsigned long increment, String plotcolor,
                           double yoffset = 0.0,
                           int plotwidth = 1, unsigned long xoffset = 0);
   bool __fastcall WriteFile();
__published:
   __property unsigned long SegmentLength =
         {read = FSegmentLength, write = FSegmentLength, default = 100};
   __property bool AutoScale =
         {read = FAutoScale, write = FAutoScale, default = false};
};
//---------------------------------------------------------------------------
#endif
 