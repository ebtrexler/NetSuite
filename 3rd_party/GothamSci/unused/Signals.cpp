//---------------------------------------------------------------------------


#pragma hdrstop

#include "ADCs.h"
#include "Signals.h"

#include "Amplifier.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                         TSignalIDChoice : TCollectionItem
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

__fastcall TSignalIDChoice :: TSignalIDChoice(TCollection *Collection)
{
   GainChoices = new TStringList;
   GainVolts = new TStringList;
   LPFilterChoices = new TStringList;
   LPFilterVolts = new TStringList;
   HPFilterChoices = new TStringList;
   HPFilterVolts = new TStringList;
}
//---------------------------------------------------------------------------

virtual AnsiString __fastcall TSignalIDChoice :: GetDisplayName(void)
{
   return FName == "" ? TCollectionItem::GetDisplayName() : FName;
}
//---------------------------------------------------------------------------

virtual void __fastcall TSignalIDChoice :: Assign(TPersistent* Source)
{
   TSignalIDChoice *signalidchoice = dynamic_cast<TSignalIDChoice *>(Source);
   if (signalidchoice){
      FGainChoices = signalidchoice->GainChoices;
      FGainVolts = signalidchoice->GainVolts;
      FLPFilterChoices = signalidchoice->LPFilterChoices;
      FLPFilterVolts = signalidchoice->LPFilterVolts;
      FHPFilterChoices = signalidchoice->HPFilterChoices;
      FHPFilterVolts = signalidchoice->HPFilterVolts;
   }
   else TPersistent::Assign(Source);
}
//---------------------------------------------------------------------------

void __fastcall TSignalIDChoice ::  SetGainChoices(TStringList *gains);

void __fastcall TSignalIDChoice :: SetGainVolts(TStringList *volts);

void __fastcall TSignalIDChoice :: SetLPFilterChoices(TStringList *filters);

void __fastcall TSignalIDChoice :: SetLPFilterVolts(TStringList *volts);

void __fastcall TSignalIDChoice :: SetHPFilterChoices(TStringList *filters);

void __fastcall TSignalIDChoice :: SetHPFilterVolts(TStringList *volts);

void __fastcall TSignalIDChoice :: SetName(AnsiString name);

void __fastcall TSignalIDChoice :: SetUnits(AnsiString units);

void __fastcall TSignalIDChoice :: SetGain(float gain);

void __fastcall TSignalIDChoice :: SetLPFilter(float filter);

void __fastcall TSignalIDChoice :: SetHPFilter(float filter);



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                   TSignalIDChoiceCollection : TCollection
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TSignalIDChoice* __fastcall TSignalIDChoiceCollection :: GetItem(int Index);
void __fastcall TSignalIDChoiceCollection :: SetItem(int Index, TSignalIDChoice *Value);

DYNAMIC TPersistent* __fastcall TSignalIDChoiceCollection :: GetOwner(void);

__fastcall TSignalIDChoiceCollection :: TSignalIDChoiceCollection(TSignal* signal);
TSignalIDChoice* __fastcall TSignalIDChoiceCollection :: Add();

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                         TSignal : TCollectionItem
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
__fastcall TSignal::TSignal(TCollection *Collection) : TCollectionItem(Collection)
{
   FInputADCNum = -1;
   FGainADCNum = -1;
   FFilterADCNum = -1;
}
//---------------------------------------------------------------------------

