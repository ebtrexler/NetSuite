#include "NetworkFrame.h"
#include "RT_Cell_wx.h"
#include "RT_Synapse_wx.h"
#include "RT_Electrode_wx.h"
#include "CreateItemDialog.h"
#include "ConnectionDialog.h"
#include "JsonSerializer.h"
#include "NetworkVisualizationPanel.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/artprov.h>

class ObjectTreeItemData : public wxTreeItemData
{
public:
    ObjectTreeItemData(TRTBase *obj) : m_object(obj) {}
    TRTBase *GetObject() const { return m_object; }

private:
    TRTBase *m_object;
};

wxBEGIN_EVENT_TABLE(NetworkFrame, wxFrame)
    EVT_MENU(wxID_EXIT, NetworkFrame::OnExit)
        EVT_MENU(wxID_ABOUT, NetworkFrame::OnAbout)
            EVT_MENU(wxID_OPEN, NetworkFrame::OnOpen)
                EVT_MENU(wxID_SAVE, NetworkFrame::OnSave)
                    EVT_MENU(ID_CreateCell, NetworkFrame::OnCreateCell)
                        EVT_MENU(ID_CreateSynapse, NetworkFrame::OnCreateSynapse)
                            EVT_MENU(ID_CreateElectrode, NetworkFrame::OnCreateElectrode)
                                EVT_MENU(ID_AddCurrent, NetworkFrame::OnAddCurrent)
                                    EVT_MENU(ID_RemoveItem, NetworkFrame::OnRemoveItem)
                                        EVT_MENU(ID_RunModel, NetworkFrame::OnRunModel)
                                            EVT_MENU(ID_ClearNetwork, NetworkFrame::OnClearNetwork)
                                                EVT_TREE_SEL_CHANGED(wxID_ANY, NetworkFrame::OnTreeSelChanged)
                                                    EVT_TREE_ITEM_ACTIVATED(wxID_ANY, NetworkFrame::OnTreeItemActivated)
                                                        EVT_BUTTON(ID_AddButton, NetworkFrame::OnAddButton)
                                                            EVT_BUTTON(ID_RemoveButton, NetworkFrame::OnRemoveButton)
                                                                EVT_MENU(wxID_ANY, NetworkFrame::OnVisualizationSelection)
                                                                    wxEND_EVENT_TABLE()

                                                                        NetworkFrame::NetworkFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 800))
{
    // Create menu bar
    m_menuBar = new wxMenuBar;

    // File menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O", "Open network file");
    fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S", "Save network file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
    m_menuBar->Append(fileMenu, "&File");

    // Network menu
    wxMenu *networkMenu = new wxMenu;
    networkMenu->Append(ID_CreateCell, "Create &Cell", "Create a new cell");
    networkMenu->Append(ID_CreateSynapse, "Create &Synapse", "Create a new synapse");
    networkMenu->Append(ID_CreateElectrode, "Create &Electrode", "Create a new electrode");
    networkMenu->AppendSeparator();
    networkMenu->Append(ID_AddCurrent, "&Add Current", "Add current to selected item");
    networkMenu->Append(ID_RemoveItem, "&Remove Item", "Remove selected item");
    networkMenu->AppendSeparator();
    networkMenu->Append(ID_ClearNetwork, "Clear &Network", "Clear entire network");
    m_menuBar->Append(networkMenu, "&Network");

    // Run menu
    wxMenu *runMenu = new wxMenu;
    runMenu->Append(ID_RunModel, "&Run Model\tF5", "Run the network model");
    m_menuBar->Append(runMenu, "&Run");

    // Help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
    m_menuBar->Append(helpMenu, "&Help");

    SetMenuBar(m_menuBar);

    // Create toolbar
    m_toolBar = CreateToolBar();
    m_toolBar->AddTool(wxID_OPEN, "Open", wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    m_toolBar->AddTool(wxID_SAVE, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR));
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_CreateCell, "Cell", wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR));
    m_toolBar->AddTool(ID_CreateSynapse, "Synapse", wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR));
    m_toolBar->AddTool(ID_CreateElectrode, "Electrode", wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR));
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_RunModel, "Run", wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_TOOLBAR));
    m_toolBar->Realize();

    // Create status bar
    CreateStatusBar(2);
    SetStatusText("Ready", 0);

    // Create main splitter (horizontal)
    m_mainSplitter = new wxSplitterWindow(this, wxID_ANY);

    // Create left splitter (vertical)
    m_leftSplitter = new wxSplitterWindow(m_mainSplitter, wxID_ANY);

    // Create network design notebook (top-left)
    m_networkNotebook = new wxNotebook(m_leftSplitter, wxID_ANY);

    // Add visualization panels
    m_squarePanel = new NetworkVisualizationPanel(m_networkNotebook, false);
    m_circlePanel = new NetworkVisualizationPanel(m_networkNotebook, true);
    m_networkNotebook->AddPage(m_squarePanel, "Square Lattice");
    m_networkNotebook->AddPage(m_circlePanel, "Circle Perimeter");

    // Set network reference
    m_squarePanel->SetNetwork(GetNet());
    m_circlePanel->SetNetwork(GetNet());

    // Bind visualization selection events
    m_squarePanel->Bind(wxEVT_COMMAND_MENU_SELECTED, &NetworkFrame::OnVisualizationSelection, this);
    m_circlePanel->Bind(wxEVT_COMMAND_MENU_SELECTED, &NetworkFrame::OnVisualizationSelection, this);

    // Create tree panel with buttons (bottom-left)
    m_treePanel = new wxPanel(m_leftSplitter, wxID_ANY);

    m_networkTree = new wxTreeCtrl(m_treePanel, wxID_ANY,
                                   wxDefaultPosition, wxDefaultSize,
                                   wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS);

    m_addButton = new wxButton(m_treePanel, ID_AddButton, "+", wxDefaultPosition, wxSize(30, 25));
    m_removeButton = new wxButton(m_treePanel, ID_RemoveButton, "-", wxDefaultPosition, wxSize(30, 25));

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(m_addButton, 0, wxALL, 2);
    buttonSizer->Add(m_removeButton, 0, wxALL, 2);

    wxBoxSizer *treeSizer = new wxBoxSizer(wxVERTICAL);
    treeSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 2);
    treeSizer->Add(m_networkTree, 1, wxEXPAND | wxALL, 2);
    m_treePanel->SetSizer(treeSizer);

    // Create edit panel (right side)
    m_editPanel = new wxPanel(m_mainSplitter, wxID_ANY);
    m_editScrolled = new wxScrolledWindow(m_editPanel, wxID_ANY);
    m_editScrolled->SetScrollRate(5, 5);

    wxBoxSizer *editSizer = new wxBoxSizer(wxVERTICAL);
    editSizer->Add(m_editScrolled, 1, wxEXPAND | wxALL, 5);
    m_editPanel->SetSizer(editSizer);

    // Setup splitters
    m_leftSplitter->SplitHorizontally(m_networkNotebook, m_treePanel, 300);
    m_mainSplitter->SplitVertically(m_leftSplitter, m_editPanel, 400);

    // Initialize network tree
    UpdateNetworkTree();
}

void NetworkFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void NetworkFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("NetSuite - Neural Network Modeling and Dynamic Clamp Software\n"
                 "wxWidgets Version\n"
                 "Copyright (C) 2011-2025 E. Brady Trexler, Ph.D.",
                 "About NetSuite", wxOK | wxICON_INFORMATION);
}

void NetworkFrame::OnOpen(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, "Open Network File", "", "",
                                "JSON files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (JsonSerializer::LoadNetwork(GetNet(), openFileDialog.GetPath().ToStdString()))
    {
        UpdateNetworkTree();
        ClearEditPanel();
        SetStatusText("Network loaded: " + openFileDialog.GetFilename(), 0);
    }
    else
    {
        wxMessageBox("Failed to load network file", "Error", wxOK | wxICON_ERROR);
    }
}

void NetworkFrame::OnSave(wxCommandEvent &event)
{
    wxFileDialog saveFileDialog(this, "Save Network File", "", "",
                                "JSON files (*.json)|*.json", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (JsonSerializer::SaveNetwork(GetNet(), saveFileDialog.GetPath().ToStdString()))
    {
        SetStatusText("Network saved: " + saveFileDialog.GetFilename(), 0);
    }
    else
    {
        wxMessageBox("Failed to save network file", "Error", wxOK | wxICON_ERROR);
    }
}

void NetworkFrame::OnCreateCell(wxCommandEvent &event)
{
    wxArrayString types;
    types.Add("TModelCell");
    types.Add("TBiologicalCell");

    CreateItemDialog dlg(this, "Create Cell", types);
    if (dlg.ShowModal() == wxID_OK)
    {
        // Use auto-placement from visualization panel
        wxPoint pos = m_squarePanel->GetNextCellPosition();
        GetNet()->AddCell(dlg.GetSelectedType().ToStdWstring(), dlg.GetItemName().ToStdWstring(), pos.x, pos.y);
        UpdateNetworkTree();
        SetStatusText("Cell created: " + dlg.GetItemName(), 0);
    }
}

void NetworkFrame::OnCreateSynapse(wxCommandEvent &event)
{
    if (GetNet()->GetCells().size() < 2)
    {
        wxMessageBox("Need at least 2 cells to create a synapse", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    wxArrayString types;
    types.Add("TSynapse");

    CreateItemDialog dlg(this, "Create Synapse", types);
    if (dlg.ShowModal() == wxID_OK)
    {
        SynapseConnectionDialog connDlg(this, GetNet());
        if (connDlg.ShowModal() == wxID_OK)
        {
            TSynapse *synapse = GetNet()->AddSynapse(dlg.GetSelectedType().ToStdWstring(),
                                                     dlg.GetItemName().ToStdWstring(), L"", L"", 200, 100);

            // Set connections
            std::wstring preName = connDlg.GetPreCell().ToStdWstring();
            std::wstring postName = connDlg.GetPostCell().ToStdWstring();

            auto &cells = GetNet()->GetCells();
            auto preIt = cells.find(preName);
            auto postIt = cells.find(postName);
            if (preIt != cells.end())
                synapse->SetPre(preIt->second.get());
            if (postIt != cells.end())
                synapse->SetPost(postIt->second.get());

            UpdateNetworkTree();
            SetStatusText("Synapse created: " + dlg.GetItemName(), 0);
        }
    }
}

void NetworkFrame::OnCreateElectrode(wxCommandEvent &event)
{
    if (GetNet()->GetCells().empty())
    {
        wxMessageBox("Need at least 1 cell to create an electrode", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    wxArrayString types;
    types.Add("TElectrode");

    CreateItemDialog dlg(this, "Create Electrode", types);
    if (dlg.ShowModal() == wxID_OK)
    {
        ElectrodeConnectionDialog connDlg(this, GetNet());
        if (connDlg.ShowModal() == wxID_OK)
        {
            TElectrode *electrode = GetNet()->AddElectrode(dlg.GetSelectedType().ToStdWstring(),
                                                           dlg.GetItemName().ToStdWstring(), L"", 300, 100);

            // Set owner
            std::wstring ownerName = connDlg.GetOwnerCell().ToStdWstring();
            auto &cells = GetNet()->GetCells();
            auto ownerIt = cells.find(ownerName);
            if (ownerIt != cells.end())
            {
                electrode->SetOwner(ownerIt->second.get());
            }

            UpdateNetworkTree();
            SetStatusText("Electrode created: " + dlg.GetItemName(), 0);
        }
    }
}

void NetworkFrame::OnAddCurrent(wxCommandEvent &event)
{
    // TODO: Implement current addition
    wxMessageBox("Add Current - Not yet implemented", "Info", wxOK | wxICON_INFORMATION);
}

void NetworkFrame::OnRemoveItem(wxCommandEvent &event)
{
    wxTreeItemId selection = m_networkTree->GetSelection();
    if (!selection.IsOk())
        return;

    ObjectTreeItemData *data = dynamic_cast<ObjectTreeItemData *>(m_networkTree->GetItemData(selection));
    if (!data)
        return;

    TRTBase *obj = data->GetObject();
    if (!obj)
        return;

    if (wxMessageBox(wxString::Format("Remove %s?", obj->Name()), "Confirm Removal",
                     wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        std::wstring name = obj->Name();
        std::wstring classKey = obj->ClassKey();

        if (classKey.find(L"Cell") != std::wstring::npos)
        {
            GetNet()->RemoveCell(name);
        }
        else if (classKey.find(L"Synapse") != std::wstring::npos)
        {
            GetNet()->RemoveSynapse(name);
        }
        else if (classKey.find(L"Electrode") != std::wstring::npos)
        {
            GetNet()->RemoveElectrode(name);
        }

        UpdateNetworkTree();
        ClearEditPanel();
        SetStatusText("Item removed: " + wxString(name), 0);
    }
}

void NetworkFrame::OnRunModel(wxCommandEvent &event)
{
    // TODO: Implement model execution
    wxMessageBox("Run Model - Not yet implemented", "Info", wxOK | wxICON_INFORMATION);
}

void NetworkFrame::OnClearNetwork(wxCommandEvent &event)
{
    if (wxMessageBox("Are you sure you want to clear the entire network?", "Confirm Clear",
                     wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        GetNet()->Clear();
        UpdateNetworkTree();
        ClearEditPanel();
        SetStatusText("Network cleared", 0);
    }
}

void NetworkFrame::OnTreeSelChanged(wxTreeEvent &event)
{
    wxTreeItemId item = event.GetItem();
    if (!item.IsOk())
        return;

    ObjectTreeItemData *data = dynamic_cast<ObjectTreeItemData *>(m_networkTree->GetItemData(item));
    if (data)
    {
        TRTBase *obj = data->GetObject();
        ShowEditPanel(obj);

        // Update visualization selection
        m_squarePanel->SetSelectedItem(obj);
        m_circlePanel->SetSelectedItem(obj);
    }
    else
    {
        ClearEditPanel();
        m_squarePanel->SetSelectedItem(nullptr);
        m_circlePanel->SetSelectedItem(nullptr);
    }

    wxString itemText = m_networkTree->GetItemText(item);
    SetStatusText("Selected: " + itemText, 1);
}

void NetworkFrame::OnTreeItemActivated(wxTreeEvent &event)
{
    wxTreeItemId item = event.GetItem();
    if (!item.IsOk())
        return;

    // TODO: Open edit dialog for item
    wxString itemText = m_networkTree->GetItemText(item);
    wxMessageBox("Edit " + itemText + " - Not yet implemented", "Info", wxOK | wxICON_INFORMATION);
}

void NetworkFrame::UpdateNetworkTree()
{
    // Store current selection
    TRTBase *currentSelection = nullptr;
    wxTreeItemId selection = m_networkTree->GetSelection();
    if (selection.IsOk())
    {
        ObjectTreeItemData *data = dynamic_cast<ObjectTreeItemData *>(m_networkTree->GetItemData(selection));
        if (data)
            currentSelection = data->GetObject();
    }

    m_networkTree->DeleteAllItems();

    wxTreeItemId root = m_networkTree->AddRoot("Network");
    m_networkTree->SetItemData(root, new ObjectTreeItemData(GetNet()));

    // Add main categories
    wxTreeItemId cellsRoot = m_networkTree->AppendItem(root, "Cells");
    wxTreeItemId synapsesRoot = m_networkTree->AppendItem(root, "Synapses");
    wxTreeItemId electrodesRoot = m_networkTree->AppendItem(root, "Electrodes");

    // Populate with actual network data
    AddCellsToTree(GetNet()->GetCells(), cellsRoot);
    AddSynapsesToTree(GetNet()->GetSynapses(), synapsesRoot);
    AddElectrodesToTree(GetNet()->GetElectrodes(), electrodesRoot);

    m_networkTree->ExpandAll();

    // Restore selection
    if (currentSelection)
    {
        SelectTreeItem(currentSelection);
    }

    // Update visualization panels
    m_squarePanel->Refresh();
    m_circlePanel->Refresh();
}

void NetworkFrame::AddCellsToTree(const TCellsMap &cells, wxTreeItemId parent)
{
    for (const auto &pair : cells)
    {
        wxString itemText = wxString::Format("%s (%s)", pair.second->Name(), pair.second->ClassKey());
        wxTreeItemId item = m_networkTree->AppendItem(parent, itemText);
        m_networkTree->SetItemData(item, new ObjectTreeItemData(pair.second.get()));
    }
}

void NetworkFrame::AddSynapsesToTree(const TSynapsesMap &synapses, wxTreeItemId parent)
{
    for (const auto &pair : synapses)
    {
        wxString itemText = wxString::Format("%s (%s)", pair.second->Name(), pair.second->ClassKey());
        wxTreeItemId item = m_networkTree->AppendItem(parent, itemText);
        m_networkTree->SetItemData(item, new ObjectTreeItemData(pair.second.get()));
    }
}

void NetworkFrame::AddElectrodesToTree(const TElectrodesMap &electrodes, wxTreeItemId parent)
{
    for (const auto &pair : electrodes)
    {
        wxString itemText = wxString::Format("%s (%s)", pair.second->Name(), pair.second->ClassKey());
        wxTreeItemId item = m_networkTree->AppendItem(parent, itemText);
        m_networkTree->SetItemData(item, new ObjectTreeItemData(pair.second.get()));
    }
}

void NetworkFrame::ShowEditPanel(TRTBase *obj)
{
    ClearEditPanel();
    if (obj)
    {
        wxPanel *editPanel = obj->CreateEditPanel(m_editScrolled);
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(editPanel, 1, wxEXPAND | wxALL, 5);
        m_editScrolled->SetSizer(sizer);
        m_editScrolled->FitInside();
        m_editScrolled->Refresh();
    }
}

void NetworkFrame::OnAddButton(wxCommandEvent &event)
{
    wxTreeItemId selection = m_networkTree->GetSelection();
    if (!selection.IsOk())
        return;

    wxString itemText = m_networkTree->GetItemText(selection);

    if (itemText == "Cells")
    {
        OnCreateCell(event);
    }
    else if (itemText == "Synapses")
    {
        OnCreateSynapse(event);
    }
    else if (itemText == "Electrodes")
    {
        OnCreateElectrode(event);
    }
}

void NetworkFrame::OnRemoveButton(wxCommandEvent &event)
{
    wxTreeItemId selection = m_networkTree->GetSelection();
    if (!selection.IsOk())
        return;

    ObjectTreeItemData *data = dynamic_cast<ObjectTreeItemData *>(m_networkTree->GetItemData(selection));
    if (data && data->GetObject())
    {
        OnRemoveItem(event);
    }
}

void NetworkFrame::OnVisualizationSelection(wxCommandEvent &event)
{
    TRTBase *obj = static_cast<TRTBase *>(event.GetClientData());
    if (obj)
    {
        if (event.GetString() == "position_changed")
        {
            // Just refresh edit panel, don't change selection
            ShowEditPanel(obj);
        }
        else
        {
            // Normal selection change
            SelectTreeItem(obj);
            ShowEditPanel(obj);
        }
    }
}

void NetworkFrame::SelectTreeItem(TRTBase *obj)
{
    if (!obj)
        return;

    wxTreeItemId root = m_networkTree->GetRootItem();
    wxTreeItemIdValue cookie;
    wxTreeItemId child = m_networkTree->GetFirstChild(root, cookie);

    while (child.IsOk())
    {
        wxTreeItemIdValue childCookie;
        wxTreeItemId grandChild = m_networkTree->GetFirstChild(child, childCookie);

        while (grandChild.IsOk())
        {
            ObjectTreeItemData *data = dynamic_cast<ObjectTreeItemData *>(m_networkTree->GetItemData(grandChild));
            if (data && data->GetObject() == obj)
            {
                m_networkTree->SelectItem(grandChild);
                return;
            }
            grandChild = m_networkTree->GetNextChild(child, childCookie);
        }
        child = m_networkTree->GetNextChild(root, cookie);
    }
}

void NetworkFrame::ClearEditPanel()
{
    // Clear all children from edit panel
    m_editScrolled->DestroyChildren();
    m_editScrolled->Refresh();
}