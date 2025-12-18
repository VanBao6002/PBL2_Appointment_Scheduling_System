#include "medicalrecorddetaildialog.h"
#include "gui/ui_medicalrecorddetaildialog.h"
#include <QScreen>
#include <QGuiApplication>
#include "medicalRecordManager.h"
#include "patientManager.h"
#include "doctorManager.h"

MedicalRecordDetailDialog::MedicalRecordDetailDialog(const MedicalRecord& record, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MedicalRecordDetailDialog)
    , m_record(record)
{
    ui->setupUi(this);

    // Set window flags để có thể di chuyển
    setWindowFlags(Qt::FramelessWindowHint);

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Populate data
    populateData();
}

MedicalRecordDetailDialog::~MedicalRecordDetailDialog()
{
    delete ui;
}

void MedicalRecordDetailDialog::populateData()
{
    // Set title với ID hồ sơ
    QString title = QString("CHI TIẾT HỒ SƠ BỆNH ÁN - ID: %1").arg(m_record.getID());
    ui->lblMainTitle->setText(title);

    // Thông tin cơ bản
    ui->lblIDValue->setText(QString::number(m_record.getID()));
    ui->lblPatientIDValue->setText(QString::number(m_record.getPatientID()));
    ui->lblDoctorIDValue->setText(QString::number(m_record.getDoctorID()));
    ui->lblCreationDateValue->setText(QString::fromStdString(m_record.getCreationDate().toString()));
    ui->lblLastUpdatedValue->setText(QString::fromStdString(m_record.getLastUpdated().toString()));

    // Lấy tên bệnh nhân và bác sĩ
    QString patientName = "N/A";
    QString doctorName = "N/A";

    try {
        patientName = QString::fromStdString(PatientManager::getInstance().getPatientByID(m_record.getPatientID()).getName());
    } catch (...) {}

    try {
        doctorName = QString::fromStdString(DoctorManager::getInstance().getDoctorByID(m_record.getDoctorID()).getName());
    } catch (...) {}

    ui->lblPatientNameValue->setText(patientName);
    ui->lblDoctorNameValue->setText(doctorName);

    // Thông tin y tế
    ui->lblDiagnosisValue->setText(QString::fromStdString(m_record.getDiagnosis()));
    ui->lblSymptomsValue->setText(QString::fromStdString(m_record.getSymptoms()));
    ui->lblTestResultsValue->setText(QString::fromStdString(m_record.getTestResults()));

    ui->lblBloodPressureValue->setText(QString::fromStdString(m_record.getBloodPressure()));
    ui->lblHeartRateValue->setText(QString::number(m_record.getHeartRate()));
    ui->lblBodyTemperatureValue->setText(QString::number(m_record.getBodyTemperature()));

    ui->lblTreatmentValue->setText(QString::fromStdString(m_record.getTreatment()));
    ui->lblDoctorNotesValue->setText(QString::fromStdString(m_record.getDoctorNotes()));

    // Đặt title cho window
    setWindowTitle(QString("Chi Tiết Hồ Sơ Bệnh Án - ID: %1").arg(m_record.getID()));
}

void MedicalRecordDetailDialog::on_btnClose_clicked()
{
    accept();
}

void MedicalRecordDetailDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MedicalRecordDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void MedicalRecordDetailDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
