//---------------------------------------------------------------------------

#ifndef DAQPlotSettingsH
#define DAQPlotSettingsH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DAQPlot.h"

#define MAXPLOTS 8

class TMultiDAQPlotSettings;  //forward definition
//---------------------------------------------------------------------------
class TDAQPlotSettings : public TPersistent
{
private:

   TMultiDAQPlotSettings      *Multi;
   TDAQPlot                   **Plot;
   int                        Index;

protected:

   float __fastcall           GetFullScaleMinY();
   void __fastcall            SetFullScaleMinY(float fsminy);
   float __fastcall           GetVisibleMinY();
   void __fastcall            SetVisibleMinY(float visminy);
   float __fastcall           GetFullScaleMaxY();
   void __fastcall            SetFullScaleMaxY(float fsmaxy);
   float __fastcall           GetVisibleMaxY();
   void __fastcall            SetVisibleMaxY(float vismaxy);
   float __fastcall           GetFullScaleMaxX();
   void __fastcall            SetFullScaleMaxX(float fsmaxx);
   float __fastcall           GetVisibleMaxX();
   void __fastcall            SetVisibleMaxX(float vismaxx);
   float __fastcall           GetFullScaleMinX();
   void __fastcall            SetFullScaleMinX(float fsminx);
   float __fastcall           GetVisibleMinX();
   void __fastcall            SetVisibleMinX(float visminx);
   bool  __fastcall           GetEnableGridLines();
   void __fastcall            SetEnableGridLines(bool set);
   TColor __fastcall          GetPenColor();
   void __fastcall            SetPenColor(TColor color);
   TColor __fastcall          GetBackgroundColor();
   void __fastcall            SetBackgroundColor(TColor color);
   TColor __fastcall          GetAxesColor();
   void __fastcall            SetAxesColor(TColor color);
   TColor __fastcall          GetGridLinesColor();
   void __fastcall            SetGridLinesColor(TColor color);
   TFont* __fastcall          GetAxesFont();
   void __fastcall            SetAxesFont(TFont *font);
   AnsiString __fastcall      GetUserUnits();
   void __fastcall            SetUserUnits(AnsiString units);
   bool __fastcall            GetIsRealTimePlot();
   void __fastcall            SetIsRealTimePlot(bool set);
   // from TDAQPlot
   bool __fastcall            GetEnableTraceStorage();
   void __fastcall            SetEnableTraceStorage(bool set);
   unsigned char __fastcall   GetTraceStorageFade();
   void __fastcall            SetTraceStorageFade(unsigned char fade);

public:
   __fastcall TDAQPlotSettings(TMultiDAQPlotSettings *multi, int index);
   __fastcall ~TDAQPlotSettings();

__published:

   // from TCustomDAQPlot
   __property float					FullScaleMinY =
				{read = GetFullScaleMinY, write = SetFullScaleMinY};

   __property float					VisibleMinY =
				{read = GetVisibleMinY, write = SetVisibleMinY};

   __property float					FullScaleMaxY =
				{read = GetFullScaleMaxY, write = SetFullScaleMaxY};

   __property float					VisibleMaxY =
				{read = GetVisibleMaxY, write = SetVisibleMaxY};

   __property float					FullScaleMaxX =
				{read = GetFullScaleMaxX, write = SetFullScaleMaxX};

   __property float					VisibleMaxX =
				{read = GetVisibleMaxX, write = SetVisibleMaxX};

   __property float					FullScaleMinX =
				{read = GetFullScaleMinX, write = SetFullScaleMinX};

   __property float					VisibleMinX =
				{read = GetVisibleMinX, write = SetVisibleMinX};

   __property bool					EnableGridLines =
				{read = GetEnableGridLines, write = SetEnableGridLines};

   __property TColor					PenColor =
				{read = GetPenColor, write = SetPenColor};

