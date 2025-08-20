#include "RT_Cell_wx.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>

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
    basicSizer->Add(activeBox, 0, wxALL, 5);
    
    wxBoxSizer* posSizer = new wxBoxSizer(wxHORIZONTAL);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "X:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", GetX())), 0, wxALL, 5);
    posSizer->Add(new wxStaticText(panel, wxID_ANY, "Y:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    posSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", GetY())), 0, wxALL, 5);
    basicSizer->Add(posSizer, 0, wxEXPAND);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Membrane Properties
    wxStaticBoxSizer* membraneSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Membrane Properties");
    
    wxBoxSizer* capSizer = new wxBoxSizer(wxHORIZONTAL);
    capSizer->Add(new wxStaticText(panel, wxID_ANY, "Capacitance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    capSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_capacitance)), 1, wxALL, 5);
    capSizer->Add(new wxStaticText(panel, wxID_ANY, "pF"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(capSizer, 0, wxEXPAND);
    
    wxBoxSizer* resSizer = new wxBoxSizer(wxHORIZONTAL);
    resSizer->Add(new wxStaticText(panel, wxID_ANY, "Resistance:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    resSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_resistance)), 1, wxALL, 5);
    resSizer->Add(new wxStaticText(panel, wxID_ANY, "MΩ"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(resSizer, 0, wxEXPAND);
    
    wxBoxSizer* restSizer = new wxBoxSizer(wxHORIZONTAL);
    restSizer->Add(new wxStaticText(panel, wxID_ANY, "Resting Potential:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    restSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_restingPotential)), 1, wxALL, 5);
    restSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(restSizer, 0, wxEXPAND);
    
    wxBoxSizer* threshSizer = new wxBoxSizer(wxHORIZONTAL);
    threshSizer->Add(new wxStaticText(panel, wxID_ANY, "Threshold:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    threshSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_threshold)), 1, wxALL, 5);
    threshSizer->Add(new wxStaticText(panel, wxID_ANY, "mV"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    membraneSizer->Add(threshSizer, 0, wxEXPAND);
    
    mainSizer->Add(membraneSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}