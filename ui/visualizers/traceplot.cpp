#include "traceplot.h"
#include <QPainter>
#include <cmath>
#include <algorithm>

static const size_t DEFAULT_CAPACITY = 500000;

TracePlot::TracePlot(const QString &title, QWidget *parent)
    : QWidget(parent), title(title), m_ring(DEFAULT_CAPACITY),
      timeMin(0), timeMax(1000),
      valueMin(-80), valueMax(40), autoScale(true)
{
    setMinimumHeight(100);
    setMouseTracking(true);
}

void TracePlot::setBufferCapacity(size_t capacity) { m_ring.resize(capacity); }

void TracePlot::addDataPoint(double time, double value)
{
    m_ring.push({time, value});
    if (time > timeMax) {
        double window = timeMax - timeMin;
        timeMax = time;
        timeMin = time - window;
    }
    if (autoScale) updateValueRange();
    update();
}

void TracePlot::clearData() { m_ring.clear(); update(); }

void TracePlot::setTimeRange(double tMin, double tMax)
{
    timeMin = tMin; timeMax = tMax; update();
}

void TracePlot::setValueRange(double vMin, double vMax)
{
    valueMin = vMin; valueMax = vMax; autoScale = false; update();
}

void TracePlot::setAutoScale(bool enabled)
{
    autoScale = enabled;
    if (enabled) updateValueRange();
}

void TracePlot::resetZoom()
{
    autoScale = true;
    updateValueRange();
    if (!m_ring.empty()) {
        timeMin = m_ring.front().time;
        timeMax = m_ring.back().time;
        if (timeMax <= timeMin) timeMax = timeMin + 1000;
        emit timeRangeChanged(timeMin, timeMax);
    }
    update();
}

void TracePlot::updateValueRange()
{
    if (m_ring.empty()) return;
    double vmin = m_ring[0].value, vmax = vmin;
    for (size_t i = 1; i < m_ring.size(); ++i) {
        double v = m_ring[i].value;
        if (v < vmin) vmin = v;
        if (v > vmax) vmax = v;
    }
    double margin = (vmax - vmin) * 0.1;
    if (margin < 1.0) margin = 1.0;
    valueMin = vmin - margin;
    valueMax = vmax + margin;
}

QRect TracePlot::plotRect() const
{
    return QRect(leftMargin, topMargin,
                 width() - leftMargin - rightMargin,
                 height() - topMargin - bottomMargin);
}

QPointF TracePlot::dataToScreen(double t, double v) const
{
    QRect pr = plotRect();
    double x = pr.left() + (t - timeMin) / (timeMax - timeMin) * pr.width();
    double y = pr.top() + (1.0 - (v - valueMin) / (valueMax - valueMin)) * pr.height();
    return QPointF(x, y);
}

void TracePlot::screenToData(const QPoint &p, double &t, double &v) const
{
    QRect pr = plotRect();
    t = timeMin + (p.x() - pr.left()) / (double)pr.width() * (timeMax - timeMin);
    v = valueMax - (p.y() - pr.top()) / (double)pr.height() * (valueMax - valueMin);
}

