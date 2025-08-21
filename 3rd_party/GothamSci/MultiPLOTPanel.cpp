//---------------------------------------------------------------------------

//#include <basepch.h>
#include <vcl.h>

#pragma hdrstop

#include "tools.h"
#include "MultiPLOTPanel.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------

__fastcall TDAQFloat::TDAQFloat(TCollection *Collection) :
                                    TCollectionItem(Collection)
{
}
//---------------------------------------------------------------------------

String __fastcall TDAQFloat::GetDisplayName(void)
{
   return FName == "" ? TCollectionItem::GetDisplayName() : FName;
}
//---------------------------------------------------------------------------

void __fastcall TDAQFloat::Assign(TPersistent* Source)
{
   TDAQFloat *df = dynamic_cast<TDAQFloat *>(Source);
   if (df){
      Name = df->Name;
      Value = df->Value;
   }
   else TPersistent::Assign(Source);
}
//---------------------------------------------------------------------------

void __fastcall TDAQFloat::SetName(String name)
{
   if (FName != name) FName = name;
}
//---------------------------------------------------------------------------

__fastcall TSplitterPositions::TSplitterPositions(TComponent *owner) :
                                 TCollection(__classid(TDAQFloat))
{
   FComponentOwner = owner;
}
//---------------------------------------------------------------------------

TDAQFloat* __fastcall TSplitterPositions::Add()
{
   TDAQFloat *df = NULL;
   df = dynamic_cast<TDAQFloat*>(TCollection::Add());
   return df;
}
//---------------------------------------------------------------------------

TDAQFloat* __fastcall TSplitterPositions::GetItem(int Index)
{
   return dynamic_cast<TDAQFloat*>(TCollection::GetItem(Index));
}
//---------------------------------------------------------------------------

void __fastcall TSplitterPositions::SetItem(int Index, TDAQFloat *Value)
{
   TCollection::SetItem(Index, Value);
}
//---------------------------------------------------------------------------

TPersistent* __fastcall TSplitterPositions::GetOwner(void)
{
   return (TPersistent *)FComponentOwner;
}
//---------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMultiPLOTPanel *)
{
   new TMultiPLOTPanel(NULL);
}


