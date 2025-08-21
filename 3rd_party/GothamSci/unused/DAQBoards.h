//---------------------------------------------------------------------------

#ifndef DAQBoardsH
#define DAQBoardsH

//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

class TDAQBoard;
class TADCInput;
class TDACOutput;

typedef struct tagTInputRange
{
   float                Low;
   float                High;
} TInputRange;

typedef enum TADCPolarityType {apUnipolar, apBipolar, apNumADCModes};
typedef enum TADCGndCxnType {agcDIFF, agcRSE, agcNRSE, agcNumADCGndCxnTypes};
typedef enum TADCInputType  {aiSignal, aiGainTelegraph, aiFilterTelegraph,
                              aiModeTelegraph, aiNumADCInputTypes};

class TADCInput : public TPersistent
{
private:
protected:
   short                   FWhichADC;
   void __fastcall         SetWhichADC(short adc);
   AnsiString              FName;
   void __fastcall         SetName(AnsiString name);
   short                   FADCGain;
   void __fastcall         SetADCGain(short gain);
   TADCPolarityType        FADCPolarity;
   void __fastcall         SetADCPolarity(TADCPolarityType polarity);
   TADCGndCxnType          FADCGndCxn
   void __fastcall         SetADCGndCxn(TADCGndCxn gndcxn);
   TADCInputType           FADCInputType;
   void __fastcall         SetADCInputType(TADCInputType input);
   TSignal                 *FSignal;
   void __fastcall         SetSignal(TSignal *signal);

   TInputRange __fastcall  GetInputRange;
   AnsiString  __fastcall  GetUnits;
public:
   virtual void __fastcall ProcessData(short *buf, unsigned long numsamples);

   __property TSignal      *Signal = {read = FSignal, write = SetSignal};

   TADCInput();

__published:
   __property short        WhichADC = {read = FWhichADC, write = SetWhichADC};
   __property AnsiString   Name = {read = FName, write = SetName};
   __property short        ADCGain = {read = FADCGain, write = SetADCGain};
   __property TADCPolarityType ADCPolarity = {read = FADCPolarity, write = SetADCPolarity);
   __property TADCGndCxnType ADCGndCxn = {read = FADCGndCxn, write = SetADCGndCxn};
   __property TADCInputType ADCInput = {read = FADCInput, write = SetADCInput};
   __property TInputRange  InputRange = {read = GetInputRange};
   __property AnsiString   Units = {read = GetUnits};
};


//---------------------------------------------------------------------------
#endif
 