//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "tools.h"
#include "DAQPlotSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMultiDAQPlotSettings::TMultiDAQPlotSettings(TDAQPlot **plot)
{
   Plot = plot;

   FTimeLocked = false;
   FGlobalChanges = false;
   FZoomMode = DEFAULTZOOMMODE;
   FEnableMouseZoom = DEFAULTENABLEMOUSEZOOM;
   FDrawEraseExtra = DEFAULTDRAWERASEEXTRA;
   FIgnoreOverrun = DEFAULTIGNOREOVERRUN;

   FPlot0 = new TDAQPlotSettings(this, 0);
   FPlot1 = new TDAQPlotSettings(this, 1);
   FPlot2 = new TDAQPlotSettings(this, 2);
   FPlot3 = new TDAQPlotSettings(this, 3);
   FPlot4 = new TDAQPlotSettings(this, 4);
   FPlot5 = new TDAQPlotSettings(this, 5);
   FPlot6 = new TDAQPlotSettings(this, 6);
   FPlot7 = new TDAQPlotSettings(this, 7);
}
__fastcall TMultiDAQPlotSettings::~TMultiDAQPlotSettings()
{
   delete FPlot0;
   delete FPlot1;
   delete FPlot2;
   delete FPlot3;
   delete FPlot4;
   delete FPlot5;
   delete FPlot6;
   delete FPlot7;
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetTimeLocked(bool set)
{
   FTimeLocked = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetGlobalChanges(bool set)
{
   FGlobalChanges = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetZoomMode(ZoomModeEnum mode)
{
   FZoomMode = mode;
   for (int i=0; i<MAXPLOTS; i++)
      Plot[i]->SetZoomMode(mode);
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetEnableMouseZoom(bool set)
{
   FEnableMouseZoom = set;
   for (int i=0; i<MAXPLOTS; i++)
      Plot[i]->EnableMouseZoom = set;
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetDrawEraseExtra(int extra)
{
   FDrawEraseExtra = extra;
   for (int i=0; i<MAXPLOTS; i++)
      Plot[i]->DrawEraseExtra = extra;
}
//---------------------------------------------------------------------------

void __fastcall TMultiDAQPlotSettings::SetIgnoreOverrun(bool set)
{
   FIgnoreOverrun = set;
   for (int i=0; i<MAXPLOTS; i++)
      Plot[i]->IgnoreOverrun = set;
}
//---------------------------------------------------------------------------

float __fastcall TMultiDAQPlotSettings::GetPlotFullScaleMaxX(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return 0;
   return Plot[index]->FullScaleMaxX;
}

void __fastcall TMultiDAQPlotSettings::SetPlotFullScaleMaxX(int index, float fsmaxx)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->FullScaleMaxX = fsmaxx;
   }
   else{
      Plot[index]->FullScaleMaxX = fsmaxx;
   }
}
//---------------------------------------------------------------------------

float __fastcall TMultiDAQPlotSettings::GetPlotVisibleMaxX(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return 0;
   return Plot[index]->VisibleMaxX;
}

void __fastcall TMultiDAQPlotSettings::SetPlotVisibleMaxX(int index, float vismaxx)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->VisibleMaxX = vismaxx;
   }
   else{
      Plot[index]->VisibleMaxX = vismaxx;
   }
}
//---------------------------------------------------------------------------

float __fastcall TMultiDAQPlotSettings::GetPlotFullScaleMinX(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return 0;
   return Plot[index]->FullScaleMinX;
}

void __fastcall TMultiDAQPlotSettings::SetPlotFullScaleMinX(int index, float fsminx)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->FullScaleMinX = fsminx;
   }
   else{
      Plot[index]->FullScaleMinX = fsminx;
   }
}
//---------------------------------------------------------------------------

float __fastcall TMultiDAQPlotSettings::GetPlotVisibleMinX(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return 0;
   return Plot[index]->VisibleMinX;
}

void __fastcall TMultiDAQPlotSettings::SetPlotVisibleMinX(int index, float visminx)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->VisibleMinX = visminx;
   }
   else{
      Plot[index]->VisibleMinX = visminx;
   }
}
//---------------------------------------------------------------------------

bool __fastcall TMultiDAQPlotSettings::GetPlotEnableGridLines(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return false;
   return Plot[index]->EnableGridLines;
}

void __fastcall TMultiDAQPlotSettings::SetPlotEnableGridLines(int index, bool set)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->EnableGridLines = set;
   }
   else{
      Plot[index]->EnableGridLines = set;
   }
}
//---------------------------------------------------------------------------

