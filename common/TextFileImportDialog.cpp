#include "TextFileImportDialog.h"

wxBEGIN_EVENT_TABLE(TextFileImportDialog, wxDialog)
    EVT_CHECKBOX(wxID_ANY, TextFileImportDialog::OnFirstColumnTimeCheck)
    EVT_BUTTON(wxID_OK, TextFileImportDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, TextFileImportDialog::OnCancel)
wxEND_EVENT_TABLE()

TextFileImportDialog::TextFileImportDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Text File Import Options", wxDefaultPosition, wxSize(354, 120))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // File description group
    wxStaticBoxSizer* groupSizer = new wxStaticBoxSizer(wxVERTICAL, this, "File Description");
    
    m_firstColumnTimeCheck = new wxCheckBox(this, wxID_ANY, "First Column Time");
    groupSizer->Add(m_firstColumnTimeCheck, 0, wxALL, 5);
    
    m_firstRowHeaderCheck = new wxCheckBox(this, wxID_ANY, "First Row Header");
    groupSizer->Add(m_firstRowHeaderCheck, 0, wxALL, 5);
    
    wxBoxSizer* rateSizer = new wxBoxSizer(wxHORIZONTAL);
    rateSizer->Add(new wxStaticText(this, wxID_ANY, "Sample Rate (ms)"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_sampleRateEdit = new wxTextCtrl(this, wxID_ANY, "1", wxDefaultPosition, wxSize(77, -1), wxTE_RIGHT);
    rateSizer->Add(m_sampleRateEdit, 0, wxALL, 5);
    groupSizer->Add(rateSizer, 0, wxALL, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(new wxButton(this, wxID_OK, "Load File"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
    
    mainSizer->Add(groupSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(buttonSizer, 0, wxALL, 5);
    
    SetSizer(mainSizer);
}

bool TextFileImportDialog::GetFirstColumnTime() const
{
    return m_firstColumnTimeCheck->GetValue();
}

bool TextFileImportDialog::GetFirstRowHeader() const
{
    return m_firstRowHeaderCheck->GetValue();
}

double TextFileImportDialog::GetSampleRate() const
{
    double value;
    if (m_sampleRateEdit->GetValue().ToDouble(&value))
        return value;
    return 1.0;
}

void TextFileImportDialog::OnFirstColumnTimeCheck(wxCommandEvent& event)
{
    // Handle first column time checkbox logic if needed
}

void TextFileImportDialog::OnOK(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void TextFileImportDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}