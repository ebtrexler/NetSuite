#include "AdvancedPlotPanel.h"
#include <algorithm>
#include <cmath>

wxBEGIN_EVENT_TABLE(AdvancedPlotPanel, wxPanel)
    EVT_PAINT(AdvancedPlotPanel::OnPaint)
    EVT_SIZE(AdvancedPlotPanel::OnSize)
    EVT_LEFT_DOWN(AdvancedPlotPanel::OnMouseDown)
    EVT_MOTION(AdvancedPlotPanel::OnMouseMove)
    EVT_LEFT_UP(AdvancedPlotPanel::OnMouseUp)
    EVT_MOUSEWHEEL(AdvancedPlotPanel::OnMouseWheel)
    EVT_RIGHT_DOWN(AdvancedPlotPanel::OnRightClick)
wxEND_EVENT_TABLE()

AdvancedPlotPanel::AdvancedPlotPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_minX(0), m_maxX(1)
    , m_minY(-100), m_maxY(100)
    , m_fullScaleMinX(0), m_fullScaleMaxX(1)
    , m_fullScaleMinY(-100), m_fullScaleMaxY(100)
    , m_mouseMode(MouseMode::Idle)
    , m_dragging(false)
    , m_showCursor(false)
    , m_showGrid(true)
    , m_backgroundColor(*wxWHITE)
    , m_gridColor(wxColour(200, 200, 200))
    , m_axesColor(*wxBLACK)
{
    SetBackgroundColour(m_backgroundColor);
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

void AdvancedPlotPanel::AddTrace(const std::vector<double>& data, double sampleTime, 
                                 const wxString& label, const wxColour& color)
{
    auto buffer = std::make_unique<PlotBuffer>(data.size(), sampleTime, label, color);
    buffer->data = data;
    
    // Calculate min/max for this trace
    if (!data.empty()) {
        auto minmax = std::minmax_element(data.begin(), data.end());
        buffer->minY = *minmax.first;
        buffer->maxY = *minmax.second;
    }
    
    m_traces.push_back(std::move(buffer));
    
    // Update full scale ranges
    if (m_traces.size() == 1) {
        m_fullScaleMinX = 0;
        m_fullScaleMaxX = data.size() * sampleTime;
        m_fullScaleMinY = m_traces[0]->minY;
        m_fullScaleMaxY = m_traces[0]->maxY;
    } else {
        m_fullScaleMaxX = std::max(m_fullScaleMaxX, data.size() * sampleTime);
        m_fullScaleMinY = std::min(m_fullScaleMinY, m_traces.back()->minY);
        m_fullScaleMaxY = std::max(m_fullScaleMaxY, m_traces.back()->maxY);
    }
    
    Refresh();
}

void AdvancedPlotPanel::ClearTraces()
{
    m_traces.clear();
    Refresh();
}

void AdvancedPlotPanel::AutoScale()
{
    m_minX = m_fullScaleMinX;
    m_maxX = m_fullScaleMaxX;
    m_minY = m_fullScaleMinY;
    m_maxY = m_fullScaleMaxY;
    
    // Add 5% padding
    double yRange = m_maxY - m_minY;
    if (yRange > 0) {
        m_minY -= yRange * 0.05;
        m_maxY += yRange * 0.05;
    }
    
    Refresh();
}

void AdvancedPlotPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize size = GetSize();
    
    // Clear background
    dc.SetBackground(wxBrush(m_backgroundColor));
    dc.Clear();
    
    if (size.GetWidth() < 100 || size.GetHeight() < 100) return;
    
    DrawGrid(dc);
    DrawAxes(dc);
    DrawTraces(dc);
    
    if (m_showCursor) {
        DrawCursor(dc);
    }
    
    // Draw zoom rectangle if dragging
    if (m_dragging && m_mouseMode == MouseMode::XYZoom) {
        dc.SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        wxRect rect(m_dragStart, m_dragCurrent);
        dc.DrawRectangle(rect);
    }
}

