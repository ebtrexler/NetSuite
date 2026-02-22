#include "networkview.h"
#include <QContextMenuEvent>
#include <QInputDialog>
#include <cmath>
#include <set>

NetworkView::NetworkView(QWidget *parent)
    : QWidget(parent), m_network(nullptr)
{
    setMinimumSize(200, 150);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

void NetworkView::setNetwork(TNetwork *network)
{
    m_network = network;
    update();
}

void NetworkView::computeTransform()
{
    if (m_transformLocked) return;
    if (!m_network || m_network->GetCells().empty()) return;
    const TCellsMap &cells = m_network->GetCells();

    m_minX = 1e9; m_minY = 1e9;
    double maxX = -1e9, maxY = -1e9;
    for (auto &kv : cells) {
        double cx = kv.second->GetX(), cy = kv.second->GetY();
        m_minX = std::min(m_minX, cx); m_minY = std::min(m_minY, cy);
        maxX = std::max(maxX, cx); maxY = std::max(maxY, cy);
    }
    double rangeX = std::max(maxX - m_minX, 1.0);
    double rangeY = std::max(maxY - m_minY, 1.0);
    m_scale = std::min((width() - 2 * Margin) / rangeX, (height() - 2 * Margin) / rangeY);
    m_offX = Margin + (width() - 2 * Margin - rangeX * m_scale) / 2;
    m_offY = Margin + (height() - 2 * Margin - rangeY * m_scale) / 2;
}

QPointF NetworkView::toScreen(double cx, double cy)
{
    return QPointF(m_offX + (cx - m_minX) * m_scale, m_offY + (cy - m_minY) * m_scale);
}

QPointF NetworkView::fromScreen(const QPointF &sp)
{
    return QPointF(m_minX + (sp.x() - m_offX) / m_scale, m_minY + (sp.y() - m_offY) / m_scale);
}

std::wstring NetworkView::cellAt(const QPointF &screenPos)
{
    if (!m_network) return L"";
    const TCellsMap &cells = m_network->GetCells();
    for (auto &kv : cells) {
        QPointF sp = toScreen(kv.second->GetX(), kv.second->GetY());
        if (QLineF(sp, screenPos).length() <= CellRadius)
            return kv.first;
    }
    return L"";
}

void NetworkView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    p.fillRect(rect(), QColor(250, 250, 250));
    p.setPen(QPen(QColor(230, 230, 230), 1, Qt::DotLine));
    for (int x = 0; x < width(); x += 50) p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 50) p.drawLine(0, y, width(), y);

    if (!m_network) {
        p.setPen(Qt::gray);
        p.drawText(rect(), Qt::AlignCenter, "No network loaded");
        return;
    }
    const TCellsMap &cells = m_network->GetCells();
    if (cells.empty()) {
        p.setPen(Qt::gray);
        p.drawText(rect(), Qt::AlignCenter, "Right-click to add cells");
        return;
    }

    computeTransform();

    // Build position map and electrode set
    std::map<std::wstring, QPointF> cellPos;
    std::set<std::wstring> hasElectrode;
    for (auto &kv : cells) {
        cellPos[kv.first] = toScreen(kv.second->GetX(), kv.second->GetY());
        if (!kv.second->GetElectrodes().empty())
            hasElectrode.insert(kv.first);
    }

    // Draw synapses
    const TSynapsesMap &synapses = m_network->GetSynapses();
    for (auto &kv : synapses) {
        TSynapse *syn = kv.second.get();
        auto a = cellPos.find(syn->Pre()->Name());
        auto b = cellPos.find(syn->Post()->Name());
        if (a != cellPos.end() && b != cellPos.end())
            drawSynapse(p, a->second, b->second);
    }

    // Draw cells
    for (auto &kv : cellPos)
        drawCell(p, QString::fromStdWString(kv.first), kv.second,
                 hasElectrode.count(kv.first) > 0);
}

void NetworkView::drawCell(QPainter &p, const QString &name, const QPointF &pos, bool hasElec)
{
    // Cell body
    p.setPen(QPen(Qt::black, 2));
    p.setBrush(QColor(200, 220, 255));
    p.drawEllipse(pos, CellRadius, CellRadius);

    // Name inside cell
    p.setPen(Qt::black);
    QFont f = font();
    f.setPixelSize(11);
    f.setBold(true);
    p.setFont(f);
    p.drawText(QRectF(pos.x() - CellRadius, pos.y() - CellRadius,
                       CellRadius * 2, CellRadius * 2),
               Qt::AlignCenter | Qt::TextWordWrap, name);

    // Electrode indicator
    if (hasElec) drawElectrode(p, pos);
}

