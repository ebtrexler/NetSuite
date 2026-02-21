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
#include <QTextStream>
#include <QFile>
#include <memory>
#include <vector>
#include <cmath>

#include "daq_interface.h"
#include "daq_mock.h"
#include "RT_Network.h"
#include "tracepanel.h"

class RunDialog : public QDialog
{
    Q_OBJECT
public:
    RunDialog(TNetwork *network, DAQInterface *daq, QWidget *parent = nullptr)
        : QDialog(parent), m_net(network), m_daq(daq)
    {
        setWindowTitle("Run Dynamic Clamp");
        resize(1000, 700);

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
        leftPanel->addWidget(statusLabel);

        auto *leftWidget = new QWidget;
        leftWidget->setLayout(leftPanel);
        leftWidget->setMaximumWidth(220);

        // === Middle panel: plot selection ===
        auto *midPanel = new QVBoxLayout;

        midPanel->addWidget(new QLabel("Cells (voltages):"));
        cellsList = new QListWidget;
        cellsList->setSelectionMode(QAbstractItemView::MultiSelection);
        midPanel->addWidget(cellsList);

        midPanel->addWidget(new QLabel("Electrodes (currents):"));
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

        // Populate cell/electrode lists from network
        populateLists();

        // Connections
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

        double reqRate = sampleRateEdit->value();
        double secBefore = timeBeforeEdit->value();
        double secDuring = durationEdit->value();
        double secAfter = timeAfterEdit->value();

        if (secDuring <= 0) {
            QMessageBox::warning(this, "Error", "Duration must be > 0");
            return;
        }
        if (displayedList->count() == 0) {
            QMessageBox::warning(this, "Error", "Please choose plots to display");
            return;
        }

        m_net->SetMaxRK4Timestep(rk4StepEdit->value());
        m_netDesc = m_net->DescribeNetwork();
        m_net->Initialize(true);

        m_sampleRate = reqRate;
        m_secBefore = secBefore;
        m_secDuring = secDuring;
        m_secAfter = secAfter;
        m_totalSec = secBefore + secDuring + secAfter;
        m_numRepeats = repeatSpin->value();
        m_currentRep = 0;
        m_terminated = false;

        // Set up trace panel from displayed list
        int numPlots = std::min(displayedList->count(), 8);
        tracePanel->setNumTraces(numPlots);
        tracePanel->clearAllData();
        for (int i = 0; i < numPlots; ++i)
            tracePanel->setTraceTitle(i, displayedList->item(i)->text());

        if (m_netDesc.NumVDepCells > 0 && m_daq) {
            startDAQRun();
        } else {
            startSimRun();
        }
    }

    void onStop() {
        m_terminated = true;
    }

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
        auto items = displayedList->selectedItems();
        for (auto *item : items) delete item;
    }

    void runStep() {
        if (m_terminated) {
            finishRun();
            return;
        }

        if (m_useDAQ) {
            daqStep();
        } else {
            simStep();
        }
    }

