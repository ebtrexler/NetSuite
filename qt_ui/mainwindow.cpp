#include "mainwindow.h"
#include "RT_Network.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("NetSuite - Neural Network Modeling");
    resize(800, 600);
    
    createActions();
    createMenus();
    
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
    
    statusLabel->setText("NetSuite Qt UI - Core library loaded successfully");
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
    
    // Create a test network using the core library
    TNetwork *network = new TNetwork(L"New Network");
    
    QMessageBox::information(this, tr("New Network"),
        tr("Created new network: %1").arg(QString::fromStdWString(network->Name())));
    
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
