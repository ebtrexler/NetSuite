#ifndef TRACEPLOT_H
#define TRACEPLOT_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

class TracePlot : public QWidget
{
    Q_OBJECT

public:
    explicit TracePlot(const QString &title, QWidget *parent = nullptr);
    
    void addDataPoint(double time, double value);
    void clearData();
    void setTimeRange(double tMin, double tMax);
    void setValueRange(double vMin, double vMax);
    void setAutoScale(bool enabled);
    
signals:
    void timeRangeChanged(double tMin, double tMax);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    
private:
    QString title;
    QVector<double> timeData;
    QVector<double> valueData;
    
    double timeMin, timeMax;
    double valueMin, valueMax;
    bool autoScale;
    
    bool panning;
    QPoint lastMousePos;
    
    void updateValueRange();
    QPointF dataToScreen(double t, double v) const;
};

#endif // TRACEPLOT_H