void TracePlot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect pr = plotRect();

    // Background
    painter.fillRect(rect(), Qt::white);
    painter.fillRect(pr, QColor(250, 250, 250));
    painter.setPen(Qt::black);
    painter.drawRect(pr);

    // Title
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(rect().adjusted(0, 5, 0, 0), Qt::AlignHCenter | Qt::AlignTop, title);

    // Axis labels
    painter.setFont(QFont("Arial", 8));
    painter.save();
    painter.translate(15, height() / 2);
    painter.rotate(-90);
    painter.drawText(0, 0, m_yLabel);
    painter.restore();
    painter.drawText(rect().adjusted(0, 0, 0, -5), Qt::AlignHCenter | Qt::AlignBottom, "Time (ms)");

    // Y ticks
    for (int i = 0; i <= 5; i++) {
        double v = valueMin + (valueMax - valueMin) * i / 5.0;
        QPointF pt = dataToScreen(timeMin, v);
        painter.drawLine(QPointF(pt.x() - 5, pt.y()), QPointF(pt.x(), pt.y()));
        painter.drawText(QRectF(5, pt.y() - 10, leftMargin - 15, 20),
                        Qt::AlignRight | Qt::AlignVCenter,
                        QString::number(v, 'f', 1));
    }

    // X ticks
    for (int i = 0; i <= 5; i++) {
        double t = timeMin + (timeMax - timeMin) * i / 5.0;
        QPointF pt = dataToScreen(t, valueMin);
        painter.drawLine(QPointF(pt.x(), pt.y()), QPointF(pt.x(), pt.y() + 5));
        painter.drawText(QRectF(pt.x() - 30, pt.y() + 5, 60, 20),
                        Qt::AlignHCenter | Qt::AlignTop,
                        QString::number(t, 'f', 0));
    }

    // Data
    if (m_ring.size() > 1) {
        painter.setPen(QPen(Qt::blue, 1.5));
        painter.setClipRect(pr);
        size_t start = 0;
        for (size_t i = 0; i < m_ring.size(); ++i) {
            if (m_ring[i].time >= timeMin) { start = (i > 0) ? i - 1 : 0; break; }
        }
        for (size_t i = start + 1; i < m_ring.size(); ++i) {
            if (m_ring[i - 1].time > timeMax) break;
            painter.drawLine(dataToScreen(m_ring[i-1].time, m_ring[i-1].value),
                           dataToScreen(m_ring[i].time, m_ring[i].value));
        }
        painter.setClipping(false);
    }

    // Rubber band overlay
    if (m_dragging && m_dragStart != m_dragEnd) {
        painter.setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
        painter.setBrush(QColor(100, 100, 255, 30));

        if (m_mode == ZoomXY) {
            painter.drawRect(QRect(m_dragStart, m_dragEnd).normalized());
        } else if (m_mode == ZoomX) {
            // Two vertical lines spanning plot height
            painter.drawLine(m_dragStart.x(), pr.top(), m_dragStart.x(), pr.bottom());
            painter.drawLine(m_dragEnd.x(), pr.top(), m_dragEnd.x(), pr.bottom());
            QRect band(std::min(m_dragStart.x(), m_dragEnd.x()), pr.top(),
                       std::abs(m_dragEnd.x() - m_dragStart.x()), pr.height());
            painter.drawRect(band);
        } else if (m_mode == ZoomY) {
            // Two horizontal lines spanning plot width
            painter.drawLine(pr.left(), m_dragStart.y(), pr.right(), m_dragStart.y());
            painter.drawLine(pr.left(), m_dragEnd.y(), pr.right(), m_dragEnd.y());
            QRect band(pr.left(), std::min(m_dragStart.y(), m_dragEnd.y()),
                       pr.width(), std::abs(m_dragEnd.y() - m_dragStart.y()));
            painter.drawRect(band);
        }
    }
}

void TracePlot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragStart = m_dragEnd = event->pos();
    }
}

void TracePlot::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) return;

    if (m_mode == Pan) {
        int dx = event->pos().x() - m_dragEnd.x();
        int dy = event->pos().y() - m_dragEnd.y();
        QRect pr = plotRect();
        double dt = -(timeMax - timeMin) * dx / pr.width();
        double dv = (valueMax - valueMin) * dy / pr.height();
        timeMin += dt; timeMax += dt;
        valueMin += dv; valueMax += dv;
        autoScale = false;
        emit timeRangeChanged(timeMin, timeMax);
    }

    m_dragEnd = event->pos();
    update();
}

void TracePlot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_dragging) return;
    m_dragging = false;

    if (m_mode == Pan) { update(); return; }

    // Need minimum drag distance to count as a zoom
    QRect pr = plotRect();
    int dx = std::abs(m_dragEnd.x() - m_dragStart.x());
    int dy = std::abs(m_dragEnd.y() - m_dragStart.y());
    if (dx < 5 && dy < 5) { update(); return; }

    double t1, v1, t2, v2;
    screenToData(m_dragStart, t1, v1);
    screenToData(m_dragEnd, t2, v2);

    if (m_mode == ZoomXY) {
        timeMin = std::min(t1, t2); timeMax = std::max(t1, t2);
        valueMin = std::min(v1, v2); valueMax = std::max(v1, v2);
        autoScale = false;
        emit timeRangeChanged(timeMin, timeMax);
    } else if (m_mode == ZoomX) {
        timeMin = std::min(t1, t2); timeMax = std::max(t1, t2);
        emit timeRangeChanged(timeMin, timeMax);
    } else if (m_mode == ZoomY) {
        valueMin = std::min(v1, v2); valueMax = std::max(v1, v2);
        autoScale = false;
    }

    update();
}

void TracePlot::mouseDoubleClickEvent(QMouseEvent *)
{
    resetZoom();
}

void TracePlot::getVisibleData(QVector<double> &times, QVector<double> &values) const
{
    times.clear(); values.clear();
    for (size_t i = 0; i < m_ring.size(); ++i) {
        times.append(m_ring[i].time);
        values.append(m_ring[i].value);
    }
}
