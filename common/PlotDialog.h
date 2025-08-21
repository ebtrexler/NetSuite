#pragma once
#include <wx/wx.h>
#include <vector>

class PlotPanel : public wxPanel
{
public:
    PlotPanel(wxWindow* parent);
    void SetData(const std::vector<std::vector<double>>& data, double sampleTime);
    
private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    
    std::vector<std::vector<double>> m_data;
    std::vector<wxString> m_labels;
    double m_sampleTime;
    
    wxDECLARE_EVENT_TABLE();
};

class PlotDialog : public wxDialog
{
public:
    PlotDialog(wxWindow* parent, const std::vector<std::vector<double>>& data, 
               const std::vector<wxString>& labels, double sampleTime);
    
private:
    PlotPanel* m_plotPanel;
};