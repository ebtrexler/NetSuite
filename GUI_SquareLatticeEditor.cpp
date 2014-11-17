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

#include <vcl.h>
#pragma hdrstop

#include "GUI_NetworkForm.h"
#include "GUI_SquareLatticeEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma resource "*.dfm"

#include "GR32_Polygons.hpp"

TGUI_SquareLatticeForm *GUI_SquareLatticeForm;

TGUI_SquareLatticeForm *GetSquareLatticeForm()
{
   static TGUI_SquareLatticeForm *form = NULL;
   if (!form) {
      form = new TGUI_SquareLatticeForm(NULL);
   }
   return form;
}
//---------------------------------------------------------------------------
__fastcall TGUI_SquareLatticeForm::TGUI_SquareLatticeForm(TComponent* Owner)
   : TForm(Owner)
{
   NetworkHeight = 600;
   NetworkWidth = 600;
   ImageSize = 80;
   Margin = ImageSize / 4;
//   StretchFilter = sfNearest;
   LastX = LastY = 0;
   CellNumber = SynapseNumber = ElectrodeNumber = 0;


   NetworkImage = new TBitmap32;
   CellBmp = new TBitmap32;
   SynapseBmpHORZ = new TBitmap32;
   SynapseBmpVERT = new TBitmap32;
   SynapseBmpTLBR = new TBitmap32;
   SynapseBmpBLTR = new TBitmap32;
   ElectrodeBmp = new TBitmap32;
   CellHitTestBmp = new TBitmap32;
   SynapseHitTestBmp = new TBitmap32;
   ElectrodeHitTestBmp = new TBitmap32;

   NetworkImage->SetSize(NetworkWidth, NetworkHeight);
//   NetworkImage->StretchFilter = StretchFilter;
   NetworkImage->DrawMode = dmOpaque;
   NetworkImage->Clear(clWhite32);

   NetworkPaintBox32->SetBounds(0,0,
      std::min(NetworkImage->Width, Width),
      std::min(NetworkImage->Height, Height));
   NetworkPaintBox32->Buffer->SetSize(
      std::min(NetworkImage->Width, Width),
      std::min(NetworkImage->Height, Height));

   TBitmap32 *temp = new TBitmap32;

   // load cell image
   temp->LoadFromResourceName((int)HInstance, L"BMP_CELL");
   CellBmp->DrawMode = dmTransparent;
   CellBmp->OuterColor = temp->Pixel[1][1];
   CellBmp->SetSize(ImageSize, ImageSize);
   CellBmp->Clear(clWhite32);
   CellBmp->Draw(CellBmp->BoundsRect(),temp->BoundsRect(),temp);

   // load 4 directional synapse images
   temp->LoadFromResourceName((int)HInstance, L"BMP_synapseHORZ");
   SynapseBmpHORZ->DrawMode = dmTransparent;
   SynapseBmpHORZ->OuterColor = temp->Pixel[1][1];
   SynapseBmpHORZ->SetSize(ImageSize, ImageSize);
   SynapseBmpHORZ->Clear(clWhite32);
   SynapseBmpHORZ->Draw(SynapseBmpHORZ->BoundsRect(),temp->BoundsRect(),temp);

   temp->LoadFromResourceName((int)HInstance, L"BMP_synapseVERT");
   SynapseBmpVERT->DrawMode = dmTransparent;
   SynapseBmpVERT->OuterColor = temp->Pixel[1][1];
   SynapseBmpVERT->SetSize(ImageSize, ImageSize);
   SynapseBmpVERT->Clear(clWhite32);
   SynapseBmpVERT->Draw(SynapseBmpVERT->BoundsRect(),temp->BoundsRect(),temp);

   temp->LoadFromResourceName((int)HInstance, L"BMP_synapseTLBR");
   SynapseBmpTLBR->DrawMode = dmTransparent;
   SynapseBmpTLBR->OuterColor = temp->Pixel[1][1];
   SynapseBmpTLBR->SetSize(ImageSize, ImageSize);
   SynapseBmpTLBR->Clear(clWhite32);
   SynapseBmpTLBR->Draw(SynapseBmpTLBR->BoundsRect(),temp->BoundsRect(),temp);

   temp->LoadFromResourceName((int)HInstance, L"BMP_synapseBLTR");
   SynapseBmpBLTR->DrawMode = dmTransparent;
   SynapseBmpBLTR->OuterColor = temp->Pixel[1][1];
   SynapseBmpBLTR->SetSize(ImageSize, ImageSize);
   SynapseBmpBLTR->Clear(clWhite32);
   SynapseBmpBLTR->Draw(SynapseBmpBLTR->BoundsRect(),temp->BoundsRect(),temp);


   // load electrode image
   temp->LoadFromResourceName((int)HInstance, L"BMP_ELECTRODE");
   ElectrodeBmp->DrawMode = dmTransparent;
   ElectrodeBmp->OuterColor = temp->Pixel[1][1];
   ElectrodeBmp->SetSize(ImageSize, ImageSize);
   ElectrodeBmp->Clear(clWhite32);
   ElectrodeBmp->Draw(ElectrodeBmp->BoundsRect(),temp->BoundsRect(),temp);

   // make cell hittest image
   CellHitTestBmp->DrawMode = dmTransparent;
   CellHitTestBmp->OuterColor = clWhite32;
   CellHitTestBmp->SetSize(ImageSize, ImageSize);
   CellHitTestBmp->Clear(clWhite32);
   TArrayOfFixedPoint pts;
   Circle(pts, ImageSize/2, ImageSize/2);
   PolygonXS(CellHitTestBmp, pts, clTrRed32);

   // make synapse hittest image
   temp->LoadFromResourceName((int)HInstance, L"BMP_CHOOSESYNAPSE");
   SynapseHitTestBmp->DrawMode = dmTransparent;
   SynapseHitTestBmp->OuterColor = temp->Pixel[1][1];;
   SynapseHitTestBmp->SetSize(ImageSize, ImageSize);
   SynapseHitTestBmp->Clear(clWhite32);
   SynapseHitTestBmp->Draw(SynapseHitTestBmp->BoundsRect(),temp->BoundsRect(),temp);

   // make electrode hittest image
   ElectrodeHitTestBmp->DrawMode = dmTransparent;
   ElectrodeHitTestBmp->OuterColor = clWhite32;
   ElectrodeHitTestBmp->SetSize(ImageSize, ImageSize);
   ElectrodeHitTestBmp->Clear(clWhite32);
   pts.Length = 3;
   pts[0] = FixedPoint(0,ImageSize);
   pts[1] = FixedPoint(ImageSize, ImageSize/2);
   pts[2] = FixedPoint(ImageSize/2, 0);
   PolygonXS(ElectrodeHitTestBmp, pts, clTrBlue32);

   UpdateDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TGUI_SquareLatticeForm::UpdateDisplay()
{
   NetworkImage->Clear(clWhite32);

   int X, X1, Y, Y1;
   NetworkImage->Font->Height = 14;
   NetworkImage->Font->Style = TFontStyles() << fsBold;

   // draw cells
   TCellsMapConstIterator c_iter;
   TCellsMap const &cells = GetNet()->GetCells();
   for (c_iter = cells.begin(); c_iter != cells.end(); ++c_iter) {
      X = ((TRTBase *)c_iter->second.get())->GetX();
      Y = ((TRTBase *)c_iter->second.get())->GetY();
      UnicodeString name(((TRTBase *)c_iter->second.get())->Name().c_str());
      NetworkImage->Draw(X  - ImageSize/2, Y - ImageSize/2, CellBmp);

      int th = NetworkImage->TextHeight(name)/2; // text height / 2 for centering
      int te = NetworkImage->TextWidth(name)/2;  // text width / 2 for centering
      NetworkImage->Textout(X - te, Y - th, name);
   }

   // draw synapses
   TSynapsesMapConstIterator s_iter;
   TSynapsesMap const &synapses = GetNet()->GetSynapses();
   for (s_iter = synapses.begin(); s_iter != synapses.end(); ++s_iter) {
      X = ((TSynapse *)s_iter->second.get())->Pre()->GetX();
      Y = ((TSynapse *)s_iter->second.get())->Pre()->GetY();
      X1 = ((TSynapse *)s_iter->second.get())->Post()->GetX();
      Y1 = ((TSynapse *)s_iter->second.get())->Post()->GetY();
      UnicodeString name(((TRTBase *)s_iter->second.get())->Name().c_str());

      int xdiff = (X - X1);
      int ydiff = (Y - Y1);
      if (xdiff == 0) {
         ChooseBmp = SynapseBmpVERT;
      } else if (ydiff == 0){
         ChooseBmp = SynapseBmpHORZ;
      } else if ((xdiff < 0) == (ydiff < 0)){ // same sign
         ChooseBmp = SynapseBmpTLBR;
      } else if ((xdiff < 0) != (ydiff < 0)) { // opposite sign
         ChooseBmp = SynapseBmpBLTR;
      }
      int midX = X+(X1-X)/2;
      int midY = Y+(Y1-Y)/2;
      NetworkImage->Draw(midX - ImageSize/2, midY -ImageSize/2, ChooseBmp);

      int th = NetworkImage->TextHeight(name)/2; // text height / 2 for centering
      int te = NetworkImage->TextWidth(name)/2;  // text width / 2 for centering
      NetworkImage->Textout(midX - te, midY - th, name);
   }

   // draw electrodes
   TElectrodesMapConstIterator e_iter;
   TElectrodesMap const &electrodes = GetNet()->GetElectrodes();
   for (e_iter = electrodes.begin(); e_iter != electrodes.end(); ++e_iter) {
      X = ((TRTBase *)e_iter->second.get())->GetX();
      Y = ((TRTBase *)e_iter->second.get())->GetY();
      UnicodeString name(((TRTBase *)e_iter->second.get())->Name().c_str());
      NetworkImage->Draw(X, Y - ImageSize, ElectrodeBmp);

      int th = NetworkImage->TextHeight(name); // text height
      int te = NetworkImage->TextWidth(name);  // text width
      NetworkImage->Textout(X + 3*ImageSize/4- te, Y - 7*ImageSize/8 + th, name);
   }

   NetworkPaintBox32->Buffer->Draw(0,0, NetworkImage);
   NetworkPaintBox32->Invalidate();
}
//---------------------------------------------------------------------------

std::wstring __fastcall TGUI_SquareLatticeForm::CellGridHitTestAndCoerce(int &X, int &Y)
{
   int spacing = ImageSize + Margin;
   int fcX = ImageSize/2 + Margin;  int fcY = fcX;  //position of first cell.

   double spotX = X;
   spotX -= (double)fcX;
   spotX /= (double)spacing;
   int gridX = floor(spotX);
   spotX -= (double) gridX; // now spotX is between 0 and 1;
   if (spotX < 0.5) {
      X = gridX * spacing + fcX;
   }
   else {
      X = (gridX + 1) * spacing + fcX;
   }


   double spotY = Y;
   spotY -= (double)fcY;
   spotY /= (double)spacing;
   int gridY = floor(spotY);
   spotY -= (double) gridY; // now spotY is between 0 and 1;
   if (spotY < 0.5) {
      Y = gridY * spacing + fcY;
   }
   else {
      Y = (gridY + 1) * spacing + fcY;
   }

   // search cells to see if one exists at (X,Y) already

   return GetNet()->CellGridHitTest(X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TGUI_SquareLatticeForm::PlaceObject(TBitmap32* bmp, int X, int Y)
{
   if (NetworkPaintBox32->Width < bmp->Width) return;
   if (NetworkPaintBox32->Height < bmp->Height) return;
   int halfwidth = bmp->Width/2;
   int halfheight = bmp->Height/2;
   int lastleft = std::max(LastX-halfwidth, 0);
   int lasttop = std::max(LastY-halfheight, 0);
   int lastright = std::min(LastX + halfwidth, NetworkPaintBox32->Width);
   int lastbottom = std::min(LastY + halfheight, NetworkPaintBox32->Height);
   NetworkPaintBox32->Buffer->Draw(
      Rect(lastleft, lasttop, lastright, lastbottom),
      Rect(lastleft, lasttop, lastright, lastbottom),
      NetworkImage);

   int left = std::max(X-halfwidth, 0);
   int top = std::max(Y-halfheight, 0);
   int right = std::min(X + halfwidth,NetworkPaintBox32->Width);
   int bottom = std::min(Y + halfheight, NetworkPaintBox32->Height);
   Types::TRect dst = Rect(left, top, right, bottom);
   int pickleft = X >= halfwidth ? 0 : halfwidth - X;
   int picktop = Y >= halfheight ? 0 : halfheight - Y;
   int pickright = (X + halfwidth) > NetworkPaintBox32->Width ?
      bmp->Width - ((X + halfheight) - NetworkPaintBox32->Width) : bmp->Width;
   int pickbottom = (Y + halfheight) > NetworkPaintBox32->Height ?
      bmp->Height - ((Y + halfheight) - NetworkPaintBox32->Height) : bmp->Height;
   Types::TRect src = Rect(pickleft, picktop, pickright, pickbottom);
   NetworkPaintBox32->Buffer->Draw(dst, src, bmp);
   NetworkPaintBox32->Flush(Rect(std::min(lastleft,left),
                              std::min(lasttop,top),
                              std::max(lastright,right),
                              std::max(lastbottom,bottom)));
}

void __fastcall TGUI_SquareLatticeForm::EditObject(TBitmap32* bmp, int X, int Y)
{
//
}

void __fastcall TGUI_SquareLatticeForm::NetworkPaintBox32MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
   if ((!NetworkPaintBox32->Buffer->Empty()) && (NetworkGUI->ModelMouseMode < NoMouseObject)){
      CellGridHitTestAndCoerce(X, Y);
      switch (NetworkGUI->ModelMouseMode)
      {
         case PositionCell: {
            PlaceObject(CellHitTestBmp, X, Y);
         } ;  break;
         case PositionElectrode: {
            PlaceObject(ElectrodeHitTestBmp, X, Y);
         } ; break;
         case PositionSynapse1:
         case PositionSynapse2: {
            PlaceObject(SynapseHitTestBmp, X, Y);
         } ; break;
         case EditAnObject: {
//            EditObject(ChooseBmp, X, Y);
         }; break;
         default: break;
      };

      LastX = X;
      LastY = Y;
   }
}
//---------------------------------------------------------------------------


void __fastcall TGUI_SquareLatticeForm::NetworkPaintBox32MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
   if ((!NetworkPaintBox32->Buffer->Empty()) && (NetworkGUI->ModelMouseMode < NoMouseObject)){

      std::wstring cellname = CellGridHitTestAndCoerce(X, Y);
      bool over_existing_cell = (cellname != L"");

      switch (NetworkGUI->ModelMouseMode)
      {
         case PositionCell: {
            if (!over_existing_cell){
               // get cell type and name from dialog -- create cell in net and add to treeview
               if (NetworkGUI->CreateCell(X, Y))
                  NetworkImage->Draw(X-ImageSize/2, Y-ImageSize/2, CellBmp);
            }
         } ;  break;
         case PositionElectrode: {
            if (over_existing_cell) {
               if (NetworkGUI->CreateElectrode(cellname, X, Y)) {
                  NetworkImage->Draw(X, Y - ImageSize, ElectrodeBmp);
               }
            }
         }; break;
         case PositionSynapse1: {
            if (over_existing_cell) {
               FirstCellName = cellname;
               X1 = X;
               Y1 = Y;
               NetworkPaintBox32->Buffer->Draw(X-ImageSize/2, Y-ImageSize/2, SynapseHitTestBmp);
               NetworkGUI->ModelMouseMode = PositionSynapse2;
               return;
            }
         }; break;
         case PositionSynapse2: {
            if (over_existing_cell) {
               if (cellname != FirstCellName){
                  SecondCellName = cellname;
                  int xdiff = (X - X1);
                  int ydiff = (Y - Y1);
                  int properspacing = ImageSize + Margin;
                  if ((abs(xdiff)> properspacing) || (abs(ydiff) > properspacing)) {
                     Application->MessageBoxA(L"Cells must be adjacent", L"Cannot complete synapse");
                     return;
                  }
                  if (xdiff == 0) {
                     ChooseBmp = SynapseBmpVERT;
                  } else if (ydiff == 0){
                     ChooseBmp = SynapseBmpHORZ;
                  } else if ((xdiff < 0) == (ydiff < 0)){ // same sign
                     ChooseBmp = SynapseBmpTLBR;
                  } else if ((xdiff < 0) != (ydiff < 0)) { // opposite sign
                     ChooseBmp = SynapseBmpBLTR;
                  }
                  int midX = X+(X1-X)/2;
                  int midY = Y+(Y1-Y)/2;
                  if (NetworkGUI->CreateSynapse(FirstCellName, SecondCellName, midX, midY)){
                     NetworkImage->Draw(midX - ImageSize/2, midY -ImageSize/2, ChooseBmp);
                  }
               }
            }
         }; break;
         case EditAnObject: {
            EditObject(ChooseBmp, X, Y);
         }; break;
         default: break;
      };

      NetworkGUI->ModelMouseMode = NoMouseObject;
      UpdateDisplay();
   }
}


