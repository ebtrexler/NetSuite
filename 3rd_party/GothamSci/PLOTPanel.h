//---------------------------------------------------------------------------

#ifndef PLOTPanelH
#define PLOTPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "InputBox.h"
#include "GR32_Image.hpp"

#include "synapfileheader.h"

typedef Types::TRect  TRect;


class TPLOTPanel; // forward declaration

// Definition of mouse modes
enum MouseModeEnum {SelectXRange, XZoom, SelectYRange, YZoom, SelectBox, XYZoom,
							MarkX, MarkY, MarkLine, XYPan, SelectPlot, MouseIdle};

// Selection rectangle
typedef struct {
   double Left, Top, Right, Bottom;
} SelectionRect;
SelectionRect MakeSelectionRect(double left, double top, double right, double bottom)
{
   SelectionRect temprect;
   temprect.Left = left;
   temprect.Top = top;
   temprect.Right = right;
   temprect.Bottom = bottom;
   return temprect;
}

// Events fired when user selects X, Y, or XY region --
typedef void __fastcall (__closure *TSelectRangeEvent)(System::TObject* Sender,
                                                         double Min, double Max);
typedef void __fastcall (__closure *TSelectRectEvent)(System::TObject* Sender,
                                                      SelectionRect SelectRect);
// Events fired when user selects X, Y, or XY Lines --
typedef void __fastcall (__closure *TMarkEvent)(System::TObject* Sender,
                                                   double Mark);
typedef void __fastcall (__closure *TMarkLineEvent)(System::TObject* Sender,
                                                   double X1, double Y1,
                                                   double X2, double Y2);

// Event fired for buffer overruns during realtime plotting
typedef void __fastcall (__closure *TOverrunEvent)(System::TObject* Sender,
                                                   bool &DoHandleOverrun);

// Event fired when tag is clicked
typedef unsigned long TagIndexType;
typedef void __fastcall (__closure *TEventTagClickEvent)(System::TObject *Sender,
                                                   TagIndexType TagIndex);

// Event fired when an indexed is selected
typedef unsigned long ulong;
typedef void __fastcall (__closure *TSelectIndexEvent)(System::TObject *Sender,
                                                   ulong Index);

#define DEFAULTMOUSEMODE MouseIdle
#define DEFAULTDRAWERASEEXTRA 10
#define DEFAULTIGNOREOVERRUN false

#define MAXVIEWPORTS 20 // definition of stored zooms

typedef struct {
   double MinX, MinY, MaxX, MaxY;
} ViewPortRect;

// Transfer buffer for plotting acquired time series samples 
class PACKAGE TDAQSampleBuffer
{
private:
   bool              DeleteMemory;        // flag for deletion of samples if
                                          // memory claimed in constructor
public:
   int				   Which;					// which buffer in circle
   bool				   IsFull;					// flag for input/output
	double 			   FSMaxY, FSMinY;		// in UserUnits
   double 			   UUperBit, OffsetUU;  // in UserUnits
   double				   SampleSpacing;			// in seconds for time series
   unsigned long 	   NumSamples;
   short 			   *Samples;

   TDAQSampleBuffer	*Next;
   TDAQSampleBuffer();                 // must allocate and free *Samples memory
   TDAQSampleBuffer(unsigned long numsamples);          // memory allocated here
   ~TDAQSampleBuffer();
};

// Base class for all plots
class PACKAGE TStaticPlot
{
public:
   TPLOTPanel         *Parent;
   int               Which;
   bool              Visible;
   bool              Selected;
   String            Label;
   TColor            Color;
   double 			   FSMinX, FSMaxX, FSMinY, FSMaxY;
   virtual bool      DrawToDAQPanel() = 0;
   virtual bool      HitTest(int X, int Y) = 0;
   virtual void      FixupXandYLimits(double fsxmin) = 0;
                     TStaticPlot(TPLOTPanel *parent,
                                 int which = 0, String label = "",
                                 TColor color = clBlue,
                                 double fsxmin = 0, double fsxmax = 0,
								 double fsymin = 0, double fsymax = 0);
};

