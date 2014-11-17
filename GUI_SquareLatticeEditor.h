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

#ifndef GUI_SquareLatticeEditorH
#define GUI_SquareLatticeEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"

#include "GUI_NetworkForm.h"
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TGUI_SquareLatticeForm   ***************************
//
//
/**
   @brief GUI class to display the network as cells arranged evenly in a grid.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TGUI_SquareLatticeForm : public TForm
{
__published:	// IDE-managed Components
   TScrollBox *ScrollBox1;
   TPaintBox32 *NetworkPaintBox32;
   void __fastcall NetworkPaintBox32MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall NetworkPaintBox32MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations

   TBitmap32                     *NetworkImage;
   TBitmap32                     *ChooseBmp;
   TBitmap32                     *CellHitTestBmp;
   TBitmap32                     *SynapseHitTestBmp;
   TBitmap32                     *ElectrodeHitTestBmp;
   TBitmap32                     *CellBmp;
   TBitmap32                     *SynapseBmpHORZ,
                                 *SynapseBmpVERT,
                                 *SynapseBmpTLBR,
                                 *SynapseBmpBLTR;
   TBitmap32                     *ElectrodeBmp;
//   ModelMouseModeEnum            ModelMouseMode;
   int                           Margin, ImageSize, NetworkHeight, NetworkWidth;
   int                           NumAreas;
   int                           LastX, LastY;
   TStretchFilter			         StretchFilter;
   void __fastcall PlaceObject(TBitmap32* bmp, int X, int Y);
   void __fastcall EditObject(TBitmap32* bmp, int X, int Y);
   std::wstring __fastcall CellGridHitTestAndCoerce(int &X, int &Y); // X and Y are snapped to grid
   int CellNumber, SynapseNumber, ElectrodeNumber;
   std::wstring FirstCellName, SecondCellName;
   int X1, Y1;


public:		// User declarations
   void __fastcall UpdateDisplay();
   __fastcall TGUI_SquareLatticeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGUI_SquareLatticeForm *GUI_SquareLatticeForm;

TGUI_SquareLatticeForm *GetSquareLatticeForm();
//---------------------------------------------------------------------------
#endif
