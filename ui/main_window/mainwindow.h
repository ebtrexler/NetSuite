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
#include "networkview.h"

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

private:
    void createActions();
    void createMenus();
    void createLayout();
    
    QMenu *fileMenu;
    QMenu *helpMenu;
    
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;
    
    QLabel *statusLabel;
    NetworkView *networkView;
    QTreeWidget *hierarchyTree;
    QTextEdit *resultsView;
    TNetwork *currentNetwork;
};

#endif // MAINWINDOW_H
