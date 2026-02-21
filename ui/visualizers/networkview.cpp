#include "networkview.h"
#include <QPen>
#include <QBrush>
#include <cmath>

NetworkView::NetworkView(QWidget *parent)
    : QWidget(parent), m_network(nullptr)
{
    setMinimumSize(200, 150);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void NetworkView::setNetwork(TNetwork *network)
{
    m_network = network;
    update();
}

void NetworkView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background grid
    painter.setPen(QPen(QColor(230, 230, 230), 1, Qt::DotLine));
    for (int x = 0; x < width(); x += 50) painter.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 50) painter.drawLine(0, y, width(), y);

    if (!m_network) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "No network loaded");
        return;
    }

    const TCellsMap &cells = m_network->GetCells();
    if (cells.empty()) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "Empty network");
        return;
    }

    // Compute bounding box of cell positions, then scale/translate to fit widget
    double minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
    for (auto &kv : cells) {
        double cx = kv.second->GetX(), cy = kv.second->GetY();
        minX = std::min(minX, cx); minY = std::min(minY, cy);
        maxX = std::max(maxX, cx); maxY = std::max(maxY, cy);
    }

    const double margin = 50;
    double rangeX = maxX - minX, rangeY = maxY - minY;
    if (rangeX < 1) rangeX = 1;
    if (rangeY < 1) rangeY = 1;
    double scaleX = (width() - 2 * margin) / rangeX;
    double scaleY = (height() - 2 * margin) / rangeY;
    double scale = std::min(scaleX, scaleY);
    // Center
    double offX = margin + (width() - 2 * margin - rangeX * scale) / 2;
    double offY = margin + (height() - 2 * margin - rangeY * scale) / 2;

    auto toScreen = [&](double cx, double cy) -> QPointF {
        return QPointF(offX + (cx - minX) * scale, offY + (cy - minY) * scale);
    };

    // Build name→position map
    std::map<std::wstring, QPointF> cellPos;
    for (auto &kv : cells)
        cellPos[kv.first] = toScreen(kv.second->GetX(), kv.second->GetY());

    // Draw synapses
    const TSynapsesMap &synapses = m_network->GetSynapses();
    for (auto &kv : synapses) {
        TSynapse *syn = kv.second.get();
        auto pit = cellPos.find(syn->Pre()->Name());
        auto qit = cellPos.find(syn->Post()->Name());
        if (pit != cellPos.end() && qit != cellPos.end())
            drawConnection(painter, pit->second, qit->second);
    }

    // Draw cells
    for (auto &kv : cellPos)
        drawCell(painter, QString::fromStdWString(kv.first), kv.second);
}

void NetworkView::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void NetworkView::drawCell(QPainter &painter, const QString &name, const QPointF &pos)
{
    const qreal r = 25;
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QColor(200, 220, 255));
    painter.drawEllipse(pos, r, r);
    painter.setPen(Qt::black);
    QFont f = font(); f.setPointSize(8); painter.setFont(f);
    painter.drawText(QRectF(pos.x() - 40, pos.y() + r + 2, 80, 16), Qt::AlignCenter, name);
}

void NetworkView::drawConnection(QPainter &painter, const QPointF &from, const QPointF &to)
{
    const qreal r = 25;
    QLineF line(from, to);
    double len = line.length();
    if (len < 2 * r) return;

    // Shorten line to cell edges
    QPointF dir = (to - from) / len;
    QPointF p1 = from + dir * r;
    QPointF p2 = to - dir * r;

    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawLine(p1, p2);

    // Arrowhead at p2
    double angle = std::atan2(-(p2.y() - p1.y()), p2.x() - p1.x());
    QPointF a1 = p2 - QPointF(std::sin(angle + M_PI / 3) * 8, std::cos(angle + M_PI / 3) * 8);
    QPointF a2 = p2 - QPointF(std::sin(angle + M_PI - M_PI / 3) * 8, std::cos(angle + M_PI - M_PI / 3) * 8);
    painter.setBrush(Qt::darkGray);
    painter.drawPolygon(QPolygonF() << p2 << a1 << a2);
}
