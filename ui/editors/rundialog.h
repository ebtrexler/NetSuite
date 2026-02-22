#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QSplitter>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>

#include "daq_interface.h"
#include "daq_mock.h"
#include "RT_Network.h"
#include "RT_Electrode.h"
#include "tracepanel.h"

// Tracks which network object a displayed plot refers to
struct PlotBinding {
    enum Type { CellVm, ElectrodeCurrent };
    Type type;
    std::wstring name;
    int cellIdx;       // index into Vm_out for CellVm
    TElectrode *trode; // pointer for ElectrodeCurrent
};

class RunDialog : public QDialog
{
    Q_OBJECT
public:
    RunDialog(TNetwork *network, DAQInterface *daq, QWidget *parent = nullptr)
        : QDialog(parent), m_net(network), m_daq(daq)
    {
        setWindowTitle("Run Dynamic Clamp");
        resize(1100, 700);

        auto *mainLayout = new QHBoxLayout(this);

        // === Left panel: settings ===
        auto *leftPanel = new QVBoxLayout;

        auto *rateGroup = new QGroupBox("Acquisition");
        auto *rateLayout = new QFormLayout(rateGroup);
        sampleRateEdit = new QDoubleSpinBox;
        sampleRateEdit->setRange(1, 100000);
        sampleRateEdit->setValue(10000);
        sampleRateEdit->setDecimals(0);
        rateLayout->addRow("Sample Rate (Hz):", sampleRateEdit);
        coercedLabel = new QLabel("—");
        rateLayout->addRow("Coerced Rate:", coercedLabel);
        rk4StepEdit = new QDoubleSpinBox;
        rk4StepEdit->setRange(0.001, 10);
        rk4StepEdit->setValue(m_net->GetMaxRK4Timestep());
        rk4StepEdit->setDecimals(3);
        rateLayout->addRow("Max RK4 step (ms):", rk4StepEdit);
        leftPanel->addWidget(rateGroup);

        auto *timeGroup = new QGroupBox("Timing (seconds)");
        auto *timeLayout = new QFormLayout(timeGroup);
        timeBeforeEdit = new QDoubleSpinBox;
        timeBeforeEdit->setRange(0, 3600); timeBeforeEdit->setValue(0);
        timeLayout->addRow("Before:", timeBeforeEdit);
        durationEdit = new QDoubleSpinBox;
        durationEdit->setRange(0.001, 3600); durationEdit->setValue(1.0);
        timeLayout->addRow("Duration:", durationEdit);
        timeAfterEdit = new QDoubleSpinBox;
        timeAfterEdit->setRange(0, 3600); timeAfterEdit->setValue(0);
        timeLayout->addRow("After:", timeAfterEdit);
        leftPanel->addWidget(timeGroup);

        auto *repGroup = new QGroupBox("Repeats");
        auto *repLayout = new QFormLayout(repGroup);
        repeatSpin = new QSpinBox;
        repeatSpin->setRange(0, 10000); repeatSpin->setValue(1);
        repLayout->addRow("Repeats (0=∞):", repeatSpin);
        resetCheck = new QCheckBox("Reset between runs");
        resetCheck->setChecked(true);
        repLayout->addRow(resetCheck);
        leftPanel->addWidget(repGroup);

        auto *interpGroup = new QGroupBox("Interpolation");
        auto *interpLayout = new QFormLayout(interpGroup);
        interpolateCheck = new QCheckBox("Interpolate");
        interpLayout->addRow(interpolateCheck);
        interpRateEdit = new QDoubleSpinBox;
        interpRateEdit->setRange(100, 1000000);
        interpRateEdit->setValue(100000);
        interpRateEdit->setDecimals(0);
        interpLayout->addRow("Rate (Hz):", interpRateEdit);
        leftPanel->addWidget(interpGroup);

        auto *saveGroup = new QGroupBox("Data");
        auto *saveLayout = new QFormLayout(saveGroup);
        saveCheck = new QCheckBox("Save data to file");
        saveLayout->addRow(saveCheck);
        leftPanel->addWidget(saveGroup);

        leftPanel->addStretch();

        // Buttons
        startBtn = new QPushButton("Start");
        stopBtn = new QPushButton("Stop");
        stopBtn->setEnabled(false);
        auto *closeBtn = new QPushButton("Close");
        leftPanel->addWidget(startBtn);
        leftPanel->addWidget(stopBtn);
        leftPanel->addWidget(closeBtn);

        progressBar = new QProgressBar;
        progressBar->setRange(0, 100);
        leftPanel->addWidget(progressBar);

        statusLabel = new QLabel("Ready");
        statusLabel->setWordWrap(true);
        leftPanel->addWidget(statusLabel);

        statsLabel = new QLabel("");
        statsLabel->setWordWrap(true);
        leftPanel->addWidget(statsLabel);

        auto *leftWidget = new QWidget;
        leftWidget->setLayout(leftPanel);
        leftWidget->setMaximumWidth(230);

        // === Middle panel: plot selection ===
        auto *midPanel = new QVBoxLayout;

        midPanel->addWidget(new QLabel("Cells (mV):"));
        cellsList = new QListWidget;
        cellsList->setSelectionMode(QAbstractItemView::MultiSelection);
        midPanel->addWidget(cellsList);

        midPanel->addWidget(new QLabel("Electrodes (nA):"));
        electrodesList = new QListWidget;
        electrodesList->setSelectionMode(QAbstractItemView::MultiSelection);
        midPanel->addWidget(electrodesList);

        auto *addBtn = new QPushButton("Add to Display ▶");
        auto *removeBtn = new QPushButton("◀ Remove");
        midPanel->addWidget(addBtn);
        midPanel->addWidget(removeBtn);

        midPanel->addWidget(new QLabel("Displayed Plots:"));
        displayedList = new QListWidget;
        displayedList->setSelectionMode(QAbstractItemView::MultiSelection);
        midPanel->addWidget(displayedList);

        auto *midWidget = new QWidget;
        midWidget->setLayout(midPanel);
        midWidget->setMaximumWidth(180);

        // === Right panel: trace plots ===
        tracePanel = new TracePanel;

        // Assemble
        auto *splitter = new QSplitter(Qt::Horizontal);
        splitter->addWidget(leftWidget);
        splitter->addWidget(midWidget);
        splitter->addWidget(tracePanel);
        splitter->setStretchFactor(2, 1);
        mainLayout->addWidget(splitter);

        populateLists();

        connect(startBtn, &QPushButton::clicked, this, &RunDialog::onStart);
        connect(stopBtn, &QPushButton::clicked, this, &RunDialog::onStop);
        connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
        connect(addBtn, &QPushButton::clicked, this, &RunDialog::addToDisplay);
        connect(removeBtn, &QPushButton::clicked, this, &RunDialog::removeFromDisplay);

        runTimer = new QTimer(this);
        connect(runTimer, &QTimer::timeout, this, &RunDialog::runStep);
    }

private slots:
    void onStart() {
        if (m_running) return;

        if (displayedList->count() == 0) {
            QMessageBox::warning(this, "Error", "Please choose plots to display");
            return;
        }
        if (durationEdit->value() <= 0) {
            QMessageBox::warning(this, "Error", "Duration must be > 0");
            return;
        }

        // Ask for save file if saving
        if (saveCheck->isChecked()) {
            m_saveFile = QFileDialog::getSaveFileName(this, "Save Data", "", "CSV Files (*.csv)");
            if (m_saveFile.isEmpty()) return;
        } else {
            m_saveFile.clear();
        }

        m_net->SetMaxRK4Timestep(rk4StepEdit->value());
        m_netDesc = m_net->DescribeNetwork();
        m_net->Initialize(true);

        m_sampleRate = sampleRateEdit->value();
        m_secBefore = timeBeforeEdit->value();
        m_secDuring = durationEdit->value();
        m_secAfter = timeAfterEdit->value();
        m_totalSec = m_secBefore + m_secDuring + m_secAfter;
        m_numRepeats = repeatSpin->value();
        m_currentRep = 0;
        m_terminated = false;
        m_doInterpolate = interpolateCheck->isChecked();
        m_interpRate = interpRateEdit->value();

        // Build plot bindings
        buildPlotBindings();

        int numPlots = std::min(static_cast<int>(m_bindings.size()), 8);
        tracePanel->setNumTraces(numPlots);
        tracePanel->clearAllData();
        for (int i = 0; i < numPlots; ++i)
            tracePanel->setTraceTitle(i, displayedList->item(i)->text());

        // Open save file for streaming
        openSaveFile();

        // Reset perf stats
        m_totalReads = 0;
        m_totalSamplesRead = 0;
        m_perfTimer.start();

        if (m_netDesc.NumVDepCells > 0 && m_daq) {
            startDAQRun();
        } else {
            startSimRun();
        }
    }

