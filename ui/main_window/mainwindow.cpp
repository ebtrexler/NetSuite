#include "mainwindow.h"
#include "hhcurrentdialog.h"
#include "tracepanel.h"
#include "RT_Network.h"
#include "RT_ModelCell.h"
#include "RT_HHCurrent.h"
#include "network_json.h"
#include "rundialog.h"
using json = nlohmann::json;
#include "daq_interface.h"
#include "daq_mock.h"
#ifdef HAVE_NIDAQMX
#include "daq_nidaqmx.h"
#endif
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextBrowser>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentNetwork(nullptr), isRunning(false), 
      simTime(0.0), timeStep(0.1)
{
    setWindowTitle("NETrex - Dynamic Clamp and Network Simulator");
    resize(1200, 800);
    
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, &MainWindow::simulationStep);
    
    createLayout();
    createActions();
    createMenus();
    createToolBar();
    
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
    
    updateSimulationControls();
    statusLabel->setText("NetSim ready - Create or load a network to begin");
}

void MainWindow::createLayout()
{
    // Main HORIZONTAL splitter (left/right)
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left: vertical splitter for network view and hierarchy tree
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
    
    // Network view (top left)
    networkView = new NetworkView(this);
    networkView->setMinimumSize(300, 250);
    leftSplitter->addWidget(networkView);
    
    // Hierarchy tree (bottom left)
    networkEditor = new NetworkEditor(this);
    networkEditor->setMinimumSize(300, 200);
    leftSplitter->addWidget(networkEditor);
    
    connect(networkEditor, &NetworkEditor::networkModified, this, [this]() {
        if (currentNetwork) {
            pushUndo();
            currentNetwork->DescribeNetwork();
            syncTracePanelToNetwork();
            networkView->setNetwork(currentNetwork);
            updateSimulationControls();
        }
    });

    // NetworkView context menu signals
    connect(networkView, &NetworkView::requestAddCell, this, [this](int x, int y) {
        networkEditor->addCellAt(x, y);
    });
    connect(networkView, &NetworkView::requestAddElectrode, this, [this](const std::wstring &cellName) {
        networkEditor->addElectrodeToCellByName(cellName);
    });
    connect(networkView, &NetworkView::requestAddSynapse, this, [this](const std::wstring &from, const std::wstring &to) {
        networkEditor->addSynapseBetween(from, to);
    });
    connect(networkView, &NetworkView::cellMoved, this, [this](const std::wstring &, int, int) {
        networkView->update();
    });
    
    // Right: trace panel (70% of window width)
    tracePanel = new TracePanel(this);
    tracePanel->setMinimumSize(400, 400);
    
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(tracePanel);
    
    // Set initial sizes: 30% left, 70% right
    mainSplitter->setSizes(QList<int>() << 300 << 700);
    
    setCentralWidget(mainSplitter);
}

