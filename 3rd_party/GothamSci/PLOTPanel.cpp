//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include <algorithm>
#include <stdio.h>
#include "tools.h"
#include "GR32_Blend.hpp"
#include "GR32_Polygons.hpp"
#include "PLOTPanel.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TPLOTPanel *)
{
   new TPLOTPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TPLOTPanel::TPLOTPanel(TComponent* Owner)
   : TCustomPanel(Owner)
{
   FFullScaleMinY = -100;
   FVisibleMinY = -100;
   FFullScaleMaxY = 100;
   FVisibleMaxY = 100;
   FFullScaleMaxX = 60;
   FVisibleMaxX = 1;
   FFullScaleMinX = 0;
   FVisibleMinX = 0;
	FMouseMode = DEFAULTMOUSEMODE;

	FEnableGridLines = true;
	FPenColor = clRed32;
	FBackgroundColor = clBlack32;
	FAxesColor = Color32(clBtnFace);
	FGridLinesColor = clGreen32;

	FAxesFont = new TFont;
	FAxesFont->Name = "Arial";
	FAxesFont->Height = 16;
	FAxesFont->Color = clBlack;
	FAxesFont->OnChange = AxesFontChanged;

	DelayRedraw = false;
	ZoomColor = clWhite32-FBackgroundColor+0x7F000000;
	IsResizing = 0;
	MouseLBDown = false;
	MouseHasMoved = false;
	NumXGridLines = 0;
	NumYGridLines = 0;
	XAxisHasChanged = false;
	YAxisHasChanged = false;
	NumStoredViewports = 0;
	MouseCanvas = NULL;
	Reset = false;
	IsAHit = false;

	XDecimalPlaces = 1;
	YDecimalPlaces = 1;

	NumEventTags = 0;
	NumPlots = 0;

	FEnableTraceStorage = false;
	FTraceStorageFade = 0x0;
	FDrawEraseExtra = DEFAULTDRAWERASEEXTRA;
	FIgnoreOverrun = DEFAULTIGNOREOVERRUN;

	FConcatenatePlots = false;
	FResetUponDraw = true;

	NumBuffersAllocated = 0;
	NumBuffersPerTrace = 0;
	NumBuffersPlotted = 0;

	XTimeProgress = 0;
	YSampleLast = 0;

	OverRun = false;

	FullRepaint = false;
	Height = 50;
   Width = 100;
   TagPanel = new TPanel(this);
   TagBox = new TPaintBox32(this);
   TagLabel = new TLabel(this);
   InfoPanel = new TPanel(this);
   XAxis = new TPaintBox32(this);
   YAxis = new TPaintBox32(this);
   Graph = new TPaintBox32(this);
   YMaxInputBox = new TInputBox(this);
   YMinInputBox = new TInputBox(this);
   XMaxInputBox = new TInputBox(this);
   XMinInputBox = new TInputBox(this);
   ZoomButton = new TButton(this);
   DisplayButton = new TButton(this);
   TimeLockCheckBox = new TCheckBox(this);
   ResizeTimer = new TTimer(this);
   DisplayPopupMenu = new TPopupMenu(this);
   ShowGridLines1 = new TMenuItem(this);
   PenColor1 = new TMenuItem(this);
   BackgroundColor1 = new TMenuItem(this);
   AxesColor1 = new TMenuItem(this);
   GridLinesColor1 = new TMenuItem(this);
   Font1 = new TMenuItem(this);
   ZoomPopupMenu = new TPopupMenu(this);
   XZoom1 = new TMenuItem(this);
   YZoom1 = new TMenuItem(this);
   XYZoom1 = new TMenuItem(this);
   OFF1 = new TMenuItem(this);
   ColorDialog1 = new TColorDialog(this);
   FontDialog1 = new TFontDialog(this);

   TagPanel->Parent = this;
   TagPanel->Align = alTop;
   TagPanel->Caption = "";
   TagPanel->Height = 16;
//   TagPanel->BevelInner = bvLowered;

   InfoPanel->Parent = this;
   InfoPanel->Align = alBottom;
   InfoPanel->Caption = "";
   InfoPanel->BevelInner = bvLowered;

   XAxis->Parent = this;
   XAxis->Align = alBottom;
   XAxis->Buffer->Font->Assign(Font);

   YAxis->Parent = this;
   YAxis->Align = alLeft;
   YAxis->Buffer->Font->Assign(Font);
   XAxis->Height = XAxis->Buffer->TextHeight("Bubba2341") + 2;
   YAxis->Width = YAxis->Buffer->TextWidth("Bubba2341");

   TagBox->Parent = TagPanel;
   TagBox->Top = 1;
   TagBox->Height = TagPanel->Height - 2;
   TagBox->Left = YAxis->Width;
   TagBox->Width = TagPanel->Width - YAxis->Width;
   TAnchors anchors;
   anchors << akLeft << akRight << akTop << akBottom;
   TagBox->Anchors = anchors;
   TagBox->OnMouseDown = TagBoxMouseDown;
   TagBox->OnMouseMove = TagBoxMouseMove;

   TagLabel->Parent = TagPanel;
   TagLabel->Top = 1;
   TagLabel->Height = TagPanel->Height - 2;
   TagLabel->Caption = "Tags: ";
   TagLabel->Alignment = taRightJustify;
   TagLabel->Left = 1;
   TagLabel->Width = YAxis->Width;
//   anchors.Clear();
//   anchors << akLeft << akRight;
//   TagLabel->Anchors = anchors;

   Graph->Parent = this;
   Graph->Align = alClient;
   Graph->Buffer->Font->Assign(Font);
   Graph->BufferOversize = 0;
   Graph->OnMouseDown = GraphMouseDown;
   Graph->OnMouseMove = GraphMouseMove;
   Graph->OnMouseUp = GraphMouseUp;
   Graph->OnDblClick = GraphDblClick;

   XMinInputBox->Parent = InfoPanel;
   XMinInputBox->Top = 1;
   XMinInputBox->Left = YAxis->Width - XMinInputBox->Width;
   XMinInputBox->Text = FloatToStrF(FVisibleMinX, ffFixed, 7, 2);
   XMinInputBox->OnInput = XMinInputBoxInput;
   XMinInputBox->ChangedAlert = false;

   XMaxInputBox->Parent = InfoPanel;
   XMaxInputBox->Top = 1;
   XMaxInputBox->Left = InfoPanel->Width - XMaxInputBox->Width;
   anchors.Clear();
   anchors << akTop << akRight;
   XMaxInputBox->Anchors = anchors;
   XMaxInputBox->Text = FloatToStrF(FVisibleMaxX, ffFixed, 7, 2);
   XMaxInputBox->OnInput = XMaxInputBoxInput;
   XMaxInputBox->ChangedAlert = false;

   InfoPanel->Height = XMaxInputBox->Height + 4;

   TimeLockCheckBox->Parent = InfoPanel;
   TimeLockCheckBox->Caption = "Lock";
   TimeLockCheckBox->Alignment = taLeftJustify;
   TimeLockCheckBox->Width = 45;
   TimeLockCheckBox->Top = 2;
   TimeLockCheckBox->Left = XMaxInputBox->Left - TimeLockCheckBox->Width - 1;
   TimeLockCheckBox->Anchors = anchors;

   YMaxInputBox->Parent = this;
   YMaxInputBox->Top = TagPanel->Height + 1;
   YMaxInputBox->Left = 1;
   YMaxInputBox->Width = YAxis->Width-1;
   YMaxInputBox->OnInput = YMaxInputBoxInput;
   YMaxInputBox->ChangedAlert = false;

   YMinInputBox->Parent = this;
   YMinInputBox->Left = 1;
   YMinInputBox->Width = YAxis->Width-1;
   YMinInputBox->Top = XAxis->Top - YMinInputBox->Height - 1;
   anchors.Clear();
   anchors << akBottom << akLeft;
   YMinInputBox->Anchors = anchors;
   YMinInputBox->OnInput = YMinInputBoxInput;
   YMinInputBox->ChangedAlert = false;

   ZoomButton->Parent = this;
   DisplayButton->Parent = this;
   ZoomButton->Height = 15;
   ZoomButton->Width = 15;
   DisplayButton->Height = 15;
   DisplayButton->Width = 15;
   ZoomButton->Caption = "z";
   DisplayButton->Caption = "d";
   ZoomButton->Left = 1;
   DisplayButton->Left = ZoomButton->Width + 1;
   DisplayButton->Top = YMinInputBox->Top - DisplayButton->Height - 1;
   ZoomButton->Top = DisplayButton->Top;
   ZoomButton->Anchors = anchors;
   DisplayButton->Anchors = anchors;
   ZoomButton->OnClick = ZoomButtonClick;
   DisplayButton->OnClick = DisplayButtonClick;

   ShowGridLines1->Caption = "Show Grid Lines";
   ShowGridLines1->OnClick = ShowGridLines1Click;
   GridLinesColor1->Caption = "Grid Lines Color";
   GridLinesColor1->OnClick = GridLinesColor1Click;
   BackgroundColor1->Caption = "Background Color";
   BackgroundColor1->OnClick = BackgroundColor1Click;
   AxesColor1->Caption = "Axes Color";
   AxesColor1->OnClick = AxesColor1Click;
   PenColor1->Caption = "Data Color";
   PenColor1->OnClick = PenColor1Click;
   Font1->Caption = "Axes Font";
   Font1->OnClick = Font1Click;
   DisplayPopupMenu->Items->Add(ShowGridLines1);
   DisplayPopupMenu->Items->Add(GridLinesColor1);
   DisplayPopupMenu->Items->Add(BackgroundColor1);
   DisplayPopupMenu->Items->Add(AxesColor1);
   DisplayPopupMenu->Items->Add(Font1);
   DisplayPopupMenu->Items->Add(PenColor1);
   ShowGridLines1->Checked = true;

   XZoom1->Caption = "XZoom";
   XZoom1->OnClick = XZoom1Click;
   YZoom1->Caption = "YZoom";
   YZoom1->OnClick = YZoom1Click;
   XYZoom1->Caption = "XYZoom";
   XYZoom1->OnClick = XYZoom1Click;
   OFF1->Caption = "OFF";
   OFF1->OnClick = OFF1Click;
   ZoomPopupMenu->Items->Add(XZoom1);
   ZoomPopupMenu->Items->Add(YZoom1);
   ZoomPopupMenu->Items->Add(XYZoom1);
//   ZoomPopupMenu->Items->Add(OFF1);
//   OFF1->Checked = FMouseMode == MouseIdle;

    ResizeTimer->OnTimer = ResizeTimerTimer;
    ResizeTimer->Enabled = false;
    ResizeTimer->Interval = 50;

    TimeLockCheckBox->OnClick = TimeLockCheckBoxClick;
}

__fastcall TPLOTPanel::~TPLOTPanel()
{
	FreeBuffers();
   ClearPlots();
}

void __fastcall TPLOTPanel::Loaded(void)
{
   Draw(true);
}

void __fastcall TPLOTPanel::AfterConstruction()
{
//   Draw(true);
}

void __fastcall TPLOTPanel::BeginUpdate()
{
	DelayRedraw = true;
}

void __fastcall TPLOTPanel::EndUpdate()
{
	DelayRedraw = false;
}

void __fastcall TPLOTPanel::FinalUpdate(bool flush)
{
	Draw(flush);
}

//---------------------------------------------------------------------------
namespace Plotpanel
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TPLOTPanel)};
	   RegisterComponents("Gotham Scientific", classes, 0);
   }
}