void AdvancedPlotPanel::DrawGrid(wxDC& dc)
{
    if (!m_showGrid) return;
    
    wxSize size = GetSize();
    int margin = 60;
    int plotWidth = size.GetWidth() - 2 * margin;
    int plotHeight = size.GetHeight() - 2 * margin;
    
    dc.SetPen(wxPen(m_gridColor, 1));
    
    // Vertical grid lines (time)
    double timeRange = m_maxX - m_minX;
    int numVLines = 10;
    for (int i = 0; i <= numVLines; i++) {
        int x = margin + (plotWidth * i) / numVLines;
        dc.DrawLine(x, margin, x, size.GetHeight() - margin);
    }
    
    // Horizontal grid lines (voltage)
    double voltageRange = m_maxY - m_minY;
    int numHLines = 8;
    for (int i = 0; i <= numHLines; i++) {
        int y = margin + (plotHeight * i) / numHLines;
        dc.DrawLine(margin, y, size.GetWidth() - margin, y);
    }
}

void AdvancedPlotPanel::DrawAxes(wxDC& dc)
{
    wxSize size = GetSize();
    int margin = 60;
    
    dc.SetPen(wxPen(m_axesColor, 2));
    
    // X-axis
    dc.DrawLine(margin, size.GetHeight() - margin, size.GetWidth() - margin, size.GetHeight() - margin);
    // Y-axis
    dc.DrawLine(margin, margin, margin, size.GetHeight() - margin);
    
    // Labels
    dc.SetTextForeground(m_axesColor);
    dc.DrawText(wxString::Format("%.1f mV", m_maxY), 5, margin);
    dc.DrawText(wxString::Format("%.1f mV", m_minY), 5, size.GetHeight() - margin - 15);
    dc.DrawText(wxString::Format("%.1f ms", m_minX * 1000), margin, size.GetHeight() - margin + 5);
    dc.DrawText(wxString::Format("%.1f ms", m_maxX * 1000), size.GetWidth() - margin - 40, size.GetHeight() - margin + 5);
}

void AdvancedPlotPanel::DrawTraces(wxDC& dc)
{
    if (m_traces.empty()) return;
    
    wxSize size = GetSize();
    int margin = 60;
    int plotWidth = size.GetWidth() - 2 * margin;
    int plotHeight = size.GetHeight() - 2 * margin;
    
    for (const auto& trace : m_traces) {
        if (trace->data.empty()) continue;
        
        dc.SetPen(wxPen(trace->color, 2));
        
        // Decimation for performance
        size_t step = std::max(1UL, trace->data.size() / (plotWidth * 2));
        
        wxPoint lastPoint;
        bool firstPoint = true;
        
        for (size_t i = 0; i < trace->data.size(); i += step) {
            double x = i * trace->sampleTime;
            double y = trace->data[i];
            
            // Skip points outside visible range
            if (x < m_minX || x > m_maxX) continue;
            
            wxPoint point = DataToPixel(x, y);
            
            if (!firstPoint) {
                dc.DrawLine(lastPoint, point);
            }
            
            lastPoint = point;
            firstPoint = false;
        }
    }
}

void AdvancedPlotPanel::DrawCursor(wxDC& dc)
{
    dc.SetPen(wxPen(*wxRED, 1, wxPENSTYLE_DOT));
    wxSize size = GetSize();
    
    // Vertical cursor line
    dc.DrawLine(m_cursorPos.x, 60, m_cursorPos.x, size.GetHeight() - 60);
    
    // Horizontal cursor line
    dc.DrawLine(60, m_cursorPos.y, size.GetWidth() - 60, m_cursorPos.y);
    
    // Show cursor value
    auto dataPoint = PixelToData(m_cursorPos);
    wxString cursorText = wxString::Format("%.2f ms, %.1f mV", 
                                          dataPoint.m_x * 1000, dataPoint.m_y);
    dc.DrawText(cursorText, m_cursorPos.x + 5, m_cursorPos.y - 20);
}

wxPoint AdvancedPlotPanel::DataToPixel(double x, double y)
{
    wxSize size = GetSize();
    int margin = 60;
    int plotWidth = size.GetWidth() - 2 * margin;
    int plotHeight = size.GetHeight() - 2 * margin;
    
    int pixelX = margin + (plotWidth * (x - m_minX)) / (m_maxX - m_minX);
    int pixelY = size.GetHeight() - margin - (plotHeight * (y - m_minY)) / (m_maxY - m_minY);
    
    return wxPoint(pixelX, pixelY);
}

