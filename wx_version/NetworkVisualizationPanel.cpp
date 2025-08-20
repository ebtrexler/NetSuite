#include "NetworkVisualizationPanel.h"
#include "RT_Network_wx.h"
#include "RT_Cell_wx.h"
#include "RT_Synapse_wx.h"
#include "RT_Electrode_wx.h"
#include <wx/dcbuffer.h>
#include <cmath>

wxBEGIN_EVENT_TABLE(NetworkVisualizationPanel, wxPanel)
    EVT_PAINT(NetworkVisualizationPanel::OnPaint)
        EVT_LEFT_DOWN(NetworkVisualizationPanel::OnMouseDown)
            EVT_MOTION(NetworkVisualizationPanel::OnMouseMove)
                EVT_LEFT_UP(NetworkVisualizationPanel::OnMouseUp)
                    wxEND_EVENT_TABLE()

                        NetworkVisualizationPanel::NetworkVisualizationPanel(wxWindow *parent, bool isCircle)
    : wxPanel(parent, wxID_ANY), m_network(nullptr), m_isCircleLayout(isCircle),
      m_draggedItem(nullptr), m_selectedItem(nullptr), m_gridSize(10), m_cellSize(30), m_nextCellIndex(0)
{
    SetBackgroundColour(*wxWHITE);
    SetMinSize(wxSize(300, 300));
}

void NetworkVisualizationPanel::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_network)
        return;

    if (m_isCircleLayout)
    {
        DrawCirclePerimeter(dc);
    }
    else
    {
        DrawSquareLattice(dc);
    }
}

void NetworkVisualizationPanel::DrawSquareLattice(wxPaintDC &dc)
{
    wxSize size = GetSize();
    m_cellSize = std::min(size.x, size.y) / m_gridSize;

    // Draw grid
    dc.SetPen(wxPen(*wxLIGHT_GREY, 1));
    for (int i = 0; i <= m_gridSize; i++)
    {
        dc.DrawLine(i * m_cellSize, 0, i * m_cellSize, m_gridSize * m_cellSize);
        dc.DrawLine(0, i * m_cellSize, m_gridSize * m_cellSize, i * m_cellSize);
    }

    // Draw synapses first (behind cells)
    for (const auto &pair : m_network->GetSynapses())
    {
        const TSynapse *synapse = pair.second.get();
        if (synapse->Pre() && synapse->Post())
        {
            if (synapse == m_selectedItem)
            {
                dc.SetPen(wxPen(*wxYELLOW, 4));
            }
            else
            {
                dc.SetPen(wxPen(*wxRED, 2));
            }
            wxPoint prePos = GridToPixel(synapse->Pre()->GetX(), synapse->Pre()->GetY());
            wxPoint postPos = GridToPixel(synapse->Post()->GetX(), synapse->Post()->GetY());
            dc.DrawLine(prePos.x, prePos.y, postPos.x, postPos.y);
        }
    }

    // Draw cells
    for (const auto &pair : m_network->GetCells())
    {
        const TCell *cell = pair.second.get();
        wxPoint pos = GridToPixel(cell->GetX(), cell->GetY());

        if (cell == m_selectedItem)
        {
            dc.SetBrush(wxBrush(*wxYELLOW));
            dc.SetPen(wxPen(*wxRED, 3));
        }
        else
        {
            dc.SetBrush(wxBrush(*wxBLUE));
            dc.SetPen(wxPen(*wxBLACK, 2));
        }
        dc.DrawCircle(pos.x, pos.y, m_cellSize / 4);
    }

    // Draw electrodes
    for (const auto &pair : m_network->GetElectrodes())
    {
        const TElectrode *electrode = pair.second.get();
        if (electrode->Owner())
        {
            if (electrode == m_selectedItem)
            {
                dc.SetPen(wxPen(*wxYELLOW, 4));
            }
            else
            {
                dc.SetPen(wxPen(wxColour(0, 255, 0), 3)); // Neon green
            }
            wxPoint pos = GridToPixel(electrode->Owner()->GetX(), electrode->Owner()->GetY());
            DrawElectrode(dc, pos.x, pos.y);
        }
    }
}