//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//										Event Response Functions
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::ResizeTimerTimer(TObject *Sender)
{
   if (IsResizing > 0) IsResizing--;
   else {
		ResizeTimer->Enabled = false;
      YMinInputBox->Top = YAxis->Height - YMinInputBox->Height - 1;
      Draw(true);
     	DoEndResize();
      // if Graph is resized, MouseCanvas->Handle is invalid -- must recreate
      if (FMouseMode > XYZoom && FMouseMode < XYPan) { // MarkX, MarkY, MarkLine
         MouseCanvas->Handle = Graph->Buffer->Handle;
         MouseCanvas->Pen->Color = clBlack;
         MouseCanvas->Pen->Mode = pmNotXor;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::GraphMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   DoMouseDown(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::GraphMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   DoMouseMove(Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::GraphMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   DoMouseUp(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::GraphDblClick(TObject *Sender)
{
   if (FIsRealTimePlot){
      FVisibleMaxY = FFullScaleMaxY;
		FVisibleMinY = FFullScaleMinY;
		YAxisHasChanged = true;
      Draw(true);
   }
   else {
      NumStoredViewports--;
      if (NumStoredViewports < 0){
         NumStoredViewports = 0;
         FVisibleMinX = FFullScaleMinX;
         FVisibleMinY = FFullScaleMinY;
         FVisibleMaxX = FFullScaleMaxX;
         FVisibleMaxY = FFullScaleMaxY;
      }
      else {
         FVisibleMinX = Viewports[NumStoredViewports].MinX;
         FVisibleMinY = Viewports[NumStoredViewports].MinY;
         FVisibleMaxX = Viewports[NumStoredViewports].MaxX;
         FVisibleMaxY = Viewports[NumStoredViewports].MaxY;
      }
		XAxisHasChanged = true;
		YAxisHasChanged = true;
		Draw(true);
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::YMaxInputBoxInput(TObject *Sender)
{
   BeginUpdate(); // inhibit changes for now (DelayRedraw = true)
   VisibleMaxY = StrToFloat(YMaxInputBox->Text);
   bool failedtochange = !FloatsAreEqual(VisibleMaxY, StrToFloat(YMaxInputBox->Text));
   EndUpdate();// (DelayRedraw = false;
   FinalUpdate(true); // redraw
   if (failedtochange) YMaxInputBox->Flash(2);
   else NumStoredViewports = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::YMinInputBoxInput(TObject *Sender)
{
   BeginUpdate(); // inhibit changes for now (DelayRedraw = true)
   VisibleMinY = StrToFloat(YMinInputBox->Text);
   bool failedtochange = !FloatsAreEqual(VisibleMinY, StrToFloat(YMinInputBox->Text));
   EndUpdate();// (DelayRedraw = false;
   FinalUpdate(true); // redraw
   if (failedtochange) YMinInputBox->Flash(2);
   else NumStoredViewports = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::XMinInputBoxInput(TObject *Sender)
{
   BeginUpdate(); // inhibit changes for now (DelayRedraw = true)
   VisibleMinX = StrToFloat(XMinInputBox->Text);
   bool failedtochange = !FloatsAreEqual(VisibleMinX, StrToFloat(XMinInputBox->Text));
   EndUpdate();// (DelayRedraw = false;
   FinalUpdate(true); // redraw
   if (failedtochange) XMinInputBox->Flash(2);
   else NumStoredViewports = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::XMaxInputBoxInput(TObject *Sender)
{
   BeginUpdate(); // inhibit changes for now (DelayRedraw = true)
   VisibleMaxX = StrToFloat(XMaxInputBox->Text);
   bool failedtochange = !FloatsAreEqual(VisibleMaxX, StrToFloat(XMaxInputBox->Text));
   EndUpdate();// (DelayRedraw = false;
   FinalUpdate(true); // redraw
   if (failedtochange) XMaxInputBox->Flash(2);
   else NumStoredViewports = 0;
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::ZoomButtonClick(TObject *Sender)
{
   Types::TPoint point(ZoomButton->Left, ZoomButton->Top);
   point = ClientToScreen(point);
   ZoomPopupMenu->Popup(point.x, point.y);
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::DisplayButtonClick(TObject *Sender)
{
   Types::TPoint point(DisplayButton->Left, DisplayButton->Top);
   point = ClientToScreen(point);
   DisplayPopupMenu->Popup(point.x, point.y);
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::TagBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
   DoTagBoxMouseDown(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::TagBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
   DoTagBoxMouseMove(Shift, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//										Event Handlers
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::XZoom1Click(TObject *Sender)
{
   SetMouseMode(XZoom);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::YZoom1Click(TObject *Sender)
{
   SetMouseMode(YZoom);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::XYZoom1Click(TObject *Sender)
{
   SetMouseMode(XYZoom);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::OFF1Click(TObject *Sender)
{
   SetMouseMode(MouseIdle);
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::ShowGridLines1Click(TObject *Sender)
{
   EnableGridLines = !EnableGridLines;
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::GridLinesColor1Click(TObject *Sender)
{
   ColorDialog1->Color = GridLinesColor;
   if (ColorDialog1->Execute()){
      GridLinesColor = ColorDialog1->Color;
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::PenColor1Click(TObject *Sender)
{
   ColorDialog1->Color = PenColor;
   if (ColorDialog1->Execute()){
      PenColor = ColorDialog1->Color;
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::BackgroundColor1Click(TObject *Sender)
{
   ColorDialog1->Color = BackgroundColor;
   if (ColorDialog1->Execute()){
      BackgroundColor = ColorDialog1->Color;
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::AxesColor1Click(TObject *Sender)
{
   ColorDialog1->Color = AxesColor;
   if (ColorDialog1->Execute()){
      AxesColor = ColorDialog1->Color;
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::Font1Click(TObject *Sender)
{
   FontDialog1->Font = AxesFont;
   if (FontDialog1->Execute()){
      AxesFont = FontDialog1->Font;
   }
}
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::TimeLockCheckBoxClick(TObject *Sender)
{
   TimeLocked = TimeLockCheckBox->Checked;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							Event handling and generation functions
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::DoMouseDown(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y)
{
	if (Graph->Width < 10 || Graph->Height < 10) return;

   if ((FMouseMode < MouseIdle) && Button == mbLeft)
   	MouseDown(Button, Shift, X, Y);
   else TControl::MouseDown(Button, Shift, X, Y); // enable TControl event handler
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoMouseMove(Classes::TShiftState Shift,
   									int X, int Y)
{
	if (Graph->Width < 10 || Graph->Height < 10) return;

   if (FMouseMode < MouseIdle)
   	MouseMove(Shift, X, Y);
   else TControl::MouseMove(Shift, X, Y);  // enable TControl event handler
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoMouseUp(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y)
{
	if (Graph->Width < 10 || Graph->Height < 10) return;

   if ((FMouseMode < MouseIdle) && Button == mbLeft)
     	MouseUp(Button, Shift, X, Y);
   else TControl::MouseUp(Button, Shift, X, Y);  // enable TControl event handler
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoBeginResize()
{
   if (FOnBeginResize){
      FOnBeginResize(this);
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoEndResize()
{
   if (FOnEndResize){
      FOnEndResize(this);
   }
}
////////////////////////////////////////////////////////////////////////////////

bool __fastcall TPLOTPanel::CanResize(int &NewWidth, int &NewHeight)
{
   TControl::CanResize(NewWidth, NewHeight);

   if (NewWidth == Width && NewHeight == Height) return false;
   
   if (IsResizing == 0){
   	DoBeginResize();
   }
	if (IsResizing < 3) IsResizing++;
   if (IsResizing > 0) ResizeTimer->Enabled = true;
   
   return true;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoTagBoxMouseDown(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y)
{
   if (HighlightedTag == 1000000000L) return; // mouse is not over an EventTag
   if (FOnEventTagClick) FOnEventTagClick(this, HighlightedTag);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DoTagBoxMouseMove(Classes::TShiftState Shift,
   									int X, int Y)
{
   if (FIsRealTimePlot) return;
   if (NumEventTags == 0) return;
   if (NumPlots > 1 && !FConcatenatePlots) return;

   // convert X to time range in seconds, give 3 pixel play
   double lefttime = X - 3;
   lefttime /= PixelsPerXUnit;
   lefttime += FVisibleMinX;
   double righttime = X + 3;
   righttime /= PixelsPerXUnit;
   righttime += FVisibleMinX;

   HighlightedTag = 1000000000L;
   for (unsigned long whichtag = 0; whichtag<NumEventTags; whichtag++){
      if (lefttime <= EventTags[whichtag].TimeStamp &&
            EventTags[whichtag].TimeStamp <= righttime){
         HighlightedTag = whichtag;
         break;
      }
   }

   if (HighlightedTag == 1000000000L){ // mouse is not over an EventTag
      if (TagHighlighted){
         DrawEventTags(); // erase highlight
         TagHighlighted = false;
      }
      return;
   }

   TagHighlighted = true;
   TArrayOfFixedPoint points;
   points.Length = 3;
   double time, pixel;
   time = EventTags[HighlightedTag].TimeStamp;
   pixel = (time - FVisibleMinX) * PixelsPerXUnit;
   points[0].X = pixel*65536;
   points[0].Y = (TagBox->Height-2)*65536;
   points[1].X = (pixel-5)*65536;
   points[1].Y = 65536;
   points[2].X = (pixel+5)*65536;
   points[2].Y = 65536;
   PolygonTS(TagBox->Buffer, points, 0xF0FFFFFF);

   TagBox->Flush();
   points.Length = 0;

}
//---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//									  Drawing / UI Functions
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::Draw(bool flush)
{
   if (DelayRedraw) return;
   bool doflush = FIsRealTimePlot ? flush : false;

   if (XAxisHasChanged){
      XAxisHasChanged = false;
		if (!ComponentState.Contains(csDesigning) && FOnXAxisChange){
         FOnXAxisChange(this); // notification that XAxis changed
         if (FTimeLocked) {
            return;
         }
      }
	}

	if (YAxisHasChanged){
		YAxisHasChanged = false;
		if (!ComponentState.Contains(csDesigning) && FOnYAxisChange){
         FOnYAxisChange(this); // notification that YAxis changed
			if (FTimeLocked) {
            return;
         }
      }
	}

   DrawGrid(doflush);
   if (!FIsRealTimePlot){
      DrawEventTags();
      DrawPlots(false);
      DrawMarks(false);
      DrawRanges(flush);
   }
}

void __fastcall TPLOTPanel::DrawPlots(bool flush)
{
   if (FIsRealTimePlot) return;
   if (NumPlots == 0) return;

   for (unsigned long i=0; i<NumPlots; i++){
      Plots[i]->DrawToDAQPanel();
   }
   if (flush) Graph->Flush();
}

void __fastcall TPLOTPanel::DrawEventTags()
{
   if (FIsRealTimePlot) return;
   if (NumEventTags == 0) return;
   if (NumPlots > 1 && !FConcatenatePlots){
      TagBox->Buffer->Clear(clLightGray32);
      TagBox->Flush();
      return;
   }

   TArrayOfFixedPoint points;
   points.Length = 3;
   TagBox->Buffer->Clear(clLightGray32);
   double time, pixel;

   for (unsigned long i=0; i<NumEventTags; i++){
      time = EventTags[i].TimeStamp;
      if (time >= FVisibleMinX && time <= FVisibleMaxX) {
         pixel = (time - FVisibleMinX) * PixelsPerXUnit;
         points[0].X = pixel*65536;
         points[0].Y = (TagBox->Height-2)*65536;
         points[1].X = (pixel-5)*65536;
         points[1].Y = 65536;
         points[2].X = (pixel+5)*65536;
         points[2].Y = 65536;
         PolygonTS(TagBox->Buffer, points, 0xBFFF0000); //TODO: match tag marker color to tag type
      }
   }
   TagBox->Flush();
   points.Length = 0;
}

void __fastcall TPLOTPanel::DrawMarks(bool flush)
{
   if (FIsRealTimePlot) return;
   if (NumXMarks == 0 && NumYMarks == 0) return;

   double val, pixel;

   // do XMarks, but
   //in concatenateplots mode -- time in each episode is shifted
   //so do not plot
   if (!(NumPlots > 1 && FConcatenatePlots)){
      for (unsigned long i=0; i<NumXMarks; i++){
         if (XMarks[i] >= FVisibleMinX && XMarks[i] <= FVisibleMaxX) {
            val = XMarks[i] - FVisibleMinX;
            pixel = val * PixelsPerXUnit;
            Graph->Buffer->VertLineS(pixel, 0, Graph->Height, ZoomColor);
         }
      }
   }

   // do YMarks
   for (unsigned long i=0; i<NumYMarks; i++){
      if (YMarks[i] >= FVisibleMinY && YMarks[i] <= FVisibleMaxY) {
         val = YMarks[i];
         pixel = ZeroYPixels - val * PixelsPerYUnit;
         Graph->Buffer->HorzLineS(0, pixel, Graph->Width, ZoomColor);
      }
   }

   if (flush) Graph->Flush();
}

void __fastcall TPLOTPanel::DrawRanges(bool flush)
{
   //TODO: Add DrawRanges code
   if (flush) Graph->Flush();
}

//---------------------------------------------------------------------------
int __fastcall	TPLOTPanel::GetGridLines(double max, double min,
													double *gridlines,	int tickfactor,
                                       int &decimalplaces)
{
	double range = max - min;
	double unitspertick = 0.0;
   double powTen = 0.001;
   decimalplaces = 5;
   if (range < 1e-9) {
       return 0;
   }
   do {
      if			(range <= 1*powTen) unitspertick = 0.1*powTen;
      else if  (range <= 3*powTen) unitspertick = 0.2*powTen;
      else if  (range <= 6*powTen) unitspertick = 0.5*powTen;
      powTen *= 10;
      decimalplaces--;
   } while (unitspertick == 0.0);
   if (decimalplaces < 0) decimalplaces = 0;

   int roundto;
   if (range < 0.01) roundto = 10000;
   else if (range < 0.1) roundto = 1000;
   else if (range < 1) roundto = 100;
   else if (range < 10) roundto = 10;
   else roundto = 1;
//   int maxtick = max*roundto;
//   int divisor = unitspertick*tickfactor*roundto;
	double maxtick = max*roundto;
	double divisor = unitspertick*tickfactor*roundto;
   maxtick -= fmod(maxtick,divisor);

	double firsttick = maxtick;
   firsttick /= roundto;
	double thetick = max;

   int numgridlines = 0;

   while (thetick > min+unitspertick*tickfactor){
      thetick = firsttick - unitspertick*numgridlines*tickfactor;
      gridlines[numgridlines] = thetick;
		numgridlines++;
   }

   return numgridlines;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DrawGrid(bool flush)
{
   if (!Visible) return;

   Graph->Buffer->PenColor = FGridLinesColor;
   Graph->Buffer->Font->Assign(FAxesFont);
   XAxis->Buffer->Font->Assign(FAxesFont);
   YAxis->Buffer->Font->Assign(FAxesFont);

  	XAxis->Buffer->Clear(FAxesColor);
   YAxis->Buffer->Clear(FAxesColor);
	Graph->Buffer->Clear(FBackgroundColor);

   XAxis->Height = XAxis->Buffer->TextHeight("Bubba2341") + 2;
   YMinInputBox->Top = XAxis->Top - YMinInputBox->Height - 1;
   DisplayButton->Top = YMinInputBox->Top - DisplayButton->Height - 1;
   ZoomButton->Top = DisplayButton->Top;

   if (Height < 50) return;
//   if ((FVisibleMaxY - FVisibleMinY) < 0.00001) return;
//   if ((FVisibleMaxX - FVisibleMinX) < 0.00001) return;


	if (FResetUponDraw) Reset = true; //setting Reset = true here affects realtime plotting
												// in PlotNextBuffer
//   if (XAxisHasChanged){
//      XAxisHasChanged = false;
//		if (!ComponentState.Contains(csDesigning) && FOnXAxisChange)
//         FOnXAxisChange(this); // notification that XAxis changed
//	}
//
//	if (YAxisHasChanged){
//		YAxisHasChanged = false;
//		if (!ComponentState.Contains(csDesigning) && FOnYAxisChange)
//			FOnYAxisChange(this); // notification that XAxis changed
//	}

	int plotwidth = Graph->Width;
   int plotheight = Graph->Height;

   String theLabel;
   int labelleft, labeltop;
   int tickfactor;
   int decimalplaces;
//------------------------------------

//Set up X-Axis

   PixelsPerXUnit = plotwidth; // int to double
   if (FVisibleMaxX-FVisibleMinX == 0){
//	  Application->MessageBox(UnicodeString("MaxX = MinX").w_str(), UnicodeString ("DEBUG ERROR").w_str());
//	  Application->MessageBoxA("", "");
      PixelsPerXUnit = 1;
   }
   else {
      PixelsPerXUnit /= (FVisibleMaxX-FVisibleMinX); // plotwidth / totaltime
   }

   tickfactor = (plotwidth > 450) ? 1 : 2;
   if (plotwidth < 250) tickfactor = 5;
   if (plotwidth < 100) tickfactor = 10;

   NumXGridLines =
   	GetGridLines(FVisibleMaxX, FVisibleMinX, XGridLines,
      					tickfactor, decimalplaces);

   XDecimalPlaces = decimalplaces;
   XMaxInputBox->Text = FloatToStrF(FVisibleMaxX,ffFixed,7,XDecimalPlaces+1);
   XMinInputBox->Text = FloatToStrF(FVisibleMinX,ffFixed,7,XDecimalPlaces+1);

   //draw divisions
   for (int i=0; i<NumXGridLines; i++){
      theLabel = FloatToStrF(XGridLines[i],ffFixed,7,XDecimalPlaces);

      // convert to pixels for use in plot method
      XGridLines[i] = (XGridLines[i]-FVisibleMinX)*PixelsPerXUnit;

      labelleft = YAxis->Width + XGridLines[i]
      				 - XAxis->Buffer->TextWidth(theLabel);
      if (XGridLines[i] < plotwidth-2) labelleft += 2;
      labeltop = 1;
      if (labelleft > YAxis->Width)
         XAxis->Buffer->Textout(labelleft, labeltop, theLabel);

      if (FEnableGridLines){
         Graph->Buffer->PenColor = FGridLinesColor;
         Graph->Buffer->MoveToF(XGridLines[i], 0);
         Graph->Buffer->LineToFS(XGridLines[i],plotheight);
      }
   }

   // show x units
   labelleft = YAxis->Width - XAxis->Buffer->TextWidth(FXUnits);
   XAxis->Buffer->Textout(labelleft, 1, FXUnits);

//------------------------------------

//Set up Y-Axis

   PixelsPerYUnit = plotheight; // int to double

   if (FVisibleMaxY - FVisibleMinY == 0){
//	  Application->MessageBox(UnicodeString("MaxY = MinY").w_str(), UnicodeString("DEBUG ERROR").w_str());
      PixelsPerYUnit = 1;
   }
   else {
      PixelsPerYUnit /= (FVisibleMaxY-FVisibleMinY); // plotheight / yrange
   }

   if (FVisibleMinY >= 0 || (FVisibleMinY <= 0 && FVisibleMaxY > 0))
   	ZeroYPixels = FVisibleMinY * PixelsPerYUnit + plotheight;
   else if (FVisibleMaxY < 0)
   	ZeroYPixels = FVisibleMaxY * PixelsPerYUnit;

   tickfactor = (plotheight > 250) ? 1 : 2;
   if (plotheight < 100) tickfactor = 5;
   if (plotheight < 40) tickfactor = 10;

   NumYGridLines =
   	GetGridLines(FVisibleMaxY, FVisibleMinY, YGridLines, tickfactor, decimalplaces);

   YDecimalPlaces = decimalplaces;
   YMaxInputBox->Text = FloatToStrF(FVisibleMaxY,ffFixed,7,YDecimalPlaces+1);
   YMinInputBox->Text = FloatToStrF(FVisibleMinY,ffFixed,7,YDecimalPlaces+1);

   //draw divisions
   for (int i=0; i<NumYGridLines; i++){
   	int textwidth, textheight;
      theLabel = FloatToStrF(YGridLines[i],ffFixed,7,YDecimalPlaces);
      textwidth = YAxis->Buffer->TextWidth(theLabel);
      textheight = YAxis->Buffer->TextHeight(theLabel);

      // convert to pixels for use in plot method
      YGridLines[i] = ZeroYPixels-YGridLines[i]*PixelsPerYUnit;

      labelleft = YAxis->Width - textwidth;
      labeltop = YGridLines[i] - textheight/2;
      if (labeltop > 0 && labeltop < plotheight-textheight)
	      YAxis->Buffer->Textout(labelleft, labeltop, theLabel);

      if (FEnableGridLines){
         Graph->Buffer->PenColor = FGridLinesColor;
         Graph->Buffer->MoveToF(0, YGridLines[i]);
         Graph->Buffer->LineToFS(plotwidth, YGridLines[i]);
      }
   }

   // show y units -- rotate text 90 degrees
   if (YAxis->Buffer->Handle) {
      TCanvas *tempcanvas = new TCanvas;
	  tempcanvas->Handle = YAxis->Buffer->Handle;
	  LOGFONT lf; // Windows native font structure

      tempcanvas->Brush->Style = bsClear; // set the brush style to transparent
      ZeroMemory(&lf, sizeof(LOGFONT));

      lf.lfHeight = FAxesFont->Height;//  20;
	  lf.lfEscapement = 900; // degrees to rotate
      lf.lfOrientation = 900;
	  lf.lfCharSet = FAxesFont->Charset;// DEFAULT_CHARSET;
	  //wcscpy(lf.lfFaceName , FAxesFont->Name.c_str()); //String(FAxesFont->Name).w_str());//  "Tahoma");
	  //strcpy(lf.lfFaceName , FAxesFont->Name.c_str());
	  UnicodeString axesfontname(FAxesFont->Name);
	  wcscpy(lf.lfFaceName , axesfontname.c_str());
	  tempcanvas->Font->Handle = CreateFontIndirect(&lf);
	  tempcanvas->Font->Color = FAxesFont->Color;
	  tempcanvas->TextOut(1,
               (YAxis->Height + tempcanvas->TextWidth(FYUnits))/2, FYUnits);

      DeleteObject(tempcanvas->Font->Handle);
      delete tempcanvas;

      YAxis->Buffer->Font->Assign(FAxesFont);
   }

//------------------------------------
   XAxis->Flush();
   YAxis->Flush();
   if (flush){
      Graph->Flush();
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DrawPartialGrid(bool store,	::TRect &updateRect, int extra)
{
	::TRect eraseRect = updateRect;
   eraseRect.right += extra;
	if (!store) Graph->Buffer->FillRectTS(eraseRect, FBackgroundColor);
   if (FEnableGridLines){
      for (int i=0; i<NumYGridLines; i++){
         Graph->Buffer->LineFS(eraseRect.left, YGridLines[i], eraseRect.right,
            			YGridLines[i], FGridLinesColor, false);
      }
      for (int i=0; i<NumXGridLines; i++){
         if (XGridLines[i] >= updateRect.left-1 &&
         		XGridLines[i] <= updateRect.right+1){
            Graph->Buffer->LineFS(XGridLines[i], 0, XGridLines[i], Graph->Height,
            			FGridLinesColor, true);
         }
      }
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::InvertRGBRect(::TRect &rect, TBitmap32 *bitmap)
{
   TColor32 *p, *start;
   int pixperscanline = bitmap->Width;
   int begin = rect.top*pixperscanline + rect.left;
   int height = rect.Height();
   int width = rect.Width();

   if (height > bitmap->Height) return;
   if (width > bitmap->Width) return;
   if (height == 0 || width == 0) return;
   
   bitmap->Lock();
   start = p = bitmap->Bits[begin];
   for (int j=0; j<height; j++){
      for (int i=0; i<width; i++){
         *p ^= 0x00FFFFFF;
         ++p;
      }
      start += pixperscanline;
      p = start;
   }
   bitmap->Unlock();
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::DrawAxesSelect()
{
   if (FMouseMode == SelectXRange || FMouseMode == XZoom ||
         FMouseMode == SelectBox || FMouseMode == XYZoom){
		::TRect rect = Rect(SelectRect.left + YAxis->Width, 0,
                        SelectRect.right + YAxis->Width, XAxis->Height);
      InvertRGBRect(rect, XAxis->Buffer);
   }
   if (FMouseMode == SelectYRange || FMouseMode == YZoom ||
         FMouseMode == SelectBox || FMouseMode == XYZoom){
		::TRect rect = Rect(0, SelectRect.top ,YAxis->Width, SelectRect.bottom);
      InvertRGBRect(rect, YAxis->Buffer);
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::MouseDown(TMouseButton Button,
                                       Classes::TShiftState Shift, int X, int Y)
{
	if (MouseLBDown) return;
	MouseLBDown = true;
   MouseHasMoved = false;

   //store origin
   MousePixXOrig = X;
   MousePixYOrig = Y;
   //initialize MouseRect
   MouseRect.Left = X; MouseRect.Right = X;
   MouseRect.Top = Y; MouseRect.Bottom = Y;
   //initialize LastMarkPoint
   LastMarkPoint.x = X;
   LastMarkPoint.y = Y;

   // initialize SelectRect
   if (FMouseMode <= XYZoom || FMouseMode == MarkLine){
      SelectRect.Left = X; SelectRect.Right = X;
      SelectRect.Top = Y; SelectRect.Bottom = Y;
   }
   else {
      switch (FMouseMode){
         case MarkX: {
            MouseCanvas->MoveTo(LastMarkPoint.x, 0);
            MouseCanvas->LineTo(LastMarkPoint.x, Graph->Height);
            Graph->Flush();
         }; break;
         case MarkY: {
            MouseCanvas->MoveTo(0, LastMarkPoint.y);
            MouseCanvas->LineTo(Graph->Width, LastMarkPoint.y);
            Graph->Flush();
         }; break;
         case MarkLine: {
            MouseCanvas->MoveTo(MousePixXOrig, MousePixYOrig);
            MouseCanvas->LineTo(LastMarkPoint.x, LastMarkPoint.y);
         }; break;
         case SelectPlot: {
            if (IsAHit && FOnSelectPlot) FOnSelectPlot(this, SelectedPlotIndex);
         }; break;
      } //end switch
   } //end else
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
   if (FMouseMode == SelectPlot) {

      bool isahit = false;
      for (SelectedPlotIndex=0; SelectedPlotIndex<NumPlots; SelectedPlotIndex++){
         isahit = Plots[SelectedPlotIndex]->HitTest(X, Y);
         if (isahit) break;
      }
      if (isahit && !IsAHit){
         Graph->Buffer->BeginUpdate();
         IsAHit = true;
         int dim = Graph->Height * Graph->Width;
         int numfades = 2000000/dim;
         if (numfades < 1) numfades = 1;
         int fadedegree = 192/numfades + numfades*2;
         if (fadedegree * numfades > 250) fadedegree = 250/numfades;
			TColor32 *pixel;
         TColor32 Fade = SetAlpha(Color32(BackgroundColor), fadedegree);
         for (int j=0; j<numfades; j++){
				pixel = Graph->Buffer->Bits[0];
            for (int i = 0; i<dim; i++){
              BlendMem(Fade, *pixel++);
				}
            EMMS();
            Graph->Flush();
            ::Sleep(10);
         }
         Graph->Buffer->EndUpdate();
         Plots[SelectedPlotIndex]->DrawToDAQPanel();
         Sleep(50);
         Graph->Flush();
      }
      if (!isahit && IsAHit){
         Draw(true);
         IsAHit = false;
      }
   }

	if (!MouseLBDown) return;

   // set bounds
   if (X < 0 ) X = 0; if (X > Graph->Width) X = Graph->Width;
   if (Y < 0 ) Y = 0; if (Y > Graph->Height) Y = Graph->Height;

   // orient MouseRect with top < bottom and left < right
   if (X < MousePixXOrig){
   	MouseRect.Left = X; MouseRect.Right = MousePixXOrig;
   }
   else {
   	MouseRect.Left = MousePixXOrig; MouseRect.Right = X;
   }
   if (Y < MousePixYOrig){
   	MouseRect.Top = Y; MouseRect.Bottom = MousePixYOrig;
   }
   else {
   	MouseRect.Top = MousePixYOrig; MouseRect.Bottom = Y;
   }

   if (MouseHasMoved){
      if (FMouseMode <= XYZoom){
         //copy SelectRect for later calculation of update area
			::TRect tempRect = SelectRect;
         // Draw over old zoom outline and replace
         if (!FIsRealTimePlot) InvertRGBRect(SelectRect, Graph->Buffer);
         DrawAxesSelect();
         // get new SelectRect
         switch (FMouseMode) {
            case SelectXRange:
            case XZoom: {
               SelectRect.left = MouseRect.left;
               SelectRect.top = 0;
               SelectRect.right = MouseRect.right;
               SelectRect.bottom = Graph->Height;
            }; break;
            case SelectYRange:
            case YZoom: {
               SelectRect.left = 0;
               SelectRect.top = MouseRect.top;
               SelectRect.right = Graph->Width;
               SelectRect.bottom = MouseRect.bottom;
            }; break;
            case SelectBox:
            case XYZoom: {
               SelectRect.left = MouseRect.left;
               SelectRect.top = MouseRect.top;
               SelectRect.right = MouseRect.right;
               SelectRect.bottom = MouseRect.bottom;
            }; break;
         };
         // Draw over old zoom outline and replace
         DrawAxesSelect();
         XAxis->Flush();
         YAxis->Flush();
         if (!FIsRealTimePlot){
            InvertRGBRect(SelectRect, Graph->Buffer);
            // flush rect that includes all updated area
            tempRect.left = tempRect.left < SelectRect.left ? tempRect.left : SelectRect.left;
            tempRect.top = tempRect.top < SelectRect.top ? tempRect.top : SelectRect.top;
            tempRect.right = tempRect.right > SelectRect.right ? tempRect.right : SelectRect.right;
            tempRect.bottom = tempRect.bottom > SelectRect.bottom ? tempRect.bottom : SelectRect.bottom;
            Graph->Flush(tempRect);
         }
      }
      else {
         switch (FMouseMode){
            case MarkX: {
               // Draw over old line and replace
               MouseCanvas->MoveTo(LastMarkPoint.x, 0);
               MouseCanvas->LineTo(LastMarkPoint.x, Graph->Height);
               LastMarkPoint.x = X;
               // Draw new line
               MouseCanvas->MoveTo(LastMarkPoint.x, 0);
               MouseCanvas->LineTo(LastMarkPoint.x, Graph->Height);
               Graph->Flush();
            }; break;
            case MarkY: {
               // Draw over old line and replace
               MouseCanvas->MoveTo(0, LastMarkPoint.y);
               MouseCanvas->LineTo(Graph->Width, LastMarkPoint.y);
               LastMarkPoint.y = Y;
               // Draw new line
               MouseCanvas->MoveTo(0, LastMarkPoint.y);
               MouseCanvas->LineTo(Graph->Width, LastMarkPoint.y);
               Graph->Flush();
            }; break;
            case MarkLine: {
               // Draw over old line and replace
               MouseCanvas->MoveTo(MousePixXOrig, MousePixYOrig);
               MouseCanvas->LineTo(LastMarkPoint.x, LastMarkPoint.y);
               LastMarkPoint.x = X;
               LastMarkPoint.y = Y;
               // Draw new line
               MouseCanvas->MoveTo(MousePixXOrig, MousePixYOrig);
               MouseCanvas->LineTo(LastMarkPoint.x, LastMarkPoint.y);
               Graph->Flush();
            }; break;
            case XYPan: {
               double XMove = -(X - MousePixXOrig) / PixelsPerXUnit;
               double YMove = (Y - MousePixYOrig) / PixelsPerYUnit;
               if (Shift.Contains(ssShift)){
                  XMove *= 10;
                  YMove *= 10;
               }
               if ((FVisibleMinX + XMove) >= FFullScaleMinX &&
                  (FVisibleMaxX + XMove) <= FFullScaleMaxX){
                  FVisibleMinX = XMove + FVisibleMinX;
                  FVisibleMaxX = XMove + FVisibleMaxX;
               }
               if ((FVisibleMinY + YMove) >= FFullScaleMinY &&
                  (FVisibleMaxY + YMove) <= FFullScaleMaxY){
                  FVisibleMinY = YMove + FVisibleMinY;
                  FVisibleMaxY = YMove + FVisibleMaxY;
               }
               // store X and Y for next time
               //(MousePix_Orig not used elsewhere in this mode)
               MousePixXOrig = X;
               MousePixYOrig = Y;
               Draw(true);
            }; break;
         }// end switch MouseMode
      }// end else
   }// end if MouseHasMoved

   MouseHasMoved = true;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::MouseUp(TMouseButton Button,
                                       Classes::TShiftState Shift, int X, int Y)
{
	if (!MouseLBDown) return;
   MouseLBDown = false;
   MouseHasMoved = false;

   double left = (SelectRect.left / PixelsPerXUnit) + FVisibleMinX;
   double right = (SelectRect.right / PixelsPerXUnit) + FVisibleMinX;
   double top = (ZeroYPixels - SelectRect.top) / PixelsPerYUnit;
   double bottom = (ZeroYPixels - SelectRect.bottom) / PixelsPerYUnit;

   if (FMouseMode <= XYZoom){
      // Draw over old zoom outline and replace
      if (!FIsRealTimePlot) InvertRGBRect(SelectRect, Graph->Buffer);
      DrawAxesSelect();
      XAxis->Flush();
      YAxis->Flush();
      Graph->Flush();
   }
   switch (FMouseMode) {
      case SelectXRange: {
      	if (SelectRect.right - SelectRect.left > 10) { // some play for dblclick
            if (FOnSelectXRange) FOnSelectXRange(this, left, right);
         }
      }; break;
   	case XZoom: {
      	if (SelectRect.right - SelectRect.left > 10) { // some play for dblclick
            StoreViewport();
            ZoomX();
            Draw(true);
         }
      }; break;
      case SelectYRange: {
      	if (SelectRect.right - SelectRect.left > 10) { // some play for dblclick
            if (FOnSelectYRange) FOnSelectYRange(this, bottom, top);
         }
      }; break;
      case YZoom: {
      	if (SelectRect.bottom - SelectRect.top > 10){ // some play for dblclick
            StoreViewport();
				ZoomY();
            Draw(true);
         }
      }; break;
      case SelectBox: {
         bool doX = false, doY = false;
      	if (SelectRect.right - SelectRect.left > 10) { // give play for dblclick
            doX = true;
         }
         if (SelectRect.bottom - SelectRect.top > 10){ // give some play for dblclick
            doY = true;
         }
        	if (doX && doY){
            if (FOnSelectRect)
               FOnSelectRect(this, MakeSelectionRect(left, top, right, bottom));
         }
      }; break;
      case XYZoom: {
         bool doX = false, doY = false;
      	if (SelectRect.right - SelectRect.left > 10) { // give play for dblclick
            doX = true;
         }
         if (SelectRect.bottom - SelectRect.top > 10){ // give some play for dblclick
            doY = true;
         }
        	if (doX || doY){
            StoreViewport();
            if (doX) ZoomX();
            if (doY) ZoomY();
            Draw(true);
         }
      }; break;
      case MarkX: {
         // Draw over old line and replace
         MouseCanvas->MoveTo(LastMarkPoint.x, 0);
         MouseCanvas->LineTo(LastMarkPoint.x, Graph->Height);
         Graph->Flush();
         //in concatenateplots mode -- time in each episode is shifted
         //so do not fire event
         if (!(NumPlots > 1 && FConcatenatePlots)) {
            if (FOnMarkX){
               FOnMarkX(this, (X / PixelsPerXUnit) + FVisibleMinX);
            }
         }
      }; break;
      case MarkY: {
         // Draw over old line and replace
         MouseCanvas->MoveTo(0, LastMarkPoint.y);
         MouseCanvas->LineTo(Graph->Width, LastMarkPoint.y);
         Graph->Flush();
         if (FOnMarkY) FOnMarkY(this, (ZeroYPixels - Y) / PixelsPerYUnit);
      }; break;
      case MarkLine: {
         // Draw over old line and replace
         MouseCanvas->MoveTo(MousePixXOrig, MousePixYOrig);
         MouseCanvas->LineTo(LastMarkPoint.x, LastMarkPoint.y);
         Graph->Flush();
         if (FOnMarkLine){
				int leftx = std::min(MousePixXOrig, int(LastMarkPoint.x));
				int rightx = std::max(MousePixXOrig, int(LastMarkPoint.x));
            int lefty = (leftx == MousePixXOrig) ? MousePixYOrig : LastMarkPoint.y;
            int righty = (leftx == MousePixXOrig) ? LastMarkPoint.y : MousePixYOrig;
            FOnMarkLine(this, (leftx/PixelsPerXUnit) + FVisibleMinX,
                              (ZeroYPixels - lefty) / PixelsPerYUnit,
                              (rightx/PixelsPerXUnit) + FVisibleMinX,
                              (ZeroYPixels - righty) / PixelsPerYUnit);
         }
      }; break;
      case XYPan: {
         if (FOnXAxisChange) FOnXAxisChange(this); // notification that XAxis changed
      }; break;
   };
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::ZoomX()
{
   double range = (SelectRect.right - SelectRect.left) / PixelsPerXUnit;

	// can't zoom to less than 0.5% XUnit
	if (range < 0.005){
      range = 0.005;
      InfoPanel->Caption = L"Can't zoom to less than 0.005 " +
         FXUnits == "" ? L"X Units" : FXUnits.c_str();
      return;
   }
   else   InfoPanel->Caption = "";

   if (FIsRealTimePlot){
		FVisibleMaxX = range;
   }
   else {
      double left = SelectRect.left / PixelsPerXUnit + FVisibleMinX;
      FVisibleMinX = left;
      FVisibleMaxX = left + range;
   }
	XAxisHasChanged = true;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::ZoomY()
{
	if ((FVisibleMaxY - FVisibleMinY) < (FFullScaleMaxY-FFullScaleMinY)/2000){
      InfoPanel->Caption = L"Cannot Zoom to less than 0.05% Full Scale";
      return;
   }
   else InfoPanel->Caption = L"";

   FVisibleMaxY = ZeroYPixels - SelectRect.top;
   FVisibleMaxY /= PixelsPerYUnit;
	FVisibleMinY = ZeroYPixels - SelectRect.bottom;
	FVisibleMinY /= PixelsPerYUnit;

	YAxisHasChanged = true;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::StoreViewport()
{
	if (NumStoredViewports == MAXVIEWPORTS) return;
	Viewports[NumStoredViewports].MinX = FVisibleMinX;
	Viewports[NumStoredViewports].MinY = FVisibleMinY;
	Viewports[NumStoredViewports].MaxX = FVisibleMaxX;
	Viewports[NumStoredViewports].MaxY = FVisibleMaxY;
	NumStoredViewports++;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::AddEventTags(unsigned long numtags, NewDAQ::EventTagStruct *tags)
{
	NumEventTags = numtags;
	EventTags = tags;
	DrawEventTags();
}
////////////////////////////////////////////////////////////////////////////////

bool __fastcall TPLOTPanel::AddXMark(double mark, bool flush)
{
	if (mark >= FFullScaleMinX && mark <= FFullScaleMaxX){
      double *temp;
      try{
         temp = new double[NumXMarks + 1];
      }
      catch(...){
         return false;
      }

      for (unsigned long i = 0; i<NumXMarks; i++){
         temp[i] = XMarks[i];
      }
      temp[NumXMarks] = mark;
      if (NumXMarks > 0) delete[] XMarks;
      XMarks = temp;
      NumXMarks++;

      Draw(flush);
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
bool __fastcall TPLOTPanel::AddYMark(double mark, bool flush)
{
   if (mark >= FFullScaleMinY && mark <= FFullScaleMaxY){
      double *temp;
      try{
         temp = new double[NumYMarks + 1];
      }
      catch(...){
         return false;
      }
      for (unsigned long i = 0; i<NumYMarks; i++){
         temp[i] = YMarks[i];
      }
      temp[NumYMarks] = mark;
      if (NumYMarks > 0) delete[] YMarks;
      YMarks = temp;
      NumYMarks++;

      Draw(flush);
      return true;
   }
   return false;
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::ClearXMarks()
{
   if (NumXMarks > 0){
      delete[] XMarks;
      NumXMarks = 0;
      Draw(true);
   }
}
//---------------------------------------------------------------------------
void __fastcall TPLOTPanel::ClearYMarks()
{
   if (NumYMarks > 0) {
      delete[] YMarks;
      NumYMarks = 0;
      Draw(true);
   }
}
//---------------------------------------------------------------------------

