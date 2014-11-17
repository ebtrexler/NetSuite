// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_image.pas' rev: 21.00

#ifndef Gr32_imageHPP
#define Gr32_imageHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Gr32.hpp>	// Pascal unit
#include <Gr32_layers.hpp>	// Pascal unit
#include <Gr32_rangebars.hpp>	// Pascal unit
#include <Gr32_lowlevel.hpp>	// Pascal unit
#include <Gr32_system.hpp>	// Pascal unit
#include <Gr32_containers.hpp>	// Pascal unit
#include <Gr32_repaintopt.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_image
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TPaintStageEvent)(System::TObject* Sender, Gr32::TBitmap32* Buffer, unsigned StageNum);

struct TPaintStage;
typedef TPaintStage *PPaintStage;

struct TPaintStage
{
	
public:
	bool DsgnTime;
	bool RunTime;
	unsigned Stage;
	unsigned Parameter;
};


class DELPHICLASS TPaintStages;
class PASCALIMPLEMENTATION TPaintStages : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	typedef DynamicArray<TPaintStage> _TPaintStages__1;
	
	
public:
	PPaintStage operator[](int Index) { return Items[Index]; }
	
private:
	_TPaintStages__1 FItems;
	PPaintStage __fastcall GetItem(int Index);
	
public:
	__fastcall virtual ~TPaintStages(void);
	PPaintStage __fastcall Add(void);
	void __fastcall Clear(void);
	int __fastcall Count(void);
	void __fastcall Delete(int Index);
	PPaintStage __fastcall Insert(int Index);
	__property PPaintStage Items[int Index] = {read=GetItem/*, default*/};
public:
	/* TObject.Create */ inline __fastcall TPaintStages(void) : System::TObject() { }
	
};


#pragma option push -b-
enum TBitmapAlign { baTopLeft, baCenter, baTile, baCustom };
#pragma option pop

#pragma option push -b-
enum TScaleMode { smNormal, smStretch, smScale, smResize, smOptimal, smOptimalScaled };
#pragma option pop

#pragma option push -b-
enum Gr32_image__2 { pboWantArrowKeys, pboAutoFocus };
#pragma option pop

typedef Set<Gr32_image__2, pboWantArrowKeys, pboAutoFocus>  TPaintBoxOptions;

#pragma option push -b-
enum TRepaintMode { rmFull, rmDirect, rmOptimizer };
#pragma option pop

