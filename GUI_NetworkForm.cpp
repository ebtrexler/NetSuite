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
#include "GUI_RunModelForm.h"
#include "RT_Network.h"
#include "ChooseTypeAndName.h"

#include "GUI_SquareLatticeEditor.h"
#include "GUI_CirclePerimeterEditor.h"
#include "GUI_CopyCurrentsForm.h"

#include "VersionInfo.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma link "GenericUtilities"
#pragma resource "*.dfm"
TNetworkGUI *NetworkGUI;

#define COMPLEXTREE_SYNAPSES

const int NetworkIndex = 0;
const int NetworkEditIndex = 0;
const int CellIndex = 1;
const int CellEditIndex = 1;
const int SynapseIndex = 2;
const int SynapseEditIndex = 2;
const int ElectrodeIndex = 3;
const int ElectrodeEditIndex = 3;
const int CurrentIndex = 4;
const int CurrentEditIndex = 4;
const int Active = 1;
const int Inactive = 0;

//int      TNetworkGUI::ModelMouseMode;
//TNetwork *TNetworkGUI::net;

void Circle(TArrayOfFixedPoint &pts, int center, int radius)
{
	double deg = 0;
   int index = 0;
   pts.set_length(1257);
   do {       //Fixed := Round(Single * 65536);
      pts[index].X = round((center + radius * cos(deg)) * 65536);
      pts[index].Y = round((center - radius * sin(deg)) * 65536);
      deg += 0.005;
      ++index;
   } while (deg <= 6.283185307179586476925286766559); // 2 pi radians
}

void Circle(TArrayOfFixedPoint &pts, double Xcenter, double Ycenter, double radius)
{
   double deg = 0;
   int index = 0;
   pts.set_length(1257);
   do {       //Fixed := Round(Single * 65536);
      pts[index].X = round((Xcenter + radius * cos(deg)) * 65536);
      pts[index].Y = round((Ycenter - radius * sin(deg)) * 65536);
      deg += 0.005;
      ++index;
   } while (deg <= 6.283185307179586476925286766559); // 2 pi radians
}

//TNetwork *TNetworkGUI::net;
//TNetwork* GetNet()
//{
//   return TNetworkGUI::net;
//}


//---------------------------------------------------------------------------
__fastcall TNetworkGUI::TNetworkGUI(TComponent* Owner)
   : TForm(Owner)
{
   ModelMouseMode = NoMouseObject;
//   net = new TNetwork(L"HeapNet");

   TVersionInfo *info = new TVersionInfo(Application->ExeName);
   ExeVersion.sprintf(L"Version %d.%d.%d.%d",
                     info->MajorVersion,
                     info->MinorVersion,
                     info->ReleaseVersion,
                     info->Build);

   delete info;

   ActionVersionInfo->Caption = ExeVersion;

#ifdef SIM_ONLY_NO_NIDAQ
      Application->Title = "NetSim-- Network Modeling Software";
#else
      Application->Title = "NetClamp-- Network Modeling and Dynamic Clamp Software";
#endif

   Caption = Application->Title;

   UpdateDisplay();
   SyncTreeViewToNetwork();
}

void __fastcall TNetworkGUI::LoadNetwork(UnicodeString filename, TNetwork *net)
{
#ifdef SERIALIZE
   std::ifstream ifs(filename.c_str(), ios::in | ios::binary);
   assert(ifs.good());
   delete net;
   net = NULL;
   TNetwork *newnet = new TNetwork(filename.c_str());
   restore_network(*newnet, ifs);
   net = newnet;
   UpdateDisplay();
   SyncTreeViewToNetwork();
   RemoveEditPaneContents();
#endif
}
void __fastcall TNetworkGUI::WriteNetwork(UnicodeString filename, TNetwork *net)
{
#ifdef SERIALIZE
   std::ofstream ofs(filename.c_str(), ios::out | ios::trunc | ios::binary);
   assert(ofs.good());
   save_network(*net, ofs);
#endif
}

