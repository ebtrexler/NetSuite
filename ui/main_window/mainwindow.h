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
#include <QDoubleSpinBox>
#include <QSettings>
#include <QStringList>
#include <memory>
#include "networkview.h"
#include "tracepanel.h"
#include "networkeditor.h"

class DAQInterface;

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
    void syncTracePanelToNetwork();
    void openRunDialog();
    void openFile(const QString &path);
    void addRecentFile(const QString &path);
    void updateRecentFilesMenu();
    void pushUndo();
    void undo();
    void redo();
    
    QMenu *fileMenu;
    QMenu *recentFilesMenu;
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
    QDoubleSpinBox *durationSpin;
    
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
    std::unique_ptr<DAQInterface> m_daq;
    
    // Undo/redo (JSON snapshots)
    std::vector<std::string> m_undoStack;
    std::vector<std::string> m_redoStack;
    QAction *undoAct = nullptr;
    QAction *redoAct = nullptr;
};

#endif // MAINWINDOW_H
