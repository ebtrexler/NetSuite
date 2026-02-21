#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QSplitter>
#include <QTreeWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QTimer>
#include "networkview.h"
#include "tracepanel.h"
#include "networkeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newNetwork();
    void openNetwork();
    void saveNetwork();
    void about();
    
    // Simulation controls
    void runSimulation();
    void pauseSimulation();
    void stopSimulation();
    void stepSimulation();
    void simulationStep();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createLayout();
    void updateSimulationControls();
    
    QMenu *fileMenu;
    QMenu *simulateMenu;
    QMenu *helpMenu;
    
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;
    
    QAction *runAct;
    QAction *pauseAct;
    QAction *stopAct;
    QAction *stepAct;
    
    QToolBar *simToolBar;
    QLabel *statusLabel;
    NetworkView *networkView;
    NetworkEditor *networkEditor;
    TracePanel *tracePanel;
    
    TNetwork *currentNetwork;
    QTimer *simTimer;
    bool isRunning;
    double simTime;
    double timeStep;
};

#endif // MAINWINDOW_H
