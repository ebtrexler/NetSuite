#ifndef NETWORKFRAME_H
#define NETWORKFRAME_H

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/filedlg.h>

#include "RT_Network_wx.h"

class NetworkVisualizationPanel;

enum
{
    ID_CreateCell = 1000,
    ID_CreateSynapse,
    ID_CreateElectrode,
    ID_AddCurrent,
    ID_RemoveItem,
    ID_RunModel,
    ID_ClearNetwork,
    ID_CopyCurrents,
    ID_AddButton,
    ID_RemoveButton
};

class NetworkFrame : public wxFrame
{
public:
    NetworkFrame(const wxString& title);

private:
    // Event handlers
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnCreateCell(wxCommandEvent& event);
    void OnCreateSynapse(wxCommandEvent& event);
    void OnCreateElectrode(wxCommandEvent& event);
    void OnAddCurrent(wxCommandEvent& event);
    void OnRemoveItem(wxCommandEvent& event);
    void OnRunModel(wxCommandEvent& event);
    void OnClearNetwork(wxCommandEvent& event);
    void OnTreeSelChanged(wxTreeEvent& event);
    void OnTreeItemActivated(wxTreeEvent& event);
    void OnAddButton(wxCommandEvent& event);
    void OnRemoveButton(wxCommandEvent& event);
    void OnVisualizationSelection(wxCommandEvent& event);
    void SelectTreeItem(TRTBase* obj);

    // GUI components
    wxMenuBar* m_menuBar;
    wxToolBar* m_toolBar;
    wxSplitterWindow* m_mainSplitter;
    wxSplitterWindow* m_leftSplitter;
    wxNotebook* m_networkNotebook;
    wxTreeCtrl* m_networkTree;
    wxPanel* m_treePanel;
    wxButton* m_addButton;
    wxButton* m_removeButton;
    wxPanel* m_editPanel;
    wxScrolledWindow* m_editScrolled;
    NetworkVisualizationPanel* m_squarePanel;
    NetworkVisualizationPanel* m_circlePanel;

    // Network management
    void UpdateNetworkTree();
    void AddCellsToTree(const TCellsMap& cells, wxTreeItemId parent);
    void AddSynapsesToTree(const TSynapsesMap& synapses, wxTreeItemId parent);
    void AddElectrodesToTree(const TElectrodesMap& electrodes, wxTreeItemId parent);
    void ClearEditPanel();
    void ShowEditPanel(TRTBase* obj);

    wxDECLARE_EVENT_TABLE();
};

#endif // NETWORKFRAME_H