#include "CopyCurrentsDialog.h"

wxBEGIN_EVENT_TABLE(CopyCurrentsDialog, wxDialog)
    EVT_BUTTON(wxID_OK, CopyCurrentsDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, CopyCurrentsDialog::OnCancel)
wxEND_EVENT_TABLE()

CopyCurrentsDialog::CopyCurrentsDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Copy Currents", wxDefaultPosition, wxSize(375, 229))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Left panel
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(new wxStaticText(this, wxID_ANY, "Copy Currents from:"), 0, wxALL, 5);
    m_copyFromCombo = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxSize(145, -1));
    leftSizer->Add(m_copyFromCombo, 0, wxALL | wxEXPAND, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(new wxButton(this, wxID_OK, "Copy"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
    leftSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 10);
    
    // Right panel
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(new wxStaticText(this, wxID_ANY, "Copy Currents To Target(s)"), 0, wxALL, 5);
    m_copyToList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(189, 167), 0, nullptr, wxLB_MULTIPLE);
    rightSizer->Add(m_copyToList, 1, wxALL | wxEXPAND, 5);
    
    m_clearCheckBox = new wxCheckBox(this, wxID_ANY, "Clear target currents before copying");
    rightSizer->Add(m_clearCheckBox, 0, wxALL, 5);
    
    mainSizer->Add(leftSizer, 0, wxALL, 10);
    mainSizer->Add(rightSizer, 1, wxALL | wxEXPAND, 10);
    
    SetSizer(mainSizer);
}

void CopyCurrentsDialog::SetCopyFromItems(const wxArrayString& items)
{
    m_copyFromCombo->Clear();
    m_copyFromCombo->Append(items);
}

void CopyCurrentsDialog::SetCopyToItems(const wxArrayString& items)
{
    m_copyToList->Clear();
    m_copyToList->Append(items);
}

wxString CopyCurrentsDialog::GetCopyFromSelection() const
{
    return m_copyFromCombo->GetStringSelection();
}

wxArrayInt CopyCurrentsDialog::GetCopyToSelections() const
{
    wxArrayInt selections;
    m_copyToList->GetSelections(selections);
    return selections;
}

bool CopyCurrentsDialog::GetClearBeforeCopy() const
{
    return m_clearCheckBox->GetValue();
}

void CopyCurrentsDialog::OnOK(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void CopyCurrentsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}