void NetworkVisualizationPanel::DrawCirclePerimeter(wxPaintDC &dc)
{
    wxSize size = GetSize();
    int centerX = size.x / 2;
    int centerY = size.y / 2;
    int radius = std::min(centerX, centerY) - 20;

    // Draw circle outline
    dc.SetPen(wxPen(*wxLIGHT_GREY, 2));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawCircle(centerX, centerY, radius);

    // Draw cells around perimeter
    const auto &cells = m_network->GetCells();
    int cellCount = cells.size();
    if (cellCount > 0)
    {
        double angleStep = 2.0 * M_PI / cellCount;
        int i = 0;

        for (const auto &pair : cells)
        {
            const TCell *cell = pair.second.get();
            double angle = i * angleStep;
            int x = centerX + radius * cos(angle);
            int y = centerY + radius * sin(angle);

            if (cell == m_selectedItem)
            {
                dc.SetBrush(wxBrush(*wxYELLOW));
                dc.SetPen(wxPen(*wxRED, 3));
            }
            else
            {
                dc.SetBrush(wxBrush(*wxBLUE));
                dc.SetPen(wxPen(*wxBLACK, 2));
            }
            dc.DrawCircle(x, y, 8);
            i++;
        }
    }

    // Draw synapses as lines between connected cells
    for (const auto &pair : m_network->GetSynapses())
    {
        const TSynapse *synapse = pair.second.get();
        if (synapse->Pre() && synapse->Post() && cellCount > 0)
        {
            if (synapse == m_selectedItem)
            {
                dc.SetPen(wxPen(*wxYELLOW, 4));
            }
            else
            {
                dc.SetPen(wxPen(*wxRED, 2));
            }

            // Find angles for pre and post cells
            double angleStep = 2.0 * M_PI / cellCount;
            int preIndex = 0, postIndex = 0;
            int i = 0;

            for (const auto &cellPair : cells)
            {
                if (cellPair.second.get() == synapse->Pre())
                    preIndex = i;
                if (cellPair.second.get() == synapse->Post())
                    postIndex = i;
                i++;
            }

            double angle1 = preIndex * angleStep;
            double angle2 = postIndex * angleStep;

            int x1 = centerX + radius * cos(angle1);
            int y1 = centerY + radius * sin(angle1);
            int x2 = centerX + radius * cos(angle2);
            int y2 = centerY + radius * sin(angle2);

            dc.DrawLine(x1, y1, x2, y2);
        }
    }

    // Draw electrodes
    for (const auto &pair : m_network->GetElectrodes())
    {
        const TElectrode *electrode = pair.second.get();
        if (electrode->Owner() && cellCount > 0)
        {
            if (electrode == m_selectedItem)
            {
                dc.SetPen(wxPen(*wxYELLOW, 4));
            }
            else
            {
                dc.SetPen(wxPen(wxColour(0, 255, 0), 3)); // Neon green
            }

            double angleStep = 2.0 * M_PI / cellCount;
            int ownerIndex = 0;
            int i = 0;

            for (const auto &cellPair : cells)
            {
                if (cellPair.second.get() == electrode->Owner())
                {
                    ownerIndex = i;
                    break;
                }
                i++;
            }

            double angle = ownerIndex * angleStep;
            int x = centerX + radius * cos(angle);
            int y = centerY + radius * sin(angle);

            DrawElectrode(dc, x, y);
        }
    }
}

void NetworkVisualizationPanel::DrawElectrode(wxPaintDC &dc, int x, int y)
{
    // Draw glass needle electrode from southwest to center (45 degrees)
    const double angle = 45.0 * M_PI / 180.0;  // From bottom-left to center
    const int needleLength = m_cellSize * 0.8; // Larger electrode

    // Calculate start and end points (tip at cell center)
    int startX = x - needleLength * cos(angle);
    int startY = y + needleLength * sin(angle); // Start below and left
    int endX = x;                               // Tip at cell center
    int endY = y;

    // Draw black outline first (thicker)
    dc.SetPen(wxPen(*wxBLACK, 4));
    dc.DrawLine(startX, startY, endX, endY);

    // Draw needle tip outline
    int tipX1 = endX - 4 * cos(angle - 0.3);
    int tipY1 = endY + 4 * sin(angle - 0.3);
    int tipX2 = endX - 5 * cos(1.85 * angle);
    int tipY2 = endY + 5 * sin(1.85 * angle);
    dc.DrawLine(endX, endY, tipX1, tipY1);
    dc.DrawLine(endX, endY, tipX2, tipY2);

    // Draw green needle on top (thinner)
    dc.SetPen(wxPen(wxColour(0, 255, 0), 2));
    dc.DrawLine(startX, startY, endX, endY);
    dc.DrawLine(endX, endY, tipX1, tipY1);
    dc.DrawLine(endX, endY, tipX2, tipY2);
}

wxPoint NetworkVisualizationPanel::GetNextCellPosition()
{
    int x = m_nextCellIndex % m_gridSize;
    int y = m_nextCellIndex / m_gridSize;
    m_nextCellIndex++;
    return wxPoint(x, y);
}

wxPoint NetworkVisualizationPanel::GridToPixel(int gridX, int gridY)
{
    return wxPoint(gridX * m_cellSize + m_cellSize / 2, gridY * m_cellSize + m_cellSize / 2);
}

