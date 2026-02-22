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

Please direct correspondence to ebtrexler _at_ hudsonvalleysci _dot_ com
///////////////////////////////////////////////////////////////////////////// */

//---------------------------------------------------------------------------

#ifndef GUI_RT_Edit_PlaybackElectrodeH
#define GUI_RT_Edit_PlaybackElectrodeH
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
class TPlaybackElectrode; // forward declaration

////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TPlaybackElectrodeForm   ***************************
//
//
/// GUI Editor for TPlaybackElectrode
/*!
   @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TPlaybackElectrodeForm : public TForm
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
   TEdit *ConvertnAEdit;
   TLabel *Label4;
   TLabel *SamplePeriodLabel;
private:	// User declarations
public:		// User declarations
   TPlaybackElectrode *Electrode;
   __fastcall TPlaybackElectrodeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlaybackElectrodeForm *PlaybackElectrodeForm;
//---------------------------------------------------------------------------
#endif