private:
    void populateLists() {
        cellsList->clear();
        electrodesList->clear();
        for (auto &pair : m_net->GetCells()) {
            if (pair.second->IsActive())
                cellsList->addItem(QString::fromStdWString(pair.first));
        }
        for (auto &pair : m_net->GetElectrodes()) {
            if (pair.second->IsActive())
                electrodesList->addItem(QString::fromStdWString(pair.first));
        }
        // Auto-populate displayed list with all cells
        if (displayedList->count() == 0) {
            for (int i = 0; i < cellsList->count() && i < 8; ++i)
                displayedList->addItem(cellsList->item(i)->text());
        }
    }

    // --- Simulation-only run (no VDep cells) ---
    void startSimRun() {
        m_useDAQ = false;
        m_simTime = 0;
        m_step_ms = 1000.0 / m_sampleRate;
        m_totalSamples = static_cast<int>(m_secDuring * m_sampleRate);
        m_sampleCount = 0;
        coercedLabel->setText(QString::number(m_sampleRate));

        setRunning(true);
        runTimer->start(10); // 10ms timer ticks
    }

    void simStep() {
        int stepsPerTick = std::min(1000, m_totalSamples - m_sampleCount);
        int numCells = m_net->GetCells().size();
        double Vm_out[6] = {};

        for (int s = 0; s < stepsPerTick && !m_terminated; ++s) {
            m_net->Update(m_step_ms, nullptr, Vm_out, nullptr);
            m_simTime += m_step_ms;
            m_sampleCount++;

            if (s % 10 == 0) {
                addPlotData(Vm_out, numCells, m_simTime);
            }
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

    // --- DAQ run (with VDep cells) ---
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

            // Write zeros initially
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);

            setRunning(true);
            runTimer->start(1); // 1ms for tight DAQ loop
        } catch (const DAQException &e) {
            QMessageBox::critical(this, "DAQ Error", e.what());
        }
    }

    void daqStep() {
        int numCells = m_netDesc.NumVDepCells + m_netDesc.NumTimeCells;
        double Vm_out[6] = {};

        int32_t read = m_daq->readAI(m_readBuf.data(), static_cast<int32_t>(m_readBuf.size()));
        if (read <= 0) return;

        m_sampleCount += read;
        double step = read * m_step_ms;
        m_simTime += step;

        // Use last sample from read buffer
        double *lastSample = &m_readBuf[(read - 1) * m_numAI];

        m_net->Update(step, lastSample, Vm_out, m_I_nA.data());

        // Determine phase and write AO only during "during" phase
        double secElapsed = m_sampleCount / m_sampleRate;
        bool duringPhase = (secElapsed >= m_secBefore) &&
                           (secElapsed < m_secBefore + m_secDuring);

        if (duringPhase) {
            // Clamp to ±10V
            for (auto &v : m_I_nA) v = std::max(-10.0, std::min(10.0, v));
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);
        } else {
            std::fill(m_I_nA.begin(), m_I_nA.end(), 0.0);
            m_daq->writeAO(m_I_nA.data(), m_netDesc.NumVDepCells);
        }

        addPlotData(Vm_out, numCells, m_simTime);

        double progress = 100.0 * m_sampleCount / m_totalSamples;
        progressBar->setValue(std::min(100, static_cast<int>(progress)));
        statusLabel->setText(QString("DAQ: t=%1 s").arg(secElapsed, 0, 'f', 2));

        if (m_sampleCount >= m_totalSamples) {
            // Zero outputs
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

    void addPlotData(double *Vm_out, int numCells, double time_ms) {
        // Map displayed plot names to cell indices
        for (int p = 0; p < displayedList->count() && p < 8; ++p) {
            QString name = displayedList->item(p)->text();
            int idx = 0;
            for (auto &pair : m_net->GetCells()) {
                if (QString::fromStdWString(pair.first) == name) {
                    if (idx < numCells)
                        tracePanel->addDataPoint(p, time_ms, Vm_out[idx]);
                    break;
                }
                idx++;
            }
        }
    }

    bool shouldContinue() {
        if (m_terminated) return false;
        if (m_numRepeats == 0) return true; // infinite
        return m_currentRep < m_numRepeats;
    }

    void finishRun() {
        runTimer->stop();
        if (m_useDAQ && m_daq) {
            try { m_daq->stop(); } catch (...) {}
        }
        setRunning(false);
        statusLabel->setText("Complete");
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
    QSpinBox *repeatSpin;
    QCheckBox *resetCheck, *saveCheck;
    QPushButton *startBtn, *stopBtn;
    QProgressBar *progressBar;
    QLabel *coercedLabel, *statusLabel;
    QListWidget *cellsList, *electrodesList, *displayedList;
    TracePanel *tracePanel;
    QTimer *runTimer;

    // Run state
    bool m_running = false;
    bool m_terminated = false;
    bool m_useDAQ = false;
    double m_sampleRate = 10000;
    double m_step_ms = 0.1;
    double m_secBefore = 0, m_secDuring = 1, m_secAfter = 0, m_totalSec = 1;
    double m_simTime = 0;
    int m_totalSamples = 0;
    int m_sampleCount = 0;
    int m_numRepeats = 1;
    int m_currentRep = 0;
    int m_numAI = 0;
    std::vector<double> m_readBuf;
    std::vector<double> m_I_nA;
};

#endif
