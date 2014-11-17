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
#include "GUI_CirclePerimeterEditor.h"
#include "GR32_Polygons.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma resource "*.dfm"
TGUI_CirclePerimeterForm *GUI_CirclePerimeterForm;

TGUI_CirclePerimeterForm *GetCirclePerimeterForm()
{
   static TGUI_CirclePerimeterForm *form = NULL;
   if (!form) {
      form = new TGUI_CirclePerimeterForm(NULL);
   }
   return form;
}

//---------------------------------------------------------------------------
__fastcall TGUI_CirclePerimeterForm::TGUI_CirclePerimeterForm(TComponent* Owner)
   : TForm(Owner)
{
   NetworkImage = new TBitmap32;
   CellHitTestBmp = new TBitmap32;
   SynapseHitTestBmp = new TBitmap32;
   ElectrodeHitTestBmp = new TBitmap32;
   temp = new TBitmap32;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_CirclePerimeterForm::UpdateDisplay()
{
   const double pi = 3.1415926535897932384626433832795;

   double diameter = Height > Width ? Width : Height; // make circle of smallest dimension
   double cellsdiam;
   NetworkImage->SetSize(diameter, diameter);
   NetworkImage->DrawMode = dmOpaque;
   NetworkImage->Clear(clWhite32);
   NetworkImage->Font->Height = 14;
   NetworkImage->Font->Style = TFontStyles() << fsBold;

   NetworkPaintBox32->SetBounds(0,0,
      std::min(NetworkImage->Width, Width),
      std::min(NetworkImage->Height, Height));
   NetworkPaintBox32->Buffer->SetSize(
      std::min(NetworkImage->Width, Width),
      std::min(NetworkImage->Height, Height));
   NetworkPaintBox32->Buffer->Clear(clWhite32);

   int cellcount = GetNet()->GetCells().size();       // how many cells?
   if (cellcount != 0) {

      double radius = diameter / 2.0;
      double origin = radius;
      cellsdiam = diameter / cellcount *0.7;
      ImageSize = cellsdiam;
      CellRadius = 0.5 * cellsdiam;
      int cellindex = 0;
      TArrayOfFixedPoint pts;

      std::wstring cellname;

      CellPosMap.clear();

      TCellPosition prePos, postPos;


      // draw cells
      // determine spacing around circle and store center points for later use
      TCellsMapConstIterator c_iter;
      TCellsMap const &cells = GetNet()->GetCells();
      for (c_iter = cells.begin(); c_iter != cells.end(); ++c_iter) {
         cellname = ((TRTBase *)c_iter->second.get())->Name();
         CellPosMap[cellname].angle = cellindex * 2 * pi / cellcount;
         CellPosMap[cellname].X = radius - (radius - CellRadius) * (cos(CellPosMap[cellname].angle) );
         CellPosMap[cellname].Y = radius - (radius - CellRadius) * (sin(CellPosMap[cellname].angle) );
         UnicodeString name(cellname.c_str());
         Circle(pts, CellPosMap[cellname].X, CellPosMap[cellname].Y, CellRadius);
         PolygonXS(NetworkImage, pts, clTrRed32);
         int th = NetworkImage->TextHeight(name)/2; // text height / 2 for centering
         int te = NetworkImage->TextWidth(name)/2;  // text width / 2 for centering
         NetworkImage->Textout(CellPosMap[cellname].X - te, CellPosMap[cellname].Y - th, name);
         cellindex++;
      }

      // draw synapses
      pts.set_length(2); // line
      TSynapsesMapConstIterator s_iter;
      TSynapsesMap const &synapses = GetNet()->GetSynapses();
      for (s_iter = synapses.begin(); s_iter != synapses.end(); ++s_iter) {
         prePos =  CellPosMap[((TSynapse *)s_iter->second.get())->Pre()->Name()];
         postPos = CellPosMap[((TSynapse *)s_iter->second.get())->Post()->Name()];
         UnicodeString name(((TRTBase *)s_iter->second.get())->Name().c_str());
         pts[0].X = round(prePos.X * 65536);
         pts[0].Y = round(prePos.Y * 65536);
         pts[1].X = round(postPos.X * 65536);
         pts[1].Y = round(postPos.Y * 65536);
         PolylineXS(NetworkImage, pts, clTrBlue32);
         int midX = prePos.X+(postPos.X-prePos.X)/2;
         int midY = prePos.Y+(postPos.Y-prePos.Y)/2;
         int th = NetworkImage->TextHeight(name)/2; // text height / 2 for centering
         int te = NetworkImage->TextWidth(name)/2;  // text width / 2 for centering
         NetworkImage->Textout(midX - te, midY - th, name);
      }

      // draw electrodes
      pts.set_length(3); //triangle
      TElectrodesMapConstIterator e_iter;
      TElectrodesMap const &electrodes = GetNet()->GetElectrodes();
      double electrodelength = CellRadius * 1.5;
      for (e_iter = electrodes.begin(); e_iter != electrodes.end(); ++e_iter) {
         cellname = e_iter->second.get()->Owner()->Name();
         pts[0].X = round(CellPosMap[cellname].X * 65536);
         pts[0].Y = round(CellPosMap[cellname].Y * 65536);
         double temp;
         double ang1 = CellPosMap[cellname].angle-0.4/cellcount;
         double ang2 = CellPosMap[cellname].angle+0.4/cellcount;
         temp = (radius - (radius - CellRadius - electrodelength) * cos(ang1));
         pts[1].X = round(temp*65536);
         temp = (radius - (radius - CellRadius - electrodelength) * sin(ang1));
         pts[1].Y = round(temp*65536);
         temp = (radius - (radius - CellRadius - electrodelength) * cos(ang2));
         pts[2].X = round(temp*65536);
         temp = (radius - (radius - CellRadius - electrodelength) * sin(ang2));
         pts[2].Y = round(temp*65536);

//         pts[1].X = round((electrodelength + (radius - electrodelength) * ( 1.0 - cos(CellPosMap[cellname].angle-0.1)))*65536);
//         pts[1].Y = round((electrodelength + (radius - electrodelength) * ( 1.0 - sin(CellPosMap[cellname].angle-0.1)))*65536);
//         pts[2].X = round((electrodelength + (radius - electrodelength) * ( 1.0 - cos(CellPosMap[cellname].angle+0.1)))*65536);
//         pts[2].Y = round((electrodelength + (radius - electrodelength) * ( 1.0 - sin(CellPosMap[cellname].angle+0.1)))*65536);
         UnicodeString name(((TRTBase *)e_iter->second.get())->Name().c_str());
         PolygonXS(NetworkImage, pts, clTrGreen32);

         int th = NetworkImage->TextHeight(name); // text height
         int te = NetworkImage->TextWidth(name);  // text width
         Types::TPoint p = Point(pts[1]); // convert to regular point
         NetworkImage->Textout(p.x - te, p.y - th, name);
      }

   }
   else {
      ImageSize = diameter;
      CellPosMap[L"None"].angle = 0;
      CellPosMap[L"None"].X = diameter / 2;
      CellPosMap[L"None"].Y = diameter / 2;
   }

   // setup chooser bmps
   // make cell hittest image
   TArrayOfFixedPoint pts;

   CellHitTestBmp->DrawMode = dmTransparent;
   CellHitTestBmp->OuterColor = clWhite32;
   CellHitTestBmp->SetSize(ImageSize, ImageSize);
   CellHitTestBmp->Clear(clWhite32);
   Circle(pts, ImageSize/2, ImageSize/2);
   PolygonXS(CellHitTestBmp, pts, 0x7f00007f);

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
   PolygonXS(ElectrodeHitTestBmp, pts, 0x7f00007f);

   NetworkPaintBox32->Buffer->Draw(0,0, NetworkImage);
   NetworkPaintBox32->Invalidate();

}
//---------------------------------------------------------------------------


void __fastcall TGUI_CirclePerimeterForm::NetworkPaintBox32Resize(TObject *Sender)

{
   UpdateDisplay();
}
//---------------------------------------------------------------------------

std::wstring __fastcall TGUI_CirclePerimeterForm::CellGridHitTestAndCoerce(int &X, int &Y)
{
   TCellPositionMapIterator c_iter;
   TCellPosition pos;
   std::wstring cellname = L"";
   for (c_iter = CellPosMap.begin(); c_iter != CellPosMap.end(); ++c_iter) {
      pos = c_iter->second;
      int xdis = abs(X - round(pos.X));
      int ydis = abs(Y - round(pos.Y));
      if (xdis < CellRadius && ydis < CellRadius){
         cellname = c_iter->first;
         X = round(pos.X);
         Y = round(pos.Y);
         break;
      }
   }
   return cellname;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_CirclePerimeterForm::PlaceObject(TBitmap32* bmp, int X, int Y)
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
//---------------------------------------------------------------------------


void __fastcall TGUI_CirclePerimeterForm::NetworkPaintBox32MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y)
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

void __fastcall TGUI_CirclePerimeterForm::NetworkPaintBox32MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if ((!NetworkPaintBox32->Buffer->Empty()) && (NetworkGUI->ModelMouseMode < NoMouseObject)){

      std::wstring cellname = CellGridHitTestAndCoerce(X, Y);
      bool over_existing_cell = (cellname != L"");

      switch (NetworkGUI->ModelMouseMode)
      {
         case PositionCell: {
            // get cell type and name from dialog -- create cell in net and add to treeview
            if (NetworkGUI->CreateCell(X, Y)){
               UpdateDisplay();
            }
         } ;  break;
         case PositionElectrode: {
            if (over_existing_cell) {
               if (NetworkGUI->CreateElectrode(cellname, X, Y)) {
                  UpdateDisplay();
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
                  int midX = X+(X1-X)/2;
                  int midY = Y+(Y1-Y)/2;
                  if (NetworkGUI->CreateSynapse(FirstCellName, SecondCellName, midX, midY)){
                     UpdateDisplay();
                  }
               }
            }
         }; break;
         case EditAnObject: {
//            EditObject(ChooseBmp, X, Y);
         }; break;
         default: break;
      };

      NetworkGUI->ModelMouseMode = NoMouseObject;
      UpdateDisplay();
   }
}
//---------------------------------------------------------------------------

void __fastcall TGUI_CirclePerimeterForm::FormResize(TObject *Sender)
{
   UpdateDisplay();
}
//---------------------------------------------------------------------------