wxPoint NetworkVisualizationPanel::PixelToGrid(const wxPoint &pixel)
{
    return wxPoint(pixel.x / m_cellSize, pixel.y / m_cellSize);
}

TRTBase *NetworkVisualizationPanel::GetItemAt(const wxPoint &pos)
{
    if (!m_network)
        return nullptr;

    if (m_isCircleLayout)
    {
        wxSize size = GetSize();
        int centerX = size.x / 2;
        int centerY = size.y / 2;
        int radius = std::min(centerX, centerY) - 20;

        const auto &cells = m_network->GetCells();
        int cellCount = cells.size();
        if (cellCount > 0)
        {
            double angleStep = 2.0 * M_PI / cellCount;
            int i = 0;

            // Check cells
            for (const auto &pair : cells)
            {
                double angle = i * angleStep;
                int x = centerX + radius * cos(angle);
                int y = centerY + radius * sin(angle);

                int dx = pos.x - x;
                int dy = pos.y - y;
                if (dx * dx + dy * dy <= 64)
                { // 8 pixel radius
                    return pair.second.get();
                }
                i++;
            }

            // Check synapses
            for (const auto &pair : m_network->GetSynapses())
            {
                TSynapse *synapse = pair.second.get();
                if (synapse->Pre() && synapse->Post())
                {
                    int preIndex = 0, postIndex = 0, idx = 0;
                    for (const auto &cellPair : cells)
                    {
                        if (cellPair.second.get() == synapse->Pre())
                            preIndex = idx;
                        if (cellPair.second.get() == synapse->Post())
                            postIndex = idx;
                        idx++;
                    }

                    double angle1 = preIndex * angleStep;
                    double angle2 = postIndex * angleStep;
                    int x1 = centerX + radius * cos(angle1);
                    int y1 = centerY + radius * sin(angle1);
                    int x2 = centerX + radius * cos(angle2);
                    int y2 = centerY + radius * sin(angle2);

                    double lineLen = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                    if (lineLen > 0)
                    {
                        double t = ((pos.x - x1) * (x2 - x1) + (pos.y - y1) * (y2 - y1)) / (lineLen * lineLen);
                        if (t >= 0 && t <= 1)
                        {
                            double projX = x1 + t * (x2 - x1);
                            double projY = y1 + t * (y2 - y1);
                            double dist = sqrt((pos.x - projX) * (pos.x - projX) + (pos.y - projY) * (pos.y - projY));
                            if (dist <= 5)
                                return synapse;
                        }
                    }
                }
            }

            // Check electrodes (needles)
            for (const auto &pair : m_network->GetElectrodes())
            {
                TElectrode *electrode = pair.second.get();
                if (electrode->Owner())
                {
                    double angleStep = 2.0 * M_PI / cellCount;
                    int ownerIndex = 0;
                    int i = 0;

                    for (const auto &cellPair : cells)
                    {
                        if (cellPair.second.get() == electrode->Owner())
                        {
                            ownerIndex = i;
                            break;
                        }
                        i++;
                    }

                    double angle = ownerIndex * angleStep;
                    int cellX = centerX + radius * cos(angle);
                    int cellY = centerY + radius * sin(angle);

                    // Needle extends radially inward
                    int startX = cellX + 18 * cos(angle);
                    int startY = cellY + 18 * sin(angle);
                    int endX = cellX - 5 * cos(angle);
                    int endY = cellY - 5 * sin(angle);

                    // Hit test along needle line
                    double lineLen = sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));
                    if (lineLen > 0)
                    {
                        double t = ((pos.x - startX) * (endX - startX) + (pos.y - startY) * (endY - startY)) / (lineLen * lineLen);
                        if (t >= 0 && t <= 1)
                        {
                            double projX = startX + t * (endX - startX);
                            double projY = startY + t * (endY - startY);
                            double dist = sqrt((pos.x - projX) * (pos.x - projX) + (pos.y - projY) * (pos.y - projY));
                            if (dist <= 3)
                                return electrode;
                        }
                    }
                }
            }
        }
    }
    else
    {
        // Square lattice hit detection
        for (const auto &pair : m_network->GetCells())
        {
            TCell *cell = pair.second.get();
            wxPoint cellPos = GridToPixel(cell->GetX(), cell->GetY());
            int dx = pos.x - cellPos.x;
            int dy = pos.y - cellPos.y;
            if (dx * dx + dy * dy <= (m_cellSize / 4) * (m_cellSize / 4))
            {
                return cell;
            }
        }

        for (const auto &pair : m_network->GetSynapses())
        {
            TSynapse *synapse = pair.second.get();
            if (synapse->Pre() && synapse->Post())
            {
                wxPoint prePos = GridToPixel(synapse->Pre()->GetX(), synapse->Pre()->GetY());
                wxPoint postPos = GridToPixel(synapse->Post()->GetX(), synapse->Post()->GetY());

                double lineLen = sqrt((postPos.x - prePos.x) * (postPos.x - prePos.x) + (postPos.y - prePos.y) * (postPos.y - prePos.y));
                if (lineLen > 0)
                {
                    double t = ((pos.x - prePos.x) * (postPos.x - prePos.x) + (pos.y - prePos.y) * (postPos.y - prePos.y)) / (lineLen * lineLen);
                    if (t >= 0 && t <= 1)
                    {
                        double projX = prePos.x + t * (postPos.x - prePos.x);
                        double projY = prePos.y + t * (postPos.y - prePos.y);
                        double dist = sqrt((pos.x - projX) * (pos.x - projX) + (pos.y - projY) * (pos.y - projY));
                        if (dist <= 5)
                            return synapse;
                    }
                }
            }
        }

        // Check electrodes (southwest needle)
        for (const auto &pair : m_network->GetElectrodes())
        {
            TElectrode *electrode = pair.second.get();
            if (electrode->Owner())
            {
                wxPoint cellPos = GridToPixel(electrode->Owner()->GetX(), electrode->Owner()->GetY());
                const double angle = 225.0 * M_PI / 180.0;
                const int needleLength = m_cellSize * 0.8;

                int startX = cellPos.x - needleLength * cos(angle);
                int startY = cellPos.y - needleLength * sin(angle);
                int endX = cellPos.x;
                int endY = cellPos.y;

                // Hit test along needle line
                double lineLen = sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));
                if (lineLen > 0)
                {
                    double t = ((pos.x - startX) * (endX - startX) + (pos.y - startY) * (endY - startY)) / (lineLen * lineLen);
                    if (t >= 0 && t <= 1)
                    {
                        double projX = startX + t * (endX - startX);
                        double projY = startY + t * (endY - startY);
                        double dist = sqrt((pos.x - projX) * (pos.x - projX) + (pos.y - projY) * (pos.y - projY));
                        if (dist <= 4)
                            return electrode;
                    }
                }
            }
        }
    }

    return nullptr;
}