    void onStop() { m_terminated = true; }

    void addToDisplay() {
        auto addSelected = [&](QListWidget *src) {
            for (auto *item : src->selectedItems()) {
                if (displayedList->findItems(item->text(), Qt::MatchExactly).isEmpty())
                    displayedList->addItem(item->text());
            }
        };
        addSelected(cellsList);
        addSelected(electrodesList);
    }

    void removeFromDisplay() {
        for (auto *item : displayedList->selectedItems()) delete item;
    }

    void runStep() {
        if (m_terminated) { finishRun(); return; }
        if (m_useDAQ) daqStep(); else simStep();
    }

private:
    void populateLists() {
        cellsList->clear();
        electrodesList->clear();
        for (auto &pair : m_net->GetCells())
            if (pair.second->IsActive())
                cellsList->addItem(QString::fromStdWString(pair.first));
        for (auto &pair : m_net->GetElectrodes())
            if (pair.second->IsActive())
                electrodesList->addItem(QString::fromStdWString(pair.first));
        // Auto-populate
        if (displayedList->count() == 0)
            for (int i = 0; i < cellsList->count() && i < 8; ++i)
                displayedList->addItem(cellsList->item(i)->text());
    }

    void buildPlotBindings() {
        m_bindings.clear();
        for (int p = 0; p < displayedList->count() && p < 8; ++p) {
            QString name = displayedList->item(p)->text();
            std::wstring wname = name.toStdWString();
            // Check cells
            int idx = 0;
            bool found = false;
            for (auto &pair : m_net->GetCells()) {
                if (pair.first == wname) {
                    m_bindings.push_back({PlotBinding::CellVm, wname, idx, nullptr});
                    found = true;
                    break;
                }
                idx++;
            }
            if (found) continue;
            // Check electrodes
            for (auto &pair : m_net->GetElectrodes()) {
                if (pair.first == wname) {
                    m_bindings.push_back({PlotBinding::ElectrodeCurrent, wname, -1, pair.second.get()});
                    break;
                }
            }
        }
    }