// Container for time series plot with evenly or unevenly spaced samples in t
// Note:  Individual DAQBuffers created and destroyed elsewhere, in file i-o
//        but array of pointers DAQBuffers*[] created and destroyed here
class PACKAGE TStaticDAQPlot : public TStaticPlot
{
public:
   unsigned long     NumDAQBuffers;
   TDAQSampleBuffer  **DAQBuffers;
   unsigned long     Increment; //used for plotting and HitTest
                     TStaticDAQPlot(TPLOTPanel *parent,
                                    int which = 0, String label = "",
                                    TColor color = clBlue, double fsxmin = 0,
                                    unsigned long numdaqbuffers = 0,
                                    TDAQSampleBuffer *firstdaqbuffer = NULL);
                     ~TStaticDAQPlot();
   virtual bool      DrawToDAQPanel();
   virtual bool      HitTest(int X, int Y);
   virtual void      FixupXandYLimits(double fsxmin);
};

// Container for x-y scatter plot
typedef struct tagXYPair
{
   double            X;
   double            Y;
} XYPair;

//class PACKAGE TStaticXYScatterPlot : public TStaticPlot
//{
//   unsigned long     NumXYPairs;
//   XYPair            *Pairs;
//					 TStaticXYScatterPlot(TPLOTPanel *parent,
//										  int which = 0, String label = "",
//										  unsigned long numxypairs = 0,
//										  XYPair *pairs = NULL) :
//										  TStaticPlot(parent, which, label),
//										  NumXYPairs(numxypairs),
//										  Pairs(pairs)
//										  {}
//   virtual bool      DrawToDAQPanel(TPLOTPanel *daqpanel);
//   virtual bool      HitTest(int X, int Y);
//   virtual void      FixupXandYLimits(double fsxmin);
//};

// Container for 2-dimensional plot with evenly spaced samples along the abscissa
// Note:  Array of plot values created and destroyed elsewhere -- watch for leak
class PACKAGE TStaticFunctionPlot : public TStaticPlot
{
public:
   unsigned long     NumPlotValues;
   double            *PlotValues;
   double            SampleSpacing; //used for plotting and HitTest
                     TStaticFunctionPlot (TPLOTPanel *parent,
                                          int which = 0, String label = "",
                                          TColor color = clBlue, double fsxmin = 0,
                                          double samplespacing = 0,
                                          unsigned long numplotvalues = 0,
                                          double *plotvalues = NULL);
                     ~TStaticFunctionPlot();
   virtual bool      DrawToDAQPanel();
   virtual bool      HitTest(int X, int Y);
   virtual void      FixupXandYLimits(double fsxmin);
};

//---------------------------------------------------------------------------
class PACKAGE TPLOTPanel : public TCustomPanel
{
private:
   TPanel *TagPanel;
   TPaintBox32 *TagBox;
   TLabel *TagLabel;
   TPanel *InfoPanel;
   TPaintBox32 *Graph;
	TPaintBox32 *YAxis;
	TPaintBox32 *XAxis;
   TInputBox *YMaxInputBox;
   TInputBox *YMinInputBox;
   TInputBox *XMaxInputBox;
   TInputBox *XMinInputBox;
   TButton *ZoomButton;
   TButton *DisplayButton;
   TCheckBox *TimeLockCheckBox;
   TTimer *ResizeTimer;
   TPopupMenu *DisplayPopupMenu;
   TMenuItem *ShowGridLines1;
   TMenuItem *PenColor1;
   TMenuItem *BackgroundColor1;
   TMenuItem *AxesColor1;
   TMenuItem *GridLinesColor1;
   TMenuItem *Font1;
   TPopupMenu *ZoomPopupMenu;
   TMenuItem *XZoom1;
   TMenuItem *YZoom1;
   TMenuItem *XYZoom1;
   TMenuItem *OFF1;
   TColorDialog *ColorDialog1;
   TFontDialog *FontDialog1;

