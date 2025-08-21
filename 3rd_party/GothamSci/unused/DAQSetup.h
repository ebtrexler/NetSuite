//---------------------------------------------------------------------------

#ifndef DAQSetupH
#define DAQSetupH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

//forward declarations
class TAcquisitionSetup;
class TEquipmentRack;
class TAmplifier;
class TSignal;

class TSignal : public TCollectionItem
{
private:
protected:
   AnsiString              FName;
   void __fastcall         SetName(AnsiString name);
   float                   FGain;
   void __fastcall         SetGain(float gain);
   float                   FFilter;
   void __fastcall         SetFilter(float filter);
   AnsiString              FUnits;
   void __fastcall         SetUnits(AnsiString units);
   short                   FInputADCNum;
   TADCInput              *FADCInput;
   void __fastcall         SetADCInput(TADCInput *input);
   TAmplifier             *FAmplifier;
   void __fastcall         SetAmplifier(TAmplifier *amp);
   TDAQPanel              *FDAQPanel;
   void __fastcall         SetDAQPanel(TDAQPanel *panel);
public:
   virtual void __fastcall ProcessData(short *buf, unsigned long numsamples,
                                       short fromwhichadc);

   __property TADCInput   *ADCInput = {read = FADCInput, write = SetADCInput};
   __property TAmplifier  *Amplifier = {read = FAmplifier, write = SetAmplifier};
   __property TDAQPanel   *DAQPanel = {read = FDAQPanel, write = SetDAQPanel};

   TSignal();

__published:
   __property AnsiString   Name = {read = FName, write = SetName};
   __property float        Gain = {read = FGain, write = SetGain};
   __property float        Filter = {read = Filter, write = SetFilter};
   __property AnsiString   Units = {read = FUnits, write = SetUnits};
   __property short        InputADCNum = {read = FInputADCNum, write = FInputADCNum};
};

class TTelegraphedSignal : public TSignal
{
private:
protected:
   short                   FGainADCNum;
   short                   FFilterADCNum;
   TADCInput              *FGainADCInput;
   void __fastcall         SetGainADCInput(TADCInput *input);
   TADCInput              *FFilterADCInput;
   void __fastcall         SetFilterADCInput(TADCInput *input);
public:
   virtual void __fastcall ProcessData(short *buf, unsigned long numsamples,
                                       short fromwhichadc);

   __property TADCInput   *GainADCInput; = {read = FGainADCInput,
                                             write = SetGainADCInput};
   __property TADCInput   *FilterADCInput; = {read = FFilterADCInput,
                                             write = SetFilterADCInput};

   __fastcall TTelegraphedSignal();

__published:
   __property short        GainADCNum = {read = FGainADCNum, write = FGainADCNum};
   __property short        FilterADCNum = {read = FFilterADCNum, write = FFilterADCNum};
};

class TAmplifier : public TComponent
{
private:
protected:
   TCollection            *Signals;
   bool                    FPresentInEquipmentRack;
   short                   FModeADCNum;
   TADCInput              *FModeADCInput;
   void __fastcall         SetModeADCInput(TADCInput *input);
public:
   __property TADCInput   *ModeADCInput; = {read = FModeADCInput,
                                             write = SetModeADCInput};

//---------------------------------------------------------------------------
#endif

