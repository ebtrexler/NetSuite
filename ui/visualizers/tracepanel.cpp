#include "tracepanel.h"

TracePanel::TracePanel(QWidget *parent)
    : QWidget(parent)
{
    auto *outerLayout = new QHBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(2);

    // Mode toolbar on the left
    auto *toolbar = new QVBoxLayout;
    toolbar->setSpacing(2);

    auto *group = new QButtonGroup(this);

    auto makeBtn = [&](const QString &text, const QString &tip, int id) {
        auto *btn = new QToolButton;
        btn->setText(text);
        btn->setToolTip(tip);
        btn->setCheckable(true);
        btn->setFixedSize(32, 32);
        group->addButton(btn, id);
        toolbar->addWidget(btn);
        return btn;
    };

    makeBtn("⊞", "Zoom XY — drag rectangle", TracePlot::ZoomXY)->setChecked(true);
    makeBtn("↔", "Zoom X — drag vertical lines", TracePlot::ZoomX);
    makeBtn("↕", "Zoom Y — drag horizontal lines", TracePlot::ZoomY);
    makeBtn("✋", "Pan — drag to scroll", TracePlot::Pan);

    toolbar->addSpacing(8);

    auto *resetBtn = new QToolButton;
    resetBtn->setText("⊙");
    resetBtn->setToolTip("Reset zoom — fit all data");
    resetBtn->setFixedSize(32, 32);
    toolbar->addWidget(resetBtn);

    toolbar->addStretch();
    outerLayout->addLayout(toolbar);

    // Plot area on the right
    plotLayout = new QVBoxLayout;
    plotLayout->setSpacing(2);
    plotLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addLayout(plotLayout, 1);

    connect(group, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &TracePanel::onModeChanged);
    connect(resetBtn, &QToolButton::clicked, this, &TracePanel::onResetZoom);
}

void TracePanel::onModeChanged(int id)
{
    m_currentMode = static_cast<TracePlot::InteractionMode>(id);
    for (TracePlot *plot : plots)
        plot->setInteractionMode(m_currentMode);
}

void TracePanel::onResetZoom()
{
    for (TracePlot *plot : plots)
        plot->resetZoom();
}

void TracePanel::setNumTraces(int count)
{
    if (count > maxPlots) count = maxPlots;
    
    while (plots.size() > count) {
        TracePlot *plot = plots.takeLast();
        plotLayout->removeWidget(plot);
        delete plot;
    }
    
    while (plots.size() < count) {
        TracePlot *plot = new TracePlot(QString("Cell %1").arg(plots.size() + 1), this);
        plot->setInteractionMode(m_currentMode);
        connect(plot, &TracePlot::timeRangeChanged, this, &TracePanel::onTimeRangeChanged);
        plots.append(plot);
        plotLayout->addWidget(plot);
    }
}

void TracePanel::addDataPoint(int traceIndex, double time, double value)
{
    if (traceIndex >= 0 && traceIndex < plots.size())
        plots[traceIndex]->addDataPoint(time, value);
}

void TracePanel::clearAllData()
{
    for (TracePlot *plot : plots) plot->clearData();
}

void TracePanel::onTimeRangeChanged(double tMin, double tMax)
{
    for (TracePlot *plot : plots) plot->setTimeRange(tMin, tMax);
}

void TracePanel::setTraceTitle(int index, const QString &title)
{
    if (index >= 0 && index < plots.size()) plots[index]->setTitle(title);
}

void TracePanel::setTimeRange(double tMin, double tMax)
{
    for (TracePlot *plot : plots) plot->setTimeRange(tMin, tMax);
}

void TracePanel::setBufferCapacity(size_t capacity)
{
    for (TracePlot *plot : plots) plot->setBufferCapacity(capacity);
}

bool TracePanel::exportCsv(const QString &filename) const
{
    if (plots.isEmpty()) return false;
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    
    out << "Time (ms)";
    for (auto *p : plots) out << "," << p->getTitle() << " (mV)";
    out << "\n";
    
    QVector<double> times, vals;
    plots[0]->getVisibleData(times, vals);

    for (int i = 0; i < times.size(); i++) {
        out << times[i];
        for (int p = 0; p < plots.size(); ++p) {
            if (p == 0) {
                out << "," << (i < vals.size() ? vals[i] : 0.0);
            } else {
                QVector<double> t2, v2;
                plots[p]->getVisibleData(t2, v2);
                out << "," << (i < v2.size() ? v2[i] : 0.0);
            }
        }
        out << "\n";
    }
    return true;
}
