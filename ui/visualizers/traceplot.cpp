#include "traceplot.h"
#include <QPainter>
#include <QFontMetrics>
#include <cmath>

TracePlot::TracePlot(const QString &title, QWidget *parent)
    : QWidget(parent), title(title), timeMin(0), timeMax(1000), 
      valueMin(-80), valueMax(40), autoScale(true), panning(false)
{
    setMinimumHeight(100);
    setMouseTracking(true);
}

void TracePlot::addDataPoint(double time, double value)
{
    timeData.append(time);
    valueData.append(value);
    
    if (autoScale) {
        updateValueRange();
    }
    
    update();
}

void TracePlot::clearData()
{
    timeData.clear();
    valueData.clear();
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
    if (valueData.isEmpty()) return;
    
    valueMin = valueData[0];
    valueMax = valueData[0];
    
    for (double v : valueData) {
        if (v < valueMin) valueMin = v;
        if (v > valueMax) valueMax = v;
    }
    
    double margin = (valueMax - valueMin) * 0.1;
    valueMin -= margin;
    valueMax += margin;
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
    
    // Y-axis ticks
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
    
    // Plot data
    if (timeData.size() > 1) {
        painter.setPen(QPen(Qt::blue, 1.5));
        for (int i = 1; i < timeData.size(); i++) {
            if (timeData[i] >= timeMin && timeData[i-1] <= timeMax) {
                QPointF p1 = dataToScreen(timeData[i-1], valueData[i-1]);
                QPointF p2 = dataToScreen(timeData[i], valueData[i]);
                painter.drawLine(p1, p2);
            }
        }
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

void TracePlot::wheelEvent(QWheelEvent *event)
{
    double zoomFactor = event->angleDelta().y() > 0 ? 0.9 : 1.1;
    
    if (event->modifiers() & Qt::ControlModifier) {
        // Zoom Y
        double center = (valueMin + valueMax) / 2;
        double range = (valueMax - valueMin) * zoomFactor / 2;
        valueMin = center - range;
        valueMax = center + range;
        autoScale = false;
    } else {
        // Zoom X
        double center = (timeMin + timeMax) / 2;
        double range = (timeMax - timeMin) * zoomFactor / 2;
        timeMin = center - range;
        timeMax = center + range;
        emit timeRangeChanged(timeMin, timeMax);
    }
    
    update();
}