MainWindow::~MainWindow()
{
    if (simTimer && simTimer->isActive()) {
        simTimer->stop();
    }
    if (currentNetwork) {
        delete currentNetwork;
    }
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New Network"), this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new network"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newNetwork);
    
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing network"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openNetwork);
    
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the network"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveNetwork);
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setEnabled(false);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    
    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setEnabled(false);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    
    // Simulation actions
    runAct = new QAction(tr("▶ Run"), this);
    runAct->setStatusTip(tr("Run simulation from the beginning"));
    runAct->setToolTip(tr("Run simulation (resets time to 0)"));
    connect(runAct, &QAction::triggered, this, &MainWindow::runSimulation);
    
    pauseAct = new QAction(tr("⏸ Pause"), this);
    pauseAct->setStatusTip(tr("Pause the running simulation"));
    pauseAct->setToolTip(tr("Pause simulation"));
    connect(pauseAct, &QAction::triggered, this, &MainWindow::pauseSimulation);
    
    stopAct = new QAction(tr("⏹ Stop"), this);
    stopAct->setStatusTip(tr("Stop simulation and reset time"));
    stopAct->setToolTip(tr("Stop simulation"));
    connect(stopAct, &QAction::triggered, this, &MainWindow::stopSimulation);
    
    stepAct = new QAction(tr("⏭ Step"), this);
    stepAct->setStatusTip(tr("Advance simulation by one time step"));
    stepAct->setToolTip(tr("Single step (one dt)"));
    connect(stepAct, &QAction::triggered, this, &MainWindow::stepSimulation);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    recentFilesMenu = fileMenu->addMenu(tr("Open &Recent"));
    updateRecentFilesMenu();
    fileMenu->addAction(saveAct);
    
    QAction *exportAct = new QAction(tr("Export &Data (CSV)..."), this);
    exportAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    connect(exportAct, &QAction::triggered, this, [this]() {
        QString fn = QFileDialog::getSaveFileName(this, "Export Data", "", "CSV Files (*.csv)");
        if (fn.isEmpty()) return;
        if (tracePanel->exportCsv(fn))
            statusLabel->setText("Data exported to " + QFileInfo(fn).fileName());
        else
            QMessageBox::warning(this, "Export", "Failed to export data.");
    });
    fileMenu->addAction(exportAct);
    
    QAction *exportImgAct = new QAction(tr("Export Plot &Image..."), this);
    connect(exportImgAct, &QAction::triggered, this, [this]() {
        QString fn = QFileDialog::getSaveFileName(this, "Export Plot Image", "",
            "PNG Image (*.png);;SVG Image (*.svg)");
        if (fn.isEmpty()) return;
        if (tracePanel->exportImage(fn))
            statusLabel->setText("Plot exported to " + QFileInfo(fn).fileName());
        else
            QMessageBox::warning(this, "Export", "Failed to export plot image.");
    });
    fileMenu->addAction(exportImgAct);
    
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    
    simulateMenu = menuBar()->addMenu(tr("&Simulate"));
    simulateMenu->addAction(runAct);
    simulateMenu->addAction(pauseAct);
    simulateMenu->addAction(stopAct);
    simulateMenu->addAction(stepAct);
    simulateMenu->addSeparator();
    auto *runDCAct = new QAction(tr("Run Dynamic Clamp..."), this);
    connect(runDCAct, &QAction::triggered, this, &MainWindow::openRunDialog);
    simulateMenu->addAction(runDCAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    auto *docsAct = new QAction(tr("&User Guide"), this);
    connect(docsAct, &QAction::triggered, this, [this]() {
        QDialog dlg(this);
        dlg.setWindowTitle("NETrex User Guide");
        dlg.resize(640, 520);
        auto *layout = new QVBoxLayout(&dlg);
        auto *text = new QTextBrowser;
        text->setOpenExternalLinks(true);
        text->setHtml(
            "<h2>NETrex User Guide</h2>"
            "<p>NETrex is a real-time neural network modeling and dynamic clamp application.</p>"

            "<h3>Getting Started</h3>"
            "<ol>"
            "<li><b>File &gt; New Network</b> creates a sample 3-cell network to explore.</li>"
            "<li><b>File &gt; Open</b> loads a previously saved network (.json).</li>"
            "<li>Edit cells, currents, synapses, and electrodes in the <b>Network Editor</b> tree (left panel).</li>"
            "<li>Click <b>▶ Run</b> in the toolbar to simulate, or use <b>Simulate &gt; Run Dynamic Clamp</b> for DAQ hardware.</li>"
            "</ol>"

            "<h3>Network Editor</h3>"
            "<p>Right-click items in the tree to add or remove cells, currents, electrodes, and synapses. "
            "Double-click any item to edit its parameters.</p>"
            "<ul>"
            "<li><b>Model Cell</b> — integrate-and-fire neuron with configurable HH conductances</li>"
            "<li><b>Vm Playback Cell</b> — plays back a voltage waveform from file</li>"
            "<li><b>Biological Cell</b> — real neuron connected via DAQ hardware</li>"
            "</ul>"

            "<h3>Simulation</h3>"
            "<ul>"
            "<li><b>▶ Run</b> — start from t=0 for the duration shown in the toolbar</li>"
            "<li><b>⏸ Pause</b> — pause at current time</li>"
            "<li><b>⏹ Stop</b> — stop and reset</li>"
            "<li><b>⏭ Step</b> — advance one time step</li>"
            "</ul>"

            "<h3>Dynamic Clamp</h3>"
            "<p><b>Simulate &gt; Run Dynamic Clamp</b> opens the run dialog where you configure "
            "timing (Before / During / After phases), select which traces to display, and start "
            "real-time current injection via NI-DAQmx hardware.</p>"
            "<p>On systems without NI hardware, a mock DAQ backend simulates a biological cell.</p>"

            "<h3>Plot Controls</h3>"
            "<p>Use the toolbar buttons on the left side of the plot area:</p>"
            "<ul>"
            "<li><b>⊞</b> Zoom XY — drag a rectangle to zoom both axes</li>"
            "<li><b>↔</b> Zoom X — drag to zoom the time axis only</li>"
            "<li><b>↕</b> Zoom Y — drag to zoom the value axis only</li>"
            "<li><b>✋</b> Pan — drag to scroll the view</li>"
            "<li><b>⊙</b> Reset — fit all data in view</li>"
            "</ul>"
            "<p>Double-click any plot to reset its zoom.</p>"

            "<h3>Exporting Data</h3>"
            "<ul>"
            "<li><b>File &gt; Export Data (CSV)</b> — save trace data as a CSV file</li>"
            "<li><b>File &gt; Export Plot Image</b> — save plots as PNG (high-res) or SVG</li>"
            "</ul>"
            "<p>During dynamic clamp runs, data is also streamed to CSV in real time.</p>"

            "<h3>Keyboard Shortcuts</h3>"
            "<table cellpadding='4'>"
            "<tr><td><b>Ctrl+N</b></td><td>New Network</td></tr>"
            "<tr><td><b>Ctrl+O</b></td><td>Open Network</td></tr>"
            "<tr><td><b>Ctrl+S</b></td><td>Save Network</td></tr>"
            "<tr><td><b>Ctrl+Z</b></td><td>Undo</td></tr>"
            "<tr><td><b>Ctrl+Shift+Z</b></td><td>Redo</td></tr>"
            "<tr><td><b>Ctrl+D</b></td><td>Export Data (CSV)</td></tr>"
            "</table>"

            "<h3>File Format</h3>"
            "<p>Networks are saved as JSON files. They can be edited in any text editor.</p>"

            "<h3>More Information</h3>"
            "<p><a href='https://github.com/ebtrexler/NetSuite'>github.com/ebtrexler/NetSuite</a></p>"
            "<p><a href='https://hudsonvalleysci.com'>hudsonvalleysci.com</a></p>"
        );
        layout->addWidget(text);
        auto *closeBtn = new QPushButton("Close");
        connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);
        dlg.exec();
    });
    helpMenu->addAction(docsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}

