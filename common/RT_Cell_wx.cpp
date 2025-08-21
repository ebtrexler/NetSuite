#include "RT_Cell_wx.h"
#include "RT_Current_wx.h"
#include <algorithm>
#include <sstream>
#include "json.hpp"
#include <wx/collpane.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
using json = nlohmann::json;

wxPanel* TCell::CreateEditPanel(wxWindow* parent)
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
    
    wxBoxSizer* posSizer = new wxBoxSizer(wxHORIZONTAL);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "X:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, wxString::Format("%d", GetX())), 0, wxALL, 5);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "Y:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, wxString::Format("%d", GetY())), 0, wxALL, 5);
    basicSizer->Add(posSizer, 0, wxEXPAND);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Membrane Properties
    wxStaticBoxSizer* membraneSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Membrane Properties");
    
    wxBoxSizer* capSizer = new wxBoxSizer(wxHORIZONTAL);
    capSizer->Add(new wxStaticText(panel, wxID_ANY, "Capacitance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* capCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_capacitance));
    capCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_capacitance = val; });
    capSizer->Add(capCtrl, 1, wxALL, 5);
    capSizer->Add(new wxStaticText(panel, wxID_ANY, "pF"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(capSizer, 0, wxEXPAND);
    
    wxBoxSizer* resSizer = new wxBoxSizer(wxHORIZONTAL);
    resSizer->Add(new wxStaticText(panel, wxID_ANY, "Resistance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* resCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_resistance));
    resCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_resistance = val; });
    resSizer->Add(resCtrl, 1, wxALL, 5);
    resSizer->Add(new wxStaticText(panel, wxID_ANY, "MΩ"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(resSizer, 0, wxEXPAND);
    
    wxBoxSizer* restSizer = new wxBoxSizer(wxHORIZONTAL);
    restSizer->Add(new wxStaticText(panel, wxID_ANY, "Resting Potential:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* restCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_restingPotential));
    restCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_restingPotential = val; });
    restSizer->Add(restCtrl, 1, wxALL, 5);
    restSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(restSizer, 0, wxEXPAND);
    
    wxBoxSizer* threshSizer = new wxBoxSizer(wxHORIZONTAL);
    threshSizer->Add(new wxStaticText(panel, wxID_ANY, "Threshold:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* threshCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_threshold));
    threshCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) m_threshold = val; });
    threshSizer->Add(threshCtrl, 1, wxALL, 5);
    threshSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(threshSizer, 0, wxEXPAND);
    
    mainSizer->Add(membraneSizer, 0, wxEXPAND | wxALL, 5);
    
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

void TCell::AddCurrent(TCurrentPtr current)
{
    current->SetOwner(this);
    m_currents.push_back(current);
}

void TCell::RemoveCurrent(TCurrent* current)
{
    m_currents.erase(
        std::remove_if(m_currents.begin(), m_currents.end(),
            [current](const TCurrentPtr& ptr) { return ptr.get() == current; }),
        m_currents.end());
}

std::string TCell::SerializeParameters() const
{
    std::ostringstream json;
    json << "{";
    json << "\"capacitance\":" << m_capacitance << ",";
    json << "\"restingPotential\":" << m_restingPotential << ",";
    json << "\"threshold\":" << m_threshold << ",";
    json << "\"resistance\":" << m_resistance;
    json << "}";
    return json.str();
}

void TCell::DeserializeParameters(const std::string& jsonStr)
{
    try {
        json params = json::parse(jsonStr);
        
        if (params.contains("capacitance")) {
            m_capacitance = params["capacitance"];
        }
        if (params.contains("restingPotential")) {
            m_restingPotential = params["restingPotential"];
        }
        if (params.contains("threshold")) {
            m_threshold = params["threshold"];
        }
        if (params.contains("resistance")) {
            m_resistance = params["resistance"];
        }
    } catch (...) {
        // Ignore parsing errors, keep defaults
    }
}