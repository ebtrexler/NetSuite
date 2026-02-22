// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gr32_xpthemes.pas' rev: 21.00

#ifndef Gr32_xpthemesHPP
#define Gr32_xpthemesHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_xpthemes
{
//-- type declarations -------------------------------------------------------
typedef unsigned HIMAGELIST;

typedef unsigned HTHEME;

struct _MARGINS
{
	
public:
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
};


typedef _MARGINS MARGINS;

typedef _MARGINS *PMARGINS;

typedef _MARGINS TMargins;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool USE_THEMES;
extern PACKAGE unsigned SCROLLBAR_THEME;
extern PACKAGE unsigned GLOBALS_THEME;
static const ShortInt THEMEMGR_VERSION = 0x1;
static const Word WM_THEMECHANGED = 0x31a;
static const ShortInt SBP_ARROWBTN = 0x1;
static const ShortInt SBP_THUMBBTNHORZ = 0x2;
static const ShortInt SBP_THUMBBTNVERT = 0x3;
static const ShortInt SBP_LOWERTRACKHORZ = 0x4;
static const ShortInt SBP_UPPERTRACKHORZ = 0x5;
static const ShortInt SBP_LOWERTRACKVERT = 0x6;
static const ShortInt SBP_UPPERTRACKVERT = 0x7;
static const ShortInt SBP_GRIPPERHORZ = 0x8;
static const ShortInt SBP_GRIPPERVERT = 0x9;
static const ShortInt SBP_SIZEBOX = 0xa;
static const ShortInt ABS_UPNORMAL = 0x1;
static const ShortInt ABS_UPHOT = 0x2;
static const ShortInt ABS_UPPRESSED = 0x3;
static const ShortInt ABS_UPDISABLED = 0x4;
static const ShortInt ABS_DOWNNORMAL = 0x5;
static const ShortInt ABS_DOWNHOT = 0x6;
static const ShortInt ABS_DOWNPRESSED = 0x7;
static const ShortInt ABS_DOWNDISABLED = 0x8;
static const ShortInt ABS_LEFTNORMAL = 0x9;
static const ShortInt ABS_LEFTHOT = 0xa;
static const ShortInt ABS_LEFTPRESSED = 0xb;
static const ShortInt ABS_LEFTDISABLED = 0xc;
static const ShortInt ABS_RIGHTNORMAL = 0xd;
static const ShortInt ABS_RIGHTHOT = 0xe;
static const ShortInt ABS_RIGHTPRESSED = 0xf;
static const ShortInt ABS_RIGHTDISABLED = 0x10;
static const ShortInt SCRBS_NORMAL = 0x1;
static const ShortInt SCRBS_HOT = 0x2;
static const ShortInt SCRBS_PRESSED = 0x3;
static const ShortInt SCRBS_DISABLED = 0x4;
static const ShortInt SZB_RIGHTALIGN = 0x1;
static const ShortInt SZB_LEFTALIGN = 0x2;
extern PACKAGE unsigned __stdcall (*OpenThemeData)(HWND hwnd, System::WideChar * pszClassList);
extern PACKAGE HRESULT __stdcall (*CloseThemeData)(unsigned hTheme);
extern PACKAGE HRESULT __stdcall (*DrawThemeBackground)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, const Types::TRect &Rect, Types::PRect pClipRect);
extern PACKAGE HRESULT __stdcall (*DrawThemeEdge)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, const Types::TRect &pDestRect, unsigned uEdge, unsigned uFlags, Types::PRect pContentRect);
extern PACKAGE HRESULT __stdcall (*GetThemeColor)(unsigned hTheme, int iPartId, int iStateId, int iPropId, unsigned &pColor);
extern PACKAGE HRESULT __stdcall (*GetThemeMetric)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int &piVal);
extern PACKAGE HRESULT __stdcall (*GetThemeMargins)(unsigned hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, Types::PRect prc, _MARGINS &pMargins);
extern PACKAGE HRESULT __stdcall (*SetWindowTheme)(HWND hwnd, System::WideChar * pszSubAppName, System::WideChar * pszSubIdList);
extern PACKAGE BOOL __stdcall (*IsThemeActive)(void);
extern PACKAGE BOOL __stdcall (*IsAppThemed)(void);
extern PACKAGE HRESULT __stdcall (*EnableTheming)(BOOL fEnable);

}	/* namespace Gr32_xpthemes */
using namespace Gr32_xpthemes;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gr32_xpthemesHPP
