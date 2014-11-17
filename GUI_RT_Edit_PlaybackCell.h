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

#ifndef GUI_RT_Edit_PlaybackCellH
#define GUI_RT_Edit_PlaybackCellH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "MultiPLOTPanel.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include "PLOTPanel.h"
//---------------------------------------------------------------------------
class TPlaybackCell; // forward declaration

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackCellForm   ***************************
//
//
/// GUI Editor for TPlaybackCell
/*!
   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TPlaybackCellForm : public TForm
{
__published:	// IDE-managed Components
   TOpenDialog *OpenDialog1;
   TImageList *ImageList1;
   TPLOTPanel *PLOTPanel1;
   TLabel *Label1;
   TLabel *FileLabel;
   TBitBtn *BrowseBitBtn;
   TLabel *Label2;
   TComboBox *ChannelComboBox;
   TLabel *Label3;
   TEdit *ConvertmVEdit;
   TLabel *Label4;
   TLabel *SamplePeriodLabel;
   void __fastcall BrowseBitBtnClick(TObject *Sender);
   void __fastcall ChannelComboBoxChange(TObject *Sender);
   void __fastcall ConvertmVEditKeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
public:		// User declarations
   TPlaybackCell *Cell;
   __fastcall TPlaybackCellForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlaybackCellForm *PlaybackCellForm;
//---------------------------------------------------------------------------
#endif
