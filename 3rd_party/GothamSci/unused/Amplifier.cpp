//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include "Amplifier.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAmplifier *)
{
   new TAmplifier(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAmplifier::TAmplifier(TComponent* Owner)
   : TPanel(Owner)
{
/*
      FSignals = new TSignalCollection(this);
      FModes = new TStringList();
      FModeVolts = new TStringList();
      FGains = new TStringList();
      FGainVolts = new TStringList();
      FFilters = new TStringList();
      FFilterVolts = new TStringList();

*/
/*
   	Bevel1 = new TBevel(this);
   	CommandUnitsLabel = new TLabel(this);
   	CommandInputBox = new TInputBox(this);
      AmplifierNameStaticText = new TStaticText(this);
   	StaticText2 = new TStaticText(this);
   	StaticText3 = new TStaticText(this);
      CommandUpDown = new TUpDown(this);
   	StatusBar1 = new TStatusBar(this);
   	ModePanel = new TPanel(this);
   	VTrack = new TSpeedButton(this);
   	VComp = new TSpeedButton(this);
   	VClamp = new TSpeedButton(this);
   	IeqZero = new TSpeedButton(this);
   	IClamp = new TSpeedButton(this);
   	IResist = new TSpeedButton(this);
      IFollow = new TSpeedButton(this);
   	StaticText1 = new TStaticText(this);
   	Setup = new TButton(this);
   
   	Caption = "";
      Width = 205;
      Height = 92;
      BevelInner = bvRaised;
      BorderWidth = 1;
      BorderStyle = bsSingle;
      Constraints->MaxWidth = 205;
      Constraints->MinWidth = 205;
      DragMode = dmAutomatic;
      TabOrder = 0;
   	Bevel1->Name = "Bevel1";
      Bevel1->Parent = this;
      Bevel1->Left = 60;
      Bevel1->Top = 46;
      Bevel1->Width = 87;
      Bevel1->Height = 19;
      CommandUnitsLabel->Name = "CommandUnitsLabel";
      CommandUnitsLabel->Parent = this;
      CommandUnitsLabel->Left = 115;
   	CommandUnitsLabel->Top = 48;
      CommandUnitsLabel->Width = 24;
      CommandUnitsLabel->Height = 13;
      CommandUnitsLabel->Caption = "Units";
      CommandInputBox->Name = "CommandInputBox";
      CommandInputBox->Parent = this;
      CommandInputBox->Left = 62;
      CommandInputBox->Top = 47;
      CommandInputBox->Width = 35;
   	CommandInputBox->Height = 16;
      CommandInputBox->Alignment = taCenter;
      CommandInputBox->Text = "99999";
      AmplifierNameStaticText->Name = "AmplifierNameStaticText";
      AmplifierNameStaticText->Parent = this;
      AmplifierNameStaticText->Left = 60;
      AmplifierNameStaticText->Top = 4;
      AmplifierNameStaticText->Width = 137;
      AmplifierNameStaticText->Height = 17;
   	AmplifierNameStaticText->Alignment = taCenter;
      AmplifierNameStaticText->AutoSize = false;
   	AmplifierNameStaticText->BevelInner = bvLowered;
   	AmplifierNameStaticText->BevelKind = bkSoft;
      AmplifierNameStaticText->Caption = "A-M 2400 Patch Clamp";
      AmplifierNameStaticText->Color = clBtnFace;
      AmplifierNameStaticText->ParentColor = false;
      AmplifierNameStaticText->TabOrder = 1;
      StaticText2->Name = "StaticText2";
      StaticText2->Parent = this;
      StaticText2->Left = 4;
   	StaticText2->Top = 26;
      StaticText2->Width = 31;
      StaticText2->Height = 17;
      StaticText2->BevelInner = bvLowered;
      StaticText2->BevelKind = bkSoft;
      StaticText2->Caption = "Mode";
      StaticText2->TabOrder = 2;
      StaticText3->Name = "StaticText3";
   	StaticText3->Parent = this;
      StaticText3->Left = 4;
      StaticText3->Top = 47;
      StaticText3->Width = 51;
      StaticText3->Height = 17;
      StaticText3->BevelInner = bvLowered;
      StaticText3->BevelKind = bkSoft;
      StaticText3->Caption = "Command";
   	StaticText3->TabOrder = 3;
      CommandUpDown->Name = "CommandUpDown";
      CommandUpDown->Parent = this;
      CommandUpDown->Left = 97;
      CommandUpDown->Top = 47;
      CommandUpDown->Width = 16;
      CommandUpDown->Height = 16;
      CommandUpDown->Associate = CommandInputBox;
   	CommandUpDown->Min = -200;
      CommandUpDown->Max = 200;
      CommandUpDown->Position = 0;
      CommandUpDown->TabOrder = 4;
      CommandUpDown->Thousands = false;
      CommandUpDown->Wrap = false;
   	CommandUpDown->OnChanging = CommandUpDownChanging;
   	StatusBar1->Name = "StatusBar1";
   	StatusBar1->Parent = this;
   	StatusBar1->Left = 3;
   	StatusBar1->Top = 66;
   	StatusBar1->Width = 195;
   	StatusBar1->Height = 19;
   	{
        TStatusPanel *Panel = StatusBar1->Panels->Add();
   	  Panel->Text = "Gain";
   	  Panel->Width = 50;
   	}
   	{
   	  TStatusPanel *Panel = StatusBar1->Panels->Add();
        Panel->Text = "Filter";
   	  Panel->Width = 50;
   	}
      {
   	  TStatusPanel *Panel = StatusBar1->Panels->Add();
   	  Panel->Text = "Cap";
   	  Panel->Width = 50;
   	}
   	{
   	  TStatusPanel *Panel = StatusBar1->Panels->Add();
   	  Panel->Text = "Noise";
   	  Panel->Width = 50;
   	}
   	StatusBar1->ParentFont = true;
   	StatusBar1->SimplePanel = false;
   	StatusBar1->SizeGrip = false;
   	StatusBar1->UseSystemFont = false;
   	ModePanel->Name = "ModePanel";
   	ModePanel->Parent = this;
   	ModePanel->Left = 36;
      ModePanel->Top = 24;
   	ModePanel->Width = 163;
      ModePanel->Height = 21;
   	ModePanel->BevelInner = bvLowered;
      ModePanel->TabOrder = 6;
      ModePanel->Caption = "";
   	VTrack->Name = "VTrack";
      VTrack->Parent = ModePanel;
      VTrack->Left = 4;
   	VTrack->Top = 2;
      VTrack->Width = 21;
      VTrack->Height = 17;
   	VTrack->GroupIndex = 1;
      VTrack->Caption = "Vtk";
      VTrack->Font->Charset = DEFAULT_CHARSET;
   	VTrack->Font->Color = clWindowText;
      VTrack->Font->Height = -9;
      VTrack->Font->Name = "Trebuchet MS";
   	VTrack->Font->Style = TFontStyles() << fsBold;
      VTrack->Layout = blGlyphRight;
      VTrack->ParentFont = false;
   	VComp->Name = "VComp";
      VComp->Parent = ModePanel;
      VComp->Left = 26;
   	VComp->Top = 2;
      VComp->Width = 21;
      VComp->Height = 17;
   	VComp->GroupIndex = 1;
      VComp->Caption = "Vco";
      VComp->Font->Charset = DEFAULT_CHARSET;
   	VComp->Font->Color = clWindowText;
      VComp->Font->Height = -9;
      VComp->Font->Name = "Trebuchet MS";
   	VComp->Font->Style = TFontStyles() << fsBold;
      VComp->Layout = blGlyphRight;
      VComp->ParentFont = false;
   	VClamp->Name = "VClamp";
      VClamp->Parent = ModePanel;
      VClamp->Left = 49;
   	VClamp->Top = 2;
      VClamp->Width = 21;
      VClamp->Height = 17;
   	VClamp->GroupIndex = 1;
      VClamp->Down = true;
      VClamp->Caption = "VC";
   	VClamp->Font->Charset = DEFAULT_CHARSET;
      VClamp->Font->Color = clWindowText;
      VClamp->Font->Height = -9;
   	VClamp->Font->Name = "Trebuchet MS";
      VClamp->Font->Style = TFontStyles() << fsBold;
      VClamp->Layout = blGlyphRight;
   	VClamp->ParentFont = false;
      IeqZero->Name = "IeqZero";
      IeqZero->Parent = ModePanel;
   	IeqZero->Left = 72;
      IeqZero->Top = 2;
      IeqZero->Width = 21;
   	IeqZero->Height = 17;
      IeqZero->GroupIndex = 1;
      IeqZero->Caption = "I=0";
   	IeqZero->Font->Charset = DEFAULT_CHARSET;
      IeqZero->Font->Color = clWindowText;
      IeqZero->Font->Height = -9;
   	IeqZero->Font->Name = "Trebuchet MS";
      IeqZero->Font->Style = TFontStyles() << fsBold;
      IeqZero->Layout = blGlyphRight;
   	IeqZero->ParentFont = false;
      IClamp->Name = "IClamp";
      IClamp->Parent = ModePanel;
   	IClamp->Left = 94;
      IClamp->Top = 2;
      IClamp->Width = 21;
   	IClamp->Height = 17;
      IClamp->GroupIndex = 1;
      IClamp->Caption = "IC";
   	IClamp->Font->Charset = DEFAULT_CHARSET;
   	IClamp->Font->Color = clWindowText;
   	IClamp->Font->Height = -9;
   	IClamp->Font->Name = "Trebuchet MS";
   	IClamp->Font->Style = TFontStyles() << fsBold;
   	IClamp->Layout = blGlyphRight;
      IClamp->ParentFont = false;
   	IResist->Name = "IResist";
      IResist->Parent = ModePanel;
      IResist->Left = 117;
   	IResist->Top = 2;
      IResist->Width = 21;
      IResist->Height = 17;
   	IResist->GroupIndex = 1;
      IResist->Caption = "Ires";
      IResist->Font->Charset = DEFAULT_CHARSET;
   	IResist->Font->Color = clWindowText;
   	IResist->Font->Height = -9;
   	IResist->Font->Name = "Trebuchet MS";
      IResist->Font->Style = TFontStyles() << fsBold;
   	IResist->Layout = blGlyphRight;
      IResist->ParentFont = false;
      IFollow->Name = "IFollow";
   	IFollow->Parent = ModePanel;
      IFollow->Left = 140;
      IFollow->Top = 2;
      IFollow->Width = 21;
   	IFollow->Height = 17;
      IFollow->GroupIndex = 1;
      IFollow->Caption = "Ifol";
      IFollow->Font->Charset = DEFAULT_CHARSET;
      IFollow->Font->Color = clWindowText;
   	IFollow->Font->Height = -9;
      IFollow->Font->Name = "Trebuchet MS";
      IFollow->Font->Style = TFontStyles() << fsBold;
      IFollow->Layout = blGlyphRight;
   	IFollow->ParentFont = false;
   	StaticText1->Name = "StaticText1";
   	StaticText1->Parent = this;
   	StaticText1->Left = 4;
      StaticText1->Top = 4;
   	StaticText1->Width = 53;
      StaticText1->Height = 17;
   	StaticText1->BevelInner = bvLowered;
      StaticText1->BevelKind = bkSoft;
      StaticText1->Caption = "Amp./Inst.";
   	StaticText1->TabOrder = 7;
      Setup->Name = "Setup";
      Setup->Parent = this;
   	Setup->Left = 152;
      Setup->Top = 46;
      Setup->Width = 45;
   	Setup->Height = 17;
      Setup->Caption = "Setup";
   	Setup->TabOrder = 8;
		Setup->OnClick = SetupClick;
*/
}
//---------------------------------------------------------------------------
namespace Amplifier
{
	void __fastcall PACKAGE Register()
	{
		 TComponentClass classes[1] = {__classid(TAmplifier)};
		 RegisterComponents("DAQ", classes, 0);
   }
}
//---------------------------------------------------------------------------

void __fastcall TAmplifier::SetSignals(TSignalCollection *signals)
{
   FSignals->Assign(signals);
}
//---------------------------------------------------------------------------

void __fastcall TAmplifier::SetIsInEquipRack(bool set)
{
   if (FIsInEquipRack != set) FIsInEquipRack = set;
}
//---------------------------------------------------------------------------

/*
   void __fastcall TAmplifier::SetModes(TStringList *modes)
   {
   	FModes->Assign(modes);
   }
   //---------------------------------------------------------------------------
   
   void __fastcall TAmplifier::SetModeVolts(TStringList *volts)
   {
   	FModeVolts->Assign(volts);
   }
   //---------------------------------------------------------------------------
   
   void __fastcall TAmplifier::SetGains(TStringList *gains)
   {
   	FGains->Assign(gains);
   }
   //---------------------------------------------------------------------------
   
   void __fastcall TAmplifier::SetGainVolts(TStringList *volts)
   {
   	FGainVolts->Assign(volts);
   }
   //---------------------------------------------------------------------------
   
   void __fastcall TAmplifier::SetFilters(TStringList *filters)
   {
   	FFilters->Assign(filters);
   }
   //---------------------------------------------------------------------------
   
   void __fastcall TAmplifier::SetFilterVolts(TStringList *volts)
   {
   	FFilterVolts->Assign(volts);
	}
*/
//---------------------------------------------------------------------------

void __fastcall TAmplifier::SetAmpMode(int set)
{
	DoSetAmpMode(set);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TAmplifier::DoSetAmpMode(int set)
{
   if (FAmpMode != set){
      FAmpMode = set;
//      if (FOnModeChanged) FOnModeChanged(this);
   }
}
//---------------------------------------------------------------------------

void __fastcall TAmplifier::ProcessADCData(short *buf, unsigned long numsamples,
                                          TSignal *Signal)
{
   if (FOnProcessADCData) FOnProcessADCData(this, buf, numsamples, Signal); 
}
//---------------------------------------------------------------------------

void __fastcall TAmplifier::ProcessModeADCData(short *buf, unsigned long numsamples,
                                             TSignal *Signal)
{

}
//---------------------------------------------------------------------------

float __fastcall TAmplifier::GainFromTelegraph(short *buf, unsigned long numsamples,
                                             TSignal *Signal)
{

}
//---------------------------------------------------------------------------

float __fastcall TAmplifier::FilterFromTelegraph(short *buf, unsigned long numsamples,
                                             TSignal *Signal)
{
   
}
//---------------------------------------------------------------------------
 /*
    //---------------------------------------------------------------------------
    void __fastcall TAmplifier::SetupClick(TObject *Sender)
    {
    	CommandInputBox->Text = "0";
    }
    //---------------------------------------------------------------------------
    void __fastcall TAmplifier::CommandUpDownChanging(TObject *Sender,
    		bool &AllowChange)
    {
    	CommandInputBox->Text = CommandUpDown->Position;
    }
    //---------------------------------------------------------------------------

 */
