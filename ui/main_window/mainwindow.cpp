#include "mainwindow.h"
#include "hhcurrentdialog.h"
#include "RT_Network.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentNetwork(nullptr)
{
    setWindowTitle("NetSuite - Neural Network Modeling");
    resize(1200, 800);
    
    createLayout();
    createActions();
    createMenus();
    
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
    
    statusLabel->setText("NetSuite Qt UI - Core library loaded successfully");
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
    
    // Right side: results view (70% of window width)
    resultsView = new QTextEdit(this);
    resultsView->setReadOnly(true);
    resultsView->setPlaceholderText("Simulation/DAQ results will appear here...");
    
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(resultsView);
    mainSplitter->setStretchFactor(0, 3);  // 30% for left
    mainSplitter->setStretchFactor(1, 7);  // 70% for right
    
    setCentralWidget(mainSplitter);
}

MainWindow::~MainWindow()
{
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
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::newNetwork()
{
    statusLabel->setText("Creating new network...");
    
    // Create a new network
    if (currentNetwork) {
        delete currentNetwork;
    }
    currentNetwork = new TNetwork(L"New Network");
    networkView->setNetwork(currentNetwork);
    
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
    
    // Create a test HH current and show editor
    THHCurrent *current = new THHCurrent(nullptr, L"Test HH Current");
    
    HHCurrentDialog dialog(current, this);
    if (dialog.exec() == QDialog::Accepted) {
        resultsView->append(QString("HH Current configured:\n  Gmax: %1 μS\n  E: %2 mV\n  p: %3")
            .arg(current->Gmax())
            .arg(current->E())
            .arg(current->p()));
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
