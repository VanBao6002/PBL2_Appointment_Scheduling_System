#include "appointmentdetaildialog.h"
#include "gui/ui_appointmentdetaildialog.h"
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include "appointmentManager.h"
#include "patientManager.h"
#include "doctorManager.h"

AppointmentDetailDialog::AppointmentDetailDialog(const Appointment& appointment, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AppointmentDetailDialog)
    , m_appointment(appointment)
    , appointmentID(appointment.getID())
    , editRequested(false)
{
    ui->setupUi(this);

    // Set window flags để có thể di chuyển
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, false);

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Connect signals (buttons are now in UI file)
    connect(ui->btnEdit, &QPushButton::clicked, this, &AppointmentDetailDialog::on_btnEdit_clicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::reject);

    // Populate data
    populateData();
    applyStyles();
}

AppointmentDetailDialog::~AppointmentDetailDialog()
{
    delete ui;
}

void AppointmentDetailDialog::populateData()
{
    // Set title với ID
    QString title = QString("CHI TIẾT CUỘC HẸN - ID: %1").arg(m_appointment.getID());
    ui->lblMainTitle->setText(title);

    // Lấy thông tin chi tiết bệnh nhân
    QString patientName = "N/A";
    QString patientID = "N/A";
    QString patientPhone = "N/A";
    QString patientCCCD = "N/A";

    try {
        const Patient& patient = PatientManager::getInstance().getPatientByID(m_appointment.getPatientID());
        patientName = QString::fromStdString(patient.getName());
        patientID = QString::number(patient.getID());
        patientPhone = QString::fromStdString(patient.getPhoneNumber());
        if (patientPhone.isEmpty()) patientPhone = "N/A";
        patientCCCD = QString::fromStdString(patient.getCCCD());
        if (patientCCCD.isEmpty()) patientCCCD = "N/A";
    } catch (...) {
        qWarning() << "[WARNING] Could not load patient details for ID:" << m_appointment.getPatientID();
    }

    // Lấy thông tin chi tiết bác sĩ
    QString doctorName = "N/A";
    QString doctorID = "N/A";
    QString doctorSpecialization = "N/A";
    QString doctorPhone = "N/A";
    QString doctorCCCD = "N/A";
    QString doctorEmail = "N/A";
    QString doctorBirthday = "N/A";

    try {
        const Doctor& doctor = DoctorManager::getInstance().getDoctorByID(m_appointment.getDoctorID());
        doctorName = QString::fromStdString(doctor.getName());
        doctorID = QString::number(doctor.getID());
        doctorSpecialization = QString::fromStdString(doctor.getSpecialization());
        doctorPhone = QString::fromStdString(doctor.getPhoneNumber());
        if (doctorPhone.isEmpty()) doctorPhone = "N/A";
        doctorCCCD = QString::fromStdString(doctor.getCCCD());
        if (doctorCCCD.isEmpty()) doctorCCCD = "N/A";
        doctorEmail = QString::fromStdString(doctor.getEmail());
        if (doctorEmail.isEmpty()) doctorEmail = "N/A";
        doctorBirthday = QString::fromStdString(doctor.getBirthday().toString());
        if (doctorBirthday.isEmpty()) doctorBirthday = "N/A";
    } catch (...) {
        qWarning() << "[WARNING] Could not load doctor details for ID:" << m_appointment.getDoctorID();
    }

    // Điền dữ liệu cuộc hẹn
    ui->lblIDValue->setText(QString::number(m_appointment.getID()));
    ui->lblDateValue->setText(QString::fromStdString(m_appointment.getDate().toString()));
    ui->lblStartTimeValue->setText(QString::fromStdString(m_appointment.getStartTime()));
    ui->lblEndTimeValue->setText(QString::fromStdString(m_appointment.getEndTime()));
    ui->lblRoomValue->setText(QString::fromStdString(m_appointment.getRoom()));
    ui->lblStatusValue->setText(QString::fromStdString(Appointment::statusToString(m_appointment.getStatus())));

    // Điền thông tin bệnh nhân
    ui->lblPatientNameValue->setText(patientName);
    ui->lblPatientIDValue->setText(patientID);
    ui->lblPatientPhoneValue->setText(patientPhone);
    ui->lblPatientCCCDValue->setText(patientCCCD);

    // Điền thông tin bác sĩ
    ui->lblDoctorNameValue->setText(doctorName);
    ui->lblDoctorIDValue->setText(doctorID);
    ui->lblDoctorSpecializationValue->setText(doctorSpecialization);
    ui->lblDoctorPhoneValue->setText(doctorPhone);
    ui->lblDoctorCCCDValue->setText(doctorCCCD);
    ui->lblDoctorEmailValue->setText(doctorEmail);
    ui->lblDoctorBirthdayValue->setText(doctorBirthday);

    // Đặt title cho window
    setWindowTitle(QString("Chi Tiết Cuộc Hẹn - ID: %1").arg(m_appointment.getID()));
}

void AppointmentDetailDialog::on_btnClose_clicked()
{
    reject();  // Close without edit
}

void AppointmentDetailDialog::on_btnEdit_clicked()
{
    qDebug() << "[EDIT CLICKED] User requested to edit appointment ID:" << appointmentID;
    editRequested = true;
    accept();  // Close and signal edit request
}

bool AppointmentDetailDialog::shouldEdit() const
{
    return editRequested;
}

void AppointmentDetailDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AppointmentDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AppointmentDetailDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}

void AppointmentDetailDialog::applyStyles()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #f5f5f5;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: white;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 5px 10px;
        }
        QLabel {
            padding: 5px;
            font-size: 10pt;
        }
    )");
}
