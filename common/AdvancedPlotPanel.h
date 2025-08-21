#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <vector>
#include <memory>

// Mouse interaction modes
enum class MouseMode {
    Idle,
    XZoom,
    YZoom,
    XYZoom,
    Pan
};

// Data buffer for efficient plotting
class PlotBuffer {
public:
    std::vector<double> data;
    double sampleTime;
    double minY, maxY;
    wxString label;
    wxColour color;
    
    PlotBuffer(size_t size, double sampleTime, const wxString& label, const wxColour& color)
        : data(size), sampleTime(sampleTime), label(label), color(color), minY(0), maxY(0) {}
};

class AdvancedPlotPanel : public wxPanel
{
public:
    AdvancedPlotPanel(wxWindow* parent);
    
    void AddTrace(const std::vector<double>& data, double sampleTime, 
                  const wxString& label, const wxColour& color);
    void ClearTraces();
    void SetTimeRange(double minX, double maxX);
    void SetVoltageRange(double minY, double maxY);
    void AutoScale();
    void ZoomToFit();
    
private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnRightClick(wxMouseEvent& event);
    
    void DrawGrid(wxDC& dc);
    void DrawAxes(wxDC& dc);
    void DrawTraces(wxDC& dc);
    void DrawCursor(wxDC& dc);
    
    wxPoint DataToPixel(double x, double y);
    wxPoint2DDouble PixelToData(const wxPoint& pixel);
    
    // Data storage
    std::vector<std::unique_ptr<PlotBuffer>> m_traces;
    
    // View parameters
    double m_minX, m_maxX;
    double m_minY, m_maxY;
    double m_fullScaleMinX, m_fullScaleMaxX;
    double m_fullScaleMinY, m_fullScaleMaxY;
    
    // Interaction state
    MouseMode m_mouseMode;
    bool m_dragging;
    wxPoint m_dragStart;
    wxPoint m_dragCurrent;
    wxPoint m_cursorPos;
    bool m_showCursor;
    
    // Display options
    bool m_showGrid;
    wxColour m_backgroundColor;
    wxColour m_gridColor;
    wxColour m_axesColor;
    
    wxDECLARE_EVENT_TABLE();
};