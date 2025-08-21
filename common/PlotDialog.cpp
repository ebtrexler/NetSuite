#include "PlotDialog.h"
#include "AdvancedPlotPanel.h"
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(PlotPanel, wxPanel)
    EVT_PAINT(PlotPanel::OnPaint)
    EVT_SIZE(PlotPanel::OnSize)
wxEND_EVENT_TABLE()

PlotPanel::PlotPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_sampleTime(0.0001) // default 10kHz
{
    SetBackgroundColour(*wxWHITE);
}

void PlotPanel::SetData(const std::vector<std::vector<double>>& data, double sampleTime)
{
    m_data = data;
    m_sampleTime = sampleTime;
    Refresh();
}

void PlotPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize size = GetSize();
    
    if (m_data.empty() || size.GetWidth() < 50 || size.GetHeight() < 50) {
        dc.DrawText("No data to display", 10, 10);
        return;
    }
    
    // Draw axes
    dc.SetPen(wxPen(*wxBLACK, 1));
    int margin = 40;
    int plotWidth = size.GetWidth() - 2 * margin;
    int plotHeight = size.GetHeight() - 2 * margin;
    
    // X-axis
    dc.DrawLine(margin, size.GetHeight() - margin, size.GetWidth() - margin, size.GetHeight() - margin);
    // Y-axis  
    dc.DrawLine(margin, margin, margin, size.GetHeight() - margin);
    
    if (m_data[0].empty()) return;
    
    // Find data range
    double minY = m_data[0][0], maxY = m_data[0][0];
    for (const auto& trace : m_data) {
        for (double val : trace) {
            minY = std::min(minY, val);
            maxY = std::max(maxY, val);
        }
    }
    
    if (maxY == minY) {
        maxY += 1.0;
        minY -= 1.0;
    }
    
    // Draw data traces
    wxColour colors[] = {*wxRED, *wxBLUE, *wxGREEN, wxColour(255,0,255), *wxCYAN};
    int colorIndex = 0;
    
    for (const auto& trace : m_data) {
        if (trace.size() < 2) continue;
        
        dc.SetPen(wxPen(colors[colorIndex % 5], 2));
        
        for (size_t i = 1; i < trace.size(); i++) {
            int x1 = margin + (plotWidth * (i-1)) / (trace.size() - 1);
            int y1 = size.GetHeight() - margin - (plotHeight * (trace[i-1] - minY)) / (maxY - minY);
            int x2 = margin + (plotWidth * i) / (trace.size() - 1);
            int y2 = size.GetHeight() - margin - (plotHeight * (trace[i] - minY)) / (maxY - minY);
            
            dc.DrawLine(x1, y1, x2, y2);
        }
        colorIndex++;
    }
    
    // Draw labels
    dc.SetTextForeground(*wxBLACK);
    dc.DrawText(wxString::Format("%.1f mV", maxY), 5, margin);
    dc.DrawText(wxString::Format("%.1f mV", minY), 5, size.GetHeight() - margin - 15);
    dc.DrawText("0 ms", margin, size.GetHeight() - margin + 5);
    double totalTime = m_data[0].size() * m_sampleTime * 1000; // convert to ms
    dc.DrawText(wxString::Format("%.1f ms", totalTime), size.GetWidth() - margin - 30, size.GetHeight() - margin + 5);
}

void PlotPanel::OnSize(wxSizeEvent& event)
{
    Refresh();
    event.Skip();
}

PlotDialog::PlotDialog(wxWindow* parent, const std::vector<std::vector<double>>& data, 
                       const std::vector<wxString>& labels, double sampleTime)
    : wxDialog(parent, wxID_ANY, "Simulation Results", wxDefaultPosition, wxSize(800, 600))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Instructions
    wxStaticText* instructions = new wxStaticText(this, wxID_ANY, 
        "Mouse: Left-click for cursor, Shift+drag to zoom, Ctrl+drag to pan, Wheel to zoom, Right-click to auto-scale");
    instructions->SetFont(instructions->GetFont().Smaller());
    mainSizer->Add(instructions, 0, wxEXPAND | wxALL, 5);
    
    // Advanced plot panel
    AdvancedPlotPanel* advancedPanel = new AdvancedPlotPanel(this);
    
    // Add traces with different colors
    wxColour colors[] = {*wxRED, *wxBLUE, *wxGREEN, wxColour(255,0,255), *wxCYAN};
    for (size_t i = 0; i < data.size() && i < 5; i++) {
        wxString label = i < labels.size() ? labels[i] : wxString::Format("Trace %zu", i+1);
        advancedPanel->AddTrace(data[i], sampleTime, label, colors[i % 5]);
    }
    
    advancedPanel->AutoScale();
    mainSizer->Add(advancedPanel, 1, wxEXPAND | wxALL, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* closeBtn = new wxButton(this, wxID_OK, "Close");
    closeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { EndModal(wxID_OK); });
    buttonSizer->Add(closeBtn, 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    SetSizer(mainSizer);
}