bool __fastcall TMultiDAQPlotSettings::GetPlotEnableTraceStorage(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return false;
   return Plot[index]->EnableTraceStorage;
}

void __fastcall TMultiDAQPlotSettings::SetPlotEnableTraceStorage(int index, bool set)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->EnableTraceStorage = set;
   }
   else{
      Plot[index]->EnableTraceStorage = set;
   }
}
//---------------------------------------------------------------------------

bool __fastcall TMultiDAQPlotSettings::GetPlotIsRealTime(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return false;
   return Plot[index]->IsRealTimePlot;
}

void __fastcall TMultiDAQPlotSettings::SetPlotIsRealTime(int index, bool set)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->IsRealTimePlot = set;
   }
   else{
      Plot[index]->IsRealTimePlot = set;
   }
}
//---------------------------------------------------------------------------

TColor __fastcall TMultiDAQPlotSettings::GetPlotGridLinesColor(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return clNone;
   return Plot[index]->GridLinesColor;
}

void __fastcall TMultiDAQPlotSettings::SetPlotGridLinesColor(int index, TColor color)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->GridLinesColor = color;
   }
   else{
      Plot[index]->GridLinesColor = color;
   }
}
//---------------------------------------------------------------------------

TColor __fastcall TMultiDAQPlotSettings::GetPlotAxesColor(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return clNone;
   return Plot[index]->AxesColor;
}

void __fastcall TMultiDAQPlotSettings::SetPlotAxesColor(int index, TColor color)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->AxesColor = color;
   }
   else{
      Plot[index]->AxesColor = color;
   }
}
//---------------------------------------------------------------------------

TFont* __fastcall TMultiDAQPlotSettings::GetPlotAxesFont(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return NULL;
   return Plot[index]->AxesFont;
}

void __fastcall TMultiDAQPlotSettings::SetPlotAxesFont(int index, TFont* font)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->AxesFont = font;
   }
   else{
      Plot[index]->AxesFont = font;
   }
}
//---------------------------------------------------------------------------

TColor __fastcall TMultiDAQPlotSettings::GetPlotBackgroundColor(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return clNone;
   return Plot[index]->BackgroundColor;
}

void __fastcall TMultiDAQPlotSettings::SetPlotBackgroundColor(int index, TColor color)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->BackgroundColor = color;
   }
   else{
      Plot[index]->BackgroundColor = color;
   }
}
//---------------------------------------------------------------------------

TColor __fastcall TMultiDAQPlotSettings::GetPlotPenColor(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return clNone;
   return Plot[index]->PenColor;
}

void __fastcall TMultiDAQPlotSettings::SetPlotPenColor(int index, TColor color)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->PenColor = color;
   }
   else{
      Plot[index]->PenColor = color;
   }
}
//---------------------------------------------------------------------------

unsigned char __fastcall TMultiDAQPlotSettings::GetPlotTraceStorageFade(int index)
{
//   if (!within(index, 0, MAXPLOTS-1)) return 0;
   return Plot[index]->TraceStorageFade;
}

