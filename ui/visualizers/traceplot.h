#ifndef TRACEPLOT_H
#define TRACEPLOT_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "ring_buffer.h"

struct TracePoint {
    double time;
    double value;
};

class TracePlot : public QWidget
{
    Q_OBJECT

public:
    enum InteractionMode { ZoomXY, ZoomX, ZoomY, Pan };

    explicit TracePlot(const QString &title, QWidget *parent = nullptr);
    
    void addDataPoint(double time, double value);
    void clearData();
    void setTimeRange(double tMin, double tMax);
    void setValueRange(double vMin, double vMax);
    void setAutoScale(bool enabled);
    void setTitle(const QString &t) { title = t; update(); }
    void setYLabel(const QString &label) { m_yLabel = label; update(); }
    void setBufferCapacity(size_t capacity);
    void setInteractionMode(InteractionMode mode) { m_mode = mode; }
    void resetZoom();
    const QString& getTitle() const { return title; }
    void getVisibleData(QVector<double> &times, QVector<double> &values) const;
    
signals:
    void timeRangeChanged(double tMin, double tMax);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    
private:
    QString title;
    QString m_yLabel = "Voltage (mV)";
    RingBuffer<TracePoint> m_ring;
    
    double timeMin, timeMax;
    double valueMin, valueMax;
    bool autoScale;

    InteractionMode m_mode = ZoomXY;
    bool m_dragging = false;
    QPoint m_dragStart, m_dragEnd;
    
    static const int leftMargin = 60, rightMargin = 20;
    static const int topMargin = 30, bottomMargin = 40;

    QRect plotRect() const;
    void updateValueRange();
    QPointF dataToScreen(double t, double v) const;
    void screenToData(const QPoint &p, double &t, double &v) const;
};

#endif // TRACEPLOT_H
