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
#include "daq_thread.h"
#include "scan_writer.h"
#include "spsc_ring.h"
#include "RT_Network.h"
#include "RT_Electrode.h"
#include "tracepanel.h"
#include "traceplot.h"     // for TracePoint

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
        displayedList->setSelectionMode(QAbstractItemView::SingleSelection);
        displayedList->setDragDropMode(QAbstractItemView::InternalMove);
        midPanel->addWidget(displayedList);

        auto *moveUpBtn = new QPushButton("▲ Up");
        auto *moveDownBtn = new QPushButton("▼ Down");
        auto *moveRow = new QHBoxLayout;
        moveRow->addWidget(moveUpBtn);
        moveRow->addWidget(moveDownBtn);
        midPanel->addLayout(moveRow);

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
        connect(moveUpBtn, &QPushButton::clicked, this, [this]() {
            int row = displayedList->currentRow();
            if (row > 0) {
                auto *item = displayedList->takeItem(row);
                displayedList->insertItem(row - 1, item);
                displayedList->setCurrentRow(row - 1);
            }
        });
        connect(moveDownBtn, &QPushButton::clicked, this, [this]() {
            int row = displayedList->currentRow();
            if (row >= 0 && row < displayedList->count() - 1) {
                auto *item = displayedList->takeItem(row);
                displayedList->insertItem(row + 1, item);
                displayedList->setCurrentRow(row + 1);
            }
        });

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
        for (int i = 0; i < numPlots; ++i) {
            tracePanel->setTraceTitle(i, displayedList->item(i)->text());
            tracePanel->setTraceYLabel(i,
                m_bindings[i].type == PlotBinding::ElectrodeCurrent
                    ? "Current (nA)" : "Voltage (mV)");
        }

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

    // --- DAQ run (threaded pipeline) ---
    //
    // Flow:
    //   DAQ thread  →  readAI → net->Update → writeAO → ScanWriter.submit
    //                                                 → on_chunk callback
    //                                                     └→ push to per-trace SpscRings
    //   UI  thread  ←  30 Hz QTimer → drain each ring → tracePanel->addDataPoint
    //
    // The old single-threaded daqStep() is gone. Nothing on the UI thread
    // touches the DAQ any more.
    void startDAQRun() {
        m_useDAQ = true;
        m_saveFilePath_final.clear();
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

            const int numVDep = m_netDesc.NumVDepCells;
            const int numCells = numVDep + m_netDesc.NumTimeCells;

            // ---- Per-trace UI rings -----------------------------------
            // Capacity covers several seconds of end-of-chunk points at
            // realistic chunk rates — the UI polls at 30 Hz, the DAQ
            // thread pushes at ~100–500 Hz (one point per chunk), so
            // 8192 slots is gigantic headroom.
            m_uiRings.clear();
            m_uiCursors.clear();
            for (std::size_t i = 0; i < m_bindings.size(); ++i) {
                m_uiRings.emplace_back(new SpscRing<TracePoint>(8192));
                m_uiCursors.push_back(0);
            }

            // ---- ScanWriter (disk, optional) --------------------------
            m_scanWriter.reset();
            if (!m_saveFile.isEmpty()) {
                // Rewrite the chosen filename to end in .ntrx — the new
                // pipeline writes binary. Old CSV behavior is still
                // available through the post-run File → Export Data menu.
                QString ntrxPath = m_saveFile;
                if (!ntrxPath.endsWith(".ntrx", Qt::CaseInsensitive)) {
                    QFileInfo fi(ntrxPath);
                    ntrxPath = fi.absolutePath() + "/" +
                               fi.completeBaseName() + ".ntrx";
                }
                m_saveFilePath_final = ntrxPath;

                ScanWriter::Config wcfg;
                wcfg.path = ntrxPath.toStdString();
                wcfg.sample_rate_hz = m_sampleRate;
                wcfg.scan_size = m_numAI + numCells + numVDep;
                wcfg.channel_names.reserve(wcfg.scan_size);
                // AI channel names — split m_netDesc.AIChans on commas
                {
                    std::wstring s = m_netDesc.AIChans;
                    std::wstring cur;
                    for (wchar_t c : s) {
                        if (c == L',') { wcfg.channel_names.emplace_back(cur.begin(), cur.end()); cur.clear(); }
                        else cur += c;
                    }
                    if (!cur.empty()) wcfg.channel_names.emplace_back(cur.begin(), cur.end());
                }
                // Pad AI names if DescribeNetwork gave us fewer names
                // than physical channels (can happen with unassigned cells).
                while (static_cast<int>(wcfg.channel_names.size()) < m_numAI) {
                    wcfg.channel_names.emplace_back(
                        "ai" + std::to_string(wcfg.channel_names.size()));
                }
                // Cell Vm names, in TCellsMap iteration order. This
                // mirrors the Vm_out layout TNetwork::Update fills.
                for (auto &pair : m_net->GetCells()) {
                    if (!pair.second->IsActive()) continue;
                    std::string n(pair.first.begin(), pair.first.end());
                    wcfg.channel_names.push_back(n + "_Vm(mV)");
                }
                // I_nA names, for voltage-dependent cells only.
                for (auto &pair : m_net->GetCells()) {
                    if (!pair.second->IsActive()) continue;
                    if (!pair.second->IsVoltageDependent()) continue;
                    std::string n(pair.first.begin(), pair.first.end());
                    wcfg.channel_names.push_back(n + "_I(nA)");
                }
                // Truncate if over; production layout is trusted but
                // defend against a stale IsActive() flip.
                wcfg.channel_names.resize(wcfg.scan_size, "ch");
                wcfg.queue_capacity = 1024;

                m_scanWriter.reset(new ScanWriter(wcfg));
                if (!m_scanWriter->start()) {
                    QMessageBox::warning(this, "Save Error",
                        QString("ScanWriter: %1").arg(
                            QString::fromStdString(m_scanWriter->last_error())));
                    m_scanWriter.reset();
                    return;
                }
            }

            // ---- on_chunk callback context ---------------------------
            // The callback runs on the DAQ thread. It never allocates;
            // every ring is pre-constructed.
            m_chunkCtx = std::make_unique<ChunkCtx>();
            m_chunkCtx->step_ms = m_step_ms;
            m_chunkCtx->bindings.reserve(m_bindings.size());
            for (std::size_t i = 0; i < m_bindings.size(); ++i) {
                ChunkCtx::RingBinding rb;
                rb.type = m_bindings[i].type;
                rb.cellIdx = m_bindings[i].cellIdx;
                rb.trode = m_bindings[i].trode;
                rb.ring = m_uiRings[i].get();
                m_chunkCtx->bindings.push_back(rb);
            }

            // ---- DaqThread config ------------------------------------
            DaqThread::Config dcfg;
            dcfg.daq = m_daq;
            dcfg.net = m_net;
            dcfg.writer = m_scanWriter.get();
            dcfg.sample_rate_hz = m_sampleRate;
            dcfg.num_ai_channels = m_numAI;
            dcfg.num_cells = numCells;
            dcfg.num_vdep_cells = numVDep;
            dcfg.sec_before = m_secBefore;
            dcfg.sec_during = m_secDuring;
            dcfg.sec_after  = m_secAfter;
            dcfg.max_scans_per_read = std::max<std::size_t>(
                1024, static_cast<std::size_t>(0.1 * m_sampleRate * m_numAI));
            dcfg.on_chunk_ctx = m_chunkCtx.get();
            dcfg.on_chunk = &RunDialog::onChunkStatic;

            m_daqThread.reset(new DaqThread(dcfg));
            if (!m_daqThread->start()) {
                QMessageBox::critical(this, "DAQ Error",
                    QString::fromStdString(m_daqThread->last_error()));
                m_daqThread.reset();
                if (m_scanWriter) { m_scanWriter->stop(); m_scanWriter.reset(); }
                return;
            }

            m_perfTimer.start();
            setRunning(true);
            // UI timer now drains rings + updates stats; DAQ lives elsewhere.
            runTimer->start(33);   // ~30 Hz
        } catch (const DAQException &e) {
            QMessageBox::critical(this, "DAQ Error", e.what());
        }
    }

    // Drain all UI rings into the trace panel, refresh stats / progress,
    // and check for end-of-run. Called on the UI thread at ~30 Hz.
    void daqStep() {
        if (!m_daqThread) { m_terminated = true; return; }

        // --- Drain each ring ---
        constexpr std::size_t kBatch = 512;
        TracePoint batch[kBatch];
        uint64_t total_overflow = 0;

        for (std::size_t i = 0; i < m_uiRings.size(); ++i) {
            uint64_t lost = 0;
            while (true) {
                std::size_t n = m_uiRings[i]->snapshot_since(
                    m_uiCursors[i], batch, kBatch, &lost);
                if (lost) total_overflow += lost;
                if (n == 0) break;
                for (std::size_t k = 0; k < n; ++k) {
                    tracePanel->addDataPoint(
                        static_cast<int>(i), batch[k].time, batch[k].value);
                }
                if (n < kBatch) break;
            }
        }

        // --- Progress / stats ---
        uint64_t scans = m_daqThread->scans_acquired();
        double secElapsed = (m_sampleRate > 0)
            ? (static_cast<double>(scans) / m_sampleRate) : 0.0;
        m_sampleCount = static_cast<int>(scans);
        m_simTime = secElapsed * 1000.0;

        double progress = (m_totalSamples > 0)
            ? 100.0 * m_sampleCount / m_totalSamples : 0.0;
        progressBar->setValue(std::min(100, static_cast<int>(progress)));

        QString phase = (secElapsed < m_secBefore)
            ? "Before"
            : (secElapsed < m_secBefore + m_secDuring ? "During" : "After");
        statusLabel->setText(QString("DAQ [%1]: t=%2 s").arg(phase)
            .arg(secElapsed, 0, 'f', 2));

        double elapsed_wall = m_perfTimer.elapsed() / 1000.0;
        statsLabel->setText(QString(
            "Scans: %1 | Chunks: %2\n"
            "UI overflows: %3 | Disk rejects: %4\n"
            "Wall: %5 s | RT: %6")
            .arg(static_cast<qulonglong>(scans))
            .arg(static_cast<qulonglong>(m_daqThread->chunks_processed()))
            .arg(static_cast<qulonglong>(total_overflow))
            .arg(static_cast<qulonglong>(m_daqThread->disk_rejects()))
            .arg(elapsed_wall, 0, 'f', 1)
            .arg(QString::fromStdString(
                rt_priority::to_string(m_daqThread->rt_report().status))));

        // --- Fatal DAQ-thread error? ---
        if (!m_daqThread->ok()) {
            statusLabel->setText(QString("DAQ ERROR: %1").arg(
                QString::fromStdString(m_daqThread->last_error())));
            m_terminated = true;
            return;
        }

        // --- End of run (duration elapsed) or end of repeat ---
        if (m_sampleCount >= m_totalSamples) {
            m_currentRep++;
            if (shouldContinue()) {
                // Restart: stop + start a fresh DaqThread. Scan counters
                // reset, trace panel clears so the plot window starts at 0.
                m_daqThread->stop();
                if (resetCheck->isChecked()) m_net->Initialize(true);
                m_sampleCount = 0;
                m_simTime = 0;
                tracePanel->clearAllData();
                for (auto &c : m_uiCursors) c = 0;
                for (auto &r : m_uiRings)   r.reset(new SpscRing<TracePoint>(8192));
                // Rewire the callback rings to the freshly created
                // instances so the DAQ thread pushes into the right ones.
                for (std::size_t i = 0; i < m_chunkCtx->bindings.size(); ++i) {
                    m_chunkCtx->bindings[i].ring = m_uiRings[i].get();
                }
                m_daqThread->start();
                m_perfTimer.restart();
            } else {
                m_terminated = true;
            }
        }
    }

    // Static trampoline for DaqThread's C-style callback. Runs on the
    // DAQ thread; must not allocate or block.
    static void onChunkStatic(void* ctx,
                              double time_ms_start,
                              int32_t num_scans,
                              const double* /*ai_interleaved*/,
                              const double* vm_out,
                              const double* i_na)
    {
        auto *cx = static_cast<ChunkCtx*>(ctx);
        // One point per chunk, per binding, at end-of-chunk time. The
        // disk file has the full per-sample series; the plot only needs
        // to refresh at ~chunk-rate.
        const double t_end = time_ms_start + (num_scans - 1) * cx->step_ms;
        for (auto &rb : cx->bindings) {
            double v = 0;
            if (rb.type == PlotBinding::CellVm)
                v = vm_out[rb.cellIdx];
            else if (rb.type == PlotBinding::ElectrodeCurrent && rb.trode)
                v = rb.trode->LastCurrent();
            // Also published: i_na[i] could be a plot source; unused today.
            (void)i_na;
            rb.ring->push({t_end, v});
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
        if (m_useDAQ) {
            // Threaded DAQ path: stop the producer first so the ScanWriter
            // queue stops growing, then drain the writer, then clean up.
            if (m_daqThread) { m_daqThread->stop(); m_daqThread.reset(); }
            if (m_scanWriter) { m_scanWriter->stop(); m_scanWriter.reset(); }
            m_uiRings.clear();
            m_uiCursors.clear();
            m_chunkCtx.reset();
        }
        setRunning(false);
        closeSaveFile();  // no-op in DAQ mode (handled by ScanWriter) but
                          // idempotent and needed for sim mode.
        QString savedTo = m_useDAQ ? m_saveFilePath_final : m_saveFile;
        statusLabel->setText(savedTo.isEmpty() ? "Complete" :
            QString("Complete — saved to %1").arg(savedTo));
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

    // ---- Threaded DAQ pipeline (slice 4) ----
    // Context passed to the DaqThread::on_chunk callback. Construction,
    // lifetime, and pointer stability are managed on the UI thread
    // between startDAQRun() and finishRun().
    struct ChunkCtx {
        struct RingBinding {
            PlotBinding::Type type;
            int cellIdx;
            TElectrode* trode;
            SpscRing<TracePoint>* ring;  // non-owning
        };
        std::vector<RingBinding> bindings;
        double step_ms = 0.0;
    };
    std::unique_ptr<ScanWriter> m_scanWriter;
    std::unique_ptr<DaqThread>  m_daqThread;
    std::vector<std::unique_ptr<SpscRing<TracePoint>>> m_uiRings;
    std::vector<uint64_t>       m_uiCursors;
    std::unique_ptr<ChunkCtx>   m_chunkCtx;
    QString                     m_saveFilePath_final;

    // Data saving (streaming) — used only by the simulation path now.
    QString m_saveFile;
    std::unique_ptr<QFile> m_saveFileObj;
    std::unique_ptr<QTextStream> m_saveStream;

    // Performance stats
    QElapsedTimer m_perfTimer;
    int64_t m_totalReads = 0, m_totalSamplesRead = 0;
};

#endif