AnsiString __fastcall TSignal::GetDisplayName(void)
{
   return FName == "" ? TCollectionItem::GetDisplayName() : FName;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::Assign(TPersistent* Source)
{
   TSignal *signal = dynamic_cast<TSignal *>(Source);
   if (signal){
      ADCInput = signal->ADCInput;
      GainADCInput = signal->GainADCInput;
      FilterADCInput = signal->FilterADCInput;
      ModeADCInput = signal->ModeADCInput;
      Amplifier = signal->Amplifier;
      DAQPanel = signal->DAQPanel;
      Name = signal->Name;
      Gain = signal->Gain;
      Filter = signal->Filter;
      Units = signal->Units;
      AmpModeDependent = signal->AmpModeDependent;
      InputADCNum = signal->InputADCNum;
      GainADCNum  = signal->GainADCNum;
      FilterADCNum = signal->FilterADCNum;
      ModeADCNum = signal->ModeADCNum;
   }
   else TPersistent::Assign(Source);
}
//---------------------------------------------------------------------------

void __fastcall TSignal::ProcessData(short *buf, unsigned long numsamples,
                                       short fromwhichadc)
{
   TADCInputType inputtype;
   if (FInputADCNum == fromwhichadc) {
      inputtype = FADCInput->ADCInputType;
   }
   else if (FGainADCNum == fromwhichadc) {
      inputtype = FGainADCInput->ADCInputType;
   }
   else if (FFilterADCNum == fromwhichadc) {
      inputtype = FFilterADCInput->ADCInputType;
   }
   else if (FModeADCNum == fromwhichadc) {
      inputtype = FModeADCInput->ADCInputType;
   }
   else {
      return; // calling function error -- signals crossed
   }

   switch (inputtype) {
      case aiSignal: {
         FAmplifier->ProcessADCData(buf, numsamples, this);
      }; break;
      case aiGainTelegraph: {
         FGain = FAmplifier->GainFromTelegraph(buf, numsamples, this);
      }; break;
      case aiFilterTelegraph: {
         FFilter = FAmplifier->FilterFromTelegraph(buf, numsamples, this);
      }; break;
      case aiModeTelegraph: {
         FAmplifier->ProcessModeADCData(buf, numsamples, this);
      }; break;
   };
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//TSignal Properties/////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

void __fastcall TSignal::SetName(AnsiString name)
{
   if (FName != name) FName = name;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetGain(float gain)
{
   if (FGain != gain) FGain = gain;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetFilter(float filter)
{
   if (FFilter != filter) FFilter = filter;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetUnits(AnsiString units)
{
   if (FUnits != units) FUnits = units;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetADCInput(TADCInput *input)
{
   if (FADCInput != input){
      FADCInput = input;
      FInputADCNum = FADCInput->WhichADC;
      if (FADCInput->Signal != this){
         FADCInput->Signal = this;
         FADCInput->ADCInputType = aiSignal;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetGainADCInput(TADCInput *input)
{
   if (FGainADCInput != input){
      FGainADCInput = input;
      FGainADCNum = FGainADCInput->WhichADC;
      if (FGainADCInput->Signal != this){
         FGainADCInput->Signal = this;
         FGainADCInput->ADCInputType = aiGainTelegraph;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetFilterADCInput(TADCInput *input)
{
   if (FFilterADCInput != input){
      FFilterADCInput = input;
      FFilterADCNum = FFilterADCInput->WhichADC;
      if (FFilterADCInput->Signal != this){
         FFilterADCInput->Signal = this;
         FFilterADCInput->ADCInputType = aiFilterTelegraph;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetModeADCInput(TADCInput *input)
{
   if (FModeADCInput != input){
      FModeADCInput = input;
      FModeADCNum = FModeADCInput->WhichADC;
      if (FModeADCInput->Signal != this){
         FModeADCInput->Signal = this;
         FModeADCInput->ADCInputType = aiModeTelegraph;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetAmplifier(TAmplifier *amp)
{
   if (FAmplifier != amp) FAmplifier = amp;
}
//---------------------------------------------------------------------------

void __fastcall TSignal::SetDAQPanel(TDAQPanel *panel)
{
   if (FDAQPanel != panel) FDAQPanel = panel;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                   TSignalCollection : TCollection
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

__fastcall TSignalCollection::TSignalCollection(TAmplifier *amp) : TCollection(__classid(TSignal))
{
   FAmplifier = amp;
}
//---------------------------------------------------------------------------

TSignal* __fastcall TSignalCollection::Add()
{
   TSignal *signal = NULL;
   signal = dynamic_cast<TSignal*>(TCollection::Add());
   if (signal) signal->Amplifier = FAmplifier;
   return signal;
}
//---------------------------------------------------------------------------

TSignal* __fastcall TSignalCollection::GetItem(int Index)
{
   return dynamic_cast<TSignal*>(TCollection::GetItem(Index));
}
//---------------------------------------------------------------------------

void __fastcall TSignalCollection::SetItem(int Index, TSignal *Value)
{
   TCollection::SetItem(Index, Value);
}
//---------------------------------------------------------------------------

TPersistent* __fastcall TSignalCollection::GetOwner(void)
{
   return (TPersistent *)FAmplifier;
}
//---------------------------------------------------------------------------
