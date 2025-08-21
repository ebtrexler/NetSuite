#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/progdlg.h>

class RunModelDialog : public wxDialog
{
public:
    RunModelDialog(wxWindow* parent);
    
private:
    wxSpinCtrlDouble* m_sampleRateCtrl;
    wxSpinCtrlDouble* m_durationCtrl;
    wxSpinCtrl* m_repeatsCtrl;
    wxButton* m_runButton;
    wxButton* m_stopButton;
    
    bool m_running;
    
    void OnRun(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    
    void RunModel();
    
    wxDECLARE_EVENT_TABLE();
};