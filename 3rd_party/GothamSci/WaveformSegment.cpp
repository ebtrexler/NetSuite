//---------------------------------------------------------------------------


#pragma hdrstop

#include "WaveformDefinition.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                         TWaveformSegment : TCollectionItem
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

char *SegmentNames[NumSegmentTypes] = {"OFF", "Step", "Ramp", "File", "Square",
													"Sawtooth", "Triangle", "Sine", "User"};

__fastcall TWaveformSegment::TWaveformSegment(TCollection *Collection) : TCollectionItem(Collection)
{
//   FileSettings = new TStringList;

//   AnsiString format = "FileName=%s,FileData=%s,FileSampleSpacing=%f,FileNumPoints=%d";
//         result.sprintf(format, Filename, CompressToString(FileData,FileNumPoints),
//                        FileSampleSpacing, FileNumPoints);
}
//---------------------------------------------------------------------------

void __fastcall   TWaveformSegment::SetWaveformDef(TWaveformDefinition *wfd)
{
   FWaveformDef = wfd;
}
//---------------------------------------------------------------------------

String __fastcall TWaveformSegment::GetDisplayName(void)
{
   return FName == "" ? TCollectionItem::GetDisplayName() : FName;
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::Assign(TPersistent* Source)
{
   TWaveformSegment *segment = dynamic_cast<TWaveformSegment *>(Source);
   if (segment){
      Name = segment->Name;
   }
   else TPersistent::Assign(Source);
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetName(String name)
{
   if (FName != name) FName = name;
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::WaveformHasChanged()
{
   if (FWaveformDef) FWaveformDef->WaveformHasChanged();
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetSegmentType(SegmentTypesEnum segmenttype)
{
   if (FSegmentType != segmenttype){
      FSegmentType = segmenttype;
      WaveformHasChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetLevel(double level)
{
   if (FLevel != level){
      FLevel = level;
      WaveformHasChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetLevelIncr(double levelincr)
{
   if (FLevelIncr != levelincr){
      FLevelIncr = levelincr;
      WaveformHasChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetDuration(double duration)
{
   if (FDuration != duration){
      FDuration = duration;
      WaveformHasChanged();
   }
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetDurationIncr(double durationincr)
{
   if (FDurationIncr != durationincr){
      FDurationIncr = durationincr;
      WaveformHasChanged();
   }
}
//---------------------------------------------------------------------------

String __fastcall TWaveformSegment::GetParamString()
{
   String result = "";
   switch (FSegmentType)
   {
      case stOff:
      case stStep:
      case stRamp:
         result = "No Params"; break;
      case stFile:
      case stSquare:
      case stSawtooth:
      case stTriangle:
      case stSine:
      case stUserF_t:
      default: break;
   };
   return result;
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegment::SetParamString(String  str)
{
   switch (FSegmentType){
      case stOff:
      case stStep:
      case stRamp:
      case stFile:
      case stSquare:
      case stSawtooth:
      case stTriangle:
      case stSine:
      case stUserF_t:
      default: break;
   };
//   WaveformHasChanged();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                   TWaveformSegmentCollection : TCollection
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

__fastcall TWaveformSegmentCollection::TWaveformSegmentCollection(TWaveformDefinition *wfd) :
                                       TCollection(__classid(TWaveformSegment))
{
   FWaveformDef = wfd;
}
//---------------------------------------------------------------------------

TWaveformSegment* __fastcall TWaveformSegmentCollection::Add()
{
   TWaveformSegment *segment = NULL;
   segment = dynamic_cast<TWaveformSegment*>(TCollection::Add());
   if (segment) segment->WaveformDef = FWaveformDef;
   return segment;
}
//---------------------------------------------------------------------------

TWaveformSegment* __fastcall TWaveformSegmentCollection::GetItem(int Index)
{
   return dynamic_cast<TWaveformSegment*>(TCollection::GetItem(Index));
}
//---------------------------------------------------------------------------

void __fastcall TWaveformSegmentCollection::SetItem(int Index, TWaveformSegment *Value)
{
   TCollection::SetItem(Index, Value);
}
//---------------------------------------------------------------------------

TPersistent* __fastcall TWaveformSegmentCollection::GetOwner(void)
{
   return (TPersistent *)FWaveformDef;
}
//---------------------------------------------------------------------------