   void __fastcall ResizeTimerTimer(TObject *Sender);
   void __fastcall GraphMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall GraphMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall GraphMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall GraphDblClick(TObject *Sender);
   void __fastcall YMaxInputBoxInput(TObject *Sender);
   void __fastcall YMinInputBoxInput(TObject *Sender);
   void __fastcall XMinInputBoxInput(TObject *Sender);
   void __fastcall XMaxInputBoxInput(TObject *Sender);
   void __fastcall XZoom1Click(TObject *Sender);
   void __fastcall YZoom1Click(TObject *Sender);
   void __fastcall XYZoom1Click(TObject *Sender);
   void __fastcall OFF1Click(TObject *Sender);
   void __fastcall ShowGridLines1Click(TObject *Sender);
   void __fastcall GridLinesColor1Click(TObject *Sender);
   void __fastcall PenColor1Click(TObject *Sender);
   void __fastcall BackgroundColor1Click(TObject *Sender);
   void __fastcall AxesColor1Click(TObject *Sender);
   void __fastcall Font1Click(TObject *Sender);
   void __fastcall ZoomButtonClick(TObject *Sender);
   void __fastcall DisplayButtonClick(TObject *Sender);
   void __fastcall TimeLockCheckBoxClick(TObject *Sender);
   void __fastcall TagBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall TagBoxMouseMove(TObject *Sender, TShiftState Shift,
			 int X, int Y);

// property events storage
	TNotifyEvent				FOnBeginResize;
	TNotifyEvent				FOnEndResize;
	TNotifyEvent            FOnXAxisChange;
	TNotifyEvent            FOnYAxisChange;
	TNotifyEvent            FOnGridLinesChange;
	TNotifyEvent            FOnPenColorChange;
	TNotifyEvent            FOnBackgroundColorChange;
	TNotifyEvent            FOnAxesColorChange;
	TNotifyEvent            FOnGridLinesColorChange;
	TNotifyEvent            FOnAxesFontChange;
	TNotifyEvent            FOnMouseModeChange;
	TSelectRangeEvent       FOnSelectXRange;
	TSelectRangeEvent       FOnSelectYRange;
	TSelectRectEvent        FOnSelectRect;
	TMarkEvent              FOnMarkX;
   TMarkEvent              FOnMarkY;
   TMarkLineEvent          FOnMarkLine;
   TOverrunEvent           FOnOverrun;
   TSelectIndexEvent       FOnEventTagClick;
   TSelectIndexEvent       FOnSelectPlot;

	// property variables storage
   double							FFullScaleMinY, FVisibleMinY,	//in YUnits
   								FFullScaleMaxY, FVisibleMaxY;	//
   double							FFullScaleMaxX, FVisibleMaxX, // in seconds
   								FFullScaleMinX, FVisibleMinX; //
   MouseModeEnum				FMouseMode;
   bool							FEnableGridLines;
   TColor32						FPenColor;
   TColor32						FBackgroundColor;
   TColor32						FAxesColor;
   TColor32						FGridLinesColor;
   String					   FYUnits;
   String                  FXUnits;
   bool							FIsRealTimePlot;
   bool                    FTimeLocked;
   TFont                   *FAxesFont;
   bool					      FEnableTraceStorage;
   unsigned char		      FTraceStorageFade;
   bool                    FIgnoreOverrun;
   int                     FDrawEraseExtra;
	bool                    FConcatenatePlots;
	bool							FResetUponDraw;

   // drawing variables
   bool							DelayRedraw;
   TColor32						ZoomColor;
	int 							IsResizing;
   bool							MouseLBDown;
   bool							MouseHasMoved;
   int							MousePixXOrig, MousePixYOrig;
	::TRect			 			MouseRect, SelectRect;
   ViewPortRect            Viewports[MAXVIEWPORTS];
   int                     NumStoredViewports;
   double							XGridLines[20];
   int							NumXGridLines;
   double							YGridLines[20];
   int							NumYGridLines;
   int                     XDecimalPlaces;
   int                     YDecimalPlaces;
	bool                    XAxisHasChanged;
	bool							YAxisHasChanged;
   TCanvas                 *MouseCanvas;
   Types::TPoint           LastMarkPoint;
   bool                    IsAHit;
   unsigned long           SelectedPlotIndex;
   unsigned long           HighlightedTag;
   bool                    TagHighlighted;

   // realtime circular buffer plotting variables
   TDAQSampleBuffer	      *FirstEmptyBuffer,
   						      *FirstFullBuffer;
   double					      XTimeProgress;
   double					      YSampleLast;
   unsigned short		      NumBuffersAllocated;
   unsigned				      NumBuffersPlotted;
   unsigned				      NumBuffersPerTrace;
   bool            			OverRun;

