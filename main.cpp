#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QMessageBox>
#include "gui/mainwindow.h"
#include "core.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    try {
        Core::loadAll();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", QString("Failed to load data: %1").arg(e.what()));
        return 1;
    }
    MainWindow w;
    w.show();

    QObject::connect(&a, &QApplication::aboutToQuit, [](){ Core::saveAll(); });
    return a.exec();
}