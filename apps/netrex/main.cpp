#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("NETrex");
    app.setOrganizationName("HudsonValleySci");
    app.setWindowIcon(QIcon(":/icons/NETrex.png"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