	// drawing functions
	int __fastcall				GetGridLines(double max, double range,
   									double *gridlines, int tickfactor,
   									int &decimalplaces);
	void __fastcall         InvertRGBRect(::TRect &rect, TBitmap32 *bitmap);
   void __fastcall         DrawAxesSelect();
   void __fastcall			ZoomX();
   void __fastcall			ZoomY();
   void __fastcall         StoreViewport();

   // circular buffer methods
   void __fastcall	      FreeBuffers();
   bool __fastcall	      AnyEmptyBuffers();
   TDAQSampleBuffer* __fastcall
   						      GetNextEmptyBuffer();
   bool __fastcall	      AnyFullBuffers();
   TDAQSampleBuffer* __fastcall
   						      GetNextFullBuffer();

   // property access functions
   void __fastcall			SetFullScaleMinY(double fsminy);
   void __fastcall			SetFullScaleMaxY(double fsmaxy);
   void __fastcall			SetFullScaleMinX(double fsminx);
   void __fastcall			SetFullScaleMaxX(double fsmaxx);
   void __fastcall			SetVisibleMinY(double visminy);
   void __fastcall			SetVisibleMaxY(double vismaxy);
   void __fastcall			SetVisibleMinX(double visminx);
   void __fastcall			SetVisibleMaxX(double	vismaxx);
   void __fastcall         SetEnableMouseZoom(bool enable);
   void __fastcall			SetEnableGridLines(bool enable);
   void __fastcall			SetPenColor(TColor color);
   void __fastcall			SetBackgroundColor(TColor color);
   void __fastcall			SetAxesColor(TColor color);
   void __fastcall			SetGridLinesColor(TColor color);
   TColor __fastcall			GetPenColor();
   TColor __fastcall			GetBackgroundColor();
   TColor __fastcall			GetAxesColor();
   TColor __fastcall			GetGridLinesColor();
   void __fastcall         SetAxesFont(TFont *font);
   String __fastcall	      GetYUnits();
   void __fastcall			SetYUnits(const String yu);
   String __fastcall	      GetXUnits();
   void __fastcall			SetXUnits(const String xu);
   void __fastcall			SetIsRealTimePlot(bool set);
   void __fastcall			SetTimeLocked(bool set);
   void __fastcall	      SetEnableTraceStorage(bool set);
   void __fastcall	      SetTraceStorageFade(unsigned char fade);
   void __fastcall         SetIgnoreOverrun(bool set);
   void __fastcall         SetDrawEraseExtra(int extra);
   void __fastcall         SetConcatenatePlots(bool set);

protected:
   // TComponent override -- called upon completion of reading in component
   virtual void __fastcall Loaded(void);
   virtual void __fastcall AfterConstruction();

   // drawing variables accessed in derived classes
   double							PixelsPerYUnit;
   double							PixelsPerXUnit;
   double							ZeroYPixels;

   // drawing functions accessed in derived classes
   virtual void __fastcall Draw(bool flush);
	virtual void __fastcall	DrawGrid(bool flush);
	virtual void __fastcall	DrawPartialGrid(bool store, ::TRect &updateRect, int extra);
   virtual void __fastcall DrawPlots(bool flush);
   virtual void __fastcall DrawEventTags();
   virtual void __fastcall DrawMarks(bool flush);
   virtual void __fastcall DrawRanges(bool flush);

   // event firing methods overridden from TControl
   DYNAMIC void __fastcall MouseDown(TMouseButton Button,
                                       Classes::TShiftState Shift, int X, int Y);
   DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
   DYNAMIC void __fastcall MouseUp(TMouseButton Button,
                                       Classes::TShiftState Shift, int X, int Y);
   virtual bool __fastcall CanResize(int &NewWidth, int &NewHeight);

   // event firing methods created for tags
   virtual void __fastcall DoTagBoxMouseDown(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y);
   virtual void __fastcall DoTagBoxMouseMove(Classes::TShiftState Shift,
   									int X, int Y);

   // notification of AxesFont changed
   DYNAMIC void __fastcall AxesFontChanged(TObject *Sender);

