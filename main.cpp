#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include "gui/mainwindow.h"
#include "core.h"

int main(int argc, char *argv[])
{
    Core core;
    try {
        core.loadAll();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", QString("Failed to load data: %1").arg(e.what()));
        return 1;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QObject::connect(&a, &QApplication::aboutToQuit, [&core](){ core.saveAll(); });

    return a.exec();
}