//---------------------------------------------------------------------------
__fastcall TMultiPLOTPanel::TMultiPLOTPanel(TComponent* Owner)
   : TCustomPanel(Owner)
{
	PlottingThread = NULL;
	FRTPlotEnabled = false;

   FSplitterPositions = new TSplitterPositions(this);
   
	FNumVisiblePlots = 0;

	FTileDirection = tdHorizontal;

   OnResize = PanelResize;
   OnMouseDown = PanelMouseDown;
   OnMouseMove = PanelMouseMove;
   OnMouseUp = PanelMouseUp;

   Color = clBtnFace;
   FSplitterThickness = 5;

   String name = "_DAQPanel";
   String num = "";

   for (int i=0; i<MAXPANELS; i++){
		PLOTPanel[i] = new TPLOTPanel(this);
		PLOTPanel[i]->Parent = this;
		PLOTPanel[i]->SetSubComponent(true);
		num = IntToStr(i) + "_";
		PLOTPanel[i]->Name = name + num;
		PLOTPanel[i]->Tag = i;
		PLOTPanel[i]->Height = 0;
		PLOTPanel[i]->Width = 0;
		PLOTPanel[i]->OnAxesColorChange = PLOTPanelAxesColorChange;
		PLOTPanel[i]->OnAxesFontChange = PLOTPanelAxesFontChange;
		PLOTPanel[i]->OnBackgroundColorChange = PLOTPanelBackgroundColorChange;
		PLOTPanel[i]->OnGridLinesChange = PLOTPanelGridLinesChange;
		PLOTPanel[i]->OnGridLinesColorChange = PLOTPanelGridLinesColorChange;
		PLOTPanel[i]->OnPenColorChange = PLOTPanelPenColorChange;
		PLOTPanel[i]->OnMouseModeChange = PLOTPanelMouseModeChange;
		PLOTPanel[i]->OnXAxisChange = PLOTPanelXAxisChange;
		PLOTPanel[i]->OnYAxisChange = PLOTPanelYAxisChange;
		PLOTPanel[i]->OnOverrun = PLOTPanelOverrun;

//      PercentTop[i] = 1;
		TDAQFloat *df = FSplitterPositions->Add();
		if (df) df->Value = 1;
	}
	SplitterIndex = 0;

	InitializeCriticalSection(&Plot_CS);
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::Loaded()
{
   TControl::Loaded();

//   ResizePlots();

   if (!ComponentState.Contains(csDesigning)){
//		for (int i=0; i<FSplitterPositions->Count; i++)
//			PercentTop[i] = FSplitterPositions->Items[i]->Value;
////
//	  PlottingThread = new TPlottingThread(this, true);
//	  PlottingThread->OnTerminate = PlottingThreadTerminated;
//	  PlottingThread->FreeOnTerminate = true;
//	  PlottingThread->Resume();

		if (FIsRealTimePlot) RTPlotEnabled = true;
	}

   ResizePlots();
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::BeforeDestruction()
{
	TComponent::BeforeDestruction();
	if (PlottingThread){
		PlottingThread->Terminate();
//		PlottingThread->WaitFor();
//		delete PlottingThread;
	}
	DeleteCriticalSection(&Plot_CS);
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::WriteConfigFile(String filename)
{
//	FSplitterPositions->Clear();
//	if (!ComponentState.Contains(csDesigning)){
//		for (int i=0; i<FNumVisiblePlots; i++){
//			TDAQFloat *df = FSplitterPositions->Add();
//			if (df) df->Value = PercentTop[i];
//		}
//	}
   SaveComponent(filename, this);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
namespace Multiplotpanel
{
   void __fastcall PACKAGE Register()
   {
	   TComponentClass classes[1] = {__classid(TMultiPLOTPanel)};
	   RegisterComponents("Gotham Scientific", classes, 0);
   }
}

//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::ResizePlots()
{
	if (FIsRealTimePlot && !ComponentState.Contains(csDesigning)){
//		if (PlottingThread) PlottingThread->Suspend();
		EnterCriticalSection(&Plot_CS);
	}

	if (FNumVisiblePlots == 1)
		PLOTPanel[0]->SetBounds(0, 0, Width, Height);

	if (FNumVisiblePlots > 1){
		switch (FTileDirection) {

			case tdVertical: {
				PLOTPanel[0]->BoundsRect = Rect(0, 0, round(FSplitterPositions->Items[1]->Value*Width)- FSplitterThickness,	Height );

				for (int i=1; i<FNumVisiblePlots-1; i++){
					PLOTPanel[i]->BoundsRect =
								Rect(round(FSplitterPositions->Items[i]->Value*Width), 0,
									 round(FSplitterPositions->Items[i+1]->Value*Width) - FSplitterThickness,
									 Height);
				}
				PLOTPanel[FNumVisiblePlots-1]->BoundsRect =
							Rect(round(FSplitterPositions->Items[FNumVisiblePlots-1]->Value*Width), 0,
								 Width, Height);

				Cursor = crHSplit;
			} break;

			case tdHorizontal: {
				PLOTPanel[0]->BoundsRect = Rect(0, 0, Width,
					round(FSplitterPositions->Items[1]->Value*Height) - FSplitterThickness);
				for (int i=1; i<FNumVisiblePlots-1; i++){
					PLOTPanel[i]->BoundsRect =
								Rect(0, round(FSplitterPositions->Items[i]->Value*Height),
									 Width,
									 round(FSplitterPositions->Items[i+1]->Value*Height) - FSplitterThickness);
				}
				PLOTPanel[FNumVisiblePlots-1]->BoundsRect =
							Rect(0, round(FSplitterPositions->Items[FNumVisiblePlots-1]->Value*Height),
								 Width, Height);

				Cursor = crVSplit;
			} break;
			
		}

		Color = FSplitterColor;
	}
	else {
		Cursor = crDefault;
	}

	if (FIsRealTimePlot && !ComponentState.Contains(csDesigning)){
//		if (PlottingThread) PlottingThread->Resume();
		LeaveCriticalSection(&Plot_CS);
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                            Property Access
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void __fastcall TMultiPLOTPanel::SetSplitterPositions(TSplitterPositions *pos)
{
   if (!ComponentState.Contains(csDesigning)){
      FSplitterPositions->Assign(pos);
//      FNumVisiblePlots = FSplitterPositions->Count;
//      for (int i=0; i<MAXPANELS; i++)
//        PercentTop[i] = FSplitterPositions->Items[i]->Value;
      ResizePlots();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetSplitterThickness(unsigned short thickness)
{
   FSplitterThickness = thickness;
   ResizePlots();
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetSplitterColor(TColor color)
{
	Color = FSplitterColor = color;
   ResizePlots();
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetNumVisiblePlots(int np)
{
   if (FNumVisiblePlots == np) return;
   if (np < 0) np = 0;
   if (np > MAXPANELS){
      np = MAXPANELS;
   }

   FNumVisiblePlots = np;
	if (ComponentState.Contains(csReading)) return; // This line prevents FSplitterPositions
																	// from being reset when reading in
																	// saved component.

	for (int i=FNumVisiblePlots; i<MAXPANELS; i++){
		switch (FTileDirection) {
			case tdVertical: PLOTPanel[i]->Height = 0; break;
			case tdHorizontal: PLOTPanel[i]->Width = 0; break;
			case NumTileDirections: break;
		}
		FSplitterPositions->Items[i]->Value = 1;
	}

   double percent = 0;
   if (FNumVisiblePlots != 0) percent = 1.0/((double)(FNumVisiblePlots));
   for (int i=0; i<FNumVisiblePlots; i++){
		FSplitterPositions->Items[i]->Value = i;
      FSplitterPositions->Items[i]->Value *= percent;
	}

   ResizePlots();
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetGlobalChanges(bool set)
{
	FGlobalChanges = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetMouseMode(MouseModeEnum mode)
{
	FMouseMode = mode;
	for (int i=0; i<MAXPANELS; i++)
		PLOTPanel[i]->SetMouseMode(mode);
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetDrawEraseExtra(int extra)
{
	FDrawEraseExtra = extra;
	for (int i=0; i<MAXPANELS; i++)
		PLOTPanel[i]->DrawEraseExtra = extra;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetIgnoreOverrun(bool set)
{
	FIgnoreOverrun = set;
   for (int i=0; i<MAXPANELS; i++)
	  PLOTPanel[i]->IgnoreOverrun = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetIsRealTimePlot(bool set)
{
	FIsRealTimePlot = set;
	for (int i=0; i<MAXPANELS; i++)
		PLOTPanel[i]->IsRealTimePlot = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::SetTileDirection(TileDirectionEnum td)
{
	FTileDirection = td;
	int np = FNumVisiblePlots;
	NumVisiblePlots = 0;
	NumVisiblePlots = np;
}

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                            Event Handlers
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void __fastcall TMultiPLOTPanel::PanelMouseDown(TObject* Sender, TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y)
{
	int i = -1;
	switch (FTileDirection) {
		case tdVertical: {
			for (i=0; i<MAXPANELS; i++){
				if (X < PLOTPanel[i]->Left) break;
			};
		} break;
      case tdHorizontal: {
			for (i=0; i<MAXPANELS; i++){
				if (Y < PLOTPanel[i]->Top) break;
			};
		} break;
      case NumTileDirections: break;
	default:
		;
	}

   SplitterIndex = i;
   MouseLBDown = true;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PanelMouseMove(TObject* Sender,
                                    Classes::TShiftState Shift, int X, int Y)
{
	double value = 1.0;
	if (MouseLBDown){
		switch (FTileDirection) {
			case tdVertical: value = ((double)X)/((double)Width); break;
			case tdHorizontal: value = ((double)Y)/((double)Height); break;
			case NumTileDirections: break;
      default:
			;
		}
		FSplitterPositions->Items[SplitterIndex]->Value = value;
      ResizePlots();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PanelMouseUp(TObject* Sender, TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y)
{  /* TODO : problem with timelocked reset when resize panels */
	//add this so if timelocked panel moved,
	//other time locked panels will be synchronized
	//PLOTPanel[SplitterIndex]->  XAxisHasChanged = true;
	PLOTPanel[SplitterIndex]->FinalUpdate(true);
	MouseLBDown = false;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PanelResize(TObject *Sender)
{
	for (int i=FNumVisiblePlots; i<MAXPANELS; i++){
		switch (FTileDirection) {
			case tdVertical: PLOTPanel[i]->Height = 0; break;
			case tdHorizontal: PLOTPanel[i]->Width = 0; break;
			case NumTileDirections: break;
		}
	}
	ResizePlots();
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelAxesColorChange(
	  TObject *Sender)
{
   if (FGlobalChanges){
	  TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
	  TColor color = panel->AxesColor;
	  for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->AxesColor = color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelAxesFontChange(
      TObject *Sender)
{
   if (FGlobalChanges){
      TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
      TFont *font = panel->AxesFont;
      for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->AxesFont = font;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelBackgroundColorChange(
      TObject *Sender)
{
   if (FGlobalChanges){
      TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
      TColor color = panel->BackgroundColor;
      for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->BackgroundColor = color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelGridLinesChange(
      TObject *Sender)
{
   if (FGlobalChanges){
      TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
      bool haslines = panel->EnableGridLines;
      for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->EnableGridLines = haslines;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelGridLinesColorChange(
      TObject *Sender)
{
   if (FGlobalChanges){
      TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
      TColor color = panel->GridLinesColor;
      for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->GridLinesColor = color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelPenColorChange(
      TObject *Sender)
{
   if (FGlobalChanges){
      TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
      TColor color = panel->PenColor;
	  for (int i=0; i<MAXPANELS; i++)
		 PLOTPanel[i]->PenColor = color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelMouseModeChange(
      TObject *Sender)
{
   TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
   MouseModeEnum mode = panel->MouseMode;
   for (int i=0; i<MAXPANELS; i++)
	  PLOTPanel[i]->MouseMode = mode;
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelXAxisChange(TObject *Sender)
{
   TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
   if (panel->TimeLocked){
		if (FIsRealTimePlot) {
//			if (PlottingThread) PlottingThread->Suspend();
			EnterCriticalSection(&Plot_CS);
		}

		double xmin = panel->VisibleMinX;
      double xmax = panel->VisibleMaxX;
		for (int i=0; i<MAXPANELS; i++){
			if (PLOTPanel[i]->TimeLocked && PLOTPanel[i]->Visible /*(PLOTPanel[i] != panel) &&*/ ){
			PLOTPanel[i]->BeginUpdate();
				PLOTPanel[i]->VisibleMinX = xmin;
				PLOTPanel[i]->VisibleMaxX = xmax;
			PLOTPanel[i]->EndUpdate();
				PLOTPanel[i]->FinalUpdate(true);
			}
      }

	  if (FIsRealTimePlot){
//		if (PlottingThread) PlottingThread->Resume();
		LeaveCriticalSection(&Plot_CS);
	  }
	}
	if (FOnXAxisChange) FOnXAxisChange(Sender); //relay (PLOTPanel*)Sender to event handler
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelYAxisChange(TObject *Sender)
{
	TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);
   if (panel->TimeLocked){
		if (FIsRealTimePlot) {
//			if (PlottingThread) PlottingThread->Suspend();
			EnterCriticalSection(&Plot_CS);
		}

		for (int i=0; i<MAXPANELS; i++){
			if (PLOTPanel[i]->TimeLocked  && PLOTPanel[i]->ResetUponDraw){
				PLOTPanel[i]->FinalUpdate(true);//Reset = true;
			}
		}

		if (FIsRealTimePlot) {
//			if (PlottingThread) PlottingThread->Resume();
			LeaveCriticalSection(&Plot_CS);
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PLOTPanelOverrun(TObject *Sender,
		bool &DoHandleOverrun)
{
	if (PlottingThread) {
		if (PlottingThread->Suspended) return;
	}


   TPLOTPanel *panel = dynamic_cast<TPLOTPanel *>(Sender);


	if (FIsRealTimePlot) EnterCriticalSection(&Plot_CS);
//	PlottingThread->Suspend();

	double xmax = panel->VisibleMaxX * 2;

   if (panel->TimeLocked){
		DoHandleOverrun = false;
      for (int i=0; i<MAXPANELS; i++){
		 if (PLOTPanel[i]->TimeLocked){
			PLOTPanel[i]->VisibleMaxX = xmax;
			PLOTPanel[i]->Reset = true;
         }
      }
   }
   else{
      DoHandleOverrun = true;  //TODO: Make DoHandleOverrun mean something
      panel->VisibleMaxX = xmax; // we handle it here, externally
      panel->Reset = true;
   }

   if (FIsRealTimePlot) {
		if (PlottingThread) LeaveCriticalSection(&Plot_CS);
		//	PlottingThread->Resume();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMultiPLOTPanel::PlottingThreadTerminated(TObject *Sender)
{
//	Application->MessageBoxA(L"PlottingThread Terminated", Name.w_str());
	PlottingThread = NULL;
}
//---------------------------------------------------------------------------

void __fastcall	TMultiPLOTPanel::SetRTPlotEnabled(bool set)
{
	//regardless of set T/F, get rid of thread
	if (!set && PlottingThread) {
		PlottingThread->Terminate();
//		PlottingThread->WaitFor();
		FRTPlotEnabled = false;
	}
	// only if set is true do we create the thread
	if (set) {
		PlottingThread = new TPlottingThread(this, true);
		PlottingThread->OnTerminate = PlottingThreadTerminated;
		PlottingThread->FreeOnTerminate = true;
		PlottingThread->Start();
		FRTPlotEnabled = true;
	}
}