    // --- Simulation-only run ---
    void startSimRun() {
        m_useDAQ = false;
        m_simTime = 0;
        m_step_ms = 1000.0 / m_sampleRate;
        m_totalSamples = static_cast<int>(m_secDuring * m_sampleRate);
        m_sampleCount = 0;
        coercedLabel->setText(QString::number(m_sampleRate));
        setRunning(true);
        runTimer->start(10);
    }

    void simStep() {
        int stepsPerTick = std::min(1000, m_totalSamples - m_sampleCount);
        int numCells = m_net->GetCells().size();
        double Vm_out[6] = {};

        for (int s = 0; s < stepsPerTick && !m_terminated; ++s) {
            if (m_doInterpolate) {
                double interpStep = 1000.0 / m_interpRate;
                int substeps = static_cast<int>(m_step_ms / interpStep);
                if (substeps < 1) substeps = 1;
                double subdt = m_step_ms / substeps;
                for (int ss = 0; ss < substeps; ++ss)
                    m_net->Update(subdt, nullptr, Vm_out, nullptr);
            } else {
                m_net->Update(m_step_ms, nullptr, Vm_out, nullptr);
            }
            m_simTime += m_step_ms;
            m_sampleCount++;
            m_totalSamplesRead++;

            if (s % 10 == 0)
                recordDataPoint(Vm_out, numCells, m_simTime);
        }

        double progress = 100.0 * m_sampleCount / m_totalSamples;
        progressBar->setValue(static_cast<int>(progress));
        statusLabel->setText(QString("Sim: t=%1 ms").arg(m_simTime, 0, 'f', 1));

        if (m_sampleCount >= m_totalSamples) {
            m_currentRep++;
            if (shouldContinue()) {
                if (resetCheck->isChecked()) m_net->Initialize(true);
                m_sampleCount = 0;
                m_simTime = 0;
                tracePanel->clearAllData();
            } else {
                m_terminated = true;
            }
        }
    }

