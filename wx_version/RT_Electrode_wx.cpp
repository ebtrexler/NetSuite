#include "RT_Electrode_wx.h"
#include "RT_Cell_wx.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/choice.h>

wxPanel* TElectrode::CreateEditPanel(wxWindow* parent)
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
    
    wxString ownerCell = FOwner ? wxString(FOwner->Name()) : "None";
    basicSizer->Add(new wxStaticText(panel, wxID_ANY, "Owner Cell: " + ownerCell), 0, wxALL, 5);
    
    mainSizer->Add(basicSizer, 0, wxEXPAND | wxALL, 5);
    
    // Stimulation Properties
    wxStaticBoxSizer* stimSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Stimulation Properties");
    
    wxBoxSizer* typeSizer = new wxBoxSizer(wxHORIZONTAL);
    typeSizer->Add(new wxStaticText(panel, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    wxChoice* typeChoice = new wxChoice(panel, wxID_ANY);
    typeChoice->Append("Constant Current");
    typeChoice->Append("Pulse Train");
    typeChoice->SetSelection(m_isConstant ? 0 : 1);
    typeSizer->Add(typeChoice, 1, wxALL, 5);
    stimSizer->Add(typeSizer, 0, wxEXPAND);
    
    wxBoxSizer* ampSizer = new wxBoxSizer(wxHORIZONTAL);
    ampSizer->Add(new wxStaticText(panel, wxID_ANY, "Amplitude:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    ampSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.2f", m_amplitude)), 1, wxALL, 5);
    ampSizer->Add(new wxStaticText(panel, wxID_ANY, "nA"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    stimSizer->Add(ampSizer, 0, wxEXPAND);
    
    wxBoxSizer* durSizer = new wxBoxSizer(wxHORIZONTAL);
    durSizer->Add(new wxStaticText(panel, wxID_ANY, "Duration:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    durSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_duration)), 1, wxALL, 5);
    durSizer->Add(new wxStaticText(panel, wxID_ANY, "ms"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    stimSizer->Add(durSizer, 0, wxEXPAND);
    
    wxBoxSizer* freqSizer = new wxBoxSizer(wxHORIZONTAL);
    freqSizer->Add(new wxStaticText(panel, wxID_ANY, "Frequency:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    freqSizer->Add(new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.1f", m_frequency)), 1, wxALL, 5);
    freqSizer->Add(new wxStaticText(panel, wxID_ANY, "Hz"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    stimSizer->Add(freqSizer, 0, wxEXPAND);
    
    mainSizer->Add(stimSizer, 0, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    return panel;
}