class DELPHICLASS TCustomPaintBox32;
class PASCALIMPLEMENTATION TCustomPaintBox32 : public Controls::TCustomControl
{
	typedef Controls::TCustomControl inherited;
	
private:
	Gr32::TBitmap32* FBuffer;
	int FBufferOversize;
	bool FBufferValid;
	TRepaintMode FRepaintMode;
	Gr32_containers::TRectList* FInvalidRects;
	bool FForceFullRepaint;
	Gr32_repaintopt::TCustomRepaintOptimizer* FRepaintOptimizer;
	TPaintBoxOptions FOptions;
	Classes::TNotifyEvent FOnGDIOverlay;
	bool FMouseInControl;
	Classes::TNotifyEvent FOnMouseEnter;
	Classes::TNotifyEvent FOnMouseLeave;
	void __fastcall SetBufferOversize(int Value);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TWMNoParams &Msg);
	HIDESBASE MESSAGE void __fastcall WMPaint(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseEnter(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMInvalidate(Messages::TMessage &Message);
	void __fastcall DirectAreaUpdateHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	
protected:
	virtual void __fastcall SetRepaintMode(const TRepaintMode Value);
	virtual bool __fastcall CustomRepaint(void);
	virtual bool __fastcall InvalidRectsAvailable(void);
	virtual void __fastcall DoPrepareInvalidRects(void);
	virtual void __fastcall DoPaintBuffer(void);
	virtual void __fastcall DoPaintGDIOverlay(void);
	virtual void __fastcall DoBufferResized(const int OldWidth, const int OldHeight);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall MouseEnter(void);
	virtual void __fastcall MouseLeave(void);
	virtual void __fastcall Paint(void);
	void __fastcall ResetInvalidRects(void);
	void __fastcall ResizeBuffer(void);
	__property Gr32_repaintopt::TCustomRepaintOptimizer* RepaintOptimizer = {read=FRepaintOptimizer};
	__property bool BufferValid = {read=FBufferValid, write=FBufferValid, nodefault};
	__property Gr32_containers::TRectList* InvalidRects = {read=FInvalidRects};
	
public:
	__fastcall virtual TCustomPaintBox32(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomPaintBox32(void);
	virtual Types::TRect __fastcall GetViewportRect(void);
	void __fastcall Flush(void)/* overload */;
	void __fastcall Flush(const Types::TRect &SrcRect)/* overload */;
	virtual void __fastcall Invalidate(void);
	virtual void __fastcall ForceFullInvalidate(void);
	virtual void __fastcall Loaded(void);
	DYNAMIC void __fastcall Resize(void);
	virtual void __fastcall SetBounds(int ALeft, int ATop, int AWidth, int AHeight);
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	__property Gr32::TBitmap32* Buffer = {read=FBuffer};
	__property int BufferOversize = {read=FBufferOversize, write=SetBufferOversize, nodefault};
	__property TPaintBoxOptions Options = {read=FOptions, write=FOptions, default=0};
	__property bool MouseInControl = {read=FMouseInControl, nodefault};
	__property TRepaintMode RepaintMode = {read=FRepaintMode, write=SetRepaintMode, default=0};
	__property Classes::TNotifyEvent OnMouseEnter = {read=FOnMouseEnter, write=FOnMouseEnter};
	__property Classes::TNotifyEvent OnMouseLeave = {read=FOnMouseLeave, write=FOnMouseLeave};
	__property Classes::TNotifyEvent OnGDIOverlay = {read=FOnGDIOverlay, write=FOnGDIOverlay};
public:
	/* TWinControl.CreateParented */ inline __fastcall TCustomPaintBox32(HWND ParentWindow) : Controls::TCustomControl(ParentWindow) { }
	
};


class DELPHICLASS TPaintBox32;
class PASCALIMPLEMENTATION TPaintBox32 : public TCustomPaintBox32
{
	typedef TCustomPaintBox32 inherited;
	
private:
	Classes::TNotifyEvent FOnPaintBuffer;
	
protected:
	virtual void __fastcall DoPaintBuffer(void);
	
public:
	__property Canvas;
	
__published:
	__property Align = {default=0};
	__property Anchors = {default=3};
	__property AutoSize = {default=0};
	__property Constraints;
	__property Cursor = {default=0};
	__property DragCursor = {default=-12};
	__property DragMode = {default=0};
	__property Options = {default=0};
	__property ParentShowHint = {default=1};
	__property PopupMenu;
	__property RepaintMode = {default=0};
	__property ShowHint;
	__property TabOrder = {default=-1};
	__property TabStop = {default=0};
	__property Visible = {default=1};
	__property OnCanResize;
	__property OnClick;
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnEndDrag;
	__property OnGDIOverlay;
	__property OnMouseDown;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnMouseWheel;
	__property OnMouseWheelDown;
	__property OnMouseWheelUp;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property Classes::TNotifyEvent OnPaintBuffer = {read=FOnPaintBuffer, write=FOnPaintBuffer};
	__property OnResize;
	__property OnStartDrag;
public:
	/* TCustomPaintBox32.Create */ inline __fastcall virtual TPaintBox32(Classes::TComponent* AOwner) : TCustomPaintBox32(AOwner) { }
	/* TCustomPaintBox32.Destroy */ inline __fastcall virtual ~TPaintBox32(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TPaintBox32(HWND ParentWindow) : TCustomPaintBox32(ParentWindow) { }
	
};


typedef void __fastcall (__closure *TImgMouseEvent)(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer);

typedef void __fastcall (__closure *TImgMouseMoveEvent)(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer);

typedef void __fastcall (__closure *TPaintStageHandler)(Gr32::TBitmap32* Dest, int StageNum);

class DELPHICLASS TCustomImage32;
class PASCALIMPLEMENTATION TCustomImage32 : public TCustomPaintBox32
{
	typedef TCustomPaintBox32 inherited;
	
private:
	typedef DynamicArray<TPaintStageHandler> _TCustomImage32__1;
	
	typedef DynamicArray<int> _TCustomImage32__2;
	
	
private:
	Gr32::TBitmap32* FBitmap;
	TBitmapAlign FBitmapAlign;
	Gr32_layers::TLayerCollection* FLayers;
	float FOffsetHorz;
	float FOffsetVert;
	TPaintStages* FPaintStages;
	_TCustomImage32__1 FPaintStageHandlers;
	_TCustomImage32__2 FPaintStageNum;
	float FScaleX;
	float FScaleY;
	TScaleMode FScaleMode;
	int FUpdateCount;
	Classes::TNotifyEvent FOnBitmapResize;
	Classes::TNotifyEvent FOnChange;
	Classes::TNotifyEvent FOnInitStages;
	TImgMouseEvent FOnMouseDown;
	TImgMouseMoveEvent FOnMouseMove;
	TImgMouseEvent FOnMouseUp;
	TPaintStageEvent FOnPaintStage;
	Classes::TNotifyEvent FOnScaleChange;
	void __fastcall BitmapResizeHandler(System::TObject* Sender);
	void __fastcall BitmapChangeHandler(System::TObject* Sender);
	void __fastcall BitmapAreaChangeHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	void __fastcall BitmapDirectAreaChangeHandler(System::TObject* Sender, const Types::TRect &Area, const unsigned Info);
	void __fastcall LayerCollectionChangeHandler(System::TObject* Sender);
	void __fastcall LayerCollectionGDIUpdateHandler(System::TObject* Sender);
	void __fastcall LayerCollectionGetViewportScaleHandler(System::TObject* Sender, /* out */ float &ScaleX, /* out */ float &ScaleY);
	void __fastcall LayerCollectionGetViewportShiftHandler(System::TObject* Sender, /* out */ float &ShiftX, /* out */ float &ShiftY);
	Gr32::TPixelCombineEvent __fastcall GetOnPixelCombine(void);
	void __fastcall SetBitmap(Gr32::TBitmap32* Value);
	void __fastcall SetBitmapAlign(TBitmapAlign Value);
	void __fastcall SetLayers(Gr32_layers::TLayerCollection* Value);
	void __fastcall SetOffsetHorz(float Value);
	void __fastcall SetOffsetVert(float Value);
	void __fastcall SetScale(float Value);
	void __fastcall SetScaleX(float Value);
	void __fastcall SetScaleY(float Value);
	void __fastcall SetOnPixelCombine(Gr32::TPixelCombineEvent Value);
	
protected:
	Types::TRect CachedBitmapRect;
	float CachedShiftX;
	float CachedShiftY;
	float CachedScaleX;
	float CachedScaleY;
	float CachedRecScaleX;
	float CachedRecScaleY;
	bool CacheValid;
	int OldSzX;
	int OldSzY;
	bool PaintToMode;
	virtual void __fastcall BitmapResized(void);
	virtual void __fastcall BitmapChanged(const Types::TRect &Area);
	virtual bool __fastcall CanAutoSize(int &NewWidth, int &NewHeight);
	virtual void __fastcall DoInitStages(void);
	virtual void __fastcall DoPaintBuffer(void);
	virtual void __fastcall DoPaintGDIOverlay(void);
	virtual void __fastcall DoScaleChange(void);
	virtual void __fastcall InitDefaultStages(void);
	void __fastcall InvalidateCache(void);
	virtual bool __fastcall InvalidRectsAvailable(void);
	DYNAMIC void __fastcall DblClick(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y)/* overload */;
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y)/* overload */;
	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y)/* overload */;
	HIDESBASE virtual void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer)/* overload */;
	HIDESBASE virtual void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer)/* overload */;
	HIDESBASE virtual void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer)/* overload */;
	virtual void __fastcall MouseLeave(void);
	virtual void __fastcall SetRepaintMode(const TRepaintMode Value);
	virtual void __fastcall SetScaleMode(TScaleMode Value);
	void __fastcall SetXForm(float ShiftX, float ShiftY, float ScaleX, float ScaleY);
	virtual void __fastcall UpdateCache(void);
	__property int UpdateCount = {read=FUpdateCount, nodefault};
	
