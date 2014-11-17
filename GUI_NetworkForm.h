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

#ifndef GUI_NetworkFormH
#define GUI_NetworkFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>

#include "RT_Network.h"
#include "GenericUtilities.h"
#include <ExtActns.hpp>
#include <StdActns.hpp>
#include <Tabs.hpp>
#include <System.Actions.hpp>

enum ModelMouseModeEnum {PositionCell, PositionElectrode, PositionSynapse1,
      PositionSynapse2, AddCurrent, EditAnObject, NoMouseObject};

void Circle(TArrayOfFixedPoint &pts, int center, int radius);
void Circle(TArrayOfFixedPoint &pts, double Xcenter, double Ycenter, double radius);

//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
//    ********************   TNetworkGUI   ***************************
//
//
/// Graphical interface for creating, editing, and running a network
/*!
   Master network builder and editor that contains 3 regions:
   - An area with forms for designing connections (TGUI_CirclePerimeterForm and TGUI_SquareLatticeForm)
   - A "tree" organization of the network that is used for selecting network elements, and
   - An area to display the GUI edit forms for the selected network component.

   @author E. Brady Trexler <ebtrexler (at) gothamsci.com>, 2011 - 2013
*/
class TNetworkGUI : public TForm
{
__published:	// IDE-managed Components
   TActionManager *ActionManager1;
   TActionMainMenuBar *ActionMainMenuBar1;
   TActionToolBar *ActionToolBar1;
   TImageList *ImageList1;
   TImageList *ImageList2;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPopupMenu *pmnuNetwork;
   TMenuItem *RemoveItem1;
   TSplitter *Splitter1;
   TAction *ActionCreateCell;
   TAction *ActionCreateSynapse;
   TAction *ActionCreateElectrode;
   TScrollBox *NetworkComponentEditScrollBox;
   TGenericUtilities *GenericUtilities1;
   TActionToolBar *ActionToolBar2;
   TFileOpen *FileOpen1;
   TFileSaveAs *FileSaveAs1;
   TAction *ActionCurrentEdit;
   TAction *ActionRun;
   TAction *ActionClearNetwork;
   TPanel *Panel4;
   TTreeView *NetworkTreeView;
   TPanel *Panel5;
   TActionToolBar *ActionToolBar3;
   TAction *ActionRemoveItem;
   TPanel *Panel10;
   TSplitter *Splitter2;
   TPanel *NetworkPanel;
   TTabControl *NetworkTabControl;
   TAction *ActionVersionInfo;
   TAction *CopyCurrentsAction;
	TMenuItem *CollapseAll1;
   void __fastcall pmnuNetworkPopup(TObject *Sender);
   void __fastcall NetworkTreeViewMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall RemoveItem1Click(TObject *Sender);
   void __fastcall CreateItem1Click(TObject *Sender);
   void __fastcall NetworkTreeViewChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange);
   void __fastcall NetworkTreeViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionCreateCellExecute(TObject *Sender);
   void __fastcall ActionCreateCellUpdate(TObject *Sender);
   void __fastcall ActionCreateSynapseExecute(TObject *Sender);
   void __fastcall ActionCreateSynapseUpdate(TObject *Sender);
   void __fastcall ActionCreateElectrodeExecute(TObject *Sender);
   void __fastcall ActionCreateElectrodeUpdate(TObject *Sender);
   
   void __fastcall FileOpen1Accept(TObject *Sender);
   void __fastcall FileSaveAs1Accept(TObject *Sender);
   void __fastcall ActionCurrentEditExecute(TObject *Sender);
   void __fastcall ActionCurrentEditUpdate(TObject *Sender);
   void __fastcall ActionRunExecute(TObject *Sender);
   void __fastcall ActionClearNetworkExecute(TObject *Sender);
   void __fastcall ActionRemoveItemExecute(TObject *Sender);
   void __fastcall NetworkTreeViewEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
   void __fastcall NetworkTreeViewEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);
   void __fastcall NetworkTreeViewDblClick(TObject *Sender);
   void __fastcall NetworkTreeViewCancelEdit(TObject *Sender, TTreeNode *Node);
   void __fastcall NetworkTabControlChange(TObject *Sender);
   void __fastcall NetworkTreeViewMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall CopyCurrentsActionExecute(TObject *Sender);
	void __fastcall FileOpen1BeforeExecute(TObject *Sender);
	void __fastcall FileSaveAs1BeforeExecute(TObject *Sender);
	void __fastcall CollapseAll1Click(TObject *Sender);




private:	// User declarations
//   friend int      GetModelMouseMode();
//   static TNetwork *net;
//   friend TNetwork* GetNet();
//   TNetwork *net;

   UnicodeString ExeVersion;
   bool __fastcall GetTypeAndName(std::wstring & type, std::wstring & name);
   void __fastcall UpdateDisplay();

   // Network Tree Display functions
   TTreeNode *NetworkNode, *CellRootNode, *SynapseRootNode, *ElectrodeRootNode;
   TTreeNode *EditedNode;
   void __fastcall SyncTreeViewToNetwork();
   // map based functions
   void __fastcall AddCells(const TCellsMap &cells, TTreeNode * node);
   void __fastcall AddSynapses(const TSynapsesMap &synapses, TTreeNode * node);
   void __fastcall AddElectrodes(const TElectrodesMap &electrodes, TTreeNode * node);
   // vector based functions
   void __fastcall AddSynapses(const TSynapsesArray &synapses, TTreeNode * node);
   void __fastcall AddElectrodes(const TElectrodesArray &electrodes, TTreeNode * node);
   void __fastcall AddCurrents(const TCurrentsArray &currents, TTreeNode * node);
   // ptr based functions
   TTreeNode * __fastcall AddSingleCell(const TCell * cell, TTreeNode * node);
   TTreeNode * __fastcall AddSingleSynapse(const TSynapse * synapse, TTreeNode * node);
   TTreeNode * __fastcall AddSingleElectrode(const TElectrode * electrode, TTreeNode * node);
   TTreeNode * __fastcall AddSingleCurrent(const TCurrent * current, TTreeNode * node);

   void __fastcall RemoveNetworkItem();
   void __fastcall CreateNetworkItem();

   bool __fastcall CheckNetworkTree();

   void __fastcall LoadNetwork(UnicodeString filename, TNetwork *net);
   void __fastcall WriteNetwork(UnicodeString filename, TNetwork *net);

   void __fastcall RemoveEditPaneContents();
public:		// User declarations

   int      ModelMouseMode;
   bool __fastcall CreateCell(int X, int Y);
   bool __fastcall CreateSynapse(const std::wstring & FirstCellName,
                                 const std::wstring & SecondCellName,
                                 int X, int Y);
   bool __fastcall CreateElectrode(const std::wstring & cellname, int X, int Y);

   __fastcall TNetworkGUI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNetworkGUI *NetworkGUI;
//---------------------------------------------------------------------------
#endif