//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::UpdateDisplay()
{
   int count = NetworkPanel->ControlCount;
   for (int i=0; i<count; i++){
      NetworkPanel->Controls[i]->Visible = false;
      NetworkPanel->Controls[i]->Parent = NULL;
   }
   if (NetworkTabControl->TabIndex == 0) { // Square Lattice
      GetSquareLatticeForm()->BorderStyle= bsNone;
      GetSquareLatticeForm()->Parent = NetworkPanel;
      GetSquareLatticeForm()->Align = alClient;
      GetSquareLatticeForm()->Visible = true;
      GetSquareLatticeForm()->UpdateDisplay();
   }
   else if (NetworkTabControl->TabIndex == 1) { // Circle Perimeter
      GetCirclePerimeterForm()->BorderStyle= bsNone;
      GetCirclePerimeterForm()->Parent = NetworkPanel;
      GetCirclePerimeterForm()->Align = alClient;
      GetCirclePerimeterForm()->Visible = true;
      GetCirclePerimeterForm()->UpdateDisplay();
   }
}
//---------------------------------------------------------------------------
bool __fastcall TNetworkGUI::GetTypeAndName(std::wstring & type, std::wstring & name)
{
   std::wstring caption, nametext, typetext;
   std::vector<std::wstring> namesvector;
   bool apply = false;

   switch (NetworkGUI->ModelMouseMode)
   {
      case PositionCell: {
         caption = L"Choose Cell Type and Cell Name";
         nametext = L"Cell Name:";
         typetext = L"Cell Type:";
         namesvector = GetCellFactory().GetKeys();
      } ;  break;
      case PositionElectrode: {
         caption = L"Choose Electrode Type and Electrode Name";
         nametext = L"Electrode Name:";
         typetext = L"Electrode Type:";
         namesvector = GetElectrodeFactory().GetKeys();
      } ; break;
//      case PositionSynapse1:
      case PositionSynapse2: {
         caption = L"Choose Synapse Type and Synapse Name";
         nametext = L"Synapse Name:";
         typetext = L"Synapse Type:";
         namesvector = GetSynapseFactory().GetKeys();
      }; break;
      case AddCurrent: {
         caption = L"Choose Current Type and Current Name";
         nametext = L"Current Name:";
         typetext = L"Current Type:";
         namesvector = GetCurrentFactory().GetKeys();
      }; break;
      case EditAnObject: {

      }; break;
      default: break;
   };

   TypeAndNameDlg->Caption = caption.c_str();
   TypeAndNameDlg->TypeLabel->Caption = typetext.c_str();
   TypeAndNameDlg->NameLabel->Caption = nametext.c_str();
   TypeAndNameDlg->ActiveControl = TypeAndNameDlg->TypesComboBox;
   TypeAndNameDlg->Edit1->Text = "Enter Unique Name";
   TypeAndNameDlg->TypesComboBox->Clear();
   for (unsigned long i = 0; i < namesvector.size(); i++) {
      TypeAndNameDlg->TypesComboBox->Items->Add(namesvector[i].c_str());
   }
   TypeAndNameDlg->TypesComboBox->Text = "Choose a Type";

   if (TypeAndNameDlg->ShowModal() == mrOk) {
      type = TypeAndNameDlg->TypesComboBox->Text.w_str();
      name = TypeAndNameDlg->Edit1->Text.w_str();
      apply = true;
   }

   return apply;
}

//---------------------------------------------------------------------------