void NetworkVisualizationPanel::OnMouseDown(wxMouseEvent &event)
{
    if (!m_network)
        return;

    TRTBase *item = GetItemAt(event.GetPosition());

    if (event.LeftDClick() && item)
    {
        // Double-click opens configuration dialog
        wxPanel *editPanel = item->CreateEditPanel(this);
        if (editPanel)
        {
            wxDialog dialog(this, wxID_ANY, wxString::Format("Configure %s", item->Name()));
            wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(editPanel, 1, wxEXPAND | wxALL, 10);

            wxStdDialogButtonSizer *buttonSizer = new wxStdDialogButtonSizer();
            buttonSizer->AddButton(new wxButton(&dialog, wxID_OK, "OK"));
            buttonSizer->AddButton(new wxButton(&dialog, wxID_CANCEL, "Cancel"));
            buttonSizer->Realize();
            sizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

            dialog.SetSizer(sizer);
            dialog.ShowModal();
        }
    }
    else if (item)
    {
        // Select item and notify parent
        m_selectedItem = item;
        Refresh();

        wxCommandEvent selEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_ANY);
        selEvent.SetClientData(item);
        GetParent()->GetEventHandler()->ProcessEvent(selEvent);

        if (dynamic_cast<TCell *>(item))
        {
            // Start dragging a cell
            m_draggedItem = item;
            m_dragStart = event.GetPosition();
            CaptureMouse();
        }
    }
    else
    {
        // Clear selection
        m_selectedItem = nullptr;
        Refresh();
    }
}

void NetworkVisualizationPanel::OnMouseMove(wxMouseEvent &event)
{
    if (m_draggedItem && HasCapture())
    {
        TCell *cell = dynamic_cast<TCell *>(m_draggedItem);
        if (cell)
        {
            wxPoint newGrid = PixelToGrid(event.GetPosition());
            newGrid.x = std::max(0, std::min(m_gridSize - 1, newGrid.x));
            newGrid.y = std::max(0, std::min(m_gridSize - 1, newGrid.y));

            cell->SetX(newGrid.x);
            cell->SetY(newGrid.y);
            Refresh();

            // Notify parent of position change
            wxCommandEvent updateEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_ANY);
            updateEvent.SetClientData(cell);
            updateEvent.SetString("position_changed");
            GetParent()->GetEventHandler()->ProcessEvent(updateEvent);
        }
    }
}

void NetworkVisualizationPanel::OnMouseUp(wxMouseEvent &event)
{
    if (HasCapture())
    {
        ReleaseMouse();
        m_draggedItem = nullptr;
    }
}