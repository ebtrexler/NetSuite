//---------------------------------------------------------------------------

#ifndef MultiPLOTPanelH
#define MultiPLOTPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <ExtCtrls.hpp>
#include "plottingthread.h"
#include "PLOTPanel.h"

#define MAXPANELS 8
//---------------------------------------------------------------------------

//classes to allow streaming of splitter positions (PercentTop)
class PACKAGE TDAQFloat : public TCollectionItem
{
private:
   double                  FValue;
   UnicodeString           FName;
   void __fastcall         SetName(String name);
protected:
   //override TCollectionItem::GetDisplayName
   virtual UnicodeString __fastcall GetDisplayName(void);
public:
   virtual void __fastcall Assign(TPersistent* Source);
   __fastcall              TDAQFloat(TCollection *Collection);
__published:
	__property double Value = {read = FValue, write = FValue};
   __property UnicodeString   Name = {read = FName, write = SetName};
};

class PACKAGE TSplitterPositions : public TCollection
{
private:
   TComponent              *FComponentOwner;
   TDAQFloat* __fastcall   GetItem(int Index);
   void __fastcall         SetItem(int Index, TDAQFloat *Value);
protected:
   DYNAMIC TPersistent* __fastcall
                           GetOwner(void);
public:
   __fastcall              TSplitterPositions(TComponent *owner);
   TDAQFloat* __fastcall   Add();
   __property TDAQFloat   *Items[int Index] = {read = GetItem, write = SetItem};
};
///////////////////////////////////////////////////////////////////////////////


enum TileDirectionEnum {tdVertical, tdHorizontal, NumTileDirections};

class PACKAGE TMultiPLOTPanel : public TCustomPanel
{
private:
   // property storage and access
   TSplitterPositions         *FSplitterPositions;
   void __fastcall            SetSplitterPositions(TSplitterPositions *pos);
   unsigned short             FSplitterThickness;
   void __fastcall            SetSplitterThickness(unsigned short thickness);
   TColor							FSplitterColor;
   void __fastcall				SetSplitterColor(TColor color);
   int                        FNumVisiblePlots;
   void __fastcall            SetNumVisiblePlots(int np);
   bool                       FGlobalChanges;
	void __fastcall            SetGlobalChanges(bool set);
   MouseModeEnum              FMouseMode;
   void __fastcall            SetMouseMode(MouseModeEnum mode);
   int                        FDrawEraseExtra;
   void __fastcall            SetDrawEraseExtra(int extra);
   bool                       FIgnoreOverrun;
   void __fastcall            SetIgnoreOverrun(bool set);
   bool                       FIsRealTimePlot;
	void __fastcall            SetIsRealTimePlot(bool set);
	TileDirectionEnum				FTileDirection;
	void __fastcall				SetTileDirection(TileDirectionEnum td);
	bool						      FRTPlotEnabled;
	void __fastcall				SetRTPlotEnabled(bool set);

	TNotifyEvent               FOnXAxisChange;

   // needed for resizing
   int                        SplitterIndex;
   bool                       MouseLBDown;
	void __fastcall            ResizePlots();

   void __fastcall PanelResize(TObject *Sender);
   void __fastcall PanelMouseDown(TObject* Sender, TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y);
   void __fastcall PanelMouseMove(TObject* Sender, Classes::TShiftState Shift,
                                 int X, int Y);
   void __fastcall PanelMouseUp(TObject* Sender, TMouseButton Button,
                                 Classes::TShiftState Shift, int X, int Y);
   void __fastcall PLOTPanelAxesColorChange(TObject *Sender);
   void __fastcall PLOTPanelAxesFontChange(TObject *Sender);
   void __fastcall PLOTPanelBackgroundColorChange(TObject *Sender);
   void __fastcall PLOTPanelGridLinesChange(TObject *Sender);
   void __fastcall PLOTPanelGridLinesColorChange(TObject *Sender);
   void __fastcall PLOTPanelPenColorChange(TObject *Sender);
	void __fastcall PLOTPanelMouseModeChange(TObject *Sender);
	void __fastcall PLOTPanelXAxisChange(TObject *Sender);
	void __fastcall PLOTPanelYAxisChange(TObject *Sender);
	void __fastcall PLOTPanelOverrun(TObject *Sender, bool &DoHandleOverrun);
	void __fastcall PlottingThreadTerminated(TObject *Sender);

protected:

   virtual void __fastcall Loaded();
   virtual void __fastcall BeforeDestruction();

public:
   __fastcall TMultiPLOTPanel(TComponent* Owner);

   TPLOTPanel                  *PLOTPanel[MAXPANELS];

   // plotting thread
	TPlottingThread            *PlottingThread;
	CRITICAL_SECTION				Plot_CS;

   void __fastcall            WriteConfigFile(String filename);

__published:

   __property TPLOTPanel       *PLOTPanel0 = {read = PLOTPanel[0]};
   __property TPLOTPanel       *PLOTPanel1 = {read = PLOTPanel[1]};
   __property TPLOTPanel       *PLOTPanel2 = {read = PLOTPanel[2]};
   __property TPLOTPanel       *PLOTPanel3 = {read = PLOTPanel[3]};
   __property TPLOTPanel       *PLOTPanel4 = {read = PLOTPanel[4]};
   __property TPLOTPanel       *PLOTPanel5 = {read = PLOTPanel[5]};
   __property TPLOTPanel       *PLOTPanel6 = {read = PLOTPanel[6]};
   __property TPLOTPanel       *PLOTPanel7 = {read = PLOTPanel[7]};

   __property TSplitterPositions *SplitterPositions =
	  {read = FSplitterPositions, write = SetSplitterPositions};

   __property unsigned short  SplitterThickness =
	  {read = FSplitterThickness, write = SetSplitterThickness, default = 5};

   __property TColor				SplitterColor =
	{read = FSplitterColor, write = SetSplitterColor, default = clWindow};

   __property int             NumVisiblePlots =
	  {read = FNumVisiblePlots, write = SetNumVisiblePlots};

   __property bool            GlobalChanges =
	  {read = FGlobalChanges, write = SetGlobalChanges, default = false};

   __property MouseModeEnum   MouseMode =
	  {read = FMouseMode, write = SetMouseMode, default = DEFAULTMOUSEMODE};

   __property int             DrawEraseExtra =
	  {read = FDrawEraseExtra, write = SetDrawEraseExtra, default = DEFAULTDRAWERASEEXTRA};

   __property bool            IgnoreOverrun =
	  {read = FIgnoreOverrun, write = SetIgnoreOverrun, default = DEFAULTIGNOREOVERRUN};

   __property bool            IsRealTimePlot =
		{read = FIsRealTimePlot, write = SetIsRealTimePlot, default = false};

	__property TileDirectionEnum TileDirection =
	{read = FTileDirection, write = SetTileDirection, default = tdHorizontal};

	__property bool			   RTPlotEnabled =
		{read = FRTPlotEnabled, write = SetRTPlotEnabled, default = false};

   __property TNotifyEvent    OnXAxisChange =
	  {read = FOnXAxisChange, write = FOnXAxisChange};

   // properties now published from TCustomPanel
   __property Align;
   __property Color;
   __property ParentFont;
   __property ParentShowHint;
   __property Visible;
   __property Anchors;
   __property Constraints;

};
//---------------------------------------------------------------------------
#endif
