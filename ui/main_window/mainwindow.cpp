#include "mainwindow.h"
#include "hhcurrentdialog.h"
#include "tracepanel.h"
#include "RT_Network.h"
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
    // Main horizontal splitter
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left side: vertical splitter for network view and hierarchy
    QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
    
    // Network view (top left - 30% of window width)
    networkView = new NetworkView(this);
    networkView->setMinimumSize(300, 250);
    leftSplitter->addWidget(networkView);
    
    // Hierarchy tree (bottom left)
    hierarchyTree = new QTreeWidget(this);
    hierarchyTree->setHeaderLabel("Network Hierarchy");
    hierarchyTree->setMinimumSize(300, 200);
    leftSplitter->addWidget(hierarchyTree);
    
    leftSplitter->setStretchFactor(0, 1);
    leftSplitter->setStretchFactor(1, 1);
    
    // Right side: trace panel (70% of window width)
    tracePanel = new TracePanel(this);
    
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(tracePanel);
    mainSplitter->setStretchFactor(0, 3);  // 30% for left
    mainSplitter->setStretchFactor(1, 7);  // 70% for right
    
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
    statusLabel->setText("Creating new network...");
    
    // Stop any running simulation
    if (isRunning) {
        stopSimulation();
    }
    
    // Create a new network
    if (currentNetwork) {
        delete currentNetwork;
    }
    currentNetwork = new TNetwork(L"New Network");
    networkView->setNetwork(currentNetwork);
    simTime = 0.0;
    
    // Update hierarchy tree
    hierarchyTree->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(hierarchyTree);
    root->setText(0, QString::fromStdWString(currentNetwork->Name()));
    root->setExpanded(true);
    
    // Add sample items
    QTreeWidgetItem *cellsItem = new QTreeWidgetItem(root);
    cellsItem->setText(0, "Cells");
    
    QTreeWidgetItem *synapsesItem = new QTreeWidgetItem(root);
    synapsesItem->setText(0, "Synapses");
    
    updateSimulationControls();
    
    // Create a test HH current and show editor
    THHCurrent *current = new THHCurrent(nullptr, L"Test HH Current");
    
    HHCurrentDialog dialog(current, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Set up trace panel for 1 cell initially
        tracePanel->setNumTraces(1);
        tracePanel->clearAllData();
    }
    
    statusLabel->setText("Network created");
}

void MainWindow::openNetwork()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Network"), "", tr("Network Files (*.net);;All Files (*)"));
    
    if (!fileName.isEmpty()) {
        statusLabel->setText(QString("Opening: %1").arg(fileName));
    }
}

void MainWindow::saveNetwork()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Network"), "", tr("Network Files (*.net);;All Files (*)"));
    
    if (!fileName.isEmpty()) {
        statusLabel->setText(QString("Saving: %1").arg(fileName));
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
    
    tracePanel->clearAllData();
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
    
    // Run one simulation step
    double *voltages = currentNetwork->Update(timeStep, nullptr, nullptr, nullptr);
    simTime += timeStep;
    
    // Add data point to trace (assuming first cell for now)
    if (voltages) {
        tracePanel->addDataPoint(0, simTime, voltages[0]);
    }
    
    statusLabel->setText(QString("Simulation: t = %1 ms").arg(simTime, 0, 'f', 1));
}
