#include "CreateItemDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

CreateItemDialog::CreateItemDialog(wxWindow* parent, const wxString& title, const wxArrayString& types)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(300, 150))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Type selection
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Type:"), 0, wxALL, 5);
    m_typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, types);
    if (types.GetCount() > 0) m_typeChoice->SetSelection(0);
    mainSizer->Add(m_typeChoice, 0, wxEXPAND | wxALL, 5);
    
    // Name input
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Name:"), 0, wxALL, 5);
    m_nameText = new wxTextCtrl(this, wxID_ANY, "Item1");
    mainSizer->Add(m_nameText, 0, wxEXPAND | wxALL, 5);
    
    // Buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
    m_nameText->SetFocus();
}