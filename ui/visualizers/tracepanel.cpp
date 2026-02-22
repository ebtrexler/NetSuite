#include "tracepanel.h"

TracePanel::TracePanel(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);
}

void TracePanel::setNumTraces(int count)
{
    if (count > maxPlots) count = maxPlots;
    
    // Remove excess plots
    while (plots.size() > count) {
        TracePlot *plot = plots.takeLast();
        layout->removeWidget(plot);
        delete plot;
    }
    
    // Add new plots
    while (plots.size() < count) {
        TracePlot *plot = new TracePlot(QString("Cell %1").arg(plots.size() + 1), this);
        connect(plot, &TracePlot::timeRangeChanged, this, &TracePanel::onTimeRangeChanged);
        plots.append(plot);
        layout->addWidget(plot);
    }
}

void TracePanel::addDataPoint(int traceIndex, double time, double value)
{
    if (traceIndex >= 0 && traceIndex < plots.size()) {
        plots[traceIndex]->addDataPoint(time, value);
    }
}

void TracePanel::clearAllData()
{
    for (TracePlot *plot : plots) {
        plot->clearData();
    }
}

void TracePanel::onTimeRangeChanged(double tMin, double tMax)
{
    for (TracePlot *plot : plots) {
        plot->setTimeRange(tMin, tMax);
    }
}

void TracePanel::setTraceTitle(int index, const QString &title)
{
    if (index >= 0 && index < plots.size()) {
        plots[index]->setTitle(title);
    }
}

void TracePanel::setTimeRange(double tMin, double tMax)
{
    for (TracePlot *plot : plots) {
        plot->setTimeRange(tMin, tMax);
    }
}

bool TracePanel::exportCsv(const QString &filename) const
{
    if (plots.isEmpty()) return false;
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    
    // Header
    out << "Time (ms)";
    for (auto *p : plots) out << "," << p->getTitle() << " (mV)";
    out << "\n";
    
    // Use the first plot's time data as reference
    const QVector<double> &times = plots[0]->getTimeData();
    for (int i = 0; i < times.size(); i++) {
        out << times[i];
        for (auto *p : plots) {
            const QVector<double> &vals = p->getValueData();
            out << "," << (i < vals.size() ? vals[i] : 0.0);
        }
        out << "\n";
    }
    return true;
}
