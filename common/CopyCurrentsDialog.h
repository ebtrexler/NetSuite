#ifndef COPYCURRENTSDIALOG_H
#define COPYCURRENTSDIALOG_H

#include <wx/wx.h>
#include <wx/listbox.h>

class CopyCurrentsDialog : public wxDialog
{
public:
    CopyCurrentsDialog(wxWindow* parent);
    
    void SetCopyFromItems(const wxArrayString& items);
    void SetCopyToItems(const wxArrayString& items);
    
    wxString GetCopyFromSelection() const;
    wxArrayInt GetCopyToSelections() const;
    bool GetClearBeforeCopy() const;

private:
    wxComboBox* m_copyFromCombo;
    wxListBox* m_copyToList;
    wxCheckBox* m_clearCheckBox;
    
    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};

#endif // COPYCURRENTSDIALOG_H