void __fastcall TNetworkGUI::SyncTreeViewToNetwork()
{
   NetworkTreeView->Items->Clear();

   NetworkNode = NetworkTreeView->Items->Add(NULL, "Network");
   NetworkNode->ImageIndex = NetworkIndex;
   NetworkNode->SelectedIndex = NetworkEditIndex;

   CellRootNode = NetworkTreeView->Items->AddChild(NetworkNode, "Cells");
   CellRootNode->ImageIndex = CellIndex;
   CellRootNode->SelectedIndex = CellEditIndex;
	AddCells(GetNet()->GetCells(), CellRootNode);

   SynapseRootNode = NetworkTreeView->Items->AddChild(NetworkNode, "Synapses");
   SynapseRootNode->ImageIndex = SynapseIndex;
	SynapseRootNode->SelectedIndex = SynapseEditIndex;
	AddSynapses(GetNet()->GetSynapses(), SynapseRootNode);

   ElectrodeRootNode = NetworkTreeView->Items->AddChild(NetworkNode, "Electrodes");
   ElectrodeRootNode->ImageIndex = ElectrodeIndex;
   ElectrodeRootNode->SelectedIndex = ElectrodeEditIndex;
   AddElectrodes(GetNet()->GetElectrodes(), ElectrodeRootNode);

   NetworkTreeView->FullExpand();
}
//---------------------------------------------------------------------------

bool __fastcall TNetworkGUI::CreateCell(int X, int Y)
{
   bool ok = true;
   std::wstring name, type;
   if (GetTypeAndName(type, name)) {
      try {
         GetNet()->AddCellToNet(type, name, X, Y);
      } catch (...) {
         Application->MessageBox(L"Name not unique, choose another", L"Name error...");
         ok = false;
      }
      SyncTreeViewToNetwork();
   }
   return ok;
}
//---------------------------------------------------------------------------

bool __fastcall TNetworkGUI::CreateSynapse(const std::wstring & FirstCellName,
                                           const std::wstring & SecondCellName,
                                           int X, int Y)
{
   bool ok = true;
   std::wstring name, type;
   if (GetTypeAndName(type,name)) {
      try {
         GetNet()->AddSynapseBetweenCells(type, name, FirstCellName, SecondCellName, X, Y);
      } catch (...) {
         Application->MessageBox(L"Name not unique, choose another", L"Name error...");
         ok = false;
      }
      SyncTreeViewToNetwork();
   }
   return ok;
}
//---------------------------------------------------------------------------