wxPoint2DDouble AdvancedPlotPanel::PixelToData(const wxPoint& pixel)
{
    wxSize size = GetSize();
    int margin = 60;
    int plotWidth = size.GetWidth() - 2 * margin;
    int plotHeight = size.GetHeight() - 2 * margin;
    
    double x = m_minX + (m_maxX - m_minX) * (pixel.x - margin) / plotWidth;
    double y = m_minY + (m_maxY - m_minY) * (size.GetHeight() - margin - pixel.y) / plotHeight;
    
    return wxPoint2DDouble(x, y);
}

void AdvancedPlotPanel::OnMouseDown(wxMouseEvent& event)
{
    m_dragStart = event.GetPosition();
    m_dragging = true;
    
    if (event.ShiftDown()) {
        m_mouseMode = MouseMode::XYZoom;
        m_showCursor = false;
    } else if (event.ControlDown()) {
        m_mouseMode = MouseMode::Pan;
        m_showCursor = false;
    } else {
        m_mouseMode = MouseMode::Idle;
        m_showCursor = true;
        m_cursorPos = event.GetPosition();
    }
    
    CaptureMouse();
    Refresh();
}

void AdvancedPlotPanel::OnMouseMove(wxMouseEvent& event)
{
    if (m_dragging) {
        m_dragCurrent = event.GetPosition();
        
        if (m_mouseMode == MouseMode::Pan) {
            // Pan the view - calculate delta in pixels then convert to data units
            wxPoint pixelDelta = m_dragCurrent - m_dragStart;
            
            wxSize size = GetSize();
            int margin = 60;
            int plotWidth = size.GetWidth() - 2 * margin;
            int plotHeight = size.GetHeight() - 2 * margin;
            
            double xRange = m_maxX - m_minX;
            double yRange = m_maxY - m_minY;
            
            double deltaX = -pixelDelta.x * xRange / plotWidth;
            double deltaY = pixelDelta.y * yRange / plotHeight;
            
            m_minX += deltaX;
            m_maxX += deltaX;
            m_minY += deltaY;
            m_maxY += deltaY;
            
            m_dragStart = m_dragCurrent;
        }
        
        Refresh();
    } else if (m_showCursor) {
        m_cursorPos = event.GetPosition();
        Refresh();
    }
}

void AdvancedPlotPanel::OnMouseUp(wxMouseEvent& event)
{
    if (m_dragging) {
        if (m_mouseMode == MouseMode::XYZoom) {
            // Zoom to selected rectangle
            auto startData = PixelToData(m_dragStart);
            auto endData = PixelToData(event.GetPosition());
            
            m_minX = std::min(startData.m_x, endData.m_x);
            m_maxX = std::max(startData.m_x, endData.m_x);
            m_minY = std::min(startData.m_y, endData.m_y);
            m_maxY = std::max(startData.m_y, endData.m_y);
        }
        
        m_dragging = false;
        ReleaseMouse();
        Refresh();
    }
}

void AdvancedPlotPanel::OnMouseWheel(wxMouseEvent& event)
{
    double zoomFactor = event.GetWheelRotation() > 0 ? 0.9 : 1.1;
    
    auto mouseData = PixelToData(event.GetPosition());
    
    double xRange = m_maxX - m_minX;
    double yRange = m_maxY - m_minY;
    
    double newXRange = xRange * zoomFactor;
    double newYRange = yRange * zoomFactor;
    
    m_minX = mouseData.m_x - newXRange * (mouseData.m_x - m_minX) / xRange;
    m_maxX = mouseData.m_x + newXRange * (m_maxX - mouseData.m_x) / xRange;
    m_minY = mouseData.m_y - newYRange * (mouseData.m_y - m_minY) / yRange;
    m_maxY = mouseData.m_y + newYRange * (m_maxY - mouseData.m_y) / yRange;
    
    Refresh();
}

void AdvancedPlotPanel::OnRightClick(wxMouseEvent& event)
{
    AutoScale();
}

void AdvancedPlotPanel::OnSize(wxSizeEvent& event)
{
    Refresh();
    event.Skip();
}