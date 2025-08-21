#include "RT_Synapse_wx.h"
#include "RT_Cell_wx.h"
#include "RT_Current_wx.h"
#include <algorithm>
#include <sstream>
#include "json.hpp"
using json = nlohmann::json;
#include <wx/collpane.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>

wxPanel* TSynapse::CreateEditPanel(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Basic Properties
    wxStaticBoxSizer* basicSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Basic Properties");
    
    wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
    nameSizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    nameSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString(Name())), 1, wxALL, 5);
    basicSizer->Add(nameSizer, 0, wxEXPAND);
    
    wxCheckBox* activeBox = new wxCheckBox(panel, wxID_ANY, "Active");
    activeBox->SetValue(IsActive());
    activeBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& e) { SetActive(e.IsChecked()); });
    basicSizer->Add(activeBox, 0, wxALL, 5);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Connection Properties
    wxStaticBoxSizer* connSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Connections");
    
    wxString preCell = FPre ? wxString(FPre->Name()) : "None";
    wxString postCell = FPost ? wxString(FPost->Name()) : "None";
    
    connSizer->Add(new wxStaticText(panel, wxID_ANY, "Pre-synaptic: " + preCell), 0, wxALL, 5);
    connSizer->Add(new wxStaticText(panel, wxID_ANY, "Post-synaptic: " + postCell), 0, wxALL, 5);
    
    mainSizer->Add(connSizer, 0, wxEXPAND | wxALL, 5);
    
    // Synaptic Properties
    wxStaticBoxSizer* synSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Synaptic Properties");
    
    wxBoxSizer* condSizer = new wxBoxSizer(wxHORIZONTAL);
    condSizer->Add(new wxStaticText(panel, wxID_ANY, "Conductance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* condCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.2f", m_conductance));
    condCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_conductance = val; });
    condSizer->Add(condCtrl, 1, wxALL, 5);
    condSizer->Add(new wxStaticText(panel, wxID_ANY, "nS"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    synSizer->Add(condSizer, 0, wxEXPAND);
    
    wxBoxSizer* revSizer = new wxBoxSizer(wxHORIZONTAL);
    revSizer->Add(new wxStaticText(panel, wxID_ANY, "Reversal Potential:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* revCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_reversalPotential));
    revCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_reversalPotential = val; });
    revSizer->Add(revCtrl, 1, wxALL, 5);
    revSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    synSizer->Add(revSizer, 0, wxEXPAND);
    
    wxBoxSizer* delaySizer = new wxBoxSizer(wxHORIZONTAL);
    delaySizer->Add(new wxStaticText(panel, wxID_ANY, "Delay:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* delayCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_delay));
    delayCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_delay = val; });
    delaySizer->Add(delayCtrl, 1, wxALL, 5);
    delaySizer->Add(new wxStaticText(panel, wxID_ANY, "ms"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    synSizer->Add(delaySizer, 0, wxEXPAND);
    
    wxBoxSizer* weightSizer = new wxBoxSizer(wxHORIZONTAL);
    weightSizer->Add(new wxStaticText(panel, wxID_ANY, "Weight:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* weightCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.2f", m_weight));
    weightCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_weight = val; });
    weightSizer->Add(weightCtrl, 1, wxALL, 5);
    synSizer->Add(weightSizer, 0, wxEXPAND);
    
    mainSizer->Add(synSizer, 0, wxEXPAND | wxALL, 5);
    
    // Currents section
    wxStaticBoxSizer* currentsSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Currents");
    
    for (const auto& current : m_currents) {
        wxCollapsiblePane* currentPane = new wxCollapsiblePane(panel, wxID_ANY, 
            wxString::Format("%s (%s)", current->Name(), current->ClassKey()));
        currentPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, [panel](wxCollapsiblePaneEvent&) {
            panel->Layout();
            wxWindow* scrolled = panel->GetParent();
            scrolled->FitInside();
            scrolled->Layout();
        });
        wxPanel* currentPanel = current->CreateEditPanel(currentPane->GetPane());
        wxBoxSizer* paneSizer = new wxBoxSizer(wxVERTICAL);
        paneSizer->Add(currentPanel, 1, wxEXPAND);
        currentPane->GetPane()->SetSizer(paneSizer);
        currentsSizer->Add(currentPane, 0, wxEXPAND | wxALL, 2);
    }
    
    if (m_currents.empty()) {
        currentsSizer->Add(new wxStaticText(panel, wxID_ANY, "No currents"), 0, wxALL, 5);
    }
    
    mainSizer->Add(currentsSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}

void TSynapse::AddCurrent(TCurrentPtr current)
{
    current->SetOwner(this);
    m_currents.push_back(current);
}

void TSynapse::RemoveCurrent(TCurrent* current)
{
    m_currents.erase(
        std::remove_if(m_currents.begin(), m_currents.end(),
            [current](const TCurrentPtr& ptr) { return ptr.get() == current; }),
        m_currents.end());
}

std::string TSynapse::SerializeParameters() const
{
    std::ostringstream json;
    json << "{";
    json << "\"conductance\":" << m_conductance << ",";
    json << "\"reversalPotential\":" << m_reversalPotential << ",";
    json << "\"delay\":" << m_delay << ",";
    json << "\"weight\":" << m_weight;
    json << "}";
    return json.str();
}

void TSynapse::DeserializeParameters(const std::string& jsonStr)
{
    try {
        json params = json::parse(jsonStr);
        
        if (params.contains("conductance")) {
            m_conductance = params["conductance"];
        }
        if (params.contains("reversalPotential")) {
            m_reversalPotential = params["reversalPotential"];
        }
        if (params.contains("delay")) {
            m_delay = params["delay"];
        }
        if (params.contains("weight")) {
            m_weight = params["weight"];
        }
    } catch (...) {
        // Ignore parsing errors, keep defaults
    }
}