    // --- DAQ run ---
    void startDAQRun() {
        m_useDAQ = true;
        try {
            std::string aiChans(m_netDesc.AIChans.begin(), m_netDesc.AIChans.end());
            std::string aoChans(m_netDesc.AOChans.begin(), m_netDesc.AOChans.end());

            m_sampleRate = m_daq->configureAI(aiChans, m_sampleRate);
            m_daq->configureAO(aoChans);
            coercedLabel->setText(QString::number(m_sampleRate));

            m_step_ms = 1000.0 / m_sampleRate;
            m_totalSamples = static_cast<int>(m_totalSec * m_sampleRate);
            m_sampleCount = 0;
            m_simTime = 0;
            m_numAI = m_daq->numAIChannels();
            m_readBuf.resize(static_cast<int>(0.01 * m_sampleRate * m_numAI));
            m_I_nA.resize(m_netDesc.NumVDepCells, 0.0);

            m_daq->start();
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);

            setRunning(true);
            runTimer->start(1);
        } catch (const DAQException &e) {
            QMessageBox::critical(this, "DAQ Error", e.what());
        }
    }

    void daqStep() {
        int numCells = m_netDesc.NumVDepCells + m_netDesc.NumTimeCells;
        double Vm_out[6] = {};

        int32_t read = m_daq->readAI(m_readBuf.data(), static_cast<int32_t>(m_readBuf.size()));
        if (read <= 0) return;

        m_totalReads++;
        m_totalSamplesRead += read;
        m_sampleCount += read;
        m_simTime += read * m_step_ms;

        double *lastSample = &m_readBuf[(read - 1) * m_numAI];

        if (m_doInterpolate && read > 0) {
            double interpStep = 1000.0 / m_interpRate;
            int substeps = static_cast<int>((read * m_step_ms) / interpStep);
            if (substeps < 1) substeps = 1;
            double subdt = (read * m_step_ms) / substeps;
            for (int ss = 0; ss < substeps; ++ss)
                m_net->Update(subdt, lastSample, Vm_out, m_I_nA.data());
        } else {
            m_net->Update(read * m_step_ms, lastSample, Vm_out, m_I_nA.data());
        }

        // Phase logic
        double secElapsed = m_sampleCount / m_sampleRate;
        bool duringPhase = (secElapsed >= m_secBefore) &&
                           (secElapsed < m_secBefore + m_secDuring);

        if (duringPhase) {
            for (auto &v : m_I_nA) v = std::max(-10.0, std::min(10.0, v));
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);
        } else {
            std::fill(m_I_nA.begin(), m_I_nA.end(), 0.0);
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);
        }

        recordDataPoint(Vm_out, numCells, m_simTime);

        double progress = 100.0 * m_sampleCount / m_totalSamples;
        progressBar->setValue(std::min(100, static_cast<int>(progress)));
        statusLabel->setText(QString("DAQ: t=%1 s").arg(secElapsed, 0, 'f', 2));
        updateStats();

        if (m_sampleCount >= m_totalSamples) {
            std::fill(m_I_nA.begin(), m_I_nA.end(), 0.0);
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);

            m_currentRep++;
            if (shouldContinue()) {
                if (resetCheck->isChecked()) m_net->Initialize(true);
                m_sampleCount = 0;
                m_simTime = 0;
                tracePanel->clearAllData();
            } else {
                m_terminated = true;
            }
        }
    }

    // --- Data recording (plot + stream to disk) ---
    void recordDataPoint(double *Vm_out, int numCells, double time_ms) {
        for (size_t p = 0; p < m_bindings.size() && p < 8; ++p) {
            double val = 0;
            auto &b = m_bindings[p];
            if (b.type == PlotBinding::CellVm && b.cellIdx < numCells)
                val = Vm_out[b.cellIdx];
            else if (b.type == PlotBinding::ElectrodeCurrent && b.trode)
                val = b.trode->LastCurrent();
            tracePanel->addDataPoint(static_cast<int>(p), time_ms, val);

            if (m_saveStream) {
                if (p == 0) *m_saveStream << (time_ms / 1000.0);
                *m_saveStream << "," << val;
            }
        }
        if (m_saveStream) *m_saveStream << "\n";
    }

    // --- Performance stats ---
    void updateStats() {
        if (m_totalReads == 0) return;
        double avgSamples = static_cast<double>(m_totalSamplesRead) / m_totalReads;
        double elapsed_s = m_perfTimer.elapsed() / 1000.0;
        statsLabel->setText(QString("Reads: %1 | Avg samp/read: %2\nElapsed: %3 s | Rep: %4")
            .arg(m_totalReads)
            .arg(avgSamples, 0, 'f', 1)
            .arg(elapsed_s, 0, 'f', 1)
            .arg(m_currentRep + 1));
    }

    // --- CSV streaming ---
    void openSaveFile() {
        closeSaveFile();
        if (m_saveFile.isEmpty()) return;

        m_saveFileObj.reset(new QFile(m_saveFile));
        if (!m_saveFileObj->open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Save Error", "Could not open file for writing");
            m_saveFileObj.reset();
            m_saveFile.clear();
            return;
        }
        m_saveStream.reset(new QTextStream(m_saveFileObj.get()));
        m_saveStream->setRealNumberPrecision(6);

        // Header
        *m_saveStream << "Time (s)";
        for (size_t i = 0; i < m_bindings.size() && i < 8; ++i) {
            *m_saveStream << "," << QString::fromStdWString(m_bindings[i].name);
            if (m_bindings[i].type == PlotBinding::CellVm)
                *m_saveStream << " (mV)";
            else
                *m_saveStream << " (nA)";
        }
        *m_saveStream << "\n";
    }

    void closeSaveFile() {
        m_saveStream.reset();
        m_saveFileObj.reset();
    }

    bool shouldContinue() {
        if (m_terminated) return false;
        if (m_numRepeats == 0) return true;
        return m_currentRep < m_numRepeats;
    }

    void finishRun() {
        runTimer->stop();
        if (m_useDAQ && m_daq) {
            try { m_daq->stop(); } catch (...) {}
        }
        setRunning(false);
        updateStats();
        closeSaveFile();
        statusLabel->setText(m_saveFile.isEmpty() ? "Complete" :
            QString("Complete — saved to %1").arg(m_saveFile));
        progressBar->setValue(100);
    }

    void setRunning(bool running) {
        m_running = running;
        startBtn->setEnabled(!running);
        stopBtn->setEnabled(running);
        sampleRateEdit->setEnabled(!running);
        durationEdit->setEnabled(!running);
        timeBeforeEdit->setEnabled(!running);
        timeAfterEdit->setEnabled(!running);
    }

    // Network & DAQ
    TNetwork *m_net;
    DAQInterface *m_daq;
    NetDescriptionStruct m_netDesc;

    // UI
    QDoubleSpinBox *sampleRateEdit, *rk4StepEdit;
    QDoubleSpinBox *timeBeforeEdit, *durationEdit, *timeAfterEdit;
    QDoubleSpinBox *interpRateEdit;
    QSpinBox *repeatSpin;
    QCheckBox *resetCheck, *saveCheck, *interpolateCheck;
    QPushButton *startBtn, *stopBtn;
    QProgressBar *progressBar;
    QLabel *coercedLabel, *statusLabel, *statsLabel;
    QListWidget *cellsList, *electrodesList, *displayedList;
    TracePanel *tracePanel;
    QTimer *runTimer;

    // Run state
    bool m_running = false, m_terminated = false, m_useDAQ = false;
    bool m_doInterpolate = false;
    double m_sampleRate = 10000, m_step_ms = 0.1;
    double m_secBefore = 0, m_secDuring = 1, m_secAfter = 0, m_totalSec = 1;
    double m_simTime = 0, m_interpRate = 100000;
    int m_totalSamples = 0, m_sampleCount = 0;
    int m_numRepeats = 1, m_currentRep = 0, m_numAI = 0;
    std::vector<double> m_readBuf, m_I_nA;

    // Plot bindings
    std::vector<PlotBinding> m_bindings;

    // Data saving (streaming)
    QString m_saveFile;
    std::unique_ptr<QFile> m_saveFileObj;
    std::unique_ptr<QTextStream> m_saveStream;

    // Performance stats
    QElapsedTimer m_perfTimer;
    int64_t m_totalReads = 0, m_totalSamplesRead = 0;
};

#endif
