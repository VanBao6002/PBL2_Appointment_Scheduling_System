#include "addappointmentdialog.h"
#include "gui/ui_addappointmentdialog.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QDebug>
#include "doctorManager.h"
#include "patientManager.h"
#include "IDHandler.h"
#include "core.h"
#include "config.h"

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog)
{
    ui->setupUi(this);
    setupStatusComboBox();
}

AddAppointmentDialog::~AddAppointmentDialog()
{
    delete ui;
}

void AddAppointmentDialog::setupStatusComboBox() {
    QComboBox* cmbRooms = ui->stackedWidget->findChild<QComboBox*>("cmbRooms");
    QComboBox* cmbStatus = ui->stackedWidget->findChild<QComboBox*>("cmbStatus");
    ui->cmbStatus->addItem("Scheduled");
    ui->cmbStatus->addItem("Occupied");
    ui->cmbStatus->addItem("Canceled");
    ui->cmbStatus->addItem("Completed");

    // ComboBox phòng - Load từ JSON
    ui->cmbRooms->clear();
    auto rooms = Core::loadRooms();
    QStringList roomList;
    for (const auto& room : rooms) {
        roomList << QString::fromStdString(room);
    }
    for (const QString& room : roomList) {
        ui->cmbRooms->addItem(room);
    }
    ui->cmbRooms->setEditable(true);

}

bool AddAppointmentDialog::isDoctorValid(int doctorID) const {
    qDebug() << "Checking Doctor ID:" << doctorID;
    try {
        Doctor d = DoctorManager::getInstance().getDoctorByID(doctorID);

        bool isActive = (d.getStatus() == Doctor::Status::Active);
        qDebug() << "Doctor ID" << doctorID << "Status:" << (isActive ? "Active" : "Not Active");
        return isActive;

    } catch (const std::exception& e) {
        qDebug() << "Error fetching Doctor ID" << doctorID << ":" << e.what();
        return false;
    } catch (...) {
        qDebug() << "Unknown error when checking Doctor ID" << doctorID;
        return false;
    }
}

bool AddAppointmentDialog::isPatientValid(int patientID) const {
    qDebug() << "Checking Patient ID:" << patientID;
    bool exists = IDHandler<Patient>::checkDuplicate(static_cast<size_t>(patientID));
    return exists;
}

// void AddAppointmentDialog::on_buttonBox_accepted()
// {
//     QString doctorIDText = ui->txtDoctorID->text();
//     QString patientIDText = ui->txtPatientID->text();
//     QString roomText = ui->txtRoom->text();

//     if (doctorIDText.isEmpty() || patientIDText.isEmpty() || roomText.isEmpty())
//     {
//         QMessageBox::warning(this, "Lỗi Nhập Liệu", "Vui lòng điền đầy đủ thông tin.");
//         return;
//     }

//     int doctorID = doctorIDText.toInt();
//     int patientID = patientIDText.toInt();

//     if (!isDoctorValid(doctorID)) {
//         QString errorMsg = QString("Doctor ID %1 không tồn tại hoặc không ở trạng thái Active (hiện tại). Vui lòng kiểm tra lại ID hoặc trạng thái.").arg(doctorID);
//         QMessageBox::critical(this, "Lỗi Dữ liệu", errorMsg);
//         return;
//     }

//     if (!isPatientValid(patientID)) {
//         QMessageBox::critical(this, "Lỗi Dữ liệu", QString("Patient ID %1 không tồn tại trong hệ thống. Vui lòng kiểm tra lại ID.").arg(patientID));
//         return;
//     }

//     accept();
// }

void AddAppointmentDialog::on_buttonBox_rejected()
{
    reject();
}

Appointment AddAppointmentDialog::getAppointmentData() const {
    int doctorID = ui->txtDoctorID->text().toInt();
    int patientID = ui->txtPatientID->text().toInt();

    QDate qDate = ui->dateEditAppointment->date();
    Date apptDate(qDate.day(), qDate.month(), qDate.year());
    QString qTime = ui->timeEditAppointment->time().toString("HH:mm");

    std::string room = ui->txtRoom->text().toStdString();
    std::string statusStr = ui->comboStatus->currentText().toStdString();
    Appointment::Status status = Appointment::statusFromString(statusStr);
    return Appointment(doctorID, patientID, apptDate.toString(), qTime.toStdString(), room, statusStr);
}
