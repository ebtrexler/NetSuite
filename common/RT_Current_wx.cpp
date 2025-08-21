#include "RT_Current_wx.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <sstream>
#include "json.hpp"
using json = nlohmann::json;

wxPanel* TCurrent::CreateEditPanel(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Active checkbox
    wxCheckBox* activeBox = new wxCheckBox(panel, wxID_ANY, "Active");
    activeBox->SetValue(IsActive());
    activeBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& e) { SetActive(e.IsChecked()); });
    mainSizer->Add(activeBox, 0, wxALL, 5);
    
    // Conductance
    wxBoxSizer* condSizer = new wxBoxSizer(wxHORIZONTAL);
    condSizer->Add(new wxStaticText(panel, wxID_ANY, "Conductance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* condCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.2f", m_conductance));
    condCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetConductance(val); });
    condSizer->Add(condCtrl, 1, wxALL, 5);
    condSizer->Add(new wxStaticText(panel, wxID_ANY, "nS"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(condSizer, 0, wxEXPAND);
    
    // Reversal Potential
    wxBoxSizer* revSizer = new wxBoxSizer(wxHORIZONTAL);
    revSizer->Add(new wxStaticText(panel, wxID_ANY, "Reversal Potential:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* revCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_reversalPotential));
    revCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetReversalPotential(val); });
    revSizer->Add(revCtrl, 1, wxALL, 5);
    revSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(revSizer, 0, wxEXPAND);
    
    // Periodicity
    wxCheckBox* periodicBox = new wxCheckBox(panel, wxID_ANY, "Periodic");
    periodicBox->SetValue(m_isPeriodic);
    periodicBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& e) { SetPeriodic(e.IsChecked()); });
    mainSizer->Add(periodicBox, 0, wxALL, 5);
    
    wxBoxSizer* periodSizer = new wxBoxSizer(wxHORIZONTAL);
    periodSizer->Add(new wxStaticText(panel, wxID_ANY, "Period:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* periodCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_period));
    periodCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetPeriod(val); });
    periodSizer->Add(periodCtrl, 1, wxALL, 5);
    periodSizer->Add(new wxStaticText(panel, wxID_ANY, "ms"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(periodSizer, 0, wxEXPAND);
    
    panel->SetSizer(mainSizer);
    return panel;
}

wxPanel* THHCurrent::CreateEditPanel(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Active checkbox
    wxCheckBox* activeBox = new wxCheckBox(panel, wxID_ANY, "Active");
    activeBox->SetValue(IsActive());
    activeBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& e) { SetActive(e.IsChecked()); });
    mainSizer->Add(activeBox, 0, wxALL, 5);
    
    // gNa
    wxBoxSizer* gNaSizer = new wxBoxSizer(wxHORIZONTAL);
    gNaSizer->Add(new wxStaticText(panel, wxID_ANY, "gNa:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* gNaCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_gNa));
    gNaCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetGNa(val); });
    gNaSizer->Add(gNaCtrl, 1, wxALL, 5);
    gNaSizer->Add(new wxStaticText(panel, wxID_ANY, "mS/cm²"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(gNaSizer, 0, wxEXPAND);
    
    // gK
    wxBoxSizer* gKSizer = new wxBoxSizer(wxHORIZONTAL);
    gKSizer->Add(new wxStaticText(panel, wxID_ANY, "gK:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* gKCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_gK));
    gKCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetGK(val); });
    gKSizer->Add(gKCtrl, 1, wxALL, 5);
    gKSizer->Add(new wxStaticText(panel, wxID_ANY, "mS/cm²"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(gKSizer, 0, wxEXPAND);
    
    // gL
    wxBoxSizer* gLSizer = new wxBoxSizer(wxHORIZONTAL);
    gLSizer->Add(new wxStaticText(panel, wxID_ANY, "gL:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxTextCtrl* gLCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.2f", m_gL));
    gLCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& e) { 
        double val; if (e.GetString().ToDouble(&val)) SetGL(val); });
    gLSizer->Add(gLCtrl, 1, wxALL, 5);
    gLSizer->Add(new wxStaticText(panel, wxID_ANY, "mS/cm²"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(gLSizer, 0, wxEXPAND);
    
    panel->SetSizer(mainSizer);
    return panel;
}

std::string TCurrent::SerializeParameters() const
{
    json j;
    j["conductance"] = m_conductance;
    j["reversal"] = m_reversalPotential;
    j["periodic"] = m_isPeriodic;
    j["period"] = m_period;
    j["active"] = IsActive();
    return j.dump();
}

void TCurrent::DeserializeParameters(const std::string& params)
{
    try {
        json j = json::parse(params);
        if (j.contains("conductance")) m_conductance = j["conductance"];
        if (j.contains("reversal")) m_reversalPotential = j["reversal"];
        if (j.contains("periodic")) m_isPeriodic = j["periodic"];
        if (j.contains("period")) m_period = j["period"];
        if (j.contains("active")) SetActive(j["active"]);
    } catch (const std::exception&) {
        // Fallback to old format if JSON parsing fails
        size_t pos = 0;
        while (pos < params.length()) {
            size_t colonPos = params.find(':', pos);
            size_t semicolonPos = params.find(';', colonPos);
            if (colonPos == std::string::npos) break;
            
            std::string key = params.substr(pos, colonPos - pos);
            std::string value = params.substr(colonPos + 1, semicolonPos - colonPos - 1);
            
            if (key == "conductance") m_conductance = std::stod(value);
            else if (key == "reversal") m_reversalPotential = std::stod(value);
            else if (key == "periodic") m_isPeriodic = (std::stoi(value) == 1);
            else if (key == "period") m_period = std::stod(value);
            else if (key == "active") SetActive(std::stoi(value) == 1);
            
            pos = semicolonPos + 1;
        }
    }
}

std::string THHCurrent::SerializeParameters() const
{
    json j = json::parse(TCurrent::SerializeParameters());
    j["gNa"] = m_gNa;
    j["gK"] = m_gK;
    j["gL"] = m_gL;
    return j.dump();
}

void THHCurrent::DeserializeParameters(const std::string& params)
{
    try {
        json j = json::parse(params);
        TCurrent::DeserializeParameters(params);
        if (j.contains("gNa")) m_gNa = j["gNa"];
        if (j.contains("gK")) m_gK = j["gK"];
        if (j.contains("gL")) m_gL = j["gL"];
    } catch (const std::exception&) {
        // Fallback to old format
        TCurrent::DeserializeParameters(params);
        size_t pos = 0;
        while (pos < params.length()) {
            size_t colonPos = params.find(':', pos);
            size_t semicolonPos = params.find(';', colonPos);
            if (colonPos == std::string::npos) break;
            
            std::string key = params.substr(pos, colonPos - pos);
            std::string value = params.substr(colonPos + 1, semicolonPos - colonPos - 1);
            
            if (key == "gNa") m_gNa = std::stod(value);
            else if (key == "gK") m_gK = std::stod(value);
            else if (key == "gL") m_gL = std::stod(value);
            
            pos = semicolonPos + 1;
        }
    }
}

wxPanel* TPlaybackCurrent::CreateEditPanel(wxWindow* parent)
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
    basicSizer->Add(activeBox, 0, wxALL, 5);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Playback Properties
    wxStaticBoxSizer* playbackSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Playback Properties");
    
    wxBoxSizer* fileSizer = new wxBoxSizer(wxHORIZONTAL);
    fileSizer->Add(new wxStaticText(panel, wxID_ANY, "File:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    fileSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString(m_filename)), 1, wxALL, 5);
    fileSizer->Add(new wxButton(panel, wxID_ANY, "Browse"), 0, wxALL, 5);
    playbackSizer->Add(fileSizer, 0, wxEXPAND);
    
    mainSizer->Add(playbackSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}

wxPanel* TVoltageClampPIDCurrent::CreateEditPanel(wxWindow* parent)
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
    basicSizer->Add(activeBox, 0, wxALL, 5);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Voltage Clamp Properties
    wxStaticBoxSizer* clampSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Voltage Clamp Properties");
    
    wxBoxSizer* targetSizer = new wxBoxSizer(wxHORIZONTAL);
    targetSizer->Add(new wxStaticText(panel, wxID_ANY, "Target Voltage:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    targetSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_targetVoltage)), 1, wxALL, 5);
    targetSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    clampSizer->Add(targetSizer, 0, wxEXPAND);
    
    wxBoxSizer* kPSizer = new wxBoxSizer(wxHORIZONTAL);
    kPSizer->Add(new wxStaticText(panel, wxID_ANY, "Proportional Gain:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    kPSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.3f", m_kP)), 1, wxALL, 5);
    clampSizer->Add(kPSizer, 0, wxEXPAND);
    
    wxBoxSizer* kISizer = new wxBoxSizer(wxHORIZONTAL);
    kISizer->Add(new wxStaticText(panel, wxID_ANY, "Integral Gain:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    kISizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.3f", m_kI)), 1, wxALL, 5);
    clampSizer->Add(kISizer, 0, wxEXPAND);
    
    wxBoxSizer* kDSizer = new wxBoxSizer(wxHORIZONTAL);
    kDSizer->Add(new wxStaticText(panel, wxID_ANY, "Derivative Gain:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    kDSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.3f", m_kD)), 1, wxALL, 5);
    clampSizer->Add(kDSizer, 0, wxEXPAND);
    
    mainSizer->Add(clampSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}