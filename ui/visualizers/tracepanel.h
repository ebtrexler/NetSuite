#ifndef TRACEPANEL_H
#define TRACEPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "traceplot.h"

class TracePanel : public QWidget
{
    Q_OBJECT

public:
    explicit TracePanel(QWidget *parent = nullptr);
    
    void setNumTraces(int count);
    void addDataPoint(int traceIndex, double time, double value);
    void clearAllData();
    void setTraceTitle(int index, const QString &title);
    void setTraceYLabel(int index, const QString &label);
    void setTimeRange(double tMin, double tMax);
    void setBufferCapacity(size_t capacity);
    bool exportCsv(const QString &filename) const;
    bool exportImage(const QString &filename);
    
private slots:
    void onTimeRangeChanged(double tMin, double tMax);
    void onModeChanged(int id);
    void onResetZoom();
    
private:
    QVBoxLayout *plotLayout;
    QVector<TracePlot*> plots;
    int maxPlots = 8;
    TracePlot::InteractionMode m_currentMode = TracePlot::ZoomXY;
};

#endif // TRACEPANEL_H
