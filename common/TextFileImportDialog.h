#ifndef TEXTFILEIMPORTDIALOG_H
#define TEXTFILEIMPORTDIALOG_H

#include <wx/wx.h>

class TextFileImportDialog : public wxDialog
{
public:
    TextFileImportDialog(wxWindow* parent);
    
    bool GetFirstColumnTime() const;
    bool GetFirstRowHeader() const;
    double GetSampleRate() const;

private:
    wxCheckBox* m_firstColumnTimeCheck;
    wxCheckBox* m_firstRowHeaderCheck;
    wxTextCtrl* m_sampleRateEdit;
    
    void OnFirstColumnTimeCheck(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};

#endif // TEXTFILEIMPORTDIALOG_H