bool __fastcall TNetworkGUI::CreateElectrode(const std::wstring & cellname, int X, int Y)
{
   bool ok = true;
   std::wstring name, type;
   if (GetTypeAndName(type,name)) {
      try {
         GetNet()->AddElectrodeToCell(type, name, cellname, X, Y);
      } catch (...) {
         Application->MessageBox(L"Name not unique, choose another", L"Name error...");
         ok = false;
      }
      SyncTreeViewToNetwork();
   }
   return ok;
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::AddCells(const TCellsMap &cells, TTreeNode * node)
{
   TCellsMapConstIterator p;
   for (p = cells.begin(); p != cells.end(); ++p) {
      AddSingleCell(p->second.get(), node);
   }
}
TTreeNode * __fastcall TNetworkGUI::AddSingleCell(const TCell * cell, TTreeNode * node)
{
   TTreeNode *cellNode;
   String name, type, text;

   name = UnicodeString(cell->Name().c_str());
   type = UnicodeString(cell->ClassType().c_str());
   text = name + " (" + type + ")";
   cellNode = NetworkTreeView->Items->AddChild(node,text);
   if (node->Level < 3) cellNode->StateIndex = cell->IsActive() + 1;
   cellNode->ImageIndex = CellIndex;
   cellNode->SelectedIndex = CellEditIndex;
   cellNode->Data = (void *)cell;
   if (node->Level < 3) { // add cell components if early enough in tree
#ifdef COMPLEXTREE_CELLS
      AddSynapses(cell->GetSynapses(), cellNode);
      AddElectrodes(cell->GetElectrodes(), cellNode);
#endif
      text = "Intrinsic Currents"; // to identify for adding currents
      TTreeNode * currentsnode = NetworkTreeView->Items->AddChild(cellNode,text);
      currentsnode->ImageIndex = CurrentIndex;
      currentsnode->SelectedIndex = CurrentEditIndex;
      AddCurrents(cell->GetCurrents(), currentsnode);
   }

   return cellNode;
}

void __fastcall TNetworkGUI::AddSynapses(const TSynapsesMap &synapses, TTreeNode * node)
{
   TSynapsesMapConstIterator p;
   for (p = synapses.begin(); p != synapses.end(); ++p) {
      AddSingleSynapse(p->second.get(), node);
   }
}
void __fastcall TNetworkGUI::AddSynapses(const TSynapsesArray &synapses, TTreeNode * node)
{
   TSynapsesArrayConstIterator p;
   for (p = synapses.begin(); p != synapses.end(); ++p) {
      AddSingleSynapse(*p, node);
   }
}

TTreeNode * __fastcall TNetworkGUI::AddSingleSynapse(const TSynapse * synapse, TTreeNode * node)
{
   TTreeNode *synapseNode;
   String name, type, text;

   name = UnicodeString(synapse->Name().c_str());
   type = UnicodeString(synapse->ClassType().c_str());
   text = name + " (" + type + ")";
   synapseNode = NetworkTreeView->Items->AddChild(node,text);
   if (node->Level < 3) synapseNode->StateIndex = synapse->IsActive() + 1;
   synapseNode->ImageIndex = SynapseIndex;
   synapseNode->SelectedIndex = SynapseEditIndex;
   synapseNode->Data = (void *)synapse;

#ifdef COMPLEXTREE_SYNAPSES

   // add child nodes
   TTreeNode *preNode = NetworkTreeView->Items->AddChild(synapseNode, "Presynaptic Cell");
   preNode->ImageIndex = CellIndex;
   preNode->SelectedIndex = CellEditIndex;
   AddSingleCell(synapse->Pre(), preNode);

   TTreeNode *postNode = NetworkTreeView->Items->AddChild(synapseNode, "Postsynaptic Cell");
   postNode->ImageIndex = CellIndex;
   postNode->SelectedIndex = CellEditIndex;
   AddSingleCell(synapse->Post(), postNode);

#endif

   TTreeNode *pre2postNode = NetworkTreeView->Items->AddChild(synapseNode, "Pre-to-Post currents");
   pre2postNode->ImageIndex = CurrentIndex;
   pre2postNode->SelectedIndex = CurrentEditIndex;
   AddCurrents(synapse->PreToPostCurrents(), pre2postNode);

   TTreeNode *post2preNode = NetworkTreeView->Items->AddChild(synapseNode, "Post-to-Pre currents");
   post2preNode->ImageIndex = CurrentIndex;
   post2preNode->SelectedIndex = CurrentEditIndex;
   AddCurrents(synapse->PostToPreCurrents(), post2preNode);

   return synapseNode;
}

void __fastcall TNetworkGUI::AddElectrodes(const TElectrodesMap &electrodes, TTreeNode * node)
{
   TElectrodesMapConstIterator p;
   for (p = electrodes.begin(); p != electrodes.end(); ++p) {
      AddSingleElectrode(p->second.get(), node);
   }
}
void __fastcall TNetworkGUI::AddElectrodes(const TElectrodesArray &electrodes, TTreeNode * node)
{
   TElectrodesArrayConstIterator p;
   for (p = electrodes.begin(); p != electrodes.end(); ++p) {
      AddSingleElectrode(*p, node);
   }
}

TTreeNode * __fastcall TNetworkGUI::AddSingleElectrode(const TElectrode * electrode, TTreeNode * node)
{
   TTreeNode *electrodeNode;
   String name, type, text;
   name = UnicodeString(electrode->Name().c_str());
   type = UnicodeString(electrode->ClassType().c_str());
   text = name + " (" + type + ")";
   electrodeNode = NetworkTreeView->Items->AddChild(node,text);
   if (node->Level < 3) electrodeNode->StateIndex = electrode->IsActive() + 1;
   electrodeNode->ImageIndex = ElectrodeIndex;
   electrodeNode->SelectedIndex = ElectrodeEditIndex;
   electrodeNode->Data = (void *)electrode;

   return electrodeNode;
}

void __fastcall TNetworkGUI::AddCurrents(const TCurrentsArray &currents, TTreeNode * node)
{
   TCurrentsArrayConstIterator p;
   for (p = currents.begin(); p != currents.end(); ++p) {
      AddSingleCurrent(*p, node);
   }
}

TTreeNode * __fastcall TNetworkGUI::AddSingleCurrent(const TCurrent * current, TTreeNode * node)
{
   TTreeNode *currentNode;
   String name, type, text;

   name = UnicodeString(current->Name().c_str());
   type = UnicodeString(current->ClassType().c_str());
   text = name + " (" + type + ")";
   currentNode = NetworkTreeView->Items->AddChild(node,text);
   currentNode->StateIndex = current->IsActive() + 1;
   currentNode->ImageIndex = CurrentIndex;
   currentNode->SelectedIndex = CurrentEditIndex;
   currentNode->Data = (void *)current;

   return currentNode;
}

void __fastcall TNetworkGUI::pmnuNetworkPopup(TObject *Sender)
{
//   POINT P;
//   GetCursorPos(&P);
//   P = NetworkTreeView->ScreenToClient(P);
//   THitTests MyHitTest = NetworkTreeView->GetHitTestInfoAt(P.x,P.y);
//   if (MyHitTest.Contains(htNowhere))
//    Panel3->Caption = ("Nowhere");
//   if (MyHitTest.Contains(htOnItem))
//    Panel3->Caption = ("OnItem");
//   if (MyHitTest.Contains(htOnButton))
//    Panel3->Caption = ("OnButton");
//   if (MyHitTest.Contains(htOnIndent))
//    Panel3->Caption = ("OnIndent");
//   if (MyHitTest.Contains(htOnLabel))
//    Panel3->Caption = ("OnLabel");
//   if (MyHitTest.Contains(htOnRight))
//    Panel3->Caption = ("OnRight");
//   if (MyHitTest.Contains(htAbove))
//    Panel3->Caption = ("Above");
//   if (MyHitTest.Contains(htBelow))
//    Panel3->Caption = ("Below");
//   if (MyHitTest.Contains(htOnIcon))
//    Panel3->Caption = ("OnIcon");
//   if (MyHitTest.Contains(htOnStateIcon))
//    Panel3->Caption = ("OnStateIcon");
//   if (MyHitTest.Contains(htToLeft))
//    Panel3->Caption = ("ToLeft");
//   if (MyHitTest.Contains(htToRight))
//    Panel3->Caption = ("ToRight");

//   TreeView1->get
//   if (htOnStateIcon in TreeView1.GetHitTestInfoAt(P.X,P.Y)) then
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange)
{
   AllowChange = CheckNetworkTree();
}
//---------------------------------------------------------------------------
bool __fastcall TNetworkGUI::CheckNetworkTree()
{
   bool Allow = true;
   if (NetworkTreeView->Selected){
      if (NetworkTreeView->Selected->Data) { // node holds object, not just a branch point
         TRTBase *pRT = (TRTBase *)NetworkTreeView->Selected->Data;
         Allow = pRT->ValidateEditForm();
      }
   }
   return Allow;
}

void __fastcall TNetworkGUI::NetworkTreeViewChange(TObject *Sender, TTreeNode *Node)

{
   RemoveEditPaneContents();
   if (Node->Data) { // node holds object, not just a branch point
      TRTBase *pRT = (TRTBase *)Node->Data;
   //            std::wstring nodetype = pRT->ClassType;
      pRT->AttachEditFormTo(this, NetworkComponentEditScrollBox);
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
   THitTests MyHitTest = NetworkTreeView->GetHitTestInfoAt(X,Y);

   if ((MyHitTest.Contains(htOnStateIcon)) && (Button == mbLeft)) {
      TTreeNode *node = NetworkTreeView->GetNodeAt(X, Y);
      if (node->Data) { // node holds object, not just a branch point
         TRTBase *pRT = (TRTBase *)node->Data;
         bool active = !pRT->IsActive();
         pRT->SetActive(active);
         node->StateIndex = active + 1;
      }
   }
   if (MyHitTest.Contains(htOnItem) && (Button == mbLeft) && (ModelMouseMode == AddCurrent)) {

      EditedNode = NetworkTreeView->GetNodeAt(X, Y);
      std::wstring nodetext, type, name;
      nodetext = EditedNode->Text.c_str();

      // Add current to a cell
      if (nodetext.find(L"Intrinsic Currents") != std::string::npos) {
         if (GetTypeAndName(type, name)) {
            TCell *cell = (TCell *)EditedNode->Parent->Data;
            TCurrent * curr = NULL;
            try {
					curr = GetNet()->AddCurrentToCell(type, name, cell->Name());
				} catch (Exception & e) {
//               Application->MessageBox(L"Name not unique, choose another", L"Name error...");
					Application->MessageBox(e.ToString().c_str(), e.Message.c_str());
					return;
				}
				TTreeNode * newnode = AddSingleCurrent(curr, EditedNode);
            EditedNode->Expand(true);
            NetworkTreeView->Select(newnode);
         }
      }

      // Add a current to a synapse in the pre-to-post direction
      if (nodetext.find(L"Pre-to-Post currents") != std::string::npos) {
         if (GetTypeAndName(type, name)) {
            TSynapse *synapse = (TSynapse *)EditedNode->Parent->Data;;
            std::wstring cellname = synapse->Post()->Name();
            TCurrent * curr = NULL;
            try {
					curr = GetNet()->AddCurrentToSynapse(synapse->Name(), type, name, cellname);
				} catch (Exception & e) {
//               Application->MessageBox(L"Name not unique, choose another", L"Name error...");
					Application->MessageBox(e.ToString().c_str(), e.Message.c_str());
					return;
				}

            TTreeNode * newnode = AddSingleCurrent(curr, EditedNode);
            EditedNode->Expand(true);
            NetworkTreeView->Select(newnode);
         }
      }

      // Add a current to a synapse in the post-to-pre direction
      if (nodetext.find(L"Post-to-Pre currents") != std::string::npos) {
         if (GetTypeAndName(type, name)) {
            TSynapse *synapse = (TSynapse *)EditedNode->Parent->Data;;
            std::wstring cellname = synapse->Pre()->Name();
            TCurrent * curr = NULL;
            try {
					curr = GetNet()->AddCurrentToSynapse(synapse->Name(), type, name, cellname);
				} catch (Exception & e) {
//               Application->MessageBox(L"Name not unique, choose another", L"Name error...");
					Application->MessageBox(e.ToString().c_str(), e.Message.c_str());
					return;
            }

            TTreeNode * newnode = AddSingleCurrent(curr, EditedNode);
            EditedNode->Expand(true);
            NetworkTreeView->Select(newnode);
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewEditing(TObject *Sender, TTreeNode *Node,
          bool &AllowEdit)
{
   AllowEdit = (Node->Data != NULL); // node holds object, not just a branch point
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewEdited(TObject *Sender, TTreeNode *Node,
          UnicodeString &S)
{
   EditedNode = Node;

   if (EditedNode->Data) { // node holds object, not just a branch point

      TRTBase *pRT = (TRTBase *)EditedNode->Data;

      std::wstring type, oldname, newname;
      type = pRT->ClassType();
      oldname = pRT->Name();
      newname = S.c_str();

      if (type.find(L"Synapse") != std::string::npos) {
			GetNet()->RenameSynapse(oldname, newname);
      }
      if (type.find(L"Cell") != std::string::npos) {
			GetNet()->RenameCell(oldname, newname);
      }
      if (type.find(L"Electrode") != std::string::npos) {
			GetNet()->RenameElectrode(oldname, newname);
      }
      if (type.find(L"Current") != std::string::npos) {
			GetNet()->RenameCurrent(oldname, newname);
      }

      UpdateDisplay();
      SyncTreeViewToNetwork();
      RemoveEditPaneContents();
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewDblClick(TObject *Sender)
{
   if (NetworkTreeView->Selected){
      if (NetworkTreeView->Selected->Data) { // node holds object, not just a branch point
         TRTBase *pRT = (TRTBase *)NetworkTreeView->Selected->Data;
         NetworkTreeView->Selected->Text = pRT->Name().c_str();
         NetworkTreeView->Selected->EditText();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewCancelEdit(TObject *Sender, TTreeNode *Node)

{
   SyncTreeViewToNetwork();
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::RemoveEditPaneContents()
{
   int count = NetworkComponentEditScrollBox->ControlCount;
   for (int i=0; i<count; i++){
      NetworkComponentEditScrollBox->Controls[i]->Visible = false;
      NetworkComponentEditScrollBox->Controls[i]->Parent = NULL;
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::RemoveItem1Click(TObject *Sender)
{
  // RemoveNetworkItem()
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::CreateItem1Click(TObject *Sender)
{
  // CreateNetworkItem();
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateCellExecute(TObject *Sender)
{
   ModelMouseMode = PositionCell;
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateCellUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateSynapseExecute(TObject *Sender)
{
   ModelMouseMode = PositionSynapse1;
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateSynapseUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateElectrodeExecute(TObject *Sender)
{
   ModelMouseMode = PositionElectrode;
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCreateElectrodeUpdate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCurrentEditExecute(TObject *Sender)
{
   if (ModelMouseMode == AddCurrent) {
      ModelMouseMode = NoMouseObject;
      ActionCurrentEdit->Caption = "Add Current";
      ActionCurrentEdit->Checked = false;
   }
   else {
      ModelMouseMode = AddCurrent;
      ActionCurrentEdit->Caption = "Stop Current Addition";
      ActionCurrentEdit->Checked = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionCurrentEditUpdate(TObject *Sender)
{
   if (ModelMouseMode == AddCurrent) {
      ActionCurrentEdit->Caption = "Stop Current Addition";
   }
   else {
      ActionCurrentEdit->Caption = "Add Current";
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::FileOpen1Accept(TObject *Sender)
{
	LoadNetwork(FileOpen1->Dialog->FileName, GetNet());
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::FileSaveAs1Accept(TObject *Sender)
{
   if (CheckNetworkTree()) {
		WriteNetwork(FileSaveAs1->Dialog->FileName, GetNet());
   }
}
//---------------------------------------------------------------------------


void __fastcall TNetworkGUI::ActionRunExecute(TObject *Sender)
{
   if (CheckNetworkTree()) {
      RunDynamicClampForm->LetErRip();
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionClearNetworkExecute(TObject *Sender)
{
   if (Application->MessageBox(  L"Are you sure?",
                                 L"Please confirm network deletion",
                                 MB_YESNO)  == IDYES )
	{
		GetNet(true);
		UpdateDisplay();
      SyncTreeViewToNetwork();
      RemoveEditPaneContents();
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::ActionRemoveItemExecute(TObject *Sender)
{
   if (NetworkTreeView->SelectionCount == 0) {
      return;
   }

   EditedNode = NetworkTreeView->Selected;

   if (EditedNode->Data) { // node holds object, not just a branch point

      TRTBase *pRT = (TRTBase *)EditedNode->Data;

      std::wstring type, name;
      type = pRT->ClassType();
      name = pRT->Name();

      // make sure before delete
      UnicodeString prompt(L"Are you sure you want to delete :");
      prompt += EditedNode->Text;
      if (Application->MessageBox(prompt.c_str(), L"Confirm Deletion...", MB_YESNO) == IDNO)
         return;

      if (type.find(L"Synapse") != std::string::npos) {
			GetNet()->RemoveSynapseFromNet(name);
      }
		if (type.find(L"Cell") != std::string::npos) {
			GetNet()->RemoveCellFromNet(name);
      }
      if (type.find(L"Electrode") != std::string::npos) {
			GetNet()->RemoveElectrodeFromNet(name);
      }
      if (type.find(L"Current") != std::string::npos) {
         TCurrent *curr = (TCurrent *) pRT;
         type = curr->Owner()->ClassType();
         if (type.find(L"Cell") != std::string::npos) {
				GetNet()->RemoveCurrentFromCell(name, curr->Owner()->Name());
         }
         if (type.find(L"Synapse") != std::string::npos) {
				GetNet()->RemoveCurrentFromSynapse(name, curr->Owner()->Name());
         }
      }

      UpdateDisplay();
      SyncTreeViewToNetwork();
      RemoveEditPaneContents();
   }
}
//---------------------------------------------------------------------------


void __fastcall TNetworkGUI::NetworkTabControlChange(TObject *Sender)
{
   UpdateDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::NetworkTreeViewMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
   TCursor cursor = crDefault;
   TTreeNode *node = NetworkTreeView->GetNodeAt(X, Y);
   if ((node != NULL) && (ModelMouseMode == AddCurrent)){
      std::wstring nodetext, type, name;
      nodetext = node->Text.c_str();
      if (nodetext.find(L"Intrinsic Currents") != std::string::npos){
         cursor = crCross;
//         node->Selected = true;
      }
      else if (nodetext.find(L"Pre-to-Post currents") != std::string::npos) {
         cursor = crCross;
//         node->Selected = true;
      }
      else if (nodetext.find(L"Post-to-Pre currents") != std::string::npos) {
         cursor = crCross;
//         node->Selected = true;
      }
      else {
         cursor = crDefault;
      }
      NetworkTreeView->Cursor = cursor;
   }

}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::CopyCurrentsActionExecute(TObject *Sender)
{
   CopyCurrentsForm->CopyFromComboBox->Items->Clear();
   CopyCurrentsForm->CopyToListBox->Items->Clear();

   TCellsMapConstIterator p;
   for (p = GetNet()->GetCells().begin(); p != GetNet()->GetCells().end(); ++p) {
      UnicodeString cellname(p->second.get()->Name().c_str());
      CopyCurrentsForm->CopyFromComboBox->Items->Add(cellname);
      CopyCurrentsForm->CopyToListBox->Items->Add(cellname);
   }
   CopyCurrentsForm->CopyFromComboBox->Text = L"Select a cell";
   if (CopyCurrentsForm->ShowModal() == mrOk) {
      if (CopyCurrentsForm->CopyFromComboBox->ItemIndex > -1){

         std::wstring fromname(CopyCurrentsForm->CopyFromComboBox->Text.c_str());
         std::vector<std::wstring> tonames;

			TListBox *lb = CopyCurrentsForm->ToNamesListBox;
         lb->Items->Clear();
         CopyCurrentsForm->CopyToListBox->CopySelection(lb);

         for (int i = 0; i < lb->Count; i++){
            tonames.push_back(lb->Items->Strings[i].c_str());
         }

			bool clearcurrents = CopyCurrentsForm->ClearThenCopyCheckBox->Checked;
			GetNet()->CopyCurrentsFromCell(fromname, tonames, clearcurrents);

         SyncTreeViewToNetwork();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::FileOpen1BeforeExecute(TObject *Sender)
{
//	FileOpen1->OpenDialog->InitialDir = ExtractFilePath(Application->ExeName)  + L"models";
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::FileSaveAs1BeforeExecute(TObject *Sender)
{
//	FileSaveAs1->SaveDialog->InitialDir = ExtractFilePath(Application->ExeName)  + L"models";
}
//---------------------------------------------------------------------------

void __fastcall TNetworkGUI::CollapseAll1Click(TObject *Sender)
{
	TTreeNode *node = NetworkTreeView->Items->GetFirstNode()->getFirstChild();
	while (node != NULL){
		node->Collapse(true);
		node = node->getNextSibling();
	}
}
//---------------------------------------------------------------------------