   __property TColor					BackgroundColor =
   	{read = GetBackgroundColor, write = SetBackgroundColor};

   __property TColor					AxesColor =
				{read = GetAxesColor, write = SetAxesColor};

   __property TColor					GridLinesColor =
		{read = GetGridLinesColor, write = SetGridLinesColor};

   __property TFont*             AxesFont =
      {read = GetAxesFont, write = SetAxesFont};

   __property AnsiString			UserUnits =
				{read = GetUserUnits, write = SetUserUnits};

   __property bool					IsRealTimePlot =
   			{read = GetIsRealTimePlot, write = SetIsRealTimePlot};

   // from TDAQPlot
   __property bool 	            EnableTraceStorage =
	         {read = GetEnableTraceStorage, write = SetEnableTraceStorage};

   __property unsigned char	   TraceStorageFade =
            {read = GetTraceStorageFade, write = SetTraceStorageFade};
};



class TMultiDAQPlotSettings : public TPersistent
{
private:

   friend                     TDAQPlotSettings;
   float                      *PercentTop;
   TDAQPlot                   **Plot;

protected:

   TDAQPlotSettings *         FPlot0;
   TDAQPlotSettings *         FPlot1;
   TDAQPlotSettings *         FPlot2;
   TDAQPlotSettings *         FPlot3;
   TDAQPlotSettings *         FPlot4;
   TDAQPlotSettings *         FPlot5;
   TDAQPlotSettings *         FPlot6;
   TDAQPlotSettings *         FPlot7;

   bool                       FTimeLocked;
   void __fastcall            SetTimeLocked(bool set);
   bool                       FGlobalChanges;
   void __fastcall            SetGlobalChanges(bool set);
   ZoomModeEnum               FZoomMode;
   void __fastcall            SetZoomMode(ZoomModeEnum mode);
   bool                       FEnableMouseZoom;
   void __fastcall            SetEnableMouseZoom(bool set);
   int                        FDrawEraseExtra;
   void __fastcall            SetDrawEraseExtra(int extra);
   bool                       FIgnoreOverrun;
   void __fastcall            SetIgnoreOverrun(bool set);

   float __fastcall           GetPlotFullScaleMaxX(int index);
   void __fastcall            SetPlotFullScaleMaxX(int index, float fsmaxx);
   float __fastcall           GetPlotVisibleMaxX(int index);
   void __fastcall            SetPlotVisibleMaxX(int index, float vismaxx);
   float __fastcall           GetPlotFullScaleMinX(int index);
   void __fastcall            SetPlotFullScaleMinX(int index, float fsminx);
   float __fastcall           GetPlotVisibleMinX(int index);
   void __fastcall            SetPlotVisibleMinX(int index, float visminx);
   bool __fastcall            GetPlotEnableGridLines(int index);
   void __fastcall            SetPlotEnableGridLines(int index, bool set);
   bool __fastcall            GetPlotEnableTraceStorage(int index);
   void __fastcall            SetPlotEnableTraceStorage(int index, bool set);
   bool __fastcall            GetPlotIsRealTime(int index);
   void __fastcall            SetPlotIsRealTime(int index, bool set);
   TColor __fastcall          GetPlotGridLinesColor(int index);
   void __fastcall            SetPlotGridLinesColor(int index, TColor color);
   TColor __fastcall          GetPlotAxesColor(int index);
   void __fastcall            SetPlotAxesColor(int index, TColor color);
   TFont* __fastcall          GetPlotAxesFont(int index);
   void __fastcall            SetPlotAxesFont(int index, TFont* font);
   TColor __fastcall          GetPlotBackgroundColor(int index);
   void __fastcall            SetPlotBackgroundColor(int index, TColor color);
   TColor __fastcall          GetPlotPenColor(int index);
   void __fastcall            SetPlotPenColor(int index, TColor color);
   unsigned char __fastcall   GetPlotTraceStorageFade(int index);
   void __fastcall            SetPlotTraceStorageFade(int index, unsigned char fade);

