//---------------------------------------------------------------------------

#ifndef SignalsH
#define SignalsH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

#include "DAQPanel.h"

//forward declarations
class TADCInput;
class TAcquisitionSetup;
class TEquipmentRack;
class TAmplifier;
class TSignal;

class PACKAGE TSignalIDChoice : public TCollectionItem
{
private:
   TStringList            *FGainChoices;
   void __fastcall         SetGainChoices(TStringList *gains);
   TStringList            *FGainVolts;
   void __fastcall         SetGainVolts(TStringList *volts);
   TStringList            *FLPFilterChoices;
   void __fastcall         SetLPFilterChoices(TStringList *filters);
   TStringList            *FLPFilterVolts;
   void __fastcall         SetLPFilterVolts(TStringList *volts);
   TStringList            *FHPFilterChoices;
   void __fastcall         SetHPFilterChoices(TStringList *filters);
   TStringList            *FHPFilterVolts;
   void __fastcall         SetHPFilterVolts(TStringList *volts);
   AnsiString              FName;
   void __fastcall         SetName(AnsiString name);
   AnsiString              FUnits;
   void __fastcall         SetUnits(AnsiString units);
   float                   FGain;
   void __fastcall         SetGain(float gain);
   float                   FLPFilter;
   void __fastcall         SetLPFilter(float filter);
   float                   FHPFilter;
   void __fastcall         SetHPFilter(float filter);

protected:
   //override TCollectionItem::GetDisplayName
   virtual UnicodeString __fastcall GetDisplayName(void);

public:
   __fastcall              TSignalIDChoice(TCollection *Collection);
   virtual void __fastcall Assign(TPersistent* Source);

__published:
   __property TStringList *GainChoices = {read = FGainChoices, write = SetGainChoices};
   __property TStringList *GainVolts = {read = FGainVolts, write = SetGainVolts};
   __property TStringList *LPFilterChoices = {read = FLPFilterChoices, write = SetLPFilterChoices};
   __property TStringList *LPFilterVolts = {read = FLPFilterVolts, write = SetLPFilterVolts};
   __property TStringList *HPFilterChoices = {read = FHPFilterChoices, write = SetHPFilterChoices};
   __property TStringList *HPFilterVolts = {read = FHPFilterVolts, write = SetHPFilterVolts};

   __property AnsiString   Name = {read = FName, write = SetName};
   __property float        Gain = {read = FGain, write = SetGain};
   __property AnsiString   Units = {read = FUnits, write = SetUnits};
   __property float        LPFilter = {read = FLPFilter, write = SetLPFilter};
   __property float        HPFilter = {read = FHPFilter, write = SetHPFilter};
};

class PACKAGE TSignalIDChoiceCollection : public TCollection
{
private:
   TSignalIDChoice* __fastcall     GetItem(int Index);
   void __fastcall         SetItem(int Index, TSignalIDChoice *Value);
protected:
   DYNAMIC TPersistent* __fastcall GetOwner(void);
public:
   __fastcall              TSignalIDChoiceCollection(TSignal* signal);
   TSignalIDChoice* __fastcall     Add();
   __property TSignalIDChoice     *Items[int Index] = {read = GetItem, write = SetItem};
};


class PACKAGE TSignal : public TCollectionItem
{
private:
//   TSignalType             FSignalType;
   int                     FSignalID;
   AnsiString              FName;
   void __fastcall         SetName(AnsiString name);
   AnsiString              FUnits;
   void __fastcall         SetUnits(AnsiString units);
   float                   FGain;
   void __fastcall         SetGain(float gain);
   float                   FLPFilter;
   void __fastcall         SetLPFilter(float filter);
   float                   FHPFilter;
   void __fastcall         SetHPFilter(float filter);


   short                   FInputADCNum;
   TADCInput              *FADCInput;
   void __fastcall         SetADCInput(TADCInput *input);

   TAmplifier             *FAmplifier;
   void __fastcall         SetAmplifier(TAmplifier *amp);
   TDAQPanel              *FDAQPanel;
   void __fastcall         SetDAQPanel(TDAQPanel *panel);

protected:
   //override TCollectionItem::GetDisplayName
   virtual UnicodeString __fastcall GetDisplayName(void);

public:
   __fastcall              TSignal(TCollection *Collection);
   virtual void __fastcall Assign(TPersistent* Source);
   virtual void __fastcall ProcessData(short *buf, unsigned long numsamples,
                                       short fromwhichadc);

   __property TADCInput   *ADCInput = {read = FADCInput, write = SetADCInput};
   
   __property TAmplifier  *Amplifier = {read = FAmplifier, write = SetAmplifier};
   __property TDAQPanel   *DAQPanel = {read = FDAQPanel, write = SetDAQPanel};

__published:
   __property AnsiString   Name = {read = FName, write = SetName};
   __property float        Gain = {read = FGain, write = SetGain};
   __property float        LPFilter = {read = FLPFilter, write = SetLPFilter};
   __property float        HPFilter = {read = FHPFilter, write = SetHPFilter};
   __property AnsiString   Units = {read = FUnits, write = SetUnits};
//   __property TSignalType  SignalType = {read = FSignalType, write = FSignalType};
   __property int          SignalID = {read = FSignalID, write = FSignalID};

   __property short        InputADCNum = {read = FInputADCNum,
                                          write = FInputADCNum,
                                          default = -1};
/*
   	__property short        GainADCNum = {read = FGainADCNum,
   														write = FGainADCNum,
   														default = -1};
   	__property short        FilterADCNum = {read = FFilterADCNum,
   														write = FFilterADCNum,
   														default = -1};
   	__property short        ModeADCNum = {read = FModeADCNum,
   														write = FModeADCNum,
                                             default = -1};   
*/
};

class PACKAGE TSignalCollection : public TCollection
{
private:
   TAmplifier             *FAmplifier;
   TSignal* __fastcall     GetItem(int Index);
   void __fastcall         SetItem(int Index, TSignal *Value);
protected:
   DYNAMIC TPersistent* __fastcall
                           GetOwner(void);
public:
   __fastcall              TSignalCollection(TAmplifier *amp);
   TSignal* __fastcall     Add();
   __property TSignal     *Items[int Index] = {read = GetItem, write = SetItem};
};
//---------------------------------------------------------------------------
#endif