void NetworkView::drawElectrode(QPainter &p, const QPointF &cellPos)
{
    // Small downward-pointing pipette/arrow above the cell
    double tipY = cellPos.y() - CellRadius;
    double topY = tipY - 18;
    double cx = cellPos.x();

    p.setPen(QPen(QColor(180, 0, 0), 1.5));
    p.setBrush(QColor(220, 80, 80, 180));

    QPolygonF pipette;
    pipette << QPointF(cx, tipY)          // tip touching cell
            << QPointF(cx - 5, topY)      // top left
            << QPointF(cx + 5, topY);     // top right
    p.drawPolygon(pipette);

    // Small "I" label
    QFont f = font();
    f.setPixelSize(9);
    f.setBold(true);
    p.setFont(f);
    p.setPen(QColor(180, 0, 0));
    p.drawText(QRectF(cx + 6, topY - 2, 12, 12), Qt::AlignCenter, "I");
}

void NetworkView::drawSynapse(QPainter &p, const QPointF &from, const QPointF &to)
{
    QLineF line(from, to);
    double len = line.length();
    if (len < 2 * CellRadius) return;

    QPointF dir = (to - from) / len;
    QPointF p1 = from + dir * CellRadius;
    QPointF p2 = to - dir * CellRadius;

    p.setPen(QPen(QColor(100, 100, 100), 2));
    p.drawLine(p1, p2);

    // Arrowhead
    double angle = std::atan2(-(p2.y() - p1.y()), p2.x() - p1.x());
    QPointF a1 = p2 - QPointF(std::sin(angle + M_PI / 3) * 8, std::cos(angle + M_PI / 3) * 8);
    QPointF a2 = p2 - QPointF(std::sin(angle + M_PI - M_PI / 3) * 8, std::cos(angle + M_PI - M_PI / 3) * 8);
    p.setBrush(QColor(100, 100, 100));
    p.drawPolygon(QPolygonF() << p2 << a1 << a2);
}

// --- Drag to move ---

void NetworkView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_network) {
        computeTransform();
        std::wstring name = cellAt(event->pos());
        if (!name.empty()) {
            m_dragging = true;
            m_transformLocked = true;
            m_dragCell = name;
            m_dragStart = event->pos();
            setCursor(Qt::ClosedHandCursor);
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void NetworkView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && m_network) {
        QPointF world = fromScreen(event->pos());
        const TCellsMap &cells = m_network->GetCells();
        auto it = cells.find(m_dragCell);
        if (it != cells.end()) {
            it->second->SetX((int)world.x());
            it->second->SetY((int)world.y());
            update();
        }
    } else if (m_network) {
        // Hover cursor
        computeTransform();
        std::wstring name = cellAt(event->pos());
        setCursor(name.empty() ? Qt::ArrowCursor : Qt::OpenHandCursor);
    }
}

void NetworkView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        m_transformLocked = false;
        setCursor(Qt::ArrowCursor);
        const TCellsMap &cells = m_network->GetCells();
        auto it = cells.find(m_dragCell);
        if (it != cells.end())
            emit cellMoved(m_dragCell, it->second->GetX(), it->second->GetY());
        update(); // Recompute transform with final positions
    }
    QWidget::mouseReleaseEvent(event);
}

// --- Context menu ---

void NetworkView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_network) return;
    computeTransform();

    QMenu menu(this);
    std::wstring clicked = cellAt(event->pos());

    if (clicked.empty()) {
        // Clicked on empty space
        QPointF world = fromScreen(event->pos());
        int wx = (int)world.x(), wy = (int)world.y();
        menu.addAction("Add Cell Here", [this, wx, wy]() { emit requestAddCell(wx, wy); });
    } else {
        // Clicked on a cell
        QString cellLabel = QString::fromStdWString(clicked);
        std::wstring cellName = clicked;

        menu.addAction(QString("Add Electrode to %1").arg(cellLabel),
            [this, cellName]() { emit requestAddElectrode(cellName); });

        // Add synapse submenu — list other cells as targets
        const TCellsMap &cells = m_network->GetCells();
        if (cells.size() > 1) {
            QMenu *synMenu = menu.addMenu(QString("Add Synapse from %1 →").arg(cellLabel));
            for (auto &kv : cells) {
                if (kv.first == cellName) continue;
                std::wstring target = kv.first;
                synMenu->addAction(QString::fromStdWString(target),
                    [this, cellName, target]() { emit requestAddSynapse(cellName, target); });
            }
        }
    }

    if (!menu.isEmpty()) menu.exec(event->globalPos());
}
