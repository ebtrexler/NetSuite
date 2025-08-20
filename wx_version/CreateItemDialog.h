#ifndef CREATEITEMDIALOG_H
#define CREATEITEMDIALOG_H

#include <wx/wx.h>
#include <wx/choice.h>

class CreateItemDialog : public wxDialog
{
public:
    CreateItemDialog(wxWindow* parent, const wxString& title, const wxArrayString& types);
    
    wxString GetSelectedType() const { return m_typeChoice->GetStringSelection(); }
    wxString GetItemName() const { return m_nameText->GetValue(); }

private:
    wxChoice* m_typeChoice;
    wxTextCtrl* m_nameText;
};

#endif