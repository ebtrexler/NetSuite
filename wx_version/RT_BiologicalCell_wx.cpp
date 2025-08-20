#include "RT_BiologicalCell_wx.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/choice.h>

wxPanel* TBiologicalCell::CreateEditPanel(wxWindow* parent)
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
    
    wxBoxSizer* posSizer = new wxBoxSizer(wxHORIZONTAL);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "X:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", GetX())), 0, wxALL, 5);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "Y:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", GetY())), 0, wxALL, 5);
    basicSizer->Add(posSizer, 0, wxEXPAND);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // DAQ Properties
    wxStaticBoxSizer* daqSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Data Acquisition");
    
    wxBoxSizer* aiChannelSizer = new wxBoxSizer(wxHORIZONTAL);
    aiChannelSizer->Add(new wxStaticText(panel, wxID_ANY, "AI Channel:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    aiChannelSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString(m_aiChannel)), 1, wxALL, 5);
    daqSizer->Add(aiChannelSizer, 0, wxEXPAND);
    
    wxBoxSizer* aoChannelSizer = new wxBoxSizer(wxHORIZONTAL);
    aoChannelSizer->Add(new wxStaticText(panel, wxID_ANY, "AO Channel:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    aoChannelSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString(m_aoChannel)), 1, wxALL, 5);
    daqSizer->Add(aoChannelSizer, 0, wxEXPAND);
    
    wxBoxSizer* aiGainSizer = new wxBoxSizer(wxHORIZONTAL);
    aiGainSizer->Add(new wxStaticText(panel, wxID_ANY, "AI Gain:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    aiGainSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_aiGain)), 1, wxALL, 5);
    aiGainSizer->Add(new wxStaticText(panel, wxID_ANY, "mV/V"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    daqSizer->Add(aiGainSizer, 0, wxEXPAND);
    
    wxBoxSizer* aoGainSizer = new wxBoxSizer(wxHORIZONTAL);
    aoGainSizer->Add(new wxStaticText(panel, wxID_ANY, "AO Gain:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    aoGainSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.3f", m_aoGain)), 1, wxALL, 5);
    aoGainSizer->Add(new wxStaticText(panel, wxID_ANY, "V/nA"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    daqSizer->Add(aoGainSizer, 0, wxEXPAND);
    
    mainSizer->Add(daqSizer, 0, wxEXPAND | wxALL, 5);
    
    // Current Limits
    wxStaticBoxSizer* limitSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Current Limits");
    
    wxBoxSizer* posLimitSizer = new wxBoxSizer(wxHORIZONTAL);
    posLimitSizer->Add(new wxStaticText(panel, wxID_ANY, "Positive Limit:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posLimitSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_posCurrentLimit)), 1, wxALL, 5);
    posLimitSizer->Add(new wxStaticText(panel, wxID_ANY, "nA"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    limitSizer->Add(posLimitSizer, 0, wxEXPAND);
    
    wxBoxSizer* negLimitSizer = new wxBoxSizer(wxHORIZONTAL);
    negLimitSizer->Add(new wxStaticText(panel, wxID_ANY, "Negative Limit:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    negLimitSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_negCurrentLimit)), 1, wxALL, 5);
    negLimitSizer->Add(new wxStaticText(panel, wxID_ANY, "nA"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    limitSizer->Add(negLimitSizer, 0, wxEXPAND);
    
    mainSizer->Add(limitSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}