   // event handling and generation methods
   virtual void __fastcall DoMouseDown(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y);
   virtual void __fastcall DoMouseUp(TMouseButton Button,
   									Classes::TShiftState Shift, int X, int Y);
   virtual void __fastcall DoMouseMove(Classes::TShiftState Shift,
   									int X, int Y);
   virtual void __fastcall DoBeginResize();
   virtual void __fastcall	DoEndResize();

public:
   __fastcall TPLOTPanel(TComponent* Owner);
   __fastcall ~TPLOTPanel();

   bool					      Reset; // for restarting realtime plot

   // EventTag methods (members and functions)
   unsigned long           NumEventTags;
	NewDAQ::EventTagStruct          *EventTags;
   virtual void __fastcall AddEventTags(unsigned long numtags, NewDAQ::EventTagStruct *tags);

	// TStaticPlot handlers (members and functions)
   friend class TStaticPlot;
   friend class TStaticDAQPlot;
   friend class TStaticFunctionPlot;

   unsigned long           NumPlots;
   TStaticPlot             **Plots;
   virtual bool __fastcall AddPlot(TStaticPlot *plot, bool flush);
   virtual void __fastcall ClearPlots();
   virtual void __fastcall ScalePlots(bool flush);

   // X and Y marker handlers (members and functions)
   double                   *XMarks;
   double                   *YMarks;
   unsigned long           NumXMarks;
   unsigned long           NumYMarks;
   virtual bool __fastcall AddXMark(double mark, bool flush);
   virtual bool __fastcall AddYMark(double mark, bool flush);
   virtual void __fastcall ClearXMarks();
   virtual void __fastcall ClearYMarks();

   void __fastcall 			BeginUpdate();
   void __fastcall 			EndUpdate();
   void __fastcall         FinalUpdate(bool flush);
   void __fastcall			SetMouseMode(MouseModeEnum mode);

   TDAQSampleBuffer* __fastcall
                           AllocateBuffers(unsigned short numbuffers,
   											unsigned long numsamples);
	virtual bool __fastcall FillBuffer(double maxY, double minY, double uuperbit,
										double offsetuu, double sampletime, short *samples,
										unsigned long numsamples, int *buffnum = NULL);
	virtual bool __fastcall FillBufferMUX(double maxY, double minY, double uuperbit,
										double offsetuu, double sampletime, short *muxsamples,
										unsigned long numsamples, int muxoffset, int muxincr,
										int *buffnum = NULL);
   virtual bool __fastcall PlotNextBuffer(int *buffnum = NULL);

   // event properties -- really only used in MultiDAQPanel
   __property TNotifyEvent       OnXAxisChange =
					{read = FOnXAxisChange, write = FOnXAxisChange};

	__property TNotifyEvent			OnYAxisChange =
					{read = FOnYAxisChange, write = FOnYAxisChange};

   __property TNotifyEvent       OnGridLinesChange =
               {read = FOnGridLinesChange, write = FOnGridLinesChange};

   __property TNotifyEvent       OnPenColorChange =
               {read = FOnPenColorChange, write = FOnPenColorChange};

   __property TNotifyEvent       OnBackgroundColorChange =
               {read = FOnBackgroundColorChange, write = FOnBackgroundColorChange};

   __property TNotifyEvent       OnAxesColorChange =
               {read = FOnAxesColorChange, write = FOnAxesColorChange};

   __property TNotifyEvent       OnGridLinesColorChange =
               {read = FOnGridLinesColorChange, write = FOnGridLinesColorChange};

   __property TNotifyEvent       OnAxesFontChange =
               {read = FOnAxesFontChange, write = FOnAxesFontChange};

   __property TNotifyEvent       OnMouseModeChange =
               {read = FOnMouseModeChange, write = FOnMouseModeChange};

__published:

   // properties -- events
   __property TNotifyEvent			OnBeginResize =
               {read = FOnBeginResize, write = FOnBeginResize};

   __property TNotifyEvent			OnEndResize =
   				{read = FOnEndResize, write = FOnEndResize};

   __property TSelectRangeEvent  OnSelectXRange =
               {read = FOnSelectXRange, write = FOnSelectXRange};

