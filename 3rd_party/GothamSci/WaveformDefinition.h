//---------------------------------------------------------------------------

#ifndef WaveformDefinitionH
#define WaveformDefinitionH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>

#include "WaveformSegment.h"

#define  WF_ALLOCERROR     -100
#define  WF_EXCEEDLIMITS   -101
#define  WF_NEGATIVETIME   -102

//---------------------------------------------------------------------------
class PACKAGE TWaveformDefinition : public TComponent
{
private:
//   TCommand                   *FCommand;
//   void __fastcall            SetCommand(TCommand *command);
   TWaveformSegmentCollection *FSegments;
   void __fastcall            SetSegments(TWaveformSegmentCollection *segments);
   TNotifyEvent               FOnWaveformChanged;
   AnsiString                 FLastErrorMsg;
protected:
public:
   __fastcall TWaveformDefinition(TComponent* Owner);
   void __fastcall WaveformHasChanged();
   int __fastcall GetSweep(int sweepindex, double* sweep, double holding,
                              double sampspac, unsigned long &numpoints);
__published:
   __property TWaveformSegmentCollection *Segments = {read = FSegments, write = SetSegments};
   __property TNotifyEvent OnWaveformChanged = {read = FOnWaveformChanged, write = FOnWaveformChanged};
   __property AnsiString   LastErrorMsg = {read = FLastErrorMsg};
};
//---------------------------------------------------------------------------
#endif
 