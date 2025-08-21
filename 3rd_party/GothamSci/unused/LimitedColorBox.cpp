//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "LimitedColorBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TLimitedColorBox *)
{
	new TLimitedColorBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TLimitedColorBox::TLimitedColorBox(TComponent* Owner)
	: TCustomColorBox(Owner)
{
}
//---------------------------------------------------------------------------
namespace Limitedcolorbox
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TLimitedColorBox)};
		RegisterComponents("Components", classes, 0);
	}
}
//---------------------------------------------------------------------------

