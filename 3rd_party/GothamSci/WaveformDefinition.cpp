//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include "WaveformDefinition.h"
#include "GenericUtilities.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TWaveformDefinition *)
{
   new TWaveformDefinition(NULL);
}
//---------------------------------------------------------------------------
__fastcall TWaveformDefinition::TWaveformDefinition(TComponent* Owner)
   : TComponent(Owner)
{
   FSegments = new TWaveformSegmentCollection(this);
}
//---------------------------------------------------------------------------
namespace Waveformdefinition
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TWaveformDefinition)};
	   RegisterComponents("Gotham Scientific", classes, 0);
   }
}

//---------------------------------------------------------------------------

void __fastcall TWaveformDefinition::SetSegments(TWaveformSegmentCollection *segments)
{
   FSegments->Assign(segments);
}
//---------------------------------------------------------------------------

void __fastcall TWaveformDefinition::WaveformHasChanged()
{
   if (ComponentState.Contains(csDesigning)) return;
   if (ComponentState.Contains(csLoading)) return;
   if (FOnWaveformChanged) FOnWaveformChanged(this);
}
//---------------------------------------------------------------------------

// this function fills a buffer (double *sweep) with on sweep of the waveform.
// a call to this function with sweep = NULL will calculate numpoints necessary for mem alloc
int __fastcall TWaveformDefinition::GetSweep(int sweepindex, double* sweep, double holding,
                                                double sampspacing, unsigned long &numpoints)
{
   int numsegmentpoints;
   double rampincr;
   double lastlevel = holding;
   double *val = sweep;

   TCollectionItem *item = NULL;
   TWaveformSegment *segment = NULL;
   int segmentcount = Segments->Count;

   numpoints = 0;
   sampspacing *= 1000.0; // convert to ms

   for (int i=0; i<segmentcount; i++){
      try {
         item = Segments->Items[i];
         if (item) segment = dynamic_cast<TWaveformSegment *>(item);
      }
      catch (...) { // gobble up errors
      }

      if (!item){
         return WF_ALLOCERROR;  // error - is this actually possible?
      }

      if (segment->SegmentType > stOff){
         if (segment->Duration < 0.0){
            FLastErrorMsg.sprintf("Segment #%d duration must be greater than 0.  Please check \"Duration\".");
            return WF_NEGATIVETIME;
         }
         numsegmentpoints = (segment->Duration + sweepindex*segment->DurationIncr) /
                                             sampspacing;

         if (numsegmentpoints < 0.0){
            FLastErrorMsg.sprintf("Segment #%d duration must be greater than 0. Please check \"Duration Incr\".");
            return WF_NEGATIVETIME;
         }

         numsegmentpoints = nexteven(numsegmentpoints);
         numpoints += numsegmentpoints;

         if (sweep){  // only build waveform if sweep != NULL
            switch (segment->SegmentType){
               case stStep: {
                  for (int k=0; k<numsegmentpoints; k++){
                     lastlevel = (segment->Level + sweepindex*segment->LevelIncr);
                     *val++ = lastlevel;
                  }
               }; break;
               case stRamp: {
                  if (numsegmentpoints == 0) rampincr = 0.0;
                  else {
                     rampincr = ((segment->Level + sweepindex*segment->LevelIncr)
                            - lastlevel)/(numsegmentpoints);
                  }
                  for (int k=0; k<numsegmentpoints; k++){
                     lastlevel += rampincr;
                     *val++ = lastlevel;
                  }
               }; break;
            }
         }
      }
   }

   return true;
}
//---------------------------------------------------------------------------
