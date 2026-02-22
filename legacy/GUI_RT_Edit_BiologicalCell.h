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

#ifndef GUI_RT_Edit_BiologicalCellH
#define GUI_RT_Edit_BiologicalCellH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TBiologicalCell; //Forward declaration

/////////////////////////////////////////////////////////////////////////////
//   *****************    TBiologicalCellForm   ************************
//
//
/// GUI editor for TBiologicalCell
/*!
  @author E. Brady Trexler <ebtrexler (at) hudsonvalleysci.com>, 2011 - 2013
*/
class TBiologicalCellForm : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label6;
   TListBox *lbxAIChannels;
   TLabel *Label7;
   TListBox *lbxAOChannels;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TEdit *AIGainEdit;
   TEdit *AOGainEdit;
   TLabel *Label5;
   TEdit *ADCGainEdit;
   TLabel *Label8;
   TEdit *DACGainEdit;
   TLabel *Label9;
   TPanel *Panel1;
   TPanel *Panel2;
   TLabel *Label10;
   TLabel *Label11;
   TLabel *Label12;
   TEdit *PosLimitEdit;
   TEdit *NegLimitEdit;
   TLabel *Label13;
   TLabel *Label14;
   void __fastcall AmpGainEditKeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall NetworkGainEditKeyPress(TObject *Sender, wchar_t &Key);
   void __fastcall lbxAIAOChannelsClick(TObject *Sender);
   void __fastcall PosNegLimitEditKeyPress(TObject *Sender, wchar_t &Key);
private:	// User declarations
public:		// User declarations
   TBiologicalCell *BiologicalCell;
   __fastcall TBiologicalCellForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBiologicalCellForm *BiologicalCellForm;
//---------------------------------------------------------------------------
#endif
