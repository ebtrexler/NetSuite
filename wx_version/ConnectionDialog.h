#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <wx/wx.h>
#include <wx/choice.h>

class TNetwork;

class SynapseConnectionDialog : public wxDialog
{
public:
    SynapseConnectionDialog(wxWindow* parent, TNetwork* network);
    
    wxString GetPreCell() const { return m_preChoice->GetStringSelection(); }
    wxString GetPostCell() const { return m_postChoice->GetStringSelection(); }

private:
    wxChoice* m_preChoice;
    wxChoice* m_postChoice;
};

class ElectrodeConnectionDialog : public wxDialog
{
public:
    ElectrodeConnectionDialog(wxWindow* parent, TNetwork* network);
    
    wxString GetOwnerCell() const { return m_ownerChoice->GetStringSelection(); }

private:
    wxChoice* m_ownerChoice;
};

#endif