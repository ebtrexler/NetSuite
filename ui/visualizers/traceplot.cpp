#include "traceplot.h"
#include <QPainter>
#include <QFontMetrics>
#include <cmath>

static const size_t DEFAULT_CAPACITY = 500000; // ~50s at 10kHz

TracePlot::TracePlot(const QString &title, QWidget *parent)
    : QWidget(parent), title(title), m_ring(DEFAULT_CAPACITY),
      timeMin(0), timeMax(1000),
      valueMin(-80), valueMax(40), autoScale(true), panning(false)
{
    setMinimumHeight(100);
    setMouseTracking(true);
}

void TracePlot::setBufferCapacity(size_t capacity)
{
    m_ring.resize(capacity);
}

void TracePlot::addDataPoint(double time, double value)
{
    m_ring.push({time, value});
    
    // Auto-scroll: keep the visible window following the data
    if (time > timeMax) {
        double window = timeMax - timeMin;
        timeMax = time;
        timeMin = time - window;
    }
    
    if (autoScale) {
        updateValueRange();
    }
    
    update();
}

void TracePlot::clearData()
{
    m_ring.clear();
    update();
}

void TracePlot::setTimeRange(double tMin, double tMax)
{
    timeMin = tMin;
    timeMax = tMax;
    update();
}

void TracePlot::setValueRange(double vMin, double vMax)
{
    valueMin = vMin;
    valueMax = vMax;
    autoScale = false;
    update();
}

void TracePlot::setAutoScale(bool enabled)
{
    autoScale = enabled;
    if (enabled) {
        updateValueRange();
    }
}

void TracePlot::updateValueRange()
{
    if (m_ring.empty()) return;
    
    double vmin = m_ring[0].value;
    double vmax = m_ring[0].value;
    
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

QPointF TracePlot::dataToScreen(double t, double v) const
{
    const int leftMargin = 60;
    const int rightMargin = 20;
    const int topMargin = 30;
    const int bottomMargin = 40;
    
    double plotWidth = width() - leftMargin - rightMargin;
    double plotHeight = height() - topMargin - bottomMargin;
    
    double x = leftMargin + (t - timeMin) / (timeMax - timeMin) * plotWidth;
    double y = topMargin + (1.0 - (v - valueMin) / (valueMax - valueMin)) * plotHeight;
    
    return QPointF(x, y);
}

void TracePlot::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    const int leftMargin = 60;
    const int rightMargin = 20;
    const int topMargin = 30;
    const int bottomMargin = 40;
    
    // Background
    painter.fillRect(rect(), Qt::white);
    
    // Plot area
    QRect plotRect(leftMargin, topMargin, 
                   width() - leftMargin - rightMargin,
                   height() - topMargin - bottomMargin);
    painter.fillRect(plotRect, QColor(250, 250, 250));
    painter.setPen(Qt::black);
    painter.drawRect(plotRect);
    
    // Title
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(rect().adjusted(0, 5, 0, 0), Qt::AlignHCenter | Qt::AlignTop, title);
    
    // Y-axis label
    painter.save();
    painter.translate(15, height() / 2);
    painter.rotate(-90);
    painter.drawText(0, 0, "Voltage (mV)");
    painter.restore();
    
    // X-axis label
    painter.drawText(rect().adjusted(0, 0, 0, -5), Qt::AlignHCenter | Qt::AlignBottom, "Time (ms)");
    
    // Controls hint
    painter.setFont(QFont("Arial", 7));
    painter.setPen(QColor(160, 160, 160));
    painter.drawText(plotRect.adjusted(0, 0, -4, -2), Qt::AlignRight | Qt::AlignBottom,
                     "Scroll: zoom X  |  Ctrl+Scroll: zoom Y  |  Drag: pan  |  Dbl-click: reset");
    
    // Y-axis ticks
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    for (int i = 0; i <= 5; i++) {
        double v = valueMin + (valueMax - valueMin) * i / 5.0;
        QPointF pt = dataToScreen(timeMin, v);
        painter.drawLine(pt.x() - 5, pt.y(), pt.x(), pt.y());
        painter.drawText(QRectF(5, pt.y() - 10, leftMargin - 15, 20), 
                        Qt::AlignRight | Qt::AlignVCenter, 
                        QString::number(v, 'f', 1));
    }
    
    // X-axis ticks
    for (int i = 0; i <= 5; i++) {
        double t = timeMin + (timeMax - timeMin) * i / 5.0;
        QPointF pt = dataToScreen(t, valueMin);
        painter.drawLine(pt.x(), pt.y(), pt.x(), pt.y() + 5);
        painter.drawText(QRectF(pt.x() - 30, pt.y() + 5, 60, 20), 
                        Qt::AlignHCenter | Qt::AlignTop, 
                        QString::number(t, 'f', 0));
    }
    
    // Plot data from ring buffer
    if (m_ring.size() > 1) {
        painter.setPen(QPen(Qt::blue, 1.5));
        painter.setClipRect(plotRect);

        // Find first visible point via binary-style scan
        size_t start = 0;
        for (size_t i = 0; i < m_ring.size(); ++i) {
            if (m_ring[i].time >= timeMin) { start = (i > 0) ? i - 1 : 0; break; }
        }

        for (size_t i = start + 1; i < m_ring.size(); ++i) {
            if (m_ring[i - 1].time > timeMax) break;
            QPointF p1 = dataToScreen(m_ring[i-1].time, m_ring[i-1].value);
            QPointF p2 = dataToScreen(m_ring[i].time, m_ring[i].value);
            painter.drawLine(p1, p2);
        }

        painter.setClipping(false);
    }
}

void TracePlot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        panning = true;
        lastMousePos = event->pos();
    }
}

void TracePlot::mouseMoveEvent(QMouseEvent *event)
{
    if (panning) {
        int dx = event->pos().x() - lastMousePos.x();
        double dt = -(timeMax - timeMin) * dx / (width() - 80);
        
        timeMin += dt;
        timeMax += dt;
        
        emit timeRangeChanged(timeMin, timeMax);
        lastMousePos = event->pos();
        update();
    }
}

void TracePlot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        panning = false;
    }
}

void TracePlot::mouseDoubleClickEvent(QMouseEvent *)
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

void TracePlot::wheelEvent(QWheelEvent *event)
{
    double steps = event->angleDelta().y() / 120.0;
    double zoomFactor = std::pow(0.97, steps);
    
    if (event->modifiers() & Qt::ControlModifier) {
        double center = (valueMin + valueMax) / 2;
        double range = (valueMax - valueMin) * zoomFactor / 2;
        valueMin = center - range;
        valueMax = center + range;
        autoScale = false;
    } else {
        double center = (timeMin + timeMax) / 2;
        double range = (timeMax - timeMin) * zoomFactor / 2;
        timeMin = center - range;
        timeMax = center + range;
        emit timeRangeChanged(timeMin, timeMax);
    }
    
    update();
}

void TracePlot::getVisibleData(QVector<double> &times, QVector<double> &values) const
{
    times.clear();
    values.clear();
    for (size_t i = 0; i < m_ring.size(); ++i) {
        times.append(m_ring[i].time);
        values.append(m_ring[i].value);
    }
}