   __property TSelectRangeEvent  OnSelectYRange =
               {read = FOnSelectYRange, write = FOnSelectYRange};

   __property TSelectRectEvent   OnSelectRect =
               {read = FOnSelectRect, write = FOnSelectRect};

   __property TMarkEvent         OnMarkX =
               {read = FOnMarkX, write = FOnMarkX};

   __property TMarkEvent         OnMarkY =
               {read = FOnMarkY, write = FOnMarkY};

   __property TMarkLineEvent     OnMarkLine =
               {read = FOnMarkLine, write = FOnMarkLine};

   __property TOverrunEvent      OnOverrun =
               {read = FOnOverrun, write = FOnOverrun};

   __property TSelectIndexEvent  OnEventTagClick =
               {read = FOnEventTagClick, write = FOnEventTagClick};

   __property TSelectIndexEvent  OnSelectPlot =    
               {read = FOnSelectPlot, write = FOnSelectPlot};

	// properties -- variables
   __property double					FullScaleMinY =
				{read = FFullScaleMinY, write = SetFullScaleMinY, default = -100};

   __property double					VisibleMinY =
				{read = FVisibleMinY, write = SetVisibleMinY, default = -100};

   __property double					FullScaleMaxY =
				{read = FFullScaleMaxY, write = SetFullScaleMaxY, default = 100};

   __property double					VisibleMaxY =
				{read = FVisibleMaxY, write = SetVisibleMaxY, default = 100};

   __property double					FullScaleMaxX =
				{read = FFullScaleMaxX, write = SetFullScaleMaxX, default = 1};

   __property double					VisibleMaxX =
				{read = FVisibleMaxX, write = SetVisibleMaxX, default = 1};

   __property double					FullScaleMinX =
				{read = FFullScaleMinX, write = SetFullScaleMinX, default = 0};

   __property double					VisibleMinX =
				{read = FVisibleMinX, write = SetVisibleMinX, default = 0};

   __property MouseModeEnum      MouseMode =
				{read = FMouseMode, write = SetMouseMode, default = MouseIdle};

   __property bool					EnableGridLines =
				{read = FEnableGridLines, write = SetEnableGridLines, default = true};

   __property TColor					PenColor =
				{read = GetPenColor, write = SetPenColor, default = clRed};

   __property TColor					BackgroundColor =
   	{read = GetBackgroundColor, write = SetBackgroundColor, default = clBlack};

   __property TColor					AxesColor =
				{read = GetAxesColor, write = SetAxesColor, default = clGray};

   __property TColor					GridLinesColor =
		{read = GetGridLinesColor, write = SetGridLinesColor, default = clGreen};

   __property TFont*             AxesFont =
      {read = FAxesFont, write = SetAxesFont};

   __property String			      YUnits =
				{read = GetYUnits, write = SetYUnits};

   __property String		         XUnits =
				{read = GetXUnits, write = SetXUnits};

   __property bool					IsRealTimePlot =
   			{read = FIsRealTimePlot, write = SetIsRealTimePlot};

   __property bool               TimeLocked =
            {read = FTimeLocked, write = SetTimeLocked};

    __property bool 	            EnableTraceStorage =
	{read = FEnableTraceStorage, write = SetEnableTraceStorage, default = false};

   __property unsigned char	   TraceStorageFade =
   {read = FTraceStorageFade, write = SetTraceStorageFade, default = false};

   __property bool               IgnoreOverrun =
	{read = FIgnoreOverrun, write = SetIgnoreOverrun, default = false};

	__property int                DrawEraseExtra =
	{read = FDrawEraseExtra, write = SetDrawEraseExtra, default = 10};

	__property bool               ConcatenatePlots =
	{read = FConcatenatePlots, write = SetConcatenatePlots, default = false};

	__property bool               ResetUponDraw =
	{read = FResetUponDraw, write = FResetUponDraw, default = true};


   // properties now published from TCustomPanel or other ancestor
   __property Align;
   __property ParentFont;
   __property ParentShowHint;
   __property Visible;
   __property Anchors;
   __property Constraints;
   __property PopupMenu;
   __property DragKind;
   __property DragMode;
//   __property Name;    // pity, cannot publish because of TMultiDAQPanel

};
//---------------------------------------------------------------------------
#endif
