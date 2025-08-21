//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include <values.h>

#include "CGMComponent.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCGMComponent *)
{
   new TCGMComponent(NULL);
}
//---------------------------------------------------------------------------
__fastcall TCGMComponent::TCGMComponent(TComponent* Owner)
   : TComponent(Owner)
{
   FSegmentLength = 100;
   FAutoScale = false;
}
//---------------------------------------------------------------------------
namespace Cgmcomponent
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TCGMComponent)};
		 RegisterComponents("Gotham Scientific", classes, 0);
   }
}

//---------------------------------------------------------------------------

bool __fastcall TCGMComponent::BeginCGM(
                        short *data, unsigned long numpoints, unsigned long adcbits,
                        wchar_t *units, double samplerate, unsigned long increment,
                        double scalefactor, double offset, FILE *outf)
{
   double ymin = MAXFLOAT, ymax = -MAXFLOAT, *fdata = new double[numpoints];
   double *temp = fdata;
   for (unsigned long i=0; i<numpoints; i++){
      *temp = (*data)*scalefactor + offset;
      if (ymin > *temp) ymin = *temp;
      if (ymax < *temp) ymax = *temp;
      temp++;
      data++;
   }

   if (!FAutoScale){
      ymax = adcbits/2 * scalefactor + offset;
      ymin = adcbits/2;
      ymin *= -1*scalefactor;
      ymin += offset;
   }

   bool result =  BeginCGM(fdata, numpoints, ymax, ymin, units, samplerate,
                            increment, outf);
   delete[] fdata;
   return result;
}
//---------------------------------------------------------------------------

bool __fastcall TCGMComponent::BeginCGM(
                        double *data, unsigned long numpoints, double ymax,
                        double ymin, wchar_t *units, double samplerate,
                        unsigned long increment, FILE *outf)
{
   OutputFile = outf;

   String value = L"";
	double yrange = ymax - ymin;
	YScaler = 200 / yrange;

	FAbsoluteOffset = -1.0*ymin+10;

	ymin -= ymin;
	ymax -= ymin;

   ymin *= YScaler;
	ymax *= YScaler;
   if (increment < 1) increment = 1;
	XScaler = 1000.0/numpoints;

	CGMText =
L"BEGMF 'Gotham Scientific CGM Clear Text Encoded';%Begin Metafile%\n"
L"MFVERSION 1; %Defines the Version of CGM, 1 is the smallest%\n"
L"MFDESC 'Data exported from CGM Component of gothamsci.bpl'; %Drawn by gothamsci.bpl%\n"
L"VDCTYPE REAL; %Real values will be used to define the picture%\n"
L"INTEGERPREC -32768, 32767; %Range for integers%\n"
L"REALPREC -8191.0, 8191.0, 8;  %A default exponent of 8%\n"
L"COLRPREC 255; %Colour values in the range 0 to 255%\n\n"

L"MFELEMLIST 'DRAWINGPLUS'; %Defines the set of elements that may appear%\n"
L"                         %in the Metafile%\n"
L"BEGPIC 'Gotham Scientific CGM';%Begin Picture%\n";

//	CGMText.cat_sprintf(L"VDCEXT (0.0,%7.4f) (1200.0,%7.4f);%Min and Max of Coordinates%\n", ymin, ymax);

	CGMText.cat_sprintf(L"VDCEXT (0.0,0.0) (1200.0,2000); %Min and Max of Coordinates%\n");

   CGMText +=
L"COLRMODE DIRECT; %Colour Specified Directly%\n"
L"LINEWIDTHMODE SCALED;\n"
L"BEGPICBODY;%Start of Drawing%\n"
L"LINECOLR 0 0 0;\n";

//draw the scale bars
   double yspan = 1;
   do {yspan *= 10.0;} while (yspan <= MAXFLOAT/10);
   while (yrange < yspan){
      yspan /= 10.0;          // this is the value to use for drawing bar
   }
   double ymidpoint = (ymax - ymin)/2 + ymin;
   double ybarhalflength = yspan * YScaler / 2 ;
   CGMText +=
L"\nLINEWIDTH 5; %Draw the Scale Bars%\n\n";
	value.cat_sprintf(L"LINE (1030.00, %7.2f) (1030.00, %7.2f);\n",
                        ymidpoint - ybarhalflength,
                        ymidpoint + ybarhalflength);
	CGMText += value;

   value =
L"TextFontIndex 1;\n"
L"TextPrec STROKE;\n"
L"CharExpan 0.8;\n"
L"CharSpace 0.01;\n"
L"TextColr 0 0 0;\n"
L"CharHeight 40;\n"
L"CharOri 0,1,1,0;\n"
L"TextPath RIGHT;\n";
	value.cat_sprintf(L"Text 1020,%7.2f,final,\"%7.2f %s\";\n", ymidpoint + ybarhalflength + 10, yspan, units);
   CGMText += value;

   double time = samplerate*numpoints;
   double timespan = 1000000.0;
   unsigned long secscale = 1000000;
   unsigned long msscale =  1000000000;
   while (time < timespan){
      timespan /= 10.0;
      secscale /= 10;
      msscale /= 10;
   };
   double xbarlength = (timespan/samplerate) * XScaler;
   if (xbarlength > 200){
      xbarlength /= 10.0;
      timespan /= 10.0;
   }
   value = "";
	value.cat_sprintf(L"LINE (1030.00, %7.4f) (%7.3f, %7.4f);\n",
							ymidpoint - ybarhalflength, 1030+xbarlength,
                     ymidpoint - ybarhalflength);

   if (secscale >= 1){
		units = L"s";
		value.cat_sprintf(L"Text 1040,%7.4f,final,\"%7.2f %s\";\n",
                     ymidpoint - ybarhalflength - 50, timespan, units);
   }
   else if (msscale >= 1){
      units = L"ms";
		value.cat_sprintf(L"Text 1040,%7.4f,final,\"%7.2f %s\";\n",
							ymidpoint - ybarhalflength - 10, 1000*timespan, units);
   }
	CGMText += value;

	return AddPlot(data, numpoints, increment, L"0 0 0");
}
//---------------------------------------------------------------------------

