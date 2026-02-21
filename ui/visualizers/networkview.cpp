#include "networkview.h"
#include <QPen>
#include <QBrush>

NetworkView::NetworkView(QWidget *parent)
    : QWidget(parent), m_network(nullptr)
{
    setMinimumSize(400, 300);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void NetworkView::setNetwork(TNetwork *network)
{
    m_network = network;
    update();
}

void NetworkView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background grid
    painter.setPen(QPen(Qt::lightGray, 1, Qt::DotLine));
    for (int x = 0; x < width(); x += 50) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += 50) {
        painter.drawLine(0, y, width(), y);
    }
    
    if (!m_network) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "No network loaded");
        return;
    }
    
    // Draw network info
    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Network: %1")
        .arg(QString::fromStdWString(m_network->Name())));
    
    // Draw sample cells (placeholder - would iterate through actual cells)
    drawCell(painter, "Cell 1", QPointF(100, 100));
    drawCell(painter, "Cell 2", QPointF(300, 100));
    drawCell(painter, "Cell 3", QPointF(200, 200));
    
    // Draw sample connections
    drawConnection(painter, QPointF(100, 100), QPointF(300, 100));
    drawConnection(painter, QPointF(100, 100), QPointF(200, 200));
}

void NetworkView::mousePressEvent(QMouseEvent *event)
{
    // Placeholder for cell selection
    QWidget::mousePressEvent(event);
}

void NetworkView::drawCell(QPainter &painter, const QString &name, const QPointF &pos)
{
    const qreal radius = 30;
    
    // Draw cell body
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(QColor(200, 220, 255)));
    painter.drawEllipse(pos, radius, radius);
    
    // Draw cell name
    painter.setPen(Qt::black);
    QRectF textRect(pos.x() - radius, pos.y() - 10, radius * 2, 20);
    painter.drawText(textRect, Qt::AlignCenter, name);
}

void NetworkView::drawConnection(QPainter &painter, const QPointF &from, const QPointF &to)
{
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawLine(from, to);
    
    // Draw arrow head
    QLineF line(from, to);
    qreal angle = std::atan2(-line.dy(), line.dx());
    
    QPointF arrowP1 = to - QPointF(sin(angle + M_PI / 3) * 10,
                                     cos(angle + M_PI / 3) * 10);
    QPointF arrowP2 = to - QPointF(sin(angle + M_PI - M_PI / 3) * 10,
                                     cos(angle + M_PI - M_PI / 3) * 10);
    
    QPolygonF arrowHead;
    arrowHead << to << arrowP1 << arrowP2;
    painter.setBrush(Qt::darkGray);
    painter.drawPolygon(arrowHead);
}