   __property float           PlotFullScaleMaxX[int index] =
      {read = GetPlotFullScaleMaxX, write = SetPlotFullScaleMaxX};

   __property float           PlotVisibleMaxX[int index] =
      {read = GetPlotVisibleMaxX, write = SetPlotVisibleMaxX};

   __property float           PlotFullScaleMinX[int index] =
      {read = GetPlotFullScaleMinX, write = SetPlotFullScaleMinX};

   __property float           PlotVisibleMinX[int index] =
      {read = GetPlotVisibleMinX, write = SetPlotVisibleMinX};

   __property bool            PlotEnableGridLines[int index] =
      {read = GetPlotEnableGridLines, write = SetPlotEnableGridLines};

   __property bool            PlotEnableTraceStorage[int index] =
      {read = GetPlotEnableTraceStorage, write = SetPlotEnableTraceStorage};

   __property bool            PlotIsRealTime[int index] =
      {read = GetPlotIsRealTime, write = SetPlotIsRealTime};

   __property TColor          PlotGridLinesColor[int index] =
      {read = GetPlotGridLinesColor, write = SetPlotGridLinesColor};

   __property TColor          PlotAxesColor[int index] =
      {read = GetPlotAxesColor, write = SetPlotAxesColor};

   __property TFont*          PlotAxesFont[int index] =
      {read = GetPlotAxesFont, write = SetPlotAxesFont};

   __property TColor          PlotBackgroundColor[int index] =
      {read = GetPlotBackgroundColor, write = SetPlotBackgroundColor};

   __property TColor          PlotPenColor[int index] =
      {read = GetPlotPenColor, write = SetPlotPenColor};

   __property unsigned char   PlotTraceStorageFade[int index] =
      {read = GetPlotTraceStorageFade, write = SetPlotTraceStorageFade};

/*
   __property TColor          PlotInformationColor[int index] =
      {read = GetPlotInformationColor, write = SetPlotInformationColor};
*/

__published:

   __property TDAQPlotSettings* Plot0 = {read = FPlot0, write = FPlot0};
   __property TDAQPlotSettings* Plot1 = {read = FPlot1, write = FPlot1};
   __property TDAQPlotSettings* Plot2 = {read = FPlot2, write = FPlot2};
   __property TDAQPlotSettings* Plot3 = {read = FPlot3, write = FPlot3};
   __property TDAQPlotSettings* Plot4 = {read = FPlot4, write = FPlot4};
   __property TDAQPlotSettings* Plot5 = {read = FPlot5, write = FPlot5};
   __property TDAQPlotSettings* Plot6 = {read = FPlot6, write = FPlot6};
   __property TDAQPlotSettings* Plot7 = {read = FPlot7, write = FPlot7};


   __property bool            TimeLocked =
      {read = FTimeLocked, write = SetTimeLocked, default = false};

   __property bool            GlobalChanges =
      {read = FGlobalChanges, write = SetGlobalChanges, default = false};

   __property ZoomModeEnum    ZoomMode =
      {read = FZoomMode, write = SetZoomMode, default = DEFAULTZOOMMODE};

   __property bool            EnableMouseZoom =
      {read = FEnableMouseZoom, write = SetEnableMouseZoom, default = DEFAULTENABLEMOUSEZOOM};

   __property int             DrawEraseExtra =
      {read = FDrawEraseExtra, write = SetDrawEraseExtra, default = DEFAULTDRAWERASEEXTRA};

   __property bool            IgnoreOverrun =
      {read = FIgnoreOverrun, write = SetIgnoreOverrun, default = DEFAULTIGNOREOVERRUN};

public:
   __fastcall TMultiDAQPlotSettings(TDAQPlot **plot);
   __fastcall ~TMultiDAQPlotSettings();

};

//---------------------------------------------------------------------------
#endif
