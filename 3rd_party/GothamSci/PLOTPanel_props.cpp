//---------------------------------------------------------------------------


#pragma hdrstop

#include "tools.h"
#include "GR32_Blend.hpp"
#include "PLOTPanel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							      Property access functions
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

void __fastcall TPLOTPanel::SetFullScaleMinY(double fsminy)
{
   if (FFullScaleMinY == fsminy) return;
	FFullScaleMinY = fsminy;
   if (FVisibleMinY < FFullScaleMinY){
      FVisibleMinY = FFullScaleMinY;
   }
   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetFullScaleMaxY(double fsmaxy)
{
   if (FFullScaleMaxY == fsmaxy) return;
	FFullScaleMaxY = fsmaxy;
   if (FVisibleMaxY > FFullScaleMaxY){
      FVisibleMaxY = FFullScaleMaxY;
   }
   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetFullScaleMinX(double fsminx)
{
   if (FFullScaleMinX == fsminx) return;
   if (FIsRealTimePlot && fsminx != 0.0) fsminx = 0.0;
	FFullScaleMinX = fsminx;
	if (FVisibleMinX < FFullScaleMinX){
      FVisibleMinX = FFullScaleMinX;
      XAxisHasChanged = true;
   }
   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetFullScaleMaxX(double fsmaxx)
{
   if (FFullScaleMaxX == fsmaxx) return;
	FFullScaleMaxX = fsmaxx;
   if (FVisibleMaxX > FFullScaleMaxX){
      FVisibleMaxX = FFullScaleMaxX;
      XAxisHasChanged = true;
   }
   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetVisibleMinY(double visminy)
{
	if (FVisibleMinY == visminy) return;
   if (visminy < FVisibleMaxY){
      if (visminy >= FFullScaleMinY){
         if ((FVisibleMaxY - visminy) < (FFullScaleMaxY-FFullScaleMinY)/2000){
				InfoPanel->Caption = "Cannot Zoom to less than 0.05% Full Scale";
				return;
			}
			FVisibleMinY = visminy;
		}
		else {
			FVisibleMinY = FFullScaleMinY;
		}
		YAxisHasChanged = true;
		if (!DelayRedraw) Draw(true);
	}
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetVisibleMaxY(double vismaxy)
{
	if (FVisibleMaxY == vismaxy) return;
	if (vismaxy > FVisibleMinY){
		if (vismaxy <= FFullScaleMaxY){
			if ((vismaxy - FVisibleMinY) < (FFullScaleMaxY-FFullScaleMinY)/2000){
				InfoPanel->Caption = "Cannot Zoom to less than 0.05% Full Scale";
				return;
			}
			FVisibleMaxY = vismaxy;
		}
		else {
			FVisibleMaxY = FFullScaleMaxY;
		}
		YAxisHasChanged = true;
      if (!DelayRedraw) Draw(true);
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetVisibleMinX(double visminx)
{
   if (FVisibleMinX == visminx) return;
   if (FIsRealTimePlot && visminx != 0.0) visminx = 0.0;
   if (visminx < FVisibleMaxX){
   	if (visminx >= FFullScaleMinX){
         if (visminx + 0.0049 > FVisibleMaxX){
            InfoPanel->Caption = "Can't zoom to less than 0.005 " + FXUnits;
            return;
         }
	   	FVisibleMinX = visminx;
      }
      else {
         FVisibleMinX = FFullScaleMinX;
      }
      XAxisHasChanged = true;
      if (!DelayRedraw) Draw(true);
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetVisibleMaxX(double	vismaxx)
{
   if (FVisibleMaxX == vismaxx) return;
   if (vismaxx > FVisibleMinX){
      if (vismaxx <= FFullScaleMaxX){
         if (vismaxx - 0.0049 < FVisibleMinX){
            InfoPanel->Caption = "Can't zoom to less than 0.005 " + FXUnits;
            return;
         }
		   FVisibleMaxX = vismaxx;
      }
      else {
         FVisibleMaxX = FFullScaleMaxX;
      }
      XAxisHasChanged = true;
      if (!DelayRedraw) Draw(true);
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetMouseMode(MouseModeEnum mode)
{
   if (FMouseMode == mode) return;

   // disallow certain modes for realtime plots
   if (FIsRealTimePlot && mode > XYZoom && mode < MouseIdle){
      FMouseMode = MouseIdle;
      Graph->Cursor = crDefault;
   }
   else {
      FMouseMode = mode;
      if (FOnMouseModeChange) FOnMouseModeChange(this);
   }
   XZoom1->Checked = FMouseMode == XZoom;
   YZoom1->Checked = FMouseMode == YZoom;
   XYZoom1->Checked = FMouseMode == XYZoom;
   OFF1->Checked = FMouseMode == MouseIdle;
   switch (FMouseMode) {
      case SelectXRange:
      case XZoom: Graph->Cursor = crSizeWE; break;
      case SelectYRange:
      case YZoom: Graph->Cursor = crSizeNS; break;
      case SelectBox:
      case XYZoom: Graph->Cursor = crSizeAll; break;
      case MarkX:
      case MarkY:
      case MarkLine: Graph->Cursor = crCross; break;
      case XYPan: Graph->Cursor = crHandPoint; break;
      default: Graph->Cursor = crDefault; break;
   };
   if (FMouseMode > XYZoom && FMouseMode < XYPan) { // MarkX, MarkY, MarkLine
      if (!MouseCanvas) MouseCanvas = new TCanvas;
      MouseCanvas->Handle = Graph->Buffer->Handle;
      MouseCanvas->Pen->Color = clBlack;
      MouseCanvas->Pen->Mode = pmNotXor;
   }
   else {
      if (MouseCanvas) delete MouseCanvas;
      MouseCanvas = NULL;
   }
   if (FMouseMode == SelectPlot){
      Graph->Cursor = crHelp;
      IsAHit = false;
   }
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetEnableGridLines(bool enable)
{
	if (FEnableGridLines == enable) return;
	FEnableGridLines = enable;
   ShowGridLines1->Checked = enable;

   if (FOnGridLinesChange) FOnGridLinesChange(this);

   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetPenColor(TColor color)
{
   if (FPenColor == Color32(color)) return;
	FPenColor = Color32(color);

   if (FOnPenColorChange) FOnPenColorChange(this);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetBackgroundColor(TColor color)
{
   if (FBackgroundColor == Color32(color)) return;
   TColor32 thecolor = Color32(color);

   if (thecolor == 0xFF808080) {
	  Application->MessageBox(L"Cannot use 50% gray as Background Color",
		 L"Please Choose Another Color");
      return;
   }

	FBackgroundColor = thecolor;

   //inverts unless = 0xFFF0F0F0
   ZoomColor = clWhite32-FBackgroundColor+0x7F000000;
/*
   // set stipple pattern for use in XMark and YMark methods
   TArrayOfColor32 colors;
   colors.Length = 2;
   colors[0] = ZoomColor+0x7F000000;
   colors[1] = 0xFF000000;
   Graph->Buffer->StippleStep = 1;
   Graph->Buffer->SetStipple(colors);
*/
   if (FOnBackgroundColorChange) FOnBackgroundColorChange(this);

   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetAxesColor(TColor color)
{
   if (FAxesColor == Color32(color)) return;
   TColor32 thecolor = Color32(color);

   if (thecolor == 0xFF808080) {
	  Application->MessageBox(L"Cannot use 50% gray as Axes Color",
		 L"Please Choose Another Color");
      return;
   }

	FAxesColor = thecolor;

   if (FOnAxesColorChange) FOnAxesColorChange(this);

   if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetGridLinesColor(TColor color)
{
   if (FGridLinesColor == Color32(color)) return;
   TColor32 thecolor = Color32(color);

   if (thecolor == 0xFF808080) {
	  Application->MessageBox(L"Cannot use 50% gray as Grid Lines Color",
		 L"Please Choose Another Color");
      return;
   }

	FGridLinesColor =	thecolor;

   if (FOnGridLinesColorChange) FOnGridLinesColorChange(this);

	if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

TColor __fastcall	TPLOTPanel::GetPenColor()
{
	return WinColor(FPenColor);
}
////////////////////////////////////////////////////////////////////////////////

TColor __fastcall	TPLOTPanel::GetBackgroundColor()
{
	return WinColor(FBackgroundColor);
}
////////////////////////////////////////////////////////////////////////////////

TColor __fastcall	TPLOTPanel::GetAxesColor()
{
	return WinColor(FAxesColor);
}
////////////////////////////////////////////////////////////////////////////////

TColor __fastcall	TPLOTPanel::GetGridLinesColor()
{
	return WinColor(FGridLinesColor);
}

////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::AxesFontChanged(TObject *Sender)
{
   Graph->Buffer->Font->Assign(FAxesFont);
   XAxis->Buffer->Font->Assign(FAxesFont);
   YAxis->Buffer->Font->Assign(FAxesFont);

   if (FOnAxesFontChange) FOnAxesFontChange(this);

	if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetAxesFont(TFont *font)
{
   FAxesFont->Assign(font);
   //calls AxesFontChanged
}
////////////////////////////////////////////////////////////////////////////////

String __fastcall TPLOTPanel::GetYUnits()
{
	return FYUnits;
}

////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetYUnits(String yu)
{
   if (FYUnits == yu) return;
	FYUnits = yu;

	if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

String __fastcall TPLOTPanel::GetXUnits()
{
	return FXUnits;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetXUnits(String xu)
{
   if (FXUnits == xu) return;
	FXUnits = xu;

	if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetIsRealTimePlot(bool set)
{
   if (FIsRealTimePlot == set) return;
	FIsRealTimePlot = set;

   if (FIsRealTimePlot){
//   	FFullScaleMinX = 0.0;
      FVisibleMinX = 0.0;
      XMinInputBox->Visible = false;
//      ZoomButton->Visible = false;
//      ZoomButton->Height = 0;
//      DisplayButton->Visible = false;
//      DisplayButton->Height = 0;
      TagPanel->Visible = false;
      TagPanel->Height = 0;
      YMaxInputBox->Top = 1;
   }
   else {
      XMinInputBox->Visible = true;
//		ZoomButton->Visible = true;
//		ZoomButton->Height = 15;
//		DisplayButton->Visible = true;
//      DisplayButton->Height = 15;
      TagPanel->Visible = true;
      TagPanel->Height = 16;
      YMaxInputBox->Top = TagPanel->Height + 1;
   }
   
	if (!DelayRedraw) Draw(true);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetTimeLocked(bool set)
{
   FTimeLocked = set;
   TimeLockCheckBox->Checked = FTimeLocked;
   if (FTimeLocked && FOnXAxisChange) FOnXAxisChange(this); 
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetEnableTraceStorage(bool set)
{
	FEnableTraceStorage = set;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetTraceStorageFade(unsigned char fade)
{
	FTraceStorageFade = fade;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetIgnoreOverrun(bool set)
{
	FIgnoreOverrun = set;
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetDrawEraseExtra(int extra)
{
   FDrawEraseExtra = constrain(extra, 1, 10);
}
////////////////////////////////////////////////////////////////////////////////

void __fastcall TPLOTPanel::SetConcatenatePlots(bool set)
{
   if (FConcatenatePlots == set) return;
   FConcatenatePlots = set;
//   if (ComponentState.Contains(csDesigning)) return;
   if (!FIsRealTimePlot){
      XAxisHasChanged = true;
      if (!DelayRedraw) ScalePlots(true);
   }
}
//////////////////////////////////////////////////////////////////////////////// 