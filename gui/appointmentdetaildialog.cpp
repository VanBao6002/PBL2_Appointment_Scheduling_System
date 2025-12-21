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

    // ✅ Check if buttons already exist in UI file, if not create them
    QPushButton* btnEdit = findChild<QPushButton*>("btnEdit");
    QPushButton* btnClose = findChild<QPushButton*>("btnClose");
    
    if (!btnEdit || !btnClose) {
        // Buttons don't exist in UI file, create them dynamically
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch();
        
        // Edit button
        btnEdit = new QPushButton("Chỉnh sửa");
        btnEdit->setObjectName("btnEdit");
        btnEdit->setMinimumSize(120, 40);
        btnEdit->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f39c12, stop:1 #e67e22);
                color: white;
                border: none;
                border-radius: 6px;
                font-weight: bold;
                font-size: 11pt;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #e67e22, stop:1 #d35400);
            }
        )");
        
        // Close button
        btnClose = new QPushButton("Đóng");
        btnClose->setObjectName("btnClose");
        btnClose->setMinimumSize(120, 40);
        btnClose->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #95a5a6, stop:1 #7f8c8d);
                color: white;
                border: none;
                border-radius: 6px;
                font-weight: bold;
                font-size: 11pt;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #7f8c8d, stop:1 #6c7a7b);
            }
        )");
        
        buttonLayout->addWidget(btnEdit);
        buttonLayout->addWidget(btnClose);
        buttonLayout->addStretch();
        
        // Add to main layout
        if (ui->verticalLayout) {
            ui->verticalLayout->addLayout(buttonLayout);
        }
    } else {
        // Buttons exist in UI file, just apply styles
        btnEdit->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f39c12, stop:1 #e67e22);
                color: white;
                border: none;
                border-radius: 6px;
                font-weight: bold;
                font-size: 11pt;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #e67e22, stop:1 #d35400);
            }
        )");
        
        btnClose->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #95a5a6, stop:1 #7f8c8d);
                color: white;
                border: none;
                border-radius: 6px;
                font-weight: bold;
                font-size: 11pt;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #7f8c8d, stop:1 #6c7a7b);
            }
        )");
    }
    
    // Connect signals (whether buttons were found or created)
    connect(btnEdit, &QPushButton::clicked, this, &AppointmentDetailDialog::on_btnEdit_clicked);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
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