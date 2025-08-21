//---------------------------------------------------------------------------

#ifndef AmplifierH
#define AmplifierH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include "InputBox.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Signals.h"

//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TProcessADCDataEvent)(System::TObject* Sender,
                      short *buf, ulong numsamples, TSignal *WhichSignal);

class PACKAGE TAmplifier : public TPanel
{
private:
protected:

   bool                    FIsInEquipRack;
   void __fastcall         SetIsInEquipRack(bool set);


   TStringList            *FModeChoices;
   void __fastcall         SetModeChoices(TStringList *modes);
   TStringList            *FModeVolts;
   void __fastcall         SetModeVolts(TStringList *volts);
   int                     FAmpMode;
   void __fastcall         SetAmpMode(int set);
   virtual void __fastcall DoSetAmpMode(int set);

	TSignalCollection      *FSignals;
   void __fastcall         SetSignals(TSignalCollection *signals);

//   TCommandCollection     *FCommands;
//	void __fastcall         SetCommands(TCommandCollection *commands);

   TADCInput              *FGainADCInput;
   void __fastcall         SetGainADCInput(TADCInput *input);
   TADCInput              *FFilterADCInput;
   void __fastcall         SetFilterADCInput(TADCInput *input);
   TADCInput              *FModeADCInput;
   void __fastcall         SetModeADCInput(TADCInput *input);
   short                   FGainADCNum;
   short                   FFilterADCNum;
   short                   FModeADCNum;

   TProcessADCDataEvent    FOnProcessADCData;

public:
	__fastcall              TAmplifier(TComponent* Owner);
   virtual void __fastcall ProcessADCData(short *buf, unsigned long numsamples,
                                          TSignal *Signal);
   virtual void __fastcall ProcessModeADCData(short *buf, unsigned long numsamples,
                                             TSignal *Signal);
   virtual float __fastcall GainFromTelegraph(short *buf, unsigned long numsamples,
                                             TSignal *Signal);
   virtual float __fastcall FilterFromTelegraph(short *buf, unsigned long numsamples,
                                             TSignal *Signal);
   void __fastcall         ConnectToADCs(); // called when component is loaded.

__published:
//	__property TStringList *ModeChoicess = {read = FModeChoices, write = SetModeChoices};
//   __property TStringList *ModeVolts = {read = FModeVolts, write = SetModeVolts};
	__property int          AmpMode = {read = FAmpMode, write = SetAmpMode};

	__property bool         IsInEquipRack = {read = FIsInEquipRack,
														write = SetIsInEquipRack};

//	__property TSignalCollection
//								  *Signals = {read = FSignals, write = SetSignals};

		/*
		__property TCommandCollection
										  *Commands = {read = FCommands, write = SetCommands};

			__property TADCInput   *GainADCInput = {read = FGainADCInput,
																	write = SetGainADCInput};

			__property TADCInput   *LPFilterADCInput = {read = FLPFilterADCInput,
																			write = SetLPFilterADCInput};
			__property TADCInput   *HPFilterADCInput = {read = FHPFilterADCInput,
																			write = SetHPFilterADCInput};

			__property TADCInput   *ModeADCInput = {read = FModeADCInput,
																	write = SetModeADCInput};

			__property TProcessADCDataEvent    OnProcessADCData =
											{read = FOnProcessADCData, write = FOnProcessADCData};
	*/


};
//---------------------------------------------------------------------------
#endif



