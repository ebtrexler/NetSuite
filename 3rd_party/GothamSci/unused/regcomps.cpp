//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "PLOTPanel.h"
#include "MultiPLOTPanel.h"
#include "WaveformDefinition.h"
#include "CGMComponent.h"
#include "InputBox.h"
#include "InputEdit.h"
#include "GenericUtilities.h"
#include "AudioIO.hpp"
#include "Knob.hpp"
#include "Parser10.hpp"


//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
namespace GothamSci
{
	void __fastcall PACKAGE Register()
	{
		#define classessize 12
		#define classindex (classessize - 1)

		TComponentClass classes[classessize] = {
									__classid(TPLOTPanel),
									__classid(TMultiPLOTPanel),
									__classid(TWaveformDefinition),
									__classid(TCGMComponent),
									__classid(TInputBox),
									__classid(TInputEdit),
									__classid(TGenericUtilities),
									__classid(TAudioOut),
									__classid(TAudioIn),
									__classid(TKnob),
									__classid(TFParser),
									__classid(TFParserASM)
									};
		RegisterComponents("Gotham Scientific", classes, classindex);
	}
}
//---------------------------------------------------------------------------