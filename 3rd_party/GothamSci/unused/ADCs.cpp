//---------------------------------------------------------------------------


#pragma hdrstop

#include "ADCs.h"
#include "Signals.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TADCInput::TADCInput(TCollection *Collection) : TCollectionItem(Collection)
{
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::ProcessData(short *buf, unsigned long numsamples)
{
   FSignal->ProcessData(buf, numsamples, WhichADC);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//TADCInput Properties///////////////////////////////////////////////////////
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetWhichADC(short adc)
{
   if (FWhichADC != adc) FWhichADC = adc;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetName(AnsiString name)
{
   if (FName != name) FName = name;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetADCGain(short gain)
{
   if (FADCGain != gain) FADCGain = gain;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetADCPolarity(TADCPolarityType polarity)
{
   if (FADCPolarity != polarity) FADCPolarity = polarity;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetADCGndCxn(TADCGndCxnType gndcxn)
{
   if (FADCGndCxn != gndcxn) FADCGndCxn = gndcxn;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetADCInputType(TADCInputType inputtype)
{
   if (FADCInputType != inputtype) FADCInputType = inputtype;
}
//---------------------------------------------------------------------------

void __fastcall TADCInput::SetSignal(TSignal *signal)
{
   if (FSignal != signal) FSignal = signal;
}
//---------------------------------------------------------------------------

TInputRange __fastcall TADCInput::GetInputRange()
{
   TInputRange range;
   range.Low = 0;
   range.High = 10;
   return range;
}
//---------------------------------------------------------------------------

AnsiString  __fastcall TADCInput::GetUnits()
{
   if (FSignal) return FSignal->Units;
   return "";
}
//---------------------------------------------------------------------------