public:
	__fastcall virtual TCustomImage32(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomImage32(void);
	virtual void __fastcall BeginUpdate(void);
	Types::TPoint __fastcall BitmapToControl(const Types::TPoint &APoint)/* overload */;
	Gr32::TFloatPoint __fastcall BitmapToControl(const Gr32::TFloatPoint &APoint)/* overload */;
	HIDESBASE virtual void __fastcall Changed(void);
	HIDESBASE virtual void __fastcall Update(const Types::TRect &Rect)/* overload */;
	Types::TPoint __fastcall ControlToBitmap(const Types::TPoint &APoint)/* overload */;
	Gr32::TFloatPoint __fastcall ControlToBitmap(const Gr32::TFloatPoint &APoint)/* overload */;
	virtual void __fastcall EndUpdate(void);
	virtual void __fastcall ExecBitmapFrame(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecClearBuffer(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecClearBackgnd(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecControlFrame(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecCustom(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecDrawBitmap(Gr32::TBitmap32* Dest, int StageNum);
	virtual void __fastcall ExecDrawLayers(Gr32::TBitmap32* Dest, int StageNum);
	virtual Types::TRect __fastcall GetBitmapRect(void);
	virtual tagSIZE __fastcall GetBitmapSize(void);
	virtual void __fastcall Invalidate(void);
	virtual void __fastcall Loaded(void);
	HIDESBASE virtual void __fastcall PaintTo(Gr32::TBitmap32* Dest, const Types::TRect &DestRect);
	DYNAMIC void __fastcall Resize(void);
	virtual void __fastcall SetupBitmap(bool DoClear = false, Gr32::TColor32 ClearColor = (Gr32::TColor32)(0xff000000));
	__property Gr32::TBitmap32* Bitmap = {read=FBitmap, write=SetBitmap};
	__property TBitmapAlign BitmapAlign = {read=FBitmapAlign, write=SetBitmapAlign, nodefault};
	__property Canvas;
	__property Gr32_layers::TLayerCollection* Layers = {read=FLayers, write=SetLayers};
	__property float OffsetHorz = {read=FOffsetHorz, write=SetOffsetHorz};
	__property float OffsetVert = {read=FOffsetVert, write=SetOffsetVert};
	__property TPaintStages* PaintStages = {read=FPaintStages};
	__property float Scale = {read=FScaleX, write=SetScale};
	__property float ScaleX = {read=FScaleX, write=SetScaleX};
	__property float ScaleY = {read=FScaleY, write=SetScaleY};
	__property TScaleMode ScaleMode = {read=FScaleMode, write=SetScaleMode, nodefault};
	__property Classes::TNotifyEvent OnBitmapResize = {read=FOnBitmapResize, write=FOnBitmapResize};
	__property Gr32::TPixelCombineEvent OnBitmapPixelCombine = {read=GetOnPixelCombine, write=SetOnPixelCombine};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property Classes::TNotifyEvent OnInitStages = {read=FOnInitStages, write=FOnInitStages};
	__property TImgMouseEvent OnMouseDown = {read=FOnMouseDown, write=FOnMouseDown};
	__property TImgMouseMoveEvent OnMouseMove = {read=FOnMouseMove, write=FOnMouseMove};
	__property TImgMouseEvent OnMouseUp = {read=FOnMouseUp, write=FOnMouseUp};
	__property TPaintStageEvent OnPaintStage = {read=FOnPaintStage, write=FOnPaintStage};
	__property Classes::TNotifyEvent OnScaleChange = {read=FOnScaleChange, write=FOnScaleChange};
public:
	/* TWinControl.CreateParented */ inline __fastcall TCustomImage32(HWND ParentWindow) : TCustomPaintBox32(ParentWindow) { }
	
};


class DELPHICLASS TImage32;
class PASCALIMPLEMENTATION TImage32 : public TCustomImage32
{
	typedef TCustomImage32 inherited;
	
__published:
	__property Align = {default=0};
	__property Anchors = {default=3};
	__property AutoSize = {default=0};
	__property Bitmap;
	__property BitmapAlign;
	__property Color = {default=-16777211};
	__property Constraints;
	__property Cursor = {default=0};
	__property DragCursor = {default=-12};
	__property DragMode = {default=0};
	__property ParentColor = {default=1};
	__property ParentShowHint = {default=1};
	__property PopupMenu;
	__property RepaintMode = {default=0};
	__property Scale;
	__property ScaleMode;
	__property ShowHint;
	__property TabOrder = {default=-1};
	__property TabStop = {default=0};
	__property Visible = {default=1};
	__property OnBitmapResize;
	__property OnCanResize;
	__property OnClick;
	__property OnChange;
	__property OnDblClick;
	__property OnGDIOverlay;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnEndDrag;
	__property OnInitStages;
	__property OnKeyDown;
	__property OnKeyPress;
	__property OnKeyUp;
	__property OnMouseDown;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnMouseWheel;
	__property OnMouseWheelDown;
	__property OnMouseWheelUp;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property OnPaintStage;
	__property OnResize;
	__property OnStartDrag;
public:
	/* TCustomImage32.Create */ inline __fastcall virtual TImage32(Classes::TComponent* AOwner) : TCustomImage32(AOwner) { }
	/* TCustomImage32.Destroy */ inline __fastcall virtual ~TImage32(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TImage32(HWND ParentWindow) : TCustomImage32(ParentWindow) { }
	
};


#pragma option push -b-
enum TScrollBarVisibility { svAlways, svHidden, svAuto };
#pragma option pop

class DELPHICLASS TIVScrollProperties;
class DELPHICLASS TCustomImgView32;
class PASCALIMPLEMENTATION TIVScrollProperties : public Gr32_rangebars::TArrowBarAccess
{
	typedef Gr32_rangebars::TArrowBarAccess inherited;
	
private:
	int __fastcall GetIncrement(void);
	int __fastcall GetSize(void);
	TScrollBarVisibility __fastcall GetVisibility(void);
	void __fastcall SetIncrement(int Value);
	void __fastcall SetSize(int Value);
	void __fastcall SetVisibility(const TScrollBarVisibility Value);
	
protected:
	TCustomImgView32* ImgView;
	
__published:
	__property int Increment = {read=GetIncrement, write=SetIncrement, default=8};
	__property int Size = {read=GetSize, write=SetSize, default=0};
	__property TScrollBarVisibility Visibility = {read=GetVisibility, write=SetVisibility, default=0};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TIVScrollProperties(void) { }
	
public:
	/* TObject.Create */ inline __fastcall TIVScrollProperties(void) : Gr32_rangebars::TArrowBarAccess() { }
	
};


#pragma option push -b-
enum TSizeGripStyle { sgAuto, sgNone, sgAlways };
#pragma option pop

class PASCALIMPLEMENTATION TCustomImgView32 : public TCustomImage32
{
	typedef TCustomImage32 inherited;
	
private:
	bool FCentered;
	int FScrollBarSize;
	TScrollBarVisibility FScrollBarVisibility;
	TIVScrollProperties* FScrollBars;
	TSizeGripStyle FSizeGrip;
	Classes::TNotifyEvent FOnScroll;
	int FOverSize;
	void __fastcall SetCentered(bool Value);
	void __fastcall SetScrollBars(TIVScrollProperties* Value);
	void __fastcall SetSizeGrip(TSizeGripStyle Value);
	void __fastcall SetOverSize(const int Value);
	
protected:
	bool DisableScrollUpdate;
	Gr32_rangebars::TCustomRangeBar* HScroll;
	Gr32_rangebars::TCustomRangeBar* VScroll;
	void __fastcall AlignAll(void);
	virtual void __fastcall BitmapResized(void);
	void __fastcall DoDrawSizeGrip(const Types::TRect &R);
	virtual void __fastcall DoScaleChange(void);
	virtual void __fastcall DoScroll(void);
	bool __fastcall GetScrollBarsVisible(void);
	int __fastcall GetScrollBarSize(void);
	Types::TRect __fastcall GetSizeGripRect(void);
	bool __fastcall IsSizeGripVisible(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y)/* overload */;
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y)/* overload */;
	virtual void __fastcall Paint(void);
	void __fastcall Recenter(void);
	virtual void __fastcall SetScaleMode(TScaleMode Value);
	virtual void __fastcall ScrollHandler(System::TObject* Sender);
	virtual void __fastcall UpdateImage(void);
	virtual void __fastcall UpdateScrollBars(void);
	
public:
	__fastcall virtual TCustomImgView32(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomImgView32(void);
	virtual Types::TRect __fastcall GetViewportRect(void);
	virtual void __fastcall Loaded(void);
	DYNAMIC void __fastcall Resize(void);
	void __fastcall ScrollToCenter(int X, int Y);
	void __fastcall Scroll(int Dx, int Dy);
	__property bool Centered = {read=FCentered, write=SetCentered, default=1};
	__property TIVScrollProperties* ScrollBars = {read=FScrollBars, write=SetScrollBars};
	__property TSizeGripStyle SizeGrip = {read=FSizeGrip, write=SetSizeGrip, default=0};
	__property int OverSize = {read=FOverSize, write=SetOverSize, nodefault};
	__property Classes::TNotifyEvent OnScroll = {read=FOnScroll, write=FOnScroll};
public:
	/* TWinControl.CreateParented */ inline __fastcall TCustomImgView32(HWND ParentWindow) : TCustomImage32(ParentWindow) { }
	
	
/* Hoisted overloads: */
	
protected:
	inline void __fastcall  MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer){ TCustomImage32::MouseDown(Button, Shift, X, Y, Layer); }
	inline void __fastcall  MouseMove(Classes::TShiftState Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer){ TCustomImage32::MouseMove(Shift, X, Y, Layer); }
	
};


class DELPHICLASS TImgView32;
class PASCALIMPLEMENTATION TImgView32 : public TCustomImgView32
{
	typedef TCustomImgView32 inherited;
	
__published:
	__property Align = {default=0};
	__property Anchors = {default=3};
	__property AutoSize = {default=0};
	__property Bitmap;
	__property BitmapAlign;
	__property Centered = {default=1};
	__property Color = {default=-16777211};
	__property Constraints;
	__property Cursor = {default=0};
	__property DragCursor = {default=-12};
	__property DragMode = {default=0};
	__property ParentColor = {default=1};
	__property ParentShowHint = {default=1};
	__property PopupMenu;
	__property RepaintMode = {default=0};
	__property Scale;
	__property ScaleMode;
	__property ScrollBars;
	__property ShowHint;
	__property SizeGrip = {default=0};
	__property OverSize;
	__property TabOrder = {default=-1};
	__property TabStop = {default=0};
	__property Visible = {default=1};
	__property OnBitmapResize;
	__property OnCanResize;
	__property OnClick;
	__property OnChange;
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnEndDrag;
	__property OnGDIOverlay;
	__property OnInitStages;
	__property OnKeyDown;
	__property OnKeyPress;
	__property OnKeyUp;
	__property OnMouseDown;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnMouseWheel;
	__property OnMouseWheelDown;
	__property OnMouseWheelUp;
	__property OnPaintStage;
	__property OnResize;
	__property OnScroll;
	__property OnStartDrag;
public:
	/* TCustomImgView32.Create */ inline __fastcall virtual TImgView32(Classes::TComponent* AOwner) : TCustomImgView32(AOwner) { }
	/* TCustomImgView32.Destroy */ inline __fastcall virtual ~TImgView32(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TImgView32(HWND ParentWindow) : TCustomImgView32(ParentWindow) { }
	
};


class DELPHICLASS TBitmap32Item;
class PASCALIMPLEMENTATION TBitmap32Item : public Classes::TCollectionItem
{
	typedef Classes::TCollectionItem inherited;
	
private:
	Gr32::TBitmap32* FBitmap;
	void __fastcall SetBitmap(Gr32::TBitmap32* ABitmap);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	
public:
	__fastcall virtual TBitmap32Item(Classes::TCollection* Collection);
	__fastcall virtual ~TBitmap32Item(void);
	
__published:
	__property Gr32::TBitmap32* Bitmap = {read=FBitmap, write=SetBitmap};
};


typedef TMetaClass* TBitmap32ItemClass;

class DELPHICLASS TBitmap32Collection;
class PASCALIMPLEMENTATION TBitmap32Collection : public Classes::TCollection
{
	typedef Classes::TCollection inherited;
	
public:
	TBitmap32Item* operator[](int Index) { return Items[Index]; }
	
private:
	Classes::TPersistent* FOwner;
	HIDESBASE TBitmap32Item* __fastcall GetItem(int Index);
	HIDESBASE void __fastcall SetItem(int Index, TBitmap32Item* Value);
	
protected:
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	
public:
	__fastcall TBitmap32Collection(Classes::TPersistent* AOwner, TBitmap32ItemClass ItemClass);
	HIDESBASE TBitmap32Item* __fastcall Add(void);
	__property TBitmap32Item* Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
public:
	/* TCollection.Destroy */ inline __fastcall virtual ~TBitmap32Collection(void) { }
	
};


class DELPHICLASS TBitmap32List;
class PASCALIMPLEMENTATION TBitmap32List : public Classes::TComponent
{
	typedef Classes::TComponent inherited;
	
public:
	Gr32::TBitmap32* operator[](int Index) { return Bitmap[Index]; }
	
private:
	TBitmap32Collection* FBitmap32Collection;
	void __fastcall SetBitmap(int Index, Gr32::TBitmap32* Value);
	Gr32::TBitmap32* __fastcall GetBitmap(int Index);
	void __fastcall SetBitmap32Collection(TBitmap32Collection* Value);
	
public:
	__fastcall virtual TBitmap32List(Classes::TComponent* AOwner);
	__fastcall virtual ~TBitmap32List(void);
	__property Gr32::TBitmap32* Bitmap[int Index] = {read=GetBitmap, write=SetBitmap/*, default*/};
	
__published:
	__property TBitmap32Collection* Bitmaps = {read=FBitmap32Collection, write=SetBitmap32Collection};
};


//-- var, const, procedure ---------------------------------------------------
static const ShortInt PST_CUSTOM = 0x1;
static const ShortInt PST_CLEAR_BUFFER = 0x2;
static const ShortInt PST_CLEAR_BACKGND = 0x3;
static const ShortInt PST_DRAW_BITMAP = 0x4;
static const ShortInt PST_DRAW_LAYERS = 0x5;
static const ShortInt PST_CONTROL_FRAME = 0x6;
static const ShortInt PST_BITMAP_FRAME = 0x7;

}	/* namespace Gr32_image */
using namespace Gr32_image;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_imageHPP
