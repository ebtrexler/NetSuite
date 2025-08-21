//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "GenericUtilities.h"
#pragma package(smart_init)

//#pragma link "sine"
//#pragma link "comp2str"
//#pragma link "tools"
//#pragma link "serial"

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TGenericUtilities *)
{
	new TGenericUtilities(NULL);
}
//---------------------------------------------------------------------------
__fastcall TGenericUtilities::TGenericUtilities(TComponent* Owner)
	: TComponent(Owner)
{
}
//---------------------------------------------------------------------------
namespace Genericutilities
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TGenericUtilities)};
		RegisterComponents("Gotham Scientific", classes, 0);
	}
}

//---------------------------------------------------------------------------
