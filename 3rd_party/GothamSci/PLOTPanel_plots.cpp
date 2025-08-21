//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <values.h>
#include <math.h>

#include "tools.h"
#include "GR32_Blend.hpp"
#include "PLOTPanel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                    TPLOTPanel Static plotting functions
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------

bool __fastcall TPLOTPanel::AddPlot(TStaticPlot *plot, bool flush)
{
   TStaticPlot **temp;
   try{
      temp = new TStaticPlot*[NumPlots + 1];
   }
   catch(...){
      return false;
   }
   for (unsigned long i = 0; i<NumPlots; i++){
      temp[i] = Plots[i];
   }
   temp[NumPlots] = plot;
   if (NumPlots > 0) delete[] Plots;
   Plots = temp;
   NumPlots++;

   plot->Visible = true;
   if (Visible){
      plot->DrawToDAQPanel();
      if (flush) Graph->Flush();
   }
   return true;
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::ClearPlots()
{
   for (unsigned long i = 0; i<NumPlots; i++){
      delete Plots[i];
   }
   if (NumPlots > 0) delete[] Plots;
   NumPlots = 0;
   Graph->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::ScalePlots(bool flush)
{
   if (NumPlots > 0){
      FFullScaleMinX = (MAXFLOAT);
      FFullScaleMaxX = -(MAXFLOAT);
      FFullScaleMinY = (MAXFLOAT);
      FFullScaleMaxY = -(MAXFLOAT);
      double minX, maxX;
      for (unsigned long i = 0; i<NumPlots; i++){
         if (Plots[i]->Visible){
            Plots[i]->FixupXandYLimits(Plots[i]->FSMinX);
            minX =
               FConcatenatePlots ? Plots[i]->FSMinX : 0.0;
            maxX =
               FConcatenatePlots ? Plots[i]->FSMaxX : Plots[i]->FSMaxX - Plots[i]->FSMinX;
            if (FFullScaleMinX > minX) FFullScaleMinX = minX;
            if (FFullScaleMaxX < maxX) FFullScaleMaxX = maxX;
            if (FFullScaleMinY > Plots[i]->FSMinY) FFullScaleMinY = Plots[i]->FSMinY;
            if (FFullScaleMaxY < Plots[i]->FSMaxY) FFullScaleMaxY = Plots[i]->FSMaxY;
         }
      }
      FFullScaleMinY -= fabs(0.01*(FFullScaleMaxY - FFullScaleMinY));
      FFullScaleMaxY += fabs(0.01*(FFullScaleMaxY - FFullScaleMinY));
      NumStoredViewports = 0;
      FVisibleMinX = FFullScaleMinX;
      FVisibleMinY = FFullScaleMinY;
      FVisibleMaxX = FFullScaleMaxX;
      FVisibleMaxY = FFullScaleMaxY;
   }
   Draw(flush);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                        TDAQSampleBuffer functions
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TDAQSampleBuffer::TDAQSampleBuffer()
{
   DeleteMemory = false;
   FSMaxY = 0; FSMinY = 0; UUperBit = 0; OffsetUU = 0;
   SampleSpacing = 0; Samples = NULL; NumSamples = 0;
   IsFull = false; Next = NULL;
}
//---------------------------------------------------------------------------

TDAQSampleBuffer::TDAQSampleBuffer(unsigned long numsamples)
{
   DeleteMemory = false;
   FSMaxY = 0; FSMinY = 0; UUperBit = 0; OffsetUU = 0;
   SampleSpacing = 0; Samples = NULL; NumSamples = 0;
   IsFull = false; Next = NULL;
   NumSamples = numsamples;
   if (NumSamples != 0){
      Samples = new short[NumSamples];
      memset(Samples, 0, NumSamples*sizeof(short));
      DeleteMemory = true;
   }
}
//---------------------------------------------------------------------------

TDAQSampleBuffer::~TDAQSampleBuffer()
{
   if (DeleteMemory) delete[] Samples;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                     TStaticPlot constructor
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TStaticPlot::TStaticPlot(TPLOTPanel *parent,
						 int which, String label,
						 TColor color,
						 double fsxmin, double fsxmax,
						 double fsymin, double fsymax):
						 Parent(parent),
						 Which(which), Label(label), Color(color),
						 FSMinX(fsxmin), FSMaxX(fsxmax),
						 FSMinY(fsymin), FSMaxY(fsymax)
{
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                     TStaticPlot descendant functions
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//TStaticDAQPlot

TStaticDAQPlot::TStaticDAQPlot(TPLOTPanel *parent,
                           int which, String label,
                           TColor color, double fsxmin,
                           unsigned long numdaqbuffers,
                           TDAQSampleBuffer *firstdaqbuffer) :
                           TStaticPlot(parent, which, label, color, fsxmin),
                           NumDAQBuffers(numdaqbuffers)
{
   DAQBuffers = new TDAQSampleBuffer*[NumDAQBuffers];
   DAQBuffers[0] = firstdaqbuffer;
   FixupXandYLimits(fsxmin);
}
//---------------------------------------------------------------------------

TStaticDAQPlot::~TStaticDAQPlot()
{
   if (NumDAQBuffers > 0) delete[] DAQBuffers;
}
//---------------------------------------------------------------------------

bool TStaticDAQPlot::DrawToDAQPanel()
{
   if (!Visible) return false;
   if (NumDAQBuffers < 1) return false;

   double minX = Parent->ConcatenatePlots ? FSMinX : 0.0;
   double maxX = Parent->ConcatenatePlots ? FSMaxX : FSMaxX - FSMinX;

   if (minX >= Parent->VisibleMaxX) return false;
   if (maxX <= Parent->VisibleMinX) return false;

   unsigned long xstart; // index of data point in buffer
   unsigned long index; // loop counter and first buffer to plot
   short *p;
   double XProgress;
   double YSampleLast;
   double plotstarttime;
   double time = 0.0;
   double temp = 0.0;

   if (minX == Parent->VisibleMinX){
      index = 0;
      xstart = 0;
      XProgress = Parent->VisibleMinX;
   }
   else if (minX < Parent->VisibleMinX){
   // find start index in appropriate buffer
      plotstarttime = Parent->VisibleMinX - minX;
      for (index = 0; index<NumDAQBuffers; index++){
         temp += DAQBuffers[index]->NumSamples * DAQBuffers[index]->SampleSpacing;
         if (plotstarttime < temp) break;
         time += DAQBuffers[index]->NumSamples * DAQBuffers[index]->SampleSpacing;
      }
      temp = (plotstarttime - time);
      xstart = round(temp/DAQBuffers[index]->SampleSpacing);
      if (xstart > 0) xstart--;
      XProgress = minX + time + xstart * DAQBuffers[index]->SampleSpacing;
   }
   else if (minX < Parent->VisibleMaxX){
      index = 0;
      xstart = 0;
      XProgress = minX;
   }
   else {
      return false;
   }

   double TotalXTime = Parent->VisibleMaxX - Parent->VisibleMinX;
   double samplesperpixel = TotalXTime;
   samplesperpixel /= DAQBuffers[index]->SampleSpacing;   // number of samples
   samplesperpixel /= Parent->Graph->Width;             // samples per pixel
   samplesperpixel /= 10;                 // no more than 10 samples per pixel

   Increment = round(samplesperpixel);
   if (Increment < 1) Increment = 1;

   XProgress += Increment * DAQBuffers[index]->SampleSpacing;

   p = DAQBuffers[index]->Samples;
   YSampleLast = (p[xstart])*(DAQBuffers[index]->UUperBit) +
                        DAQBuffers[index]->OffsetUU;
   Parent->BeginUpdate();
   Parent->Graph->Buffer->BeginUpdate();
   Parent->Graph->Buffer->PenColor = Color32(Color);

   Parent->Graph->Buffer->MoveToF(
               (XProgress - Parent->VisibleMinX) * Parent->PixelsPerXUnit,
               Parent->ZeroYPixels - YSampleLast * Parent->PixelsPerYUnit);

   xstart += Increment;
   do {
      for (unsigned long i = xstart;
            i < (DAQBuffers[index]->NumSamples); i+=Increment){
         if (XProgress >= Parent->VisibleMaxX) break;
         YSampleLast =
            (p[i])*(DAQBuffers[index]->UUperBit) + DAQBuffers[index]->OffsetUU;
         XProgress += (Increment * DAQBuffers[index]->SampleSpacing);
         Parent->Graph->Buffer->LineToFS(
               (XProgress - Parent->VisibleMinX) * Parent->PixelsPerXUnit,
               Parent->ZeroYPixels - YSampleLast * Parent->PixelsPerYUnit);
      };
      if (XProgress >= Parent->VisibleMaxX) break;
      xstart = 0;
      index++;
      if (index >= NumDAQBuffers) break;
      p = DAQBuffers[index]->Samples;
   } while (XProgress < Parent->VisibleMaxX);

   Parent->Graph->Buffer->EndUpdate();
   Parent->EndUpdate();

   return true;
}
//---------------------------------------------------------------------------

bool TStaticDAQPlot::HitTest(int X, int Y)
{
   bool IsAHit = false;

   // values in user units
   double x = X;
   x /= Parent->PixelsPerXUnit;  // pixels to x units relative to graph xmin
   x += Parent->VisibleMinX;     // convert to absolute x units

   // start and stop of each buffer in time
   double minX = Parent->ConcatenatePlots ? FSMinX : 0.0;
   double maxX = Parent->ConcatenatePlots ? FSMaxX : FSMaxX - FSMinX;

   // hittest variables
   double time = minX, temp = minX, yval;
   unsigned long xpos;
   unsigned long index;
   int ypix;

   if (x <= maxX && x >= minX){
      for (index=0; index<NumDAQBuffers; index++){
         temp += DAQBuffers[index]->NumSamples *
                  DAQBuffers[index]->SampleSpacing;
         if (x < temp) break;  // current index gives appropriate DAQBuffer
         time += DAQBuffers[index]->NumSamples *
                  DAQBuffers[index]->SampleSpacing;
      }
      temp = x - time;
      xpos = round(temp / DAQBuffers[index]->SampleSpacing); //current sample
                                                      // in DAQBuffers[index]
      yval = DAQBuffers[index]->Samples[xpos]*(DAQBuffers[index]->UUperBit) +
               DAQBuffers[index]->OffsetUU;
      ypix = Parent->ZeroYPixels - yval * Parent->PixelsPerYUnit;

      if (abs(ypix - Y) < 7) IsAHit = true;

      // if slope of plot is high, need to test adjacent samples,
      // within 3x3 pixels
      if (!IsAHit){
         time = (temp - 3.0/Parent->PixelsPerXUnit) /   // 3 pix to left
                                       DAQBuffers[index]->SampleSpacing;
         if (time < 0.0) time = 0.0;
         unsigned long start = round(time);
         time = (temp + 3.0/Parent->PixelsPerXUnit) /  // 3 pix to right
                                       DAQBuffers[index]->SampleSpacing;
         if ((time - DAQBuffers[index]->NumSamples) > 0)
            time = DAQBuffers[index]->NumSamples;
         unsigned long end = round(time);

         for (unsigned long i=start; i<end; i++){
            yval = DAQBuffers[index]->Samples[i]*(DAQBuffers[index]->UUperBit) +
               DAQBuffers[index]->OffsetUU;
            ypix = Parent->ZeroYPixels - yval * Parent->PixelsPerYUnit;
            if (abs(ypix - Y) < 3){ // 3 pix up and down
               IsAHit = true;
               break;
            }
         }
      }
   }
   return IsAHit;
}

void TStaticDAQPlot::FixupXandYLimits(double fsxmin)
{
   if (NumDAQBuffers > 0){
      double xmin = fsxmin;
      double xmax = xmin + DAQBuffers[0]->NumSamples * DAQBuffers[0]->SampleSpacing;
      double ymin = DAQBuffers[0]->FSMinY;
      double ymax = DAQBuffers[0]->FSMaxY;
      for (unsigned long i=1; i<NumDAQBuffers; i++){
         DAQBuffers[i] = DAQBuffers[i-1]->Next;
         xmax += DAQBuffers[i]->NumSamples * DAQBuffers[i]->SampleSpacing;
         if (ymin > DAQBuffers[i]->FSMinY) ymin = DAQBuffers[i]->FSMinY;
         if (ymax < DAQBuffers[i]->FSMaxY) ymax = DAQBuffers[i]->FSMaxY;
      }
		FSMinX = constrain(xmin, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMaxX = constrain(xmax, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMinY = constrain(ymin, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMaxY = constrain(ymax, (double)-MAXFLOAT, (double)MAXFLOAT);
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TStaticFunctionPlot
/////////////////////////////////////////////////////////////////////////////

TStaticFunctionPlot::TStaticFunctionPlot(TPLOTPanel *parent,
                                          int which, String label,
                                          TColor color, double fsxmin,
                                          double samplespacing,
                                          unsigned long numplotvalues,
                                          double *plotvalues) :
                           TStaticPlot(parent, which, label, color, fsxmin),
                           NumPlotValues(numplotvalues),
                           PlotValues(plotvalues),
                           SampleSpacing(samplespacing)
{
   FixupXandYLimits(fsxmin);
}
//---------------------------------------------------------------------------

TStaticFunctionPlot::~TStaticFunctionPlot()
{
// do nothing for now
}
//---------------------------------------------------------------------------

bool TStaticFunctionPlot::DrawToDAQPanel()
{
   if (!Visible) return false;
   if (NumPlotValues < 1) return false;

   unsigned long     Increment; 

   double minX = Parent->ConcatenatePlots ? FSMinX : 0.0;
   double maxX = Parent->ConcatenatePlots ? FSMaxX : FSMaxX - FSMinX;

   if (minX >= Parent->VisibleMaxX) return false;
   if (maxX <= Parent->VisibleMinX) return false;

   unsigned long xstart; // index of data point in buffer
   double *p;
   double XProgress;
   double YSampleLast;
   double plotstart;

   if (minX == Parent->VisibleMinX){
      xstart = 0;
      XProgress = Parent->VisibleMinX;
   }
   else if (minX < Parent->VisibleMinX){
   // find start index
      plotstart = Parent->VisibleMinX - minX;
      xstart = round(plotstart/SampleSpacing);
      if (xstart > 0) xstart--;
      XProgress = minX + xstart * SampleSpacing;
   }
   else if (minX < Parent->VisibleMaxX){
      xstart = 0;
      XProgress = minX;
   }
   else {
      return false;
   }

   double TotalXTime = Parent->VisibleMaxX - Parent->VisibleMinX;
   double samplesperpixel = TotalXTime;
   samplesperpixel /= SampleSpacing;                    // number of samples
   samplesperpixel /= Parent->Graph->Width;             // samples per pixel
   samplesperpixel /= 10;                 // no more than 10 samples per pixel

   Increment = round(samplesperpixel);
   if (Increment < 1) Increment = 1;

//   XProgress += Increment * SampleSpacing;

   p = PlotValues;
   YSampleLast = (p[xstart]);
   Parent->BeginUpdate();
   Parent->Graph->Buffer->BeginUpdate();
   Parent->Graph->Buffer->PenColor = Color32(Color);

   Parent->Graph->Buffer->MoveToF(
               (XProgress - Parent->VisibleMinX) * Parent->PixelsPerXUnit,
               Parent->ZeroYPixels - YSampleLast * Parent->PixelsPerYUnit);

   xstart += Increment;
//   do {
      for (unsigned long i = xstart;
            i < NumPlotValues; i+=Increment){
         if (XProgress >= Parent->VisibleMaxX) break;
         YSampleLast = (p[i]);
         XProgress += (Increment * SampleSpacing);
         Parent->Graph->Buffer->LineToFS(
               (XProgress - Parent->VisibleMinX) * Parent->PixelsPerXUnit,
               Parent->ZeroYPixels - YSampleLast * Parent->PixelsPerYUnit);
      };
//   } while (XProgress < Parent->VisibleMaxX);

   Parent->Graph->Buffer->EndUpdate();
   Parent->EndUpdate();

   return true;
}
//---------------------------------------------------------------------------

bool TStaticFunctionPlot::HitTest(int X, int Y)
{
   bool IsAHit = false;

   // values in user units
   double x = X;
   x /= Parent->PixelsPerXUnit;  // pixels to x units relative to graph xmin
   x += Parent->VisibleMinX;     // convert to absolute x units

   // start and stop of each buffer in time
   double minX = Parent->ConcatenatePlots ? FSMinX : 0.0;
   double maxX = Parent->ConcatenatePlots ? FSMaxX : FSMaxX - FSMinX;

   // hittest variables
	double xval, yval;
   unsigned long xpos;
   unsigned long index;
   int ypix;

   if (x <= maxX && x >= minX){
      xpos = round((x - FSMinX) / SampleSpacing); //current sample
      yval = PlotValues[xpos];
      ypix = Parent->ZeroYPixels - yval * Parent->PixelsPerYUnit;

      if (abs(ypix - Y) < 7) IsAHit = true;

      // if slope of plot is high, need to test adjacent samples,
      // within 3x3 pixels
      if (!IsAHit){
         xval = (x - 3.0/Parent->PixelsPerXUnit) / SampleSpacing; // 3 pix to left
         unsigned long start = round((xval - FSMinX) / SampleSpacing);
         xval = (x + 3.0/Parent->PixelsPerXUnit) / SampleSpacing; // 3 pix to right
         unsigned long end = round((xval - FSMinX) / SampleSpacing);
         if (start < 0) start = 0;
         if (end > NumPlotValues) end = NumPlotValues;
         for (unsigned long i=start; i<end; i++){
            yval = PlotValues[i];
            ypix = Parent->ZeroYPixels - yval * Parent->PixelsPerYUnit;
            if (abs(ypix - Y) < 3){ // 3 pix up and down
               IsAHit = true;
               break;
            }
         }
      }
   }
   return IsAHit;
}
//---------------------------------------------------------------------------

void TStaticFunctionPlot::FixupXandYLimits(double fsxmin)
{
   if (NumPlotValues > 0 && PlotValues != NULL){
      double xmin = fsxmin;
      double xmax = xmin + NumPlotValues * SampleSpacing;
      double ymin = MAXFLOAT;
      double ymax = -MAXFLOAT;
      double *vals = PlotValues;
      double val;
      for (unsigned long i=0; i<NumPlotValues; i++){
         val = *vals++;
         if (ymin > val) ymin = val;
         if (ymax < val) ymax = val;
		}
		if (ymin == ymax) {
			if (ymin < 0) {
				ymin = 1.5 * ymin;
				ymax = -1 * ymin;
			}
			else if (ymin > 0 ) {
				ymax = -1.5 * ymin;
				ymin = 1.0 * ymax;
			}
		}
		FSMinX = constrain(xmin, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMaxX = constrain(xmax, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMinY = constrain(ymin, (double)-MAXFLOAT, (double)MAXFLOAT);
		FSMaxY = constrain(ymax, (double)-MAXFLOAT, (double)MAXFLOAT);
   }
}
//---------------------------------------------------------------------------
