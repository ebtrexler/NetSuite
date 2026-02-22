#ifndef TRACEPANEL_H
#define TRACEPANEL_H

#include <QWidget>
#include <QVBoxLayout>
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
    void setTimeRange(double tMin, double tMax);
    void setBufferCapacity(size_t capacity);
    bool exportCsv(const QString &filename) const;
    
private slots:
    void onTimeRangeChanged(double tMin, double tMax);
    
private:
    QVBoxLayout *layout;
    QVector<TracePlot*> plots;
    int maxPlots = 8;
};

#endif // TRACEPANEL_H
