#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QMessageBox>
#include "gui/mainwindow.h"
#include "core.h"
#include "patientManager.h"
#include "doctorManager.h"
#include "medicalRecordManager.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);

    // ✅ Load theo thứ tự đúng: Patient -> Doctor -> MedicalRecord -> Core
    qDebug() << "[LOAD] Loading patients...";
    PatientManager::getInstance().loadFromFile(Config::PATIENT_PATH);

    qDebug() << "[LOAD] Loading doctors...";
    DoctorManager::getInstance().loadFromFile(Config::DOCTOR_PATH);

    qDebug() << "[LOAD] Loading medical record...";
    MedicalRecordManager::getInstance().loadFromFile(Config::MEDICAL_RECORD_PATH);

    qDebug() << "[LOAD] Loading core data...";
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