bool __fastcall TCGMComponent::AddPlot(short *data, unsigned long numpoints,
                           unsigned long increment, double scalefactor,
                           double offset, String plotcolor, int plotwidth ,
                           unsigned long xoffset)
{
   double *fdata = new double[numpoints];
   double *temp = fdata;
   for (unsigned long i=0; i<numpoints; i++){
      *temp++ = (*data++)*scalefactor + offset;
   }
   bool result = AddPlot(fdata, numpoints, increment, plotcolor, offset, plotwidth, xoffset);
   delete[] fdata;
   return result;
}
//---------------------------------------------------------------------------

bool __fastcall TCGMComponent::AddPlot(double *data, unsigned long numpoints,
                           unsigned long increment, String plotcolor,
                           double yoffset, int plotwidth, unsigned long xoffset)
{
   unsigned long point = xoffset;
	double progress = 0;
   double stemp;
   String value = "";

   CGMText.cat_sprintf(L"LINEWIDTH %d;\n", plotwidth);
   CGMText.cat_sprintf(L"LINECOLR %s;\n\n", plotcolor.c_str());

   // now start to draw some lines

   if (increment < 1) increment = 1;

   yoffset += FAbsoluteOffset;

	while (point < numpoints){
		stemp = (data[point]+yoffset)*YScaler;
      value.cat_sprintf(L"LINE (%7.4f, %7.4f)", progress, stemp);
      unsigned long i;
      for (i = 1; i<FSegmentLength; i++){
         progress += increment * XScaler;
         point+=increment;
         if (point >= numpoints) break;
			stemp = (data[point]+yoffset)*YScaler;
         value.cat_sprintf(L" (%7.4f, %7.4f)", progress, stemp);
      }
      value.cat_sprintf(L";\n");
      CGMText += value;
      value = L"";
	}
   return true;
}
//---------------------------------------------------------------------------

bool __fastcall TCGMComponent::WriteFile()
{
   //close the pic and file
   CGMText +=
"ENDPIC;\n"
"ENDMF;\n";

   return fputws(CGMText.c_str(), OutputFile);
}
//---------------------------------------------------------------------------