void MainWindow::newNetwork()
{
    try {
        if (isRunning) stopSimulation();
        
        if (currentNetwork) {
            delete currentNetwork;
            currentNetwork = nullptr;
        }
        currentNetwork = new TNetwork(L"New Network");
        simTime = 0.0;
        
        // Register TModelCell with the cell factory (once)
        static bool registered = false;
        if (!registered) {
            GetCellFactory().registerBuilder(
                TModelCell_KEY, TypeID<TModelCell>(), TypeID<const std::wstring>());
            registered = true;
        }
        
        // Create 3 model cells
        currentNetwork->AddCellToNet(TModelCell_KEY, L"Cell 1", 50, 50);
        currentNetwork->AddCellToNet(TModelCell_KEY, L"Cell 2", 150, 50);
        currentNetwork->AddCellToNet(TModelCell_KEY, L"Cell 3", 100, 150);
        
        // Add HH currents to each cell
        // Manually register if static init didn't run (static lib linking issue)
        try {
            GetCurrentFactory().registerBuilder(
                THHCurrent_KEY,
                TypeID<THHCurrent>(),
                TypeID<TCurrentUser*const>(),
                TypeID<const std::wstring>());
        } catch (...) {} // already registered is OK
        
        currentNetwork->AddCurrentToCell(THHCurrent_KEY, L"Na Cell1", L"Cell 1");
        currentNetwork->AddCurrentToCell(THHCurrent_KEY, L"Na Cell2", L"Cell 2");
        currentNetwork->AddCurrentToCell(THHCurrent_KEY, L"Na Cell3", L"Cell 3");
        
        // Populate internal cell arrays
        currentNetwork->DescribeNetwork();
        
        networkView->setNetwork(currentNetwork);
        networkEditor->setNetwork(currentNetwork);
        
        // Set up trace panel for all cells
        syncTracePanelToNetwork();
        
        updateSimulationControls();
        statusLabel->setText(QString("Network created with %1 cells").arg(currentNetwork->GetCells().size()));
    } catch (std::exception &e) {
        statusLabel->setText(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::openNetwork()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Network"), "", tr("Network Files (*.json);;All Files (*)"));
    
    if (fileName.isEmpty()) return;
    openFile(fileName);
}

void MainWindow::openFile(const QString &fileName)
{
    try {
        if (isRunning) stopSimulation();
        
        TNetwork *net = NetworkJson::loadNetwork(fileName.toStdString());
        if (!net) {
            QMessageBox::warning(this, tr("Open Failed"),
                tr("Could not parse the network file.\n\n%1").arg(fileName));
            statusLabel->setText("Failed to open file");
            return;
        }
        
        if (currentNetwork) { delete currentNetwork; currentNetwork = nullptr; }
        currentNetwork = net;
        m_undoStack.clear();
        m_redoStack.clear();
        undoAct->setEnabled(false);
        redoAct->setEnabled(false);
        simTime = 0.0;
        
        networkView->setNetwork(currentNetwork);
        networkEditor->setNetwork(currentNetwork);
        
        syncTracePanelToNetwork();
        updateSimulationControls();
        addRecentFile(fileName);
        statusLabel->setText(QString("Loaded: %1").arg(QFileInfo(fileName).fileName()));
    } catch (json::parse_error &e) {
        QMessageBox::critical(this, tr("JSON Parse Error"),
            tr("The file is not valid JSON.\n\n%1\n\nDetails: %2")
            .arg(fileName).arg(e.what()));
        statusLabel->setText("Error: invalid JSON");
    } catch (json::type_error &e) {
        QMessageBox::critical(this, tr("Network Format Error"),
            tr("The file does not contain a valid NETrex network.\n\n%1\n\nDetails: %2")
            .arg(fileName).arg(e.what()));
        statusLabel->setText("Error: invalid network format");
    } catch (std::exception &e) {
        QMessageBox::critical(this, tr("Error"),
            tr("Failed to load network.\n\n%1").arg(e.what()));
        statusLabel->setText(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::addRecentFile(const QString &path)
{
    QSettings settings("HudsonValleySci", "NETrex");
    QStringList recent = settings.value("recentFiles").toStringList();
    recent.removeAll(path);
    recent.prepend(path);
    while (recent.size() > 8) recent.removeLast();
    settings.setValue("recentFiles", recent);
    updateRecentFilesMenu();
}

void MainWindow::updateRecentFilesMenu()
{
    recentFilesMenu->clear();
    QSettings settings("HudsonValleySci", "NETrex");
    QStringList recent = settings.value("recentFiles").toStringList();
    for (const QString &path : recent) {
        QFileInfo fi(path);
        if (!fi.exists()) continue;
        recentFilesMenu->addAction(fi.fileName(), [this, path]() { openFile(path); });
    }
    recentFilesMenu->setEnabled(!recentFilesMenu->isEmpty());
}

void MainWindow::saveNetwork()
{
    if (!currentNetwork) return;
    
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Network"), "", tr("Network Files (*.json);;All Files (*)"));
    
    if (fileName.isEmpty()) return;
    
    if (NetworkJson::saveNetwork(currentNetwork, fileName.toStdString())) {
        addRecentFile(fileName);
        statusLabel->setText(QString("Saved: %1").arg(fileName));
    } else {
        statusLabel->setText("Error saving network");
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About NETrex"),
        tr("<h2>NETrex</h2>"
           "<p>Dynamic Clamp and Network Simulator</p>"
           "<p>Copyright © 2011-2026 E. Brady Trexler, Ph.D.</p>"
           "<p>Hudson Valley Scientific, Inc.</p>"
           "<p><a href='https://hudsonvalleysci.com'>hudsonvalleysci.com</a></p>"));
}

void MainWindow::openRunDialog()
{
    if (!currentNetwork) {
        statusLabel->setText("No network loaded");
        return;
    }
    if (!m_daq) {
#ifdef HAVE_NIDAQMX
        m_daq = std::make_unique<DAQNIDAQmx>();
#else
        m_daq = std::make_unique<DAQMock>();
#endif
    }
    RunDialog dlg(currentNetwork, m_daq.get(), this);
    dlg.exec();
}

void MainWindow::createToolBar()
{
    simToolBar = addToolBar(tr("Simulation"));
    simToolBar->addAction(runAct);
    simToolBar->addAction(pauseAct);
    simToolBar->addAction(stopAct);
    simToolBar->addAction(stepAct);
    
    simToolBar->addSeparator();
    simToolBar->addWidget(new QLabel(" Duration (ms): "));
    durationSpin = new QDoubleSpinBox;
    durationSpin->setRange(10, 1000000);
    durationSpin->setValue(1000);
    durationSpin->setSingleStep(100);
    durationSpin->setDecimals(0);
    durationSpin->setToolTip(tr("Total simulation duration in milliseconds"));
    simToolBar->addWidget(durationSpin);
}

void MainWindow::syncTracePanelToNetwork()
{
    if (!currentNetwork) return;
    const TCellsMap &cells = currentNetwork->GetCells();
    tracePanel->setNumTraces(cells.size());
    tracePanel->clearAllData();
    int i = 0;
    for (auto it = cells.begin(); it != cells.end() && i < 6; ++it, ++i)
        tracePanel->setTraceTitle(i, QString::fromStdWString(it->first));
}

void MainWindow::updateSimulationControls()
{
    bool hasNetwork = (currentNetwork != nullptr);
    
    if (runAct) {
        runAct->setEnabled(hasNetwork && !isRunning);
        pauseAct->setEnabled(hasNetwork && isRunning);
        stopAct->setEnabled(hasNetwork && isRunning);
        stepAct->setEnabled(hasNetwork && !isRunning);
    }
}

void MainWindow::runSimulation()
{
    if (!currentNetwork) return;
    
    isRunning = true;
    updateSimulationControls();
    
    simTime = 0.0;
    tracePanel->clearAllData();
    tracePanel->setTimeRange(0, durationSpin->value());
    currentNetwork->Initialize(true);
    
    simTimer->start(10);
    statusLabel->setText("Simulation running...");
}

void MainWindow::pauseSimulation()
{
    isRunning = false;
    simTimer->stop();
    updateSimulationControls();
    statusLabel->setText(QString("Simulation paused at t = %1 ms").arg(simTime));
}

void MainWindow::stopSimulation()
{
    isRunning = false;
    simTimer->stop();
    simTime = 0.0;
    updateSimulationControls();
    statusLabel->setText("Simulation stopped");
}

void MainWindow::stepSimulation()
{
    if (!currentNetwork) return;
    
    if (simTime == 0.0) {
        currentNetwork->Initialize(true);
    }
    
    simulationStep();
}

void MainWindow::simulationStep()
{
    if (!currentNetwork) return;
    
    int numCells = currentNetwork->GetCells().size();
    double Vm_in[6] = {};   // voltage from ADCs (V) — zeros for sim, real values from DAQ
    double Vm_out[6] = {};
    double I_nA[6] = {};    // current output for voltage-dependent cells
    double duration = durationSpin->value();
    
    // Run multiple steps per timer tick for real-time-ish speed
    // 10ms timer interval / 0.1ms step = 100 steps per tick
    int stepsPerTick = 100;
    
    for (int s = 0; s < stepsPerTick && simTime < duration; s++) {
        currentNetwork->Update(timeStep, Vm_in, Vm_out, I_nA);
        simTime += timeStep;
        
        // Only record every few steps to avoid huge data arrays
        if (s % 10 == 0) {
            for (int i = 0; i < numCells && i < 6; i++) {
                tracePanel->addDataPoint(i, simTime, Vm_out[i]);
            }
        }
    }
    
    // Always record the last point
    for (int i = 0; i < numCells && i < 6; i++) {
        tracePanel->addDataPoint(i, simTime, Vm_out[i]);
    }
    
    statusLabel->setText(QString("Simulation: t = %1 ms").arg(simTime, 0, 'f', 1));
    
    if (simTime >= duration) {
        pauseSimulation();
        statusLabel->setText(QString("Simulation complete: %1 ms").arg(simTime, 0, 'f', 1));
    }
}

void MainWindow::pushUndo()
{
    if (!currentNetwork) return;
    json j = NetworkJson::networkToJson(currentNetwork);
    m_undoStack.push_back(j.dump());
    m_redoStack.clear();
    if (m_undoStack.size() > 50) m_undoStack.erase(m_undoStack.begin());
    undoAct->setEnabled(!m_undoStack.empty());
    redoAct->setEnabled(false);
}

void MainWindow::undo()
{
    if (m_undoStack.empty() || !currentNetwork) return;
    
    // Save current state to redo
    json cur = NetworkJson::networkToJson(currentNetwork);
    m_redoStack.push_back(cur.dump());
    
    // Restore previous state
    std::string prev = m_undoStack.back();
    m_undoStack.pop_back();
    
    json j = json::parse(prev);
    TNetwork *net = NetworkJson::networkFromJson(j);
    if (net) {
        delete currentNetwork;
        currentNetwork = net;
        networkView->setNetwork(currentNetwork);
        networkEditor->setNetwork(currentNetwork);
        syncTracePanelToNetwork();
        updateSimulationControls();
        statusLabel->setText("Undo");
    }
    undoAct->setEnabled(!m_undoStack.empty());
    redoAct->setEnabled(!m_redoStack.empty());
}

void MainWindow::redo()
{
    if (m_redoStack.empty() || !currentNetwork) return;
    
    // Save current state to undo
    json cur = NetworkJson::networkToJson(currentNetwork);
    m_undoStack.push_back(cur.dump());
    
    // Restore next state
    std::string next = m_redoStack.back();
    m_redoStack.pop_back();
    
    json j = json::parse(next);
    TNetwork *net = NetworkJson::networkFromJson(j);
    if (net) {
        delete currentNetwork;
        currentNetwork = net;
        networkView->setNetwork(currentNetwork);
        networkEditor->setNetwork(currentNetwork);
        syncTracePanelToNetwork();
        updateSimulationControls();
        statusLabel->setText("Redo");
    }
    undoAct->setEnabled(!m_undoStack.empty());
    redoAct->setEnabled(!m_redoStack.empty());
}
