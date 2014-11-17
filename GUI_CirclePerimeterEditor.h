/* /////////////////////////////////////////////////////////////////////////////
NetSuite: NetClamp/NetSim/NetFit Neural Network Modeling and Dynamic Clamp Software
	 Copyright (C) 2011-2014  E. Brady Trexler, Ph.D.

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please direct correspondence to ebtrexler _at_ gothamsci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#ifndef GUI_CirclePerimeterEditorH
#define GUI_CirclePerimeterEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TCellPosition   ***************************
//
//
/**
   @brief Class to facilitate drawing of networks as cells arranged evenly around a circle

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
typedef struct
{
   double X, Y;
   double angle;
} TCellPosition;
typedef std::map< std::wstring, TCellPosition > TCellPositionMap;
typedef TCellPositionMap::iterator TCellPositionMapIterator;


////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGUI_CirclePerimeterForm   ***************************
//
//
/**
   @brief GUI class to display the network as cells arranged evenly around a circle.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TGUI_CirclePerimeterForm : public TForm
{
__published:	// IDE-managed Components
   TPaintBox32 *NetworkPaintBox32;
   void __fastcall NetworkPaintBox32Resize(TObject *Sender);
   void __fastcall NetworkPaintBox32MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall NetworkPaintBox32MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
   TBitmap32                     *NetworkImage;
   std::wstring __fastcall       CellGridHitTestAndCoerce(int &X, int &Y);
   void __fastcall               PlaceObject(TBitmap32* bmp, int X, int Y);
   TBitmap32                     *CellHitTestBmp;
   TBitmap32                     *SynapseHitTestBmp;
   TBitmap32                     *ElectrodeHitTestBmp;
   TBitmap32                     *temp;
   int                           LastX, LastY, ImageSize;
   double                        CellRadius;
   TCellPositionMap              CellPosMap;
   std::wstring FirstCellName, SecondCellName;
   int X1, Y1;

public:		// User declarations
   void __fastcall UpdateDisplay();
   __fastcall TGUI_CirclePerimeterForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGUI_CirclePerimeterForm *GUI_CirclePerimeterForm;

TGUI_CirclePerimeterForm *GetCirclePerimeterForm();
//---------------------------------------------------------------------------
#endif
