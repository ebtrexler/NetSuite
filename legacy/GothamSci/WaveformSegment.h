//---------------------------------------------------------------------------

#ifndef WaveformSegmentH
#define WaveformSegmentH

#include <Classes.hpp>
//forward declarations
class TWaveformDefinition;

typedef enum SegmentTypesEnum{stOff, stStep, stRamp, stFile, stSquare, stSawtooth,
							stTriangle, stSine, stUserF_t, NumSegmentTypes} SegmentTypes;
extern char *SegmentNames[NumSegmentTypes];

class PACKAGE TWaveformSegment : public TCollectionItem
{
private:
   String                  FName;
   void __fastcall         SetName(String name);
   TWaveformDefinition     *FWaveformDef;
   void __fastcall         SetWaveformDef(TWaveformDefinition *wfd);
   SegmentTypesEnum        FSegmentType;
   void __fastcall         SetSegmentType(SegmentTypesEnum segmenttype);
   double                  FLevel;
   void __fastcall         SetLevel(double level);
   double                  FLevelIncr;
   void __fastcall         SetLevelIncr(double levelincr);
   double                  FDuration;
   void __fastcall         SetDuration(double duration);
   double                  FDurationIncr;
   void __fastcall         SetDurationIncr(double durationincr);
   virtual String  __fastcall
                           GetParamString();
   virtual void __fastcall
                           SetParamString(String  str);

   void __fastcall         WaveformHasChanged();

protected:
   //override TCollectionItem::GetDisplayName
   virtual UnicodeString __fastcall GetDisplayName(void);
public:
   __fastcall              TWaveformSegment(TCollection *Collection);
   virtual void __fastcall Assign(TPersistent* Source);

   __property TWaveformDefinition *WaveformDef =
                           {read = FWaveformDef, write = SetWaveformDef};

//usage of the following parameters depends on segment type.
//Streaming interface is through ParamString
//getter and setter functions

   // File type
   String                  Filename;
   double                  *FileData;
   double                  FileSampleSpacing;
   unsigned long           FileNumPoints;
   // Train types
   double                  Period;
   double                  DutyCycle;
   double                  Amplitude;
   int                     Polarity;
   // User F(t)
   String                  Function;

__published:
   __property String       Name = {read = FName, write = SetName};
   __property String       ParamString = {read = GetParamString, write = SetParamString};
   __property SegmentTypesEnum
                           SegmentType = {read = FSegmentType, write = SetSegmentType};
   __property double       Level = {read = FLevel, write = SetLevel};
   __property double       LevelIncr = {read = FLevelIncr, write = SetLevelIncr};
   __property double       Duration = {read = FDuration, write = SetDuration};
   __property double       DurationIncr = {read = FDurationIncr, write = SetDurationIncr};
};

class PACKAGE TWaveformSegmentCollection : public TCollection
{
private:
   TWaveformDefinition              *FWaveformDef;
   TWaveformSegment* __fastcall     GetItem(int Index);
   void __fastcall                  SetItem(int Index, TWaveformSegment *Value);
protected:
   DYNAMIC TPersistent* __fastcall  GetOwner(void);
public:
   __fastcall TWaveformSegmentCollection(TWaveformDefinition *wavedef);
   TWaveformSegment * __fastcall    Add();
   __property TWaveformSegment     *Items[int Index] = {read = GetItem, write = SetItem};
};

//---------------------------------------------------------------------------
#endif
