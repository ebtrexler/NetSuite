#include "mainwindow.h"
#include "hhcurrentdialog.h"
#include "tracepanel.h"
#include "RT_Network.h"
#include "RT_ModelCell.h"
#include "RT_HHCurrent.h"
#include "network_json.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentNetwork(nullptr), isRunning(false), 
      simTime(0.0), timeStep(0.1)
{
    setWindowTitle("NetSim - Neural Network Simulator");
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
            currentNetwork->DescribeNetwork();
            int numCells = currentNetwork->GetCells().size();
            tracePanel->setNumTraces(numCells);
            tracePanel->clearAllData();
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
    
    // Simulation actions
    runAct = new QAction(tr("▶ Run"), this);
    runAct->setStatusTip(tr("Run simulation"));
    connect(runAct, &QAction::triggered, this, &MainWindow::runSimulation);
    
    pauseAct = new QAction(tr("⏸ Pause"), this);
    pauseAct->setStatusTip(tr("Pause simulation"));
    connect(pauseAct, &QAction::triggered, this, &MainWindow::pauseSimulation);
    
    stopAct = new QAction(tr("⏹ Stop"), this);
    stopAct->setStatusTip(tr("Stop simulation"));
    connect(stopAct, &QAction::triggered, this, &MainWindow::stopSimulation);
    
    stepAct = new QAction(tr("⏭ Step"), this);
    stepAct->setStatusTip(tr("Step simulation"));
    connect(stepAct, &QAction::triggered, this, &MainWindow::stepSimulation);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    simulateMenu = menuBar()->addMenu(tr("&Simulate"));
    simulateMenu->addAction(runAct);
    simulateMenu->addAction(pauseAct);
    simulateMenu->addAction(stopAct);
    simulateMenu->addAction(stepAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
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
        int numCells = currentNetwork->GetCells().size();
        tracePanel->setNumTraces(numCells);
        tracePanel->clearAllData();
        
        updateSimulationControls();
        statusLabel->setText(QString("Network created with %1 cells").arg(numCells));
    } catch (std::exception &e) {
        statusLabel->setText(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::openNetwork()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Network"), "", tr("Network Files (*.json);;All Files (*)"));
    
    if (fileName.isEmpty()) return;
    
    try {
        if (isRunning) stopSimulation();
        
        TNetwork *net = NetworkJson::loadNetwork(fileName.toStdString());
        if (!net) { statusLabel->setText("Failed to open file"); return; }
        
        if (currentNetwork) { delete currentNetwork; currentNetwork = nullptr; }
        currentNetwork = net;
        simTime = 0.0;
        
        networkView->setNetwork(currentNetwork);
        networkEditor->setNetwork(currentNetwork);
        
        int numCells = currentNetwork->GetCells().size();
        tracePanel->setNumTraces(numCells);
        tracePanel->clearAllData();
        updateSimulationControls();
        statusLabel->setText(QString("Loaded: %1").arg(fileName));
    } catch (std::exception &e) {
        statusLabel->setText(QString("Error loading: %1").arg(e.what()));
    }
}

void MainWindow::saveNetwork()
{
    if (!currentNetwork) return;
    
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Network"), "", tr("Network Files (*.json);;All Files (*)"));
    
    if (fileName.isEmpty()) return;
    
    if (NetworkJson::saveNetwork(currentNetwork, fileName.toStdString())) {
        statusLabel->setText(QString("Saved: %1").arg(fileName));
    } else {
        statusLabel->setText("Error saving network");
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About NetSuite"),
        tr("<h2>NetSuite</h2>"
           "<p>Neural Network Modeling and Dynamic Clamp Software</p>"
           "<p>Copyright © 2011-2024 E. Brady Trexler, Ph.D.</p>"
           "<p>Refactored to Qt - 2026</p>"
           "<p>Core library successfully separated from UI!</p>"));
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
    simToolBar->addWidget(durationSpin);
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
    
    if (simTime == 0.0) {
        tracePanel->clearAllData();
        tracePanel->setTimeRange(0, durationSpin->value());
        currentNetwork->Initialize(true);
    }
    
    simTimer->start(10); // Update every 10ms
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
    double Vm_out[6] = {};
    double duration = durationSpin->value();
    
    // Run multiple steps per timer tick for real-time-ish speed
    // 10ms timer interval / 0.1ms step = 100 steps per tick
    int stepsPerTick = 100;
    
    for (int s = 0; s < stepsPerTick && simTime < duration; s++) {
        currentNetwork->Update(timeStep, nullptr, Vm_out, nullptr);
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
