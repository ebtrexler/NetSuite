#include "ConnectionDialog.h"
#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

SynapseConnectionDialog::SynapseConnectionDialog(wxWindow* parent, TNetwork* network)
    : wxDialog(parent, wxID_ANY, "Connect Synapse", wxDefaultPosition, wxSize(300, 200))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Pre-synaptic cell
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Pre-synaptic Cell:"), 0, wxALL, 5);
    m_preChoice = new wxChoice(this, wxID_ANY);
    mainSizer->Add(m_preChoice, 0, wxEXPAND | wxALL, 5);
    
    // Post-synaptic cell
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Post-synaptic Cell:"), 0, wxALL, 5);
    m_postChoice = new wxChoice(this, wxID_ANY);
    mainSizer->Add(m_postChoice, 0, wxEXPAND | wxALL, 5);
    
    // Populate with available cells
    for (const auto& pair : network->GetCells()) {
        wxString cellName(pair.second->Name());
        m_preChoice->Append(cellName);
        m_postChoice->Append(cellName);
    }
    
    // Buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}

ElectrodeConnectionDialog::ElectrodeConnectionDialog(wxWindow* parent, TNetwork* network)
    : wxDialog(parent, wxID_ANY, "Connect Electrode", wxDefaultPosition, wxSize(300, 150))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Owner cell
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Owner Cell:"), 0, wxALL, 5);
    m_ownerChoice = new wxChoice(this, wxID_ANY);
    mainSizer->Add(m_ownerChoice, 0, wxEXPAND | wxALL, 5);
    
    // Populate with available cells
    for (const auto& pair : network->GetCells()) {
        wxString cellName(pair.second->Name());
        m_ownerChoice->Append(cellName);
    }
    
    // Buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}