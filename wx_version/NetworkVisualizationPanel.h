#ifndef NETWORKVISUALIZATIONPANEL_H
#define NETWORKVISUALIZATIONPANEL_H

#include <wx/wx.h>

class TNetwork;
class TRTBase;
class TCell;
class TSynapse;
class TElectrode;

class NetworkVisualizationPanel : public wxPanel
{
public:
    NetworkVisualizationPanel(wxWindow* parent, bool isCircle = false);
    
    void SetNetwork(TNetwork* network) { m_network = network; Refresh(); }
    void SetSelectedItem(TRTBase* item) { m_selectedItem = item; Refresh(); }
    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    
    wxPoint GetNextCellPosition();

private:
    TNetwork* m_network;
    bool m_isCircleLayout;
    TRTBase* m_draggedItem;
    TRTBase* m_selectedItem;
    wxPoint m_dragStart;
    int m_gridSize;
    int m_cellSize;
    int m_nextCellIndex;
    
    void DrawSquareLattice(wxPaintDC& dc);
    void DrawCirclePerimeter(wxPaintDC& dc);
    void DrawElectrode(wxPaintDC& dc, int x, int y);
    
    TRTBase* GetItemAt(const wxPoint& pos);
    wxPoint GridToPixel(int gridX, int gridY);
    wxPoint PixelToGrid(const wxPoint& pixel);
    
    wxDECLARE_EVENT_TABLE();
};

#endif