#include <QApplication>
#include "gui/mainwindow.h"
#include "gui/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