void __fastcall TMultiDAQPlotSettings::SetPlotTraceStorageFade(int index,
                                                            unsigned char fade)
{
//   if (!within(index, 0, MAXPLOTS-1)) return;
   if (FGlobalChanges){
      for (int i=0; i<MAXPLOTS; i++)
         Plot[i]->TraceStorageFade = fade;
   }
   else{
      Plot[index]->TraceStorageFade = fade;
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




__fastcall TDAQPlotSettings::TDAQPlotSettings(TMultiDAQPlotSettings *multi,
                                                int index)
{
   Multi = multi;
   Plot = Multi->Plot;
   Index = index;
}
__fastcall TDAQPlotSettings::~TDAQPlotSettings(){}
//---------------------------------------------------------------------------

float	__fastcall TDAQPlotSettings::GetFullScaleMinY()
{
   return Plot[Index]->FullScaleMinY;
}

void __fastcall TDAQPlotSettings::SetFullScaleMinY(float fsminy)
{
   Plot[Index]->FullScaleMinY = fsminy;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetVisibleMinY()
{
   return Plot[Index]->VisibleMinY;
}

void __fastcall TDAQPlotSettings::SetVisibleMinY(float visminy)
{
   Plot[Index]->VisibleMinY = visminy;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetFullScaleMaxY()
{
   return Plot[Index]->FullScaleMaxY;
}

void __fastcall TDAQPlotSettings::SetFullScaleMaxY(float fsmaxy)
{
   Plot[Index]->FullScaleMaxY = fsmaxy;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetVisibleMaxY()
{
   return Plot[Index]->VisibleMaxY;
}

void __fastcall TDAQPlotSettings::SetVisibleMaxY(float vismaxy)
{
   Plot[Index]->VisibleMaxY = vismaxy;
}
//---------------------------------------------------------------------------

AnsiString __fastcall TDAQPlotSettings::GetUserUnits()
{
   return Plot[Index]->UserUnits;
}

void __fastcall TDAQPlotSettings::SetUserUnits(AnsiString units)
{
   Plot[Index]->UserUnits = units;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetFullScaleMaxX()
{
   return Multi->PlotFullScaleMaxX[Index];
}

void __fastcall TDAQPlotSettings::SetFullScaleMaxX(float fsmaxx)
{
   Multi->PlotFullScaleMaxX[Index] = fsmaxx;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetVisibleMaxX()
{
   return Multi->PlotVisibleMaxX[Index];
}

void __fastcall TDAQPlotSettings::SetVisibleMaxX(float vismaxx)
{
   Multi->PlotVisibleMaxX[Index] = vismaxx;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetFullScaleMinX()
{
   return Multi->PlotFullScaleMinX[Index];
}

void __fastcall TDAQPlotSettings::SetFullScaleMinX(float fsminx)
{
   Multi->PlotFullScaleMinX[Index] = fsminx;
}
//---------------------------------------------------------------------------

float __fastcall TDAQPlotSettings::GetVisibleMinX()
{
   return Multi->PlotVisibleMinX[Index];
}

void __fastcall TDAQPlotSettings::SetVisibleMinX(float visminx)
{
   Multi->PlotVisibleMinX[Index] = visminx;
}
//---------------------------------------------------------------------------

bool __fastcall TDAQPlotSettings::GetEnableGridLines()
{
   return Multi->PlotEnableGridLines[Index];
}

void __fastcall TDAQPlotSettings::SetEnableGridLines(bool set)
{
   Multi->PlotEnableGridLines[Index] = set;
}
//---------------------------------------------------------------------------

TColor __fastcall TDAQPlotSettings::GetPenColor()
{
   return Multi->PlotPenColor[Index];
}

void __fastcall TDAQPlotSettings::SetPenColor(TColor color)
{
   Multi->PlotPenColor[Index] = color;
}
//---------------------------------------------------------------------------

TColor __fastcall TDAQPlotSettings::GetBackgroundColor()
{
   return Multi->PlotBackgroundColor[Index];
}

void __fastcall TDAQPlotSettings::SetBackgroundColor(TColor color)
{
   Multi->PlotBackgroundColor[Index] = color;
}
//---------------------------------------------------------------------------

TColor __fastcall TDAQPlotSettings::GetAxesColor()
{
   return Multi->PlotAxesColor[Index];
}

void __fastcall TDAQPlotSettings::SetAxesColor(TColor color)
{
   Multi->PlotAxesColor[Index] = color;
}
//---------------------------------------------------------------------------

TColor __fastcall TDAQPlotSettings::GetGridLinesColor()
{
   return Multi->PlotGridLinesColor[Index];
}

void __fastcall TDAQPlotSettings::SetGridLinesColor(TColor color)
{
   Multi->PlotGridLinesColor[Index] = color;
}
//---------------------------------------------------------------------------

TFont* __fastcall TDAQPlotSettings::GetAxesFont()
{
   return Multi->PlotAxesFont[Index];
}

void __fastcall TDAQPlotSettings::SetAxesFont(TFont *font)
{
   Multi->PlotAxesFont[Index] = font;
}
//---------------------------------------------------------------------------

bool __fastcall TDAQPlotSettings::GetIsRealTimePlot()
{
   return Multi->PlotIsRealTime[Index];
}

void __fastcall TDAQPlotSettings::SetIsRealTimePlot(bool set)
{
   Multi->PlotIsRealTime[Index] = set;
}
//---------------------------------------------------------------------------

// from TDAQPlot
bool __fastcall TDAQPlotSettings::GetEnableTraceStorage()
{
   return Multi->PlotEnableTraceStorage[Index];
}

void __fastcall TDAQPlotSettings::SetEnableTraceStorage(bool set)
{
   Multi->PlotEnableTraceStorage[Index] = set;
}
//---------------------------------------------------------------------------

unsigned char __fastcall TDAQPlotSettings::GetTraceStorageFade()
{
   return Multi->PlotTraceStorageFade[Index];
}

void __fastcall TDAQPlotSettings::SetTraceStorageFade(unsigned char fade)
{
   Multi->PlotTraceStorageFade[Index] = fade;
}
//---------------------------------------------------------------------------


