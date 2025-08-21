//---------------------------------------------------------------------------


#pragma hdrstop

#include "tools.h"
#include "GR32_Blend.hpp"
#include "PLOTPanel.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                        Circular buffer functions
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TDAQSampleBuffer* __fastcall
                        TPLOTPanel::AllocateBuffers(unsigned short numbuffers,
																	 unsigned long numsamples)
{
   if (numbuffers < 2) return NULL;
   if (NumBuffersAllocated != 0) FreeBuffers();

 	FirstEmptyBuffer = FirstFullBuffer = NULL;

   NumBuffersAllocated = numbuffers;

	TDAQSampleBuffer* Buff;

   int buffnum = 0;

   for (int i=1; i<NumBuffersAllocated; i++, Buff=Buff->Next)
	{
   	if (i == 1)
      {
         try {
         	FirstEmptyBuffer = Buff = new TDAQSampleBuffer(numsamples);
         }
         catch(...){
            return NULL;
         }
         Buff->Which = buffnum;
      }
      buffnum++;
      try {
         Buff->Next = new TDAQSampleBuffer(numsamples);
      }
      catch(...){
         return NULL;
      }
      Buff->Next->Which = buffnum;
   }

    // Last buffer points to first, a circular buffer scheme.

   Buff->Next = FirstEmptyBuffer;

   return FirstEmptyBuffer;
}
//---------------------------------------------------------------------------
void __fastcall	TPLOTPanel::FreeBuffers()
{
	if (NumBuffersAllocated == 0) return;

   TDAQSampleBuffer *Buff = FirstEmptyBuffer, *BuffNext;

   do {
      BuffNext = Buff->Next;
      delete Buff;
      Buff = BuffNext;
   } while (Buff != FirstEmptyBuffer);

   FirstEmptyBuffer = FirstFullBuffer = NULL;
}
//---------------------------------------------------------------------------
bool __fastcall TPLOTPanel::AnyEmptyBuffers()
{
   if (!FirstEmptyBuffer) return false;
   if (FirstEmptyBuffer->IsFull){ //overrun
   	if (!(IsResizing > 0) && !OverRun){
         OverRun = true;
         TDAQSampleBuffer* wipe_them_all;
         do {
            wipe_them_all = GetNextFullBuffer();
         } while (wipe_them_all);
         if (!IgnoreOverrun && FOnOverrun)       // Event handler may wish to
            FOnOverrun(this, OverRun); // handle overrun externally
      }
	   return false;
   }
   else{
   	OverRun = false;
   	return true;
   }
}
//---------------------------------------------------------------------------
TDAQSampleBuffer* __fastcall TPLOTPanel::GetNextEmptyBuffer()
{
	TDAQSampleBuffer *Buff = FirstEmptyBuffer;

   if (!(Buff->IsFull))
   {
      if (!FirstFullBuffer){
		  FirstFullBuffer = FirstEmptyBuffer;
      }

      Buff->IsFull = true;  //should be set by calling function?
      
      // Next buffer please...
      FirstEmptyBuffer = FirstEmptyBuffer->Next;

      return Buff;
   }
   else return NULL;
}
//---------------------------------------------------------------------------
bool __fastcall TPLOTPanel::AnyFullBuffers()
{
   if (!FirstFullBuffer) return false;

	if (FirstFullBuffer->IsFull)
   	return true;
   else
   	return false;  // underrun
}
//---------------------------------------------------------------------------
TDAQSampleBuffer* __fastcall TPLOTPanel::GetNextFullBuffer()
{
   if (!FirstFullBuffer) return NULL;

	TDAQSampleBuffer *Buff = FirstFullBuffer;

   if (Buff->IsFull)
   {
   	Buff->IsFull = false;  //should be set by calling function?
   	// Next buffer please...
      FirstFullBuffer = FirstFullBuffer->Next;
   	return Buff;
   }
   else return NULL;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                       Realtime plotting functions
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// FillBuffer will be called from data acquisition thread - samples are contiguous
bool __fastcall TPLOTPanel::FillBuffer(double maxY, double minY,
													double uuperbit, double offsetuu,
													double sampletime, short *samples,
                                       unsigned long numsamples,
													int *buffnum)
{
	*buffnum = -1;

	if (!AnyEmptyBuffers()){
		return false;
	}

	TDAQSampleBuffer *Buff = GetNextEmptyBuffer();
	if (!Buff){
		return false;
	}

	// lets calling function know which buffer is being filled
	*buffnum = Buff->Which;

	Buff->FSMaxY = maxY;
	Buff->FSMinY = minY;
	Buff->UUperBit = uuperbit;
	Buff->OffsetUU = offsetuu;
	Buff->SampleSpacing = sampletime;
	if (numsamples != Buff->NumSamples) return false; //should throw exception here
	memcpy(Buff->Samples, samples, Buff->NumSamples*sizeof(short));

	return true;
}
//---------------------------------------------------------------------------

// FillBufferMUX will be called from data acquisition thread -- samples are multiplexed
bool __fastcall TPLOTPanel::FillBufferMUX(double maxY, double minY, double uuperbit,
										double offsetuu, double sampletime, short *muxsamples,
										unsigned long numsamples, int muxoffset, int muxincr,
										int *buffnum)
{
	*buffnum = -1;

	if (!AnyEmptyBuffers()){
		return false;
	}

	TDAQSampleBuffer *Buff = GetNextEmptyBuffer();
	if (!Buff){
		return false;
	}

	// lets calling function know which buffer is being filled
	*buffnum = Buff->Which;

	short *muxsamp = &muxsamples[muxoffset];
	short *buffsamp = Buff->Samples;

	Buff->FSMaxY = maxY;
	Buff->FSMinY = minY;
	Buff->UUperBit = uuperbit;
	Buff->OffsetUU = offsetuu;
	Buff->SampleSpacing = sampletime;
	if (numsamples != Buff->NumSamples) return false; //should throw exception here
	for (unsigned long i = 0; i < numsamples; i++) {
		*buffsamp++ = *muxsamp;
		muxsamp += muxincr;
	}

	return true;
}

// PlotNextBuffer will be called from the plotting thread
bool __fastcall TPLOTPanel::PlotNextBuffer(int *buffnum)
{
   // grab buffer regardless of whether painting or not
	if (!AnyFullBuffers()){
   	return false;
	}

	TDAQSampleBuffer *Buff = GetNextFullBuffer();
	if (!Buff){
		return false;
	}

	// lets calling function know which buffer is being plotted
	if (buffnum){
		*buffnum = -1;
      *buffnum = Buff->Which;
	}

	// reasons not to paint
   if (!Visible) return false;
   if (Height < 50) return false;
	if (IsResizing > 0) return false;

	// VisibleMaxX must be a multiple of buffer time///////////////////////////
   double bufftime = Buff->NumSamples * Buff->SampleSpacing;
   unsigned long numbufferspertrace = round(VisibleMaxX/bufftime);
   if (numbufferspertrace == 0) numbufferspertrace++;
   if (NumBuffersPerTrace != numbufferspertrace){
      Reset = true;
  	   NumBuffersPerTrace = numbufferspertrace;
   }
   bool redraw = false;
   if (!OverRun){// && !IgnoreOverrun){
      if (NumBuffersPerTrace * bufftime > FFullScaleMaxX){
         while (NumBuffersPerTrace * bufftime > FFullScaleMaxX) {
            NumBuffersPerTrace--;
         };
         while ((NumBuffersPerTrace * bufftime) <= 0.0049)
            NumBuffersPerTrace++;
         FVisibleMaxX = NumBuffersPerTrace * bufftime;
         redraw = true;
      }
   }
   unsigned long numpointspertrace = round(FVisibleMaxX/Buff->SampleSpacing);
   unsigned long numpointstoplot = NumBuffersPerTrace * Buff->NumSamples;
   if (numpointspertrace != numpointstoplot){
      while ((NumBuffersPerTrace * bufftime) <= 0.0049)
         NumBuffersPerTrace++;
      FVisibleMaxX = NumBuffersPerTrace * bufftime;
      redraw = true;
   }

   if (FFullScaleMaxY != Buff->FSMaxY){
      FFullScaleMaxY = Buff->FSMaxY;
      redraw = true;
   }
	if (FFullScaleMinY != Buff->FSMinY){
      FFullScaleMinY = Buff->FSMinY;
      redraw = true;
   }

	if (redraw){
		Draw(true);
//      return false;  --had to remove because 1st buffer always lost
   }

   // begin painting
   BeginUpdate();  // halts redrawing of grid
   int height = Graph->Height;
   int width = Graph->Width;

   short *p = Buff->Samples;

   if (Reset){
   	XTimeProgress = 0;
      YSampleLast = (*p)*(Buff->UUperBit) + Buff->OffsetUU;
      NumBuffersPlotted = 0;
      Reset = false;
   }

   if (NumBuffersPlotted >= NumBuffersPerTrace){
      NumBuffersPlotted = 0;
      XTimeProgress = 0;
   }

   Graph->Buffer->BeginUpdate();

   if (NumBuffersPlotted == 0) {
      Graph->Buffer->MoveToF(0, ZeroYPixels - YSampleLast * PixelsPerYUnit);
      if (FEnableTraceStorage && (FTraceStorageFade > 0)){
         TColor32 *pixel = Graph->Buffer->Bits[0];
         TColor32 Fade = SetAlpha(Color32(BackgroundColor), FTraceStorageFade);
         for (int i = 0; i< width*height; i++){
           BlendMem(Fade, *pixel++);
         }
         EMMS();
      }
   }

   int left = round(XTimeProgress * PixelsPerXUnit);
	if (NumBuffersPlotted != 0) left +=1;
   int right = round((XTimeProgress + bufftime) * PixelsPerXUnit);
   if (left < 0) left = 0;
   if (right > width) right = width;
	::TRect plotRect = Rect(left, 0, right, height);

   DrawPartialGrid(FEnableTraceStorage, plotRect, FDrawEraseExtra);

   Graph->Buffer->PenColor = Color32(PenColor);

   double samplesperpixel = VisibleMaxX;
   samplesperpixel /= Buff->SampleSpacing;   // number of samples
   samplesperpixel /= width;                // samples per pixel
   samplesperpixel /= 10;                   // no more than 10 samples per pixel

   unsigned long increment = round(samplesperpixel);
   if (increment < 1) increment = 1;

   while (Buff->NumSamples % increment){  // Buff->NumSamples must be evenly div.
      increment++;
      unsigned long temp = Buff->NumSamples / increment; // by increment
      increment = Buff->NumSamples / temp;
   }

   for (unsigned long sample = 0; sample<Buff->NumSamples; sample+=increment){
      YSampleLast = (*(p))*(Buff->UUperBit) + Buff->OffsetUU;
      XTimeProgress += (increment * Buff->SampleSpacing);
      Graph->Buffer->LineToFS(XTimeProgress * PixelsPerXUnit,
      					ZeroYPixels - YSampleLast * PixelsPerYUnit);
		p+=increment;
   }

   left -= 1;//width/20;
	right += FDrawEraseExtra;//width/20;
   if (left < 0) left = 0;
   if (right > width) right = width;
   ::TRect updateRect = Rect(left, 0, right, height);

   Graph->Buffer->EndUpdate();

   EndUpdate();


   if (NumBuffersPerTrace == 1) Graph->Invalidate();
	else {
		try {
			Graph->Flush(updateRect);
		} catch(...) {
			//gobble up canvas not accessible errors
		}
   }
   NumBuffersPlotted++;

   return true;
}
//---------------------------------------------------------------------------
