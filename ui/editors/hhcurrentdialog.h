#ifndef HHCURRENTDIALOG_H
#define HHCURRENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QPainter>
#include <vector>
#include <cmath>
#include <algorithm>
#include "RT_HHCurrent.h"

// Simple plot widget for kinetics curves
class KineticsPlotWidget : public QWidget
{
    Q_OBJECT
public:
    struct Trace {
        std::vector<double> data;
        QColor color;
        QString label;
    };

    explicit KineticsPlotWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(250, 120);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setTitle(const QString &t) { m_title = t; }
    void setXLabel(const QString &l) { m_xLabel = l; }
    void setYLabel(const QString &l) { m_yLabel = l; }
    void setXRange(double lo, double hi) { m_xLo = lo; m_xHi = hi; }

    void clearTraces() { m_traces.clear(); }
    void addTrace(const std::vector<double> &data, const QColor &color, const QString &label) {
        m_traces.push_back({data, color, label});
    }

    void replot() { update(); }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), QColor(240, 240, 240));

        const int ml = 50, mr = 10, mt = 20, mb = 25; // margins
        QRect plotRect(ml, mt, width() - ml - mr, height() - mt - mb);
        p.fillRect(plotRect, Qt::white);
        p.setPen(Qt::gray);
        p.drawRect(plotRect);

        if (m_traces.empty()) return;

        // Find Y range across all traces
        double yMin = 1e30, yMax = -1e30;
        for (auto &t : m_traces) {
            for (double v : t.data) {
                if (std::isfinite(v)) { yMin = std::min(yMin, v); yMax = std::max(yMax, v); }
            }
        }
        if (yMin == yMax) { yMin -= 1; yMax += 1; }
        double yPad = (yMax - yMin) * 0.05;
        yMin -= yPad; yMax += yPad;

        // Draw axes labels
        p.setPen(Qt::black);
        QFont f = font(); f.setPointSize(8); p.setFont(f);
        p.drawText(plotRect, Qt::AlignTop | Qt::AlignHCenter, m_title);
        p.save();
        p.translate(8, plotRect.center().y());
        p.rotate(-90);
        p.drawText(QRect(-40, -10, 80, 20), Qt::AlignCenter, m_yLabel);
        p.restore();
        p.drawText(QRect(plotRect.left(), plotRect.bottom() + 2, plotRect.width(), mb - 2),
                   Qt::AlignCenter, m_xLabel);

        // Y tick labels
        f.setPointSize(7); p.setFont(f);
        for (int i = 0; i <= 4; i++) {
            double yv = yMin + (yMax - yMin) * i / 4.0;
            int py = plotRect.bottom() - (double)(i) / 4.0 * plotRect.height();
            p.setPen(QColor(200, 200, 200));
            p.drawLine(plotRect.left(), py, plotRect.right(), py);
            p.setPen(Qt::black);
            p.drawText(QRect(0, py - 8, ml - 4, 16), Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(yv, 'g', 3));
        }

        // X tick labels
        for (int i = 0; i <= 4; i++) {
            double xv = m_xLo + (m_xHi - m_xLo) * i / 4.0;
            int px = plotRect.left() + (double)(i) / 4.0 * plotRect.width();
            p.setPen(QColor(200, 200, 200));
            p.drawLine(px, plotRect.top(), px, plotRect.bottom());
            p.setPen(Qt::black);
            p.drawText(QRect(px - 25, plotRect.bottom() + 2, 50, mb - 2),
                       Qt::AlignCenter, QString::number(xv, 'g', 4));
        }

        // Draw traces
        for (auto &t : m_traces) {
            if (t.data.empty()) continue;
            p.setPen(QPen(t.color, 1.5));
            int n = t.data.size();
            QPointF prev;
            bool havePrev = false;
            for (int i = 0; i < n; i++) {
                if (!std::isfinite(t.data[i])) { havePrev = false; continue; }
                double fx = (double)i / (n - 1);
                double fy = (t.data[i] - yMin) / (yMax - yMin);
                QPointF pt(plotRect.left() + fx * plotRect.width(),
                           plotRect.bottom() - fy * plotRect.height());
                if (havePrev) p.drawLine(prev, pt);
                prev = pt; havePrev = true;
            }
        }

        // Legend
        f.setPointSize(7); p.setFont(f);
        int lx = plotRect.right() - 60, ly = plotRect.top() + 14;
        for (auto &t : m_traces) {
            p.setPen(QPen(t.color, 2));
            p.drawLine(lx, ly, lx + 15, ly);
            p.setPen(Qt::black);
            p.drawText(lx + 18, ly + 4, t.label);
            ly += 12;
        }
    }

private:
    QString m_title, m_xLabel, m_yLabel;
    double m_xLo = -150, m_xHi = 150;
    std::vector<Trace> m_traces;
};


class HHCurrentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HHCurrentDialog(THHCurrent *current, QWidget *parent = nullptr);
private slots:
    void accept() override;
    void updatePlots();
private:
    THHCurrent *m_current;
    QLineEdit *gmaxEdit, *eEdit, *gnoiseEdit, *pEdit, *qEdit, *rEdit;

    struct KineticsEdits {
        QLineEdit *V0, *k, *t_lo, *t_hi, *infMin;
    };
    KineticsEdits mEdits, hEdits, nEdits;

    KineticsPlotWidget *plotIV, *plotInf, *plotTau;

    QGroupBox* makeKineticsGroup(const QString &title, THHKineticsFactor &kf, KineticsEdits &edits);
    void readKinetics(KineticsEdits &edits, THHKineticsFactor &kf);
};

#endif
