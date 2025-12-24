
#include "adminwindow.h"
#include "userManager.h"
#include <QMessageBox>

bool AdminWindow::hasAppointmentAccess(bool write) const {
    const User& user = UserManager::getInstance().getUserByID(userID);
    if (user.getRole() == User::Role::ADMIN) return true;
    if (user.getRole() == User::Role::ASSISTANT) return true;
    return false;
}
bool AdminWindow::hasPatientAccess(bool write) const {
    const User& user = UserManager::getInstance().getUserByID(userID);
    if (user.getRole() == User::Role::ADMIN) return true;
    if (user.getRole() == User::Role::ASSISTANT) return true;
    return false;
}
bool AdminWindow::hasDoctorAccess(bool write) const {
    const User& user = UserManager::getInstance().getUserByID(userID);
    if (user.getRole() == User::Role::ADMIN) return true;
    if (user.getRole() == User::Role::ASSISTANT) return !write;
    return false;
}
bool AdminWindow::hasUserAccess(bool write) const {
    const User& user = UserManager::getInstance().getUserByID(userID);
    if (user.getRole() == User::Role::ADMIN) return true;
    return false;
}
bool AdminWindow::hasMedicalRecordAccess(bool write) const {
    const User& user = UserManager::getInstance().getUserByID(userID);
    if (user.getRole() == User::Role::ADMIN) return true;
    if (user.getRole() == User::Role::ASSISTANT) return !write;
    return false;
}
void AdminWindow::showNoPermissionMessage() const {
    QMessageBox::warning(nullptr, "Cảnh báo", "Bạn không có quyền truy cập vào chức năng này.");
}

#include "adminwindow.h"
#include "addeditpatientdialog.h"
#include "addeditdoctordialog.h"
#include "addedituserdialog.h"
#include "addeditmedicalrecorddialog.h"
#include "gui/ui_adminwindow.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <algorithm>
#include <QDir>
#include "patientdetaildialog.h"
#include "doctordetaildialog.h"
#include "userdetaildialog.h"
#include "medicalrecorddetaildialog.h"
#include "appointmentdetaildialog.h"
#include "person.h"
#include "appointmentManager.h"
#include "patientManager.h"
#include "doctorManager.h"
#include "medicalRecordManager.h"
#include "userManager.h"


AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    applyModernStyles();
    this->showFullScreen();

    QDir dir("data");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    currentAppointmentPage = 1;
    totalAppointmentPages = 1;

    currentPatientPage = 1;
    totalPatientPages = 1;

    currentDoctorPage = 1;
    totalDoctorPages = 1;

    currentMedicalRecordPage = 1;
    totalMedicalRecordPages = 1;

    currentUserPage = 1;
    totalUserPages = 1;

    itemsPerPage = 10;

    currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;
    currentMedicalRecordSortMode = MedicalRecordSortMode::BY_ID_ASC;
    currentUserSortMode = UserSortMode::BY_ID_ASC;

    //Appointment
    connect(ui->appointmentManagerButton, &QPushButton::clicked, this, &AdminWindow::on_appointmentManagerButton_clicked);
    connect(ui->btnAddAppointment, &QPushButton::clicked, this, &AdminWindow::on_btnAddAppointment_clicked);
    connect(ui->btnSearchAppointment, &QPushButton::clicked, this, &AdminWindow::on_btnSearchAppointment_clicked);
    connect(ui->btnPrevPage_Appointment, &QPushButton::clicked, this, &AdminWindow::on_btnPrevPage_Appointment_clicked);
    connect(ui->btnNextPage_Appointment, &QPushButton::clicked, this, &AdminWindow::on_btnNextPage_Appointment_clicked);
    connect(ui->btnPage_Appointment_1, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Appointment_1_clicked);
    connect(ui->btnPage_Appointment_2, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Appointment_2_clicked);
    connect(ui->btnPage_Appointment_3, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Appointment_3_clicked);
    connect(ui->btnSortAZAppointment, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZAppointment_clicked);
    connect(ui->btnSortZAAppointment, &QPushButton::clicked, this, &AdminWindow::on_btnSortZAAppointment_clicked);

    ui->btnSortAZAppointment->setText("🔼 A → Z");
    ui->btnSortZAAppointment->setText("🔽 Z → A");

    //Patient
    connect(ui->btnSortAZPatient, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZPatient_clicked);
    connect(ui->btnSortZAPatient, &QPushButton::clicked, this, &AdminWindow::on_btnSortZAPatient_clicked);
    connect(ui->btnPrevPage_Patient, &QPushButton::clicked, this, &AdminWindow::on_btnPrevPage_Patient_clicked);
    connect(ui->btnNextPage_Patient, &QPushButton::clicked, this, &AdminWindow::on_btnNextPage_Patient_clicked);
    connect(ui->btnPage_Patient_1, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Patient_1_clicked);
    connect(ui->btnPage_Patient_2, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Patient_2_clicked);
    connect(ui->btnPage_Patient_3, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Patient_3_clicked);

    ui->btnSortAZPatient->setText("🔼 A → Z");
    ui->btnSortZAPatient->setText("🔽 Z → A");

    //Doctor
    connect(ui->btnSortAZDoctor, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZDoctor_clicked);
    connect(ui->btnSortZADoctor, &QPushButton::clicked, this, &AdminWindow::on_btnSortZADoctor_clicked);
    connect(ui->btnPrevPage_Doctor, &QPushButton::clicked, this, &AdminWindow::on_btnPrevPage_Doctor_clicked);
    connect(ui->btnNextPage_Doctor, &QPushButton::clicked, this, &AdminWindow::on_btnNextPage_Doctor_clicked);
    connect(ui->btnPage_Doctor_1, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Doctor_1_clicked);
    connect(ui->btnPage_Doctor_2, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Doctor_2_clicked);
    connect(ui->btnPage_Doctor_3, &QPushButton::clicked, this, &AdminWindow::on_btnPage_Doctor_3_clicked);

    ui->btnSortAZDoctor->setText("🔼 A → Z");
    ui->btnSortZADoctor->setText("🔽 Z → A");

    //Medical Record
    connect(ui->btnSortAZMedicalRecord, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZMedicalRecord_clicked);
    connect(ui->btnSortZAMedicalRecord, &QPushButton::clicked, this, &AdminWindow::on_btnSortZAMedicalRecord_clicked);

    ui->btnSortAZMedicalRecord->setText("🔼 A → Z");
    ui->btnSortZAMedicalRecord->setText("🔽 Z → A");

    //User
    connect(ui->btnPrevPage_User, &QPushButton::clicked, this, &AdminWindow::on_btnPrevPage_User_clicked);
    connect(ui->btnNextPage_User, &QPushButton::clicked, this, &AdminWindow::on_btnNextPage_User_clicked);
    connect(ui->btnSortAZUser, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZUser_clicked);
    connect(ui->btnSortZAUser, &QPushButton::clicked, this, &AdminWindow::on_btnSortZAUser_clicked);

    ui->btnSortAZUser->setText("🔼 A → Z");
    ui->btnSortZAUser->setText("🔽 Z → A");

    setupAppointmentTable();
    ui->mainStack->setCurrentWidget(ui->page_appointment);
    loadAppointmentData(currentAppointmentPage);

    qDebug() << "[INFO] AdminWindow initialized successfully";
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::loadPatientData(int page, const QString& searchText) {
    qDebug() << "Loading patient data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allPatients = PatientManager::getInstance().getAllPatientsTable();
    std::vector<Patient> filteredPatients;

    // 2. Lọc theo ID hoặc CCCD (hoặc hiển thị tất cả nếu searchText rỗng)
    if (searchText.isEmpty()) {
        for (const auto& pair : allPatients) {
            filteredPatients.push_back(pair.second);
        }
    } else {
        QString search = searchText.trimmed();
        std::string searchStd = search.toStdString();
        bool isNumber = false;
        int searchID = search.toInt(&isNumber);

        // Tìm theo ID nếu là số
        if (isNumber) {
            auto it = allPatients.find(searchID);
            if (it != allPatients.end()) {
                filteredPatients.push_back(it->second);
            }
        }

        // Tìm theo CCCD
        const auto& cccdToID = PatientManager::getInstance().getCCCDtoID();
        auto cccdIt = cccdToID.find(searchStd);
        if (cccdIt != cccdToID.end()) {
            int patientID = cccdIt->second;
            auto patientIt = allPatients.find(patientID);
            if (patientIt != allPatients.end()) {
                // Tránh trùng nếu ID và CCCD trỏ cùng 1 bệnh nhân
                if (std::none_of(filteredPatients.begin(), filteredPatients.end(),
                                 [patientID](const Patient& p) { return p.getID() == patientID; })) {
                    filteredPatients.push_back(patientIt->second);
                }
            }
        }

        if (filteredPatients.empty()) {
            QMessageBox::information(this, "Kết quả tìm kiếm",
                                     QString("Không tìm thấy bệnh nhân với ID hoặc CCCD: %1").arg(search));
            for (const auto& pair : allPatients) {
                filteredPatients.push_back(pair.second);
            }
        }
    }

    // 3. SẮP XẾP theo mode hiện tại
    switch (currentPatientSortMode) {
    case PatientSortMode::BY_ID_ASC:
        std::sort(filteredPatients.begin(), filteredPatients.end(),
                  [](const Patient& a, const Patient& b) {
                      return a.getID() < b.getID();
                  });
        qDebug() << "[SORT] Applied: ID Ascending";
        break;

    case PatientSortMode::BY_NAME_ASC:
        std::sort(filteredPatients.begin(), filteredPatients.end(),
                  [](const Patient& a, const Patient& b) {
                      std::string nameA = Utils::toLower(a.getName());
                      std::string nameB = Utils::toLower(b.getName());
                      return nameA < nameB;
                  });
        qDebug() << "[SORT] Applied: Name A-Z";
        break;

    case PatientSortMode::BY_NAME_DESC:
        std::sort(filteredPatients.begin(), filteredPatients.end(),
                  [](const Patient& a, const Patient& b) {
                      std::string nameA = Utils::toLower(a.getName());
                      std::string nameB = Utils::toLower(b.getName());
                      return nameA > nameB;
                  });
        qDebug() << "[SORT] Applied: Name Z-A";
        break;
    }

    // 4. Tính toán phân trang
    int totalItems = filteredPatients.size();
    totalPatientPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalPatientPages) page = totalPatientPages;
    currentPatientPage = page;

    qDebug() << "[PATIENT] Total items:" << totalItems
             << "| Total pages:" << totalPatientPages
             << "| Current page:" << currentPatientPage;

    // 5. Đặt lại bảng và điền dữ liệu
    ui->tablePatient->setRowCount(0);
    int startIdx = (currentPatientPage - 1) * itemsPerPage;
    int endIdx = qMin(startIdx + itemsPerPage, totalItems);

    for (int i = startIdx; i < endIdx; ++i) {
        const Patient& patient = filteredPatients.at(i);
        ui->tablePatient->insertRow(ui->tablePatient->rowCount());
        int row = ui->tablePatient->rowCount() - 1;

        // ✅ Cột 0: ID (căn giữa)
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(patient.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 0, idItem);

        // ✅ Cột 1: Họ tên (căn trái)
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(patient.getName()));
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tablePatient->setItem(row, 1, nameItem);

        // ✅ Cột 2: Giới tính (căn giữa)
        QString genderDisplay;
        char gender = patient.getGender();
        if (gender == 'M') genderDisplay = "Nam";
        else if (gender == 'F') genderDisplay = "Nữ";
        else genderDisplay = "Khác";

        QTableWidgetItem* genderItem = new QTableWidgetItem(genderDisplay);
        genderItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 2, genderItem);

        // ✅ Cột 3: Ngày sinh (căn giữa)
        QTableWidgetItem* birthdayItem = new QTableWidgetItem(QString::fromStdString(patient.getBirthday().toString()));
        birthdayItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 3, birthdayItem);

        // ✅ Cột 4: CCCD (căn giữa)
        QTableWidgetItem* CCCDItem = new QTableWidgetItem(QString::fromStdString(patient.getCCCD()));
        CCCDItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 4, CCCDItem);

        // ✅ Cột 5: SĐT (căn giữa)
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem(QString::fromStdString(patient.getPhoneNumber()));
        phoneNumberItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 5, phoneNumberItem);

        // ✅ Cột 6: Email (căn trái)
        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(patient.getEmail()));
        emailItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tablePatient->setItem(row, 6, emailItem);

        // ✅ Cột 7: Tuỳ chọn - Thêm 2 nút "Xem chi tiết" và "Xoá"
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        // Button "Xem chi tiết"
        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("patientID", patient.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2196F3, stop:1 #1976D2);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #1976D2, stop:1 #1565C0);
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewPatientDetail_clicked);

        // Button "Xoá"
        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("patientID", patient.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f44336, stop:1 #d32f2f);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #d32f2f, stop:1 #c62828);
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemovePatient_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tablePatient->setCellWidget(row, 7, actionWidget);
    }

    updatePatientPaginationUI();
}

void AdminWindow::loadDoctorData(int page, const QString& searchText) {
    qDebug() << "Loading doctor data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allDoctors = DoctorManager::getInstance().getAllDoctors();
    std::vector<Doctor> filteredDoctors;

    // 2. Lọc theo ID hoặc CCCD (hoặc hiển thị tất cả nếu searchText rỗng)
    if (searchText.isEmpty()) {
        for (const auto& pair : allDoctors) {
            filteredDoctors.push_back(pair.second);
        }
    } else {
        QString search = searchText.trimmed();
        std::string searchStd = search.toStdString();
        bool isNumber = false;
        int searchID = search.toInt(&isNumber);

        // Tìm theo ID nếu là số
        if (isNumber) {
            auto it = allDoctors.find(searchID);
            if (it != allDoctors.end()) {
                filteredDoctors.push_back(it->second);
            }
        }

        // Tìm theo CCCD
        const auto& cccdToID = DoctorManager::getInstance().getCCCDtoID();
        auto cccdIt = cccdToID.find(searchStd);
        if (cccdIt != cccdToID.end()) {
            int doctorID = cccdIt->second;
            auto doctorIt = allDoctors.find(doctorID);
            if (doctorIt != allDoctors.end()) {
                // Tránh trùng nếu ID và CCCD trỏ cùng 1 bệnh nhân
                if (std::none_of(filteredDoctors.begin(), filteredDoctors.end(),
                                 [doctorID](const Doctor& p) { return p.getID() == doctorID; })) {
                    filteredDoctors.push_back(doctorIt->second);
                }
            }
        }

        if (filteredDoctors.empty()) {
            QMessageBox::information(this, "Kết quả tìm kiếm",
                                     QString("Không tìm thấy bệnh nhân với ID hoặc CCCD: %1").arg(search));
            for (const auto& pair : allDoctors) {
                filteredDoctors.push_back(pair.second);
            }
        }
    }

    // 3. SẮP XẾP theo mode hiện tại
    switch (currentDoctorSortMode) {
    case DoctorSortMode::BY_ID_ASC:
        std::sort(filteredDoctors.begin(), filteredDoctors.end(),
                  [](const Doctor& a, const Doctor& b) {
                      return a.getID() < b.getID();
                  });
        qDebug() << "[SORT] Applied: ID Ascending";
        break;

    case DoctorSortMode::BY_NAME_ASC:
        std::sort(filteredDoctors.begin(), filteredDoctors.end(),
                  [](const Doctor& a, const Doctor& b) {
                      std::string nameA = Utils::toLower(a.getName());
                      std::string nameB = Utils::toLower(b.getName());
                      return nameA < nameB;
                  });
        qDebug() << "[SORT] Applied: Name A-Z";
        break;

    case DoctorSortMode::BY_NAME_DESC:
        std::sort(filteredDoctors.begin(), filteredDoctors.end(),
                  [](const Doctor& a, const Doctor& b) {
                      std::string nameA = Utils::toLower(a.getName());
                      std::string nameB = Utils::toLower(b.getName());
                      return nameA > nameB;
                  });
        qDebug() << "[SORT] Applied: Name Z-A";
        break;
    }

    // 4. Tính toán phân trang
    int totalItems = filteredDoctors.size();
    totalDoctorPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalDoctorPages) page = totalDoctorPages;
    currentDoctorPage = page;

    qDebug() << "[DOCTOR] Total items:" << totalItems
             << "| Total pages:" << totalDoctorPages
             << "| Current page:" << currentDoctorPage;

    // 5. Đặt lại bảng và điền dữ liệu
    ui->tableDoctor->setRowCount(0);
    int startIdx = (currentDoctorPage - 1) * itemsPerPage;
    int endIdx = qMin(startIdx + itemsPerPage, totalItems);

    for (int i = startIdx; i < endIdx; ++i) {
        const Doctor& doctor = filteredDoctors.at(i);
        ui->tableDoctor->insertRow(ui->tableDoctor->rowCount());
        int row = ui->tableDoctor->rowCount() - 1;

        // Cột 0: ID (căn giữa)
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(doctor.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 0, idItem);

        // Cột 1: Họ tên (căn trái)
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(doctor.getName()));
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 1, nameItem);

        // Cột 2: Giới tính (hiển thị "Nam/Nữ/Khác")
        QString genderDisplay;
        char gender = doctor.getGender();
        if (gender == 'M') genderDisplay = "Nam";
        else if (gender == 'F') genderDisplay = "Nữ";
        else genderDisplay = "Khác";

        QTableWidgetItem* genderItem = new QTableWidgetItem(genderDisplay);
        genderItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 2, genderItem);

        // Cột 3: Ngày sinh (căn giữa)
        QTableWidgetItem* birthdayItem = new QTableWidgetItem(QString::fromStdString(doctor.getBirthday().toString()));
        birthdayItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 3, birthdayItem);

        // Cột 4: CCCD
        QTableWidgetItem* CCCDItem = new QTableWidgetItem(QString::fromStdString(doctor.getCCCD()));
        CCCDItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 4, CCCDItem);

        // Cột 5: Chuyên khoa (căn trái)
        QTableWidgetItem* specializationItem = new QTableWidgetItem(QString::fromStdString(doctor.getSpecialization()));
        specializationItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 5, specializationItem);

        // Cột 6: Email (căn trái)
        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(doctor.getEmail()));
        emailItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 6, emailItem);

        // Cột 7: SĐT (căn giữa)
        QString phoneDisplay = QString::fromStdString(doctor.getPhoneNumber());
        if (phoneDisplay.isEmpty()) {
            phoneDisplay = "None";
        }
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem(phoneDisplay);
        phoneNumberItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 7, phoneNumberItem);

        // Cột 8: Trạng thái (căn giữa)
        QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(Doctor::statusToString(doctor.getStatus())));
        statusItem->setTextAlignment(Qt::AlignCenter);

        // Tô màu theo trạng thái
        if (doctor.getStatus() == Doctor::Status::Active) {
            statusItem->setBackground(QBrush(QColor(76, 175, 80, 50)));
        } else if (doctor.getStatus() == Doctor::Status::OnLeave) {
            statusItem->setBackground(QBrush(QColor(255, 193, 7, 50)));
        } else if (doctor.getStatus() == Doctor::Status::Retired) {
            statusItem->setBackground(QBrush(QColor(158, 158, 158, 50)));
        }

        ui->tableDoctor->setItem(row, 8, statusItem);

        // Cột 9: Tuỳ chọn - Thêm 2 nút "Xem chi tiết" và "Xoá"
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        // Button "Xem chi tiết"
        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("doctorID", doctor.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2196F3, stop:1 #1976D2);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #1976D2, stop:1 #1565C0);
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewDoctorDetail_clicked);

        // Button "Xoá"
        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("doctorID", doctor.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f44336, stop:1 #d32f2f);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #d32f2f, stop:1 #c62828);
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveDoctor_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableDoctor->setCellWidget(row, 9, actionWidget);
    }
        updateDoctorPaginationUI();
}


void AdminWindow::loadMedicalRecordData(int page, const QString& searchText) {
    qDebug() << "Loading medical record data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allRecords = MedicalRecordManager::getInstance().getAllMedicalRecords();
    std::vector<MedicalRecord> filteredRecords;

    // 2. Lọc CHỈ theo ID
    if (searchText.isEmpty()) {
        for (const auto& pair : allRecords) {
            filteredRecords.push_back(pair.second);
        }
    } else {
        bool isNumber = false;
        int searchID = searchText.toInt(&isNumber);

        if (!isNumber) {
            QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập ID (số nguyên) để tìm kiếm!");
            for (const auto& pair : allRecords) {
                filteredRecords.push_back(pair.second);
            }
        } else {
            for (const auto& pair : allRecords) {
                if (pair.second.getID() == searchID) {
                    filteredRecords.push_back(pair.second);
                }
            }

            if (filteredRecords.empty()) {
                QMessageBox::information(this, "Kết quả tìm kiếm",
                                         QString("Không tìm thấy hồ sơ bệnh án có ID: %1").arg(searchID));
                for (const auto& pair : allRecords) {
                    filteredRecords.push_back(pair.second);
                }
            }
        }
    }

    // 3. SẮP XẾP theo mode hiện tại
    switch (currentMedicalRecordSortMode) {
    case MedicalRecordSortMode::BY_ID_ASC:
        std::sort(filteredRecords.begin(), filteredRecords.end(),
                  [](const MedicalRecord& a, const MedicalRecord& b) {
                      return a.getID() < b.getID();
                  });
        qDebug() << "[SORT] Applied: ID Ascending";
        break;

    case MedicalRecordSortMode::BY_PATIENT_NAME_ASC:
        std::sort(filteredRecords.begin(), filteredRecords.end(),
                  [](const MedicalRecord& a, const MedicalRecord& b) {
                      try {
                          std::string nameA = Utils::toLower(PatientManager::getInstance().getPatientByID(a.getPatientID()).getName());
                          std::string nameB = Utils::toLower(PatientManager::getInstance().getPatientByID(b.getPatientID()).getName());
                          return nameA < nameB;
                      } catch (...) {
                          return a.getID() < b.getID();
                      }
                  });
        qDebug() << "[SORT] Applied: Patient Name A-Z";
        break;

    case MedicalRecordSortMode::BY_PATIENT_NAME_DESC:
        std::sort(filteredRecords.begin(), filteredRecords.end(),
                  [](const MedicalRecord& a, const MedicalRecord& b) {
                      try {
                          std::string nameA = Utils::toLower(PatientManager::getInstance().getPatientByID(a.getPatientID()).getName());
                          std::string nameB = Utils::toLower(PatientManager::getInstance().getPatientByID(b.getPatientID()).getName());
                          return nameA > nameB;
                      } catch (...) {
                          return a.getID() < b.getID();
                      }
                  });
        qDebug() << "[SORT] Applied: Patient Name Z-A";
        break;
    }

    // 4. Tính toán phân trang
    int totalItems = filteredRecords.size();
    totalMedicalRecordPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalMedicalRecordPages) page = totalMedicalRecordPages;
    currentMedicalRecordPage = page;

    qDebug() << "[MEDICAL RECORD] Total items:" << totalItems
             << "| Total pages:" << totalMedicalRecordPages
             << "| Current page:" << currentMedicalRecordPage;

    // 5. Đặt lại bảng và điền dữ liệu
    ui->tableMedicalRecord->setRowCount(0);
    int startIdx = (currentMedicalRecordPage - 1) * itemsPerPage;
    int endIdx = qMin(startIdx + itemsPerPage, totalItems);

    for (int i = startIdx; i < endIdx; ++i) {
        const MedicalRecord& record = filteredRecords.at(i);
        ui->tableMedicalRecord->insertRow(ui->tableMedicalRecord->rowCount());
        int row = ui->tableMedicalRecord->rowCount() - 1;

        // Cột 0: ID Hồ Sơ
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(record.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableMedicalRecord->setItem(row, 0, idItem);

        // Cột 1: ID Bệnh Nhân
        QTableWidgetItem* patientIDItem = new QTableWidgetItem(QString::number(record.getPatientID()));
        patientIDItem->setTextAlignment(Qt::AlignCenter);
        ui->tableMedicalRecord->setItem(row, 1, patientIDItem);

        // Cột 2: Tên Bệnh Nhân
        QString patientName = "N/A";
        try {
            patientName = QString::fromStdString(PatientManager::getInstance().getPatientByID(record.getPatientID()).getName());
        } catch (...) {}
        QTableWidgetItem* patientNameItem = new QTableWidgetItem(patientName);
        patientNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableMedicalRecord->setItem(row, 2, patientNameItem);

        // Cột 3: ID Bác Sĩ
        QTableWidgetItem* doctorIDItem = new QTableWidgetItem(QString::number(record.getDoctorID()));
        doctorIDItem->setTextAlignment(Qt::AlignCenter);
        ui->tableMedicalRecord->setItem(row, 3, doctorIDItem);

        // Cột 4: Tên Bác Sĩ
        QString doctorName = "N/A";
        try {
            doctorName = QString::fromStdString(DoctorManager::getInstance().getDoctorByID(record.getDoctorID()).getName());
        } catch (...) {}
        QTableWidgetItem* doctorNameItem = new QTableWidgetItem(doctorName);
        doctorNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableMedicalRecord->setItem(row, 4, doctorNameItem);

        // Cột 5: Ngày Tạo
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(record.getCreationDate().toString()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        ui->tableMedicalRecord->setItem(row, 5, dateItem);

        // Cột 6: Chẩn Đoán
        QString diagnosis = QString::fromStdString(record.getDiagnosis());
        if (diagnosis.length() > 50) {
            diagnosis = diagnosis.left(47) + "...";
        }
        QTableWidgetItem* diagnosisItem = new QTableWidgetItem(diagnosis);
        diagnosisItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableMedicalRecord->setItem(row, 6, diagnosisItem);

        // Cột 7: Tùy Chọn
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("recordID", record.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2196F3, stop:1 #1976D2);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #1976D2, stop:1 #1565C0);
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewMedicalRecordDetail_clicked);

        QPushButton* btnDelete = new QPushButton("Xóa");
        btnDelete->setProperty("recordID", record.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f44336, stop:1 #d32f2f);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #d32f2f, stop:1 #c62828);
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveMedicalRecord_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableMedicalRecord->setCellWidget(row, 7, actionWidget);
    }

    updateMedicalRecordPaginationUI();
}

void AdminWindow::loadAppointmentData(int page, const QString& searchText)
{
    qDebug() << "Loading appointment data for page" << page << "with search text:" << searchText;

    // Lấy dữ liệu từ Singleton
    const auto& allAppointments = AppointmentManager::getInstance().getAllAppointmentsAsVector();
    std::vector<Appointment> filteredAppointments;

    for (const auto& appt : allAppointments) {
        // Lấy tên BN/BS từ Singleton
        QString patientName = "N/A";
        try {
            patientName = QString::fromStdString(PatientManager::getInstance().getPatientByID(appt.getPatientID()).getName());
        } catch (...) {}

        QString doctorName = "N/A";
        try {
            doctorName = QString::fromStdString(DoctorManager::getInstance().getDoctorByID(appt.getDoctorID()).getName());
        } catch (...) {}

        // Tạo list để tìm kiếm
        QStringList apptDetails;
        apptDetails << QString::number(appt.getID())
                    << patientName
                    << doctorName
                    << QString::fromStdString(appt.getDate().toString())
                    << QString::fromStdString(appt.getStartTime())
                    << QString::fromStdString(appt.getEndTime())
                    << QString::fromStdString(Appointment::statusToString(appt.getStatus()));

        bool match = false;
        if (searchText.isEmpty()) {
            match = true;
        } else {
            QString lowerSearchText = searchText.toLower();
            for (const QString& item : apptDetails) {
                if (item.toLower().contains(lowerSearchText)) {
                    match = true;
                    break;
                }
            }
        }

        if (match) {
            filteredAppointments.push_back(appt);
        }
    }

    switch (currentAppointmentSortMode) {
    case AppointmentSortMode::BY_ID_ASC:
        std::sort(filteredAppointments.begin(), filteredAppointments.end(),
                  [](const Appointment& a, const Appointment& b) {
                      return a.getID() < b.getID();
                  });
        qDebug() << "[SORT] Applied: Appointment ID Ascending";
        break;

    case AppointmentSortMode::BY_NAME_ASC:
        std::sort(filteredAppointments.begin(), filteredAppointments.end(),
                  [](const Appointment& a, const Appointment& b) {
                      try {
                          std::string nameA = Utils::toLower(PatientManager::getInstance().getPatientByID(a.getPatientID()).getName());
                          std::string nameB = Utils::toLower(PatientManager::getInstance().getPatientByID(b.getPatientID()).getName());
                          return nameA < nameB;
                      } catch (...) {
                          return a.getID() < b.getID();
                      }
                  });
        qDebug() << "[SORT] Applied: Appointment Patient Name A-Z";
        break;

    case AppointmentSortMode::BY_NAME_DESC:
        std::sort(filteredAppointments.begin(), filteredAppointments.end(),
                  [](const Appointment& a, const Appointment& b) {
                      try {
                          std::string nameA = Utils::toLower(PatientManager::getInstance().getPatientByID(a.getPatientID()).getName());
                          std::string nameB = Utils::toLower(PatientManager::getInstance().getPatientByID(b.getPatientID()).getName());
                          return nameA > nameB;
                      } catch (...) {
                          return a.getID() < b.getID();
                      }
                  });
        qDebug() << "[SORT] Applied: Appointment Patient Name Z-A";
        break;
    }

    // Tính toán phân trang
    int totalItems = filteredAppointments.size();
    totalAppointmentPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalAppointmentPages) page = totalAppointmentPages;
    currentAppointmentPage = page;

    updateAppointmentPaginationUI();

    // Đổ dữ liệu vào bảng - tăng thêm 1 cột cho Tuỳ chọn
    ui->tableAppointment->setRowCount(0);
    int startIdx = (currentAppointmentPage - 1) * itemsPerPage;
    int endIdx = qMin(startIdx + itemsPerPage, totalItems);

    for (int i = startIdx; i < endIdx; ++i) {
        const Appointment& appt = filteredAppointments.at(i);
        ui->tableAppointment->insertRow(ui->tableAppointment->rowCount());
        int rowIdx = ui->tableAppointment->rowCount() - 1;

        // Lấy lại tên để hiển thị
        QString patientName = "N/A";
        try {
            patientName = QString::fromStdString(PatientManager::getInstance().getPatientByID(appt.getPatientID()).getName());
        } catch (...) {}

        QString doctorName = "N/A";
        try {
            doctorName = QString::fromStdString(DoctorManager::getInstance().getDoctorByID(appt.getDoctorID()).getName());
        } catch (...) {}

        // Cột 0: ID (căn giữa)
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(appt.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableAppointment->setItem(rowIdx, 0, idItem);

        // Cột 1: Bệnh nhân (căn trái)
        QTableWidgetItem* patientNameItem = new QTableWidgetItem(patientName);
        patientNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableAppointment->setItem(rowIdx, 1, patientNameItem);

        // Cột 2: Bác sĩ (căn trái)
        QTableWidgetItem* doctorNameItem = new QTableWidgetItem(doctorName);
        doctorNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableAppointment->setItem(rowIdx, 2, doctorNameItem);

        // Cột 3: Ngày hẹn (căn giữa)
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(appt.getDate().toString()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        ui->tableAppointment->setItem(rowIdx, 3, dateItem);

        // Cột 4: Giờ bắt đầu (căn giữa)
        QTableWidgetItem* startTimeItem = new QTableWidgetItem(QString::fromStdString(appt.getStartTime()));
        startTimeItem->setTextAlignment(Qt::AlignCenter);
        ui->tableAppointment->setItem(rowIdx, 4, startTimeItem);

        // Cột 5: Giờ kết thúc (căn giữa)
        QTableWidgetItem* endTimeItem = new QTableWidgetItem(QString::fromStdString(appt.getEndTime()));
        endTimeItem->setTextAlignment(Qt::AlignCenter);
        ui->tableAppointment->setItem(rowIdx, 5, endTimeItem);

        // Cột 6: Trạng thái (căn giữa) - dùng đúng các giá trị enum từ appointment.h
        QString statusStr = QString::fromStdString(Appointment::statusToString(appt.getStatus()));
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusStr);
        statusItem->setTextAlignment(Qt::AlignCenter);

        // Thêm màu nền tùy theo trạng thái (dùng đúng các giá trị enum)
        if (appt.getStatus() == Appointment::Status::Occupied) {
            statusItem->setBackground(QBrush(QColor(76, 175, 80, 50))); // Xanh lá nhạt - Đã chiếm dụng
        } else if (appt.getStatus() == Appointment::Status::Canceled) {
            statusItem->setBackground(QBrush(QColor(244, 67, 54, 50))); // Đỏ nhạt - Đã hủy
        } else if (appt.getStatus() == Appointment::Status::Scheduled) {
            statusItem->setBackground(QBrush(QColor(255, 193, 7, 50))); // Vàng nhạt - Đã lên lịch
        }

        ui->tableAppointment->setItem(rowIdx, 6, statusItem);

        // Cột 7: Tuỳ chọn - Thêm 2 nút "Xem chi tiết" và "Xoá"
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        // Button "Xem chi tiết"
        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("appointmentID", appt.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2196F3, stop:1 #1976D2);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #1976D2, stop:1 #1565C0);
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewAppointmentDetail_clicked);

        // Button "Xoá"
        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("appointmentID", appt.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f44336, stop:1 #d32f2f);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #d32f2f, stop:1 #c62828);
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveAppointment_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableAppointment->setCellWidget(rowIdx, 7, actionWidget);
    }
}

void AdminWindow::loadUserData(int page, const QString& searchText) {
    qDebug() << "Loading user data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allUsers = UserManager::getInstance().getAllUsers();
    std::vector<User> filteredUsers;

    // 2. Lọc theo ID hoặc Username (hoặc hiển thị tất cả nếu searchText rỗng)
    if (searchText.isEmpty()) {
        for (const auto& pair : allUsers) {
            filteredUsers.push_back(pair.second);
        }
    } else {
        // Use Utils::trimmed for consistency
        std::string searchStd = Utils::trimmed(searchText.toStdString());
        bool isNumber = false;
        int searchID = QString::fromStdString(searchStd).toInt(&isNumber);

        // Tìm theo ID nếu là số
        if (isNumber) {
            auto it = allUsers.find(searchID);
            if (it != allUsers.end()) {
                filteredUsers.push_back(it->second);
            }
        }

        // Tìm theo Username (không phân biệt hoa thường)
        std::string searchLower = Utils::toLower(searchStd);
        for (const auto& pair : allUsers) {
            std::string username = Utils::toLower(Utils::trimmed(pair.second.getUsername()));
            if (username.find(searchLower) != std::string::npos) {
                // Tránh trùng nếu ID và username trỏ cùng 1 user
                if (std::none_of(filteredUsers.begin(), filteredUsers.end(),
                                 [id = pair.second.getID()](const User& u) { return u.getID() == id; })) {
                    filteredUsers.push_back(pair.second);
                }
            }
        }

        if (filteredUsers.empty()) {
            QMessageBox::information(this, "Kết quả tìm kiếm",
                                     QString("Không tìm thấy người dùng với ID hoặc Username: %1").arg(QString::fromStdString(searchStd)));
            for (const auto& pair : allUsers) {
                filteredUsers.push_back(pair.second);
            }
        }
    }

    // 3. SẮP XẾP theo mode hiện tại
    switch (currentUserSortMode) {
    case UserSortMode::BY_ID_ASC:
        std::sort(filteredUsers.begin(), filteredUsers.end(),
                  [](const User& a, const User& b) {
                      return a.getID() < b.getID();
                  });
        qDebug() << "[SORT] Applied: ID Ascending";
        break;

    case UserSortMode::BY_USERNAME_ASC:
        std::sort(filteredUsers.begin(), filteredUsers.end(),
                  [](const User& a, const User& b) {
                      std::string nameA = Utils::toLower(a.getUsername());
                      std::string nameB = Utils::toLower(b.getUsername());
                      return nameA < nameB;
                  });
        qDebug() << "[SORT] Applied: Username A-Z";
        break;

    case UserSortMode::BY_USERNAME_DESC:
        std::sort(filteredUsers.begin(), filteredUsers.end(),
                  [](const User& a, const User& b) {
                      std::string nameA = Utils::toLower(a.getUsername());
                      std::string nameB = Utils::toLower(b.getUsername());
                      return nameA > nameB;
                  });
        qDebug() << "[SORT] Applied: Username Z-A";
        break;
    }

    // 4. Tính toán phân trang
    int totalItems = filteredUsers.size();
    totalUserPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalUserPages) page = totalUserPages;
    currentUserPage = page;

    qDebug() << "[USER] Total items:" << totalItems
             << "| Total pages:" << totalUserPages
             << "| Current page:" << currentUserPage;

    ui->tableUser->setRowCount(0);
    int startIdx = (currentUserPage - 1) * itemsPerPage;
    int endIdx = qMin(startIdx + itemsPerPage, totalItems);
    
    for (int i = startIdx; i < endIdx; ++i) {
        const User& user = filteredUsers.at(i);
        ui->tableUser->insertRow(ui->tableUser->rowCount());
        int row = ui->tableUser->rowCount() - 1;

        // Cột 0: ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(user.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableUser->setItem(row, 0, idItem);

        // Cột 1: Vai trò
        QTableWidgetItem* roleItem = new QTableWidgetItem(QString::fromStdString(User::roleToString(user.getRole())));
        roleItem->setTextAlignment(Qt::AlignCenter);
        if (user.getRole() == User::Role::ADMIN) {
            roleItem->setBackground(QBrush(QColor(255, 87, 34, 50)));
        } else if (user.getRole() == User::Role::ASSISTANT) {
            roleItem->setBackground(QBrush(QColor(76, 175, 80, 50)));
        }
        ui->tableUser->setItem(row, 1, roleItem);

        // Cột 2: Username
        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(user.getUsername()));
        usernameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableUser->setItem(row, 2, usernameItem);

        // ✅ Cột 3: Họ và tên
        QString fullName = QString::fromStdString(user.getFullName());
        if (fullName.isEmpty()) fullName = "N/A";
        QTableWidgetItem* fullNameItem = new QTableWidgetItem(fullName);
        fullNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableUser->setItem(row, 3, fullNameItem);

        // ✅ Cột 4: CCCD
        QString cccd = QString::fromStdString(user.getCCCD());
        if (cccd.isEmpty()) cccd = "N/A";
        QTableWidgetItem* cccdItem = new QTableWidgetItem(cccd);
        cccdItem->setTextAlignment(Qt::AlignCenter);
        ui->tableUser->setItem(row, 4, cccdItem);

        // Cột 5: Mật khẩu (ẨN)
        QString plainPassword = QString::fromStdString(user.getPlainPassword());
        QString maskedPassword = QString("●").repeated(qMin(plainPassword.length(), 12));

        QTableWidgetItem* passwordItem = new QTableWidgetItem(maskedPassword);
        passwordItem->setTextAlignment(Qt::AlignCenter);
        passwordItem->setForeground(QBrush(QColor(120, 120, 120)));
        passwordItem->setToolTip(QString("Độ dài: %1 ký tự").arg(plainPassword.length()));
        ui->tableUser->setItem(row, 5, passwordItem);

        // Cột 6: Tùy chọn
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("userID", user.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2196F3, stop:1 #1976D2);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #1976D2, stop:1 #1565C0);
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewUserDetail_clicked);

        QPushButton* btnDelete = new QPushButton("Xóa");
        btnDelete->setProperty("userID", user.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f44336, stop:1 #d32f2f);
                color: white;
                border: none;
                padding: 8px 15px;
                border-radius: 6px;
                font-weight: 600;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #d32f2f, stop:1 #c62828);
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveUser_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableUser->setCellWidget(row, 6, actionWidget);
    }

    updateUserPaginationUI();
}

void AdminWindow::setupAppointmentTable() {
    ui->tableAppointment->setColumnCount(8);

    QStringList headers;
    headers << "ID Cuộc hẹn" << "Bệnh nhân" << "Bác sĩ" << "Ngày hẹn"
            << "Giờ bắt đầu" << "Giờ kết thúc" << "Trạng thái" << "Tuỳ chọn";
    ui->tableAppointment->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tableAppointment->horizontalHeader();

    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(0, 100);

    header->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableAppointment->setColumnWidth(1, 150);

    header->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableAppointment->setColumnWidth(2, 150);

    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(3, 120);

    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(4, 100);

    header->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(5, 100);

    header->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(6, 120);

    header->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableAppointment->setColumnWidth(7, 300);

    ui->tableAppointment->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableAppointment->verticalHeader()->setDefaultSectionSize(75);
    ui->tableAppointment->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableAppointment->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableAppointment->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableAppointment->setAlternatingRowColors(true);
    ui->tableAppointment->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] Appointment table configured successfully with Action column";
}

void AdminWindow::setupPatientTable() {
    ui->tablePatient->setColumnCount(8);
    QStringList headers;
    headers << "ID Bệnh nhân" << "Họ tên" << "Giới tính" << "Ngày sinh"
            << "CCCD" << "SĐT" << "Email" << "Tuỳ chọn";
    ui->tablePatient->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tablePatient->horizontalHeader();

    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(0, 100);

    header->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tablePatient->setColumnWidth(1, 100);

    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(2, 80);

    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(3, 120);

    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(4, 100);

    header->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(5, 100);

    header->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(6, 200);

    header->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(7, 300);

    // Các thiết lập khác...
    ui->tablePatient->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tablePatient->verticalHeader()->setDefaultSectionSize(75);
    ui->tablePatient->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablePatient->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tablePatient->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablePatient->setAlternatingRowColors(true);
    ui->tablePatient->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] Patient table configured successfully (without Email column)";
}

void AdminWindow::setupDoctorTable() {
    ui->tableDoctor->setColumnCount(10);

    QStringList headers;
    headers << "ID Bác sĩ" << "Họ tên" << "Giới tính" << "Ngày sinh" << "CCCD"
            << "Chuyên khoa" << "Email" << "SĐT" << "Trạng thái" << "Tuỳ chọn";
    ui->tableDoctor->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tableDoctor->horizontalHeader();

    // Cột 0: ID (Fixed)
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(0, 80);

    // Cột 1: Họ tên (Stretch)
    header->setSectionResizeMode(1, QHeaderView::Stretch);

    // Cột 2: Giới tính (Fixed)
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(2, 80);

    // Cột 3: Ngày sinh (Fixed)
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(3, 120);

    // Cột 4: CCCD (Fixed)
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(4, 100);

    // Cột 5: Chuyên khoa (Stretch)
    header->setSectionResizeMode(5, QHeaderView::Stretch);

    // Cột 6: Email (Stretch)
    header->setSectionResizeMode(6, QHeaderView::Stretch);

    // Cột 7: SĐT (Fixed)
    header->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(7, 110);

    // Cột 8: Trạng thái (Fixed)
    header->setSectionResizeMode(8, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(8, 100);

    // Cột 9: Tuỳ chọn (Fixed)
    header->setSectionResizeMode(9, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(9, 300);

    // Các thiết lập khác
    ui->tableDoctor->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableDoctor->verticalHeader()->setDefaultSectionSize(75);
    ui->tableDoctor->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableDoctor->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableDoctor->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableDoctor->setAlternatingRowColors(true);
    ui->tableDoctor->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] Doctor table configured with 10 columns including Action column";
}

void AdminWindow::setupMedicalRecordTable() {
    ui->tableMedicalRecord->setColumnCount(8);

    QStringList headers;
    headers << "ID Hồ Sơ" << "ID Bệnh Nhân" << "Tên Bệnh Nhân"
            << "ID Bác Sĩ" << "Tên Bác Sĩ" << "Ngày Tạo"
            << "Chẩn Đoán" << "Tùy Chọn";
    ui->tableMedicalRecord->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tableMedicalRecord->horizontalHeader();

    // Cột 0: ID Hồ Sơ (Fixed)
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableMedicalRecord->setColumnWidth(0, 80);

    // Cột 1: ID Bệnh Nhân (Fixed)
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableMedicalRecord->setColumnWidth(1, 100);

    // Cột 2: Tên Bệnh Nhân (Stretch)
    header->setSectionResizeMode(2, QHeaderView::Stretch);

    // Cột 3: ID Bác Sĩ (Fixed)
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableMedicalRecord->setColumnWidth(3, 80);

    // Cột 4: Tên Bác Sĩ (Stretch)
    header->setSectionResizeMode(4, QHeaderView::Stretch);

    // Cột 5: Ngày Tạo (Fixed)
    header->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableMedicalRecord->setColumnWidth(5, 120);

    // Cột 6: Chẩn Đoán (Stretch)
    header->setSectionResizeMode(6, QHeaderView::Stretch);

    // Cột 7: Tùy Chọn (Fixed)
    header->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableMedicalRecord->setColumnWidth(7, 300);

    ui->tableMedicalRecord->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableMedicalRecord->verticalHeader()->setDefaultSectionSize(75);
    ui->tableMedicalRecord->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableMedicalRecord->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableMedicalRecord->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableMedicalRecord->setAlternatingRowColors(true);
    ui->tableMedicalRecord->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] Medical Record table configured successfully";
}

void AdminWindow::setupUserTable() {
    ui->tableUser->setColumnCount(7);

    QStringList headers;
    headers << "ID" << "Vai trò" << "Tên người dùng" << "Họ và tên" << "CCCD" << "Mật khẩu" << "Tùy chọn";
    ui->tableUser->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tableUser->horizontalHeader();

    // Cột 0: ID (Fixed)
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(0, 80);

    // Cột 1: Vai trò (Fixed)
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(1, 120);

    // Cột 2: Tên người dùng (Fixed)
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(2, 150);

    // ✅ Cột 3: Họ và tên (Stretch)
    header->setSectionResizeMode(3, QHeaderView::Stretch);

    // ✅ Cột 4: CCCD (Fixed)
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(4, 120);

    // Cột 5: Mật khẩu (Fixed)
    header->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(5, 120);

    // Cột 6: Tùy chọn (Fixed)
    header->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(6, 300);

    ui->tableUser->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableUser->verticalHeader()->setDefaultSectionSize(75);
    ui->tableUser->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUser->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableUser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUser->setAlternatingRowColors(true);
    ui->tableUser->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] User table configured successfully with 7 columns";
}

void AdminWindow::updateAppointmentPaginationUI() {
    ui->lblCurrentPage_Appointment->setText(QString("Trang %1/%2").arg(currentAppointmentPage).arg(totalAppointmentPages));

    // Cập nhật trạng thái nút Prev/Next
    ui->btnPrevPage_Appointment->setEnabled(currentAppointmentPage > 1);
    ui->btnNextPage_Appointment->setEnabled(currentAppointmentPage < totalAppointmentPages);

    // Cập nhật các nút số trang
    int startPage, endPage;

    if (totalAppointmentPages <= 3) {
        startPage = 1;
        endPage = totalAppointmentPages;
    } else {
        startPage = qMax(1, currentAppointmentPage - 1);
        endPage = qMin(totalAppointmentPages, currentAppointmentPage + 1);

        if (endPage - startPage < 2) {
            if (currentAppointmentPage == 1) {
                endPage = qMin(3, totalAppointmentPages);
            } else if (currentAppointmentPage == totalAppointmentPages) {
                startPage = qMax(1, totalAppointmentPages - 2);
            }
        }
    }

    // Nút 1
    if (startPage <= totalAppointmentPages) {
        ui->btnPage_Appointment_1->setText(QString::number(startPage));
        ui->btnPage_Appointment_1->setVisible(true);
        ui->btnPage_Appointment_1->setEnabled(startPage != currentAppointmentPage);
        if (startPage == currentAppointmentPage) {
            ui->btnPage_Appointment_1->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Appointment_1->setStyleSheet("");
        }
    } else {
        ui->btnPage_Appointment_1->setVisible(false);
    }

    // Nút 2
    if (startPage + 1 <= totalAppointmentPages) {
        ui->btnPage_Appointment_2->setText(QString::number(startPage + 1));
        ui->btnPage_Appointment_2->setVisible(true);
        ui->btnPage_Appointment_2->setEnabled(startPage + 1 != currentAppointmentPage);
        if (startPage + 1 == currentAppointmentPage) {
            ui->btnPage_Appointment_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Appointment_2->setStyleSheet("");
        }
    } else {
        ui->btnPage_Appointment_2->setVisible(false);
    }

    // Nút 3
    if (startPage + 2 <= totalAppointmentPages) {
        ui->btnPage_Appointment_3->setText(QString::number(startPage + 2));
        ui->btnPage_Appointment_3->setVisible(true);
        ui->btnPage_Appointment_3->setEnabled(startPage + 2 != currentAppointmentPage);
        if (startPage + 2 == currentAppointmentPage) {
            ui->btnPage_Appointment_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Appointment_3->setStyleSheet("");
        }
    } else {
        ui->btnPage_Appointment_3->setVisible(false);
    }

    qDebug() << "[APPOINTMENT PAGINATION] Current:" << currentAppointmentPage
             << "| Total:" << totalAppointmentPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::updatePatientPaginationUI() {
    // Cập nhật label hiển thị trang hiện tại
    ui->lblCurrentPage_Patient->setText(QString("Trang %1/%2").arg(currentPatientPage).arg(totalPatientPages));

    // ✅ Cập nhật trạng thái nút Prev/Next
    ui->btnPrevPage_Patient->setEnabled(currentPatientPage > 1);
    ui->btnNextPage_Patient->setEnabled(currentPatientPage < totalPatientPages);

    // ✅ Cập nhật các nút số trang (luôn hiển thị 3 nút xung quanh trang hiện tại)
    int startPage, endPage;

    if (totalPatientPages <= 3) {
        // Nếu tổng số trang <= 3, hiển thị tất cả
        startPage = 1;
        endPage = totalPatientPages;
    } else {
        // Nếu > 3 trang, hiển thị current-1, current, current+1
        startPage = qMax(1, currentPatientPage - 1);
        endPage = qMin(totalPatientPages, currentPatientPage + 1);

        // Đảm bảo luôn có 3 nút
        if (endPage - startPage < 2) {
            if (currentPatientPage == 1) {
                endPage = qMin(3, totalPatientPages);
            } else if (currentPatientPage == totalPatientPages) {
                startPage = qMax(1, totalPatientPages - 2);
            }
        }
    }

    // Nút 1
    if (startPage <= totalPatientPages) {
        ui->btnPage_Patient_1->setText(QString::number(startPage));
        ui->btnPage_Patient_1->setVisible(true);
        ui->btnPage_Patient_1->setEnabled(startPage != currentPatientPage);
        // Highlight nút hiện tại
        if (startPage == currentPatientPage) {
            ui->btnPage_Patient_1->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Patient_1->setStyleSheet("");
        }
    } else {
        ui->btnPage_Patient_1->setVisible(false);
    }

    // Nút 2
    if (startPage + 1 <= totalPatientPages) {
        ui->btnPage_Patient_2->setText(QString::number(startPage + 1));
        ui->btnPage_Patient_2->setVisible(true);
        ui->btnPage_Patient_2->setEnabled(startPage + 1 != currentPatientPage);
        if (startPage + 1 == currentPatientPage) {
            ui->btnPage_Patient_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Patient_2->setStyleSheet("");
        }
    } else {
        ui->btnPage_Patient_2->setVisible(false);
    }

    // Nút 3
    if (startPage + 2 <= totalPatientPages) {
        ui->btnPage_Patient_3->setText(QString::number(startPage + 2));
        ui->btnPage_Patient_3->setVisible(true);
        ui->btnPage_Patient_3->setEnabled(startPage + 2 != currentPatientPage);
        if (startPage + 2 == currentPatientPage) {
            ui->btnPage_Patient_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Patient_3->setStyleSheet("");
        }
    } else {
        ui->btnPage_Patient_3->setVisible(false);
    }

    qDebug() << "[PATIENT PAGINATION] Current:" << currentPatientPage
             << "| Total:" << totalPatientPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::updateDoctorPaginationUI() {
    // Cập nhật label hiển thị trang hiện tại
    ui->lblCurrentPage_Doctor->setText(QString("Trang %1/%2").arg(currentDoctorPage).arg(totalDoctorPages));

    // ✅ Cập nhật trạng thái nút Prev/Next
    ui->btnPrevPage_Doctor->setEnabled(currentDoctorPage > 1);
    ui->btnNextPage_Doctor->setEnabled(currentDoctorPage < totalDoctorPages);

    // ✅ Cập nhật các nút số trang
    int startPage, endPage;

    if (totalDoctorPages <= 3) {
        startPage = 1;
        endPage = totalDoctorPages;
    } else {
        startPage = qMax(1, currentDoctorPage - 1);
        endPage = qMin(totalDoctorPages, currentDoctorPage + 1);

        if (endPage - startPage < 2) {
            if (currentDoctorPage == 1) {
                endPage = qMin(3, totalDoctorPages);
            } else if (currentDoctorPage == totalDoctorPages) {
                startPage = qMax(1, totalDoctorPages - 2);
            }
        }
    }

    // Nút 1
    if (startPage <= totalDoctorPages) {
        ui->btnPage_Doctor_1->setText(QString::number(startPage));
        ui->btnPage_Doctor_1->setVisible(true);
        ui->btnPage_Doctor_1->setEnabled(startPage != currentDoctorPage);
        if (startPage == currentDoctorPage) {
            ui->btnPage_Doctor_1->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Doctor_1->setStyleSheet("");
        }
    } else {
        ui->btnPage_Doctor_1->setVisible(false);
    }

    // Nút 2
    if (startPage + 1 <= totalDoctorPages) {
        ui->btnPage_Doctor_2->setText(QString::number(startPage + 1));
        ui->btnPage_Doctor_2->setVisible(true);
        ui->btnPage_Doctor_2->setEnabled(startPage + 1 != currentDoctorPage);
        if (startPage + 1 == currentDoctorPage) {
            ui->btnPage_Doctor_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Doctor_2->setStyleSheet("");
        }
    } else {
        ui->btnPage_Doctor_2->setVisible(false);
    }

    // Nút 3
    if (startPage + 2 <= totalDoctorPages) {
        ui->btnPage_Doctor_3->setText(QString::number(startPage + 2));
        ui->btnPage_Doctor_3->setVisible(true);
        ui->btnPage_Doctor_3->setEnabled(startPage + 2 != currentDoctorPage);
        if (startPage + 2 == currentDoctorPage) {
            ui->btnPage_Doctor_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_Doctor_3->setStyleSheet("");
        }
    } else {
        ui->btnPage_Doctor_3->setVisible(false);
    }

    qDebug() << "[DOCTOR PAGINATION] Current:" << currentDoctorPage
             << "| Total:" << totalDoctorPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::updateMedicalRecordPaginationUI() {
    ui->lblCurrentPage_MedicalRecord->setText(QString("Trang %1/%2").arg(currentMedicalRecordPage).arg(totalMedicalRecordPages));

    ui->btnPrevPage_MedicalRecord->setEnabled(currentMedicalRecordPage > 1);
    ui->btnNextPage_MedicalRecord->setEnabled(currentMedicalRecordPage < totalMedicalRecordPages);

    int startPage, endPage;

    if (totalMedicalRecordPages <= 3) {
        startPage = 1;
        endPage = totalMedicalRecordPages;
    } else {
        startPage = qMax(1, currentMedicalRecordPage - 1);
        endPage = qMin(totalMedicalRecordPages, currentMedicalRecordPage + 1);

        if (endPage - startPage < 2) {
            if (currentMedicalRecordPage == 1) {
                endPage = qMin(3, totalMedicalRecordPages);
            } else if (currentMedicalRecordPage == totalMedicalRecordPages) {
                startPage = qMax(1, totalMedicalRecordPages - 2);
            }
        }
    }

    // Nút 1
    if (startPage <= totalMedicalRecordPages) {
        ui->btnPage_MedicalRecord_1->setText(QString::number(startPage));
        ui->btnPage_MedicalRecord_1->setVisible(true);
        ui->btnPage_MedicalRecord_1->setEnabled(startPage != currentMedicalRecordPage);
        if (startPage == currentMedicalRecordPage) {
            ui->btnPage_MedicalRecord_1->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_MedicalRecord_1->setStyleSheet("");
        }
    } else {
        ui->btnPage_MedicalRecord_1->setVisible(false);
    }

    // Nút 2
    if (startPage + 1 <= totalMedicalRecordPages) {
        ui->btnPage_MedicalRecord_2->setText(QString::number(startPage + 1));
        ui->btnPage_MedicalRecord_2->setVisible(true);
        ui->btnPage_MedicalRecord_2->setEnabled(startPage + 1 != currentMedicalRecordPage);
        if (startPage + 1 == currentMedicalRecordPage) {
            ui->btnPage_MedicalRecord_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_MedicalRecord_2->setStyleSheet("");
        }
    } else {
        ui->btnPage_MedicalRecord_2->setVisible(false);
    }

    // Nút 3
    if (startPage + 2 <= totalMedicalRecordPages) {
        ui->btnPage_MedicalRecord_3->setText(QString::number(startPage + 2));
        ui->btnPage_MedicalRecord_3->setVisible(true);
        ui->btnPage_MedicalRecord_3->setEnabled(startPage + 2 != currentMedicalRecordPage);
        if (startPage + 2 == currentMedicalRecordPage) {
            ui->btnPage_MedicalRecord_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_MedicalRecord_3->setStyleSheet("");
        }
    } else {
        ui->btnPage_MedicalRecord_3->setVisible(false);
    }

    qDebug() << "[MEDICAL RECORD PAGINATION] Current:" << currentMedicalRecordPage
             << "| Total:" << totalMedicalRecordPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::updateUserPaginationUI() {
    ui->lblCurrentPage_User->setText(QString("Trang %1/%2").arg(currentUserPage).arg(totalUserPages));

    // Sửa tên biến: pushButton_5 → btnPrevPage_User, pushButton_4 → btnNextPage_User
    ui->btnPrevPage_User->setEnabled(currentUserPage > 1);
    ui->btnNextPage_User->setEnabled(currentUserPage < totalUserPages);

    int startPage, endPage;

    if (totalUserPages <= 3) {
        startPage = 1;
        endPage = totalUserPages;
    } else {
        startPage = qMax(1, currentUserPage - 1);
        endPage = qMin(totalUserPages, currentUserPage + 1);

        if (endPage - startPage < 2) {
            if (currentUserPage == 1) {
                endPage = qMin(3, totalUserPages);
            } else if (currentUserPage == totalUserPages) {
                startPage = qMax(1, totalUserPages - 2);
            }
        }
    }

    // Nút 1 - sửa tên biến: pushButton_3 → btnPage_User_1
    if (startPage <= totalUserPages) {
        ui->btnPage_User_1->setText(QString::number(startPage));
        ui->btnPage_User_1->setVisible(true);
        ui->btnPage_User_1->setEnabled(startPage != currentUserPage);
        if (startPage == currentUserPage) {
            ui->btnPage_User_1->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_User_1->setStyleSheet("");
        }
    } else {
        ui->btnPage_User_1->setVisible(false);
    }

    // Nút 2 - sửa tên biến: pushButton_2 → btnPage_User_2
    if (startPage + 1 <= totalUserPages) {
        ui->btnPage_User_2->setText(QString::number(startPage + 1));
        ui->btnPage_User_2->setVisible(true);
        ui->btnPage_User_2->setEnabled(startPage + 1 != currentUserPage);
        if (startPage + 1 == currentUserPage) {
            ui->btnPage_User_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_User_2->setStyleSheet("");
        }
    } else {
        ui->btnPage_User_2->setVisible(false);
    }

    // Nút 3 - sửa tên biến: pushButton → btnPage_User_3
    if (startPage + 2 <= totalUserPages) {
        ui->btnPage_User_3->setText(QString::number(startPage + 2));
        ui->btnPage_User_3->setVisible(true);
        ui->btnPage_User_3->setEnabled(startPage + 2 != currentUserPage);
        if (startPage + 2 == currentUserPage) {
            ui->btnPage_User_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->btnPage_User_3->setStyleSheet("");
        }
    } else {
        ui->btnPage_User_3->setVisible(false);
    }

    qDebug() << "[USER PAGINATION] Current:" << currentUserPage
             << "| Total:" << totalUserPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::on_appointmentManagerButton_clicked()
{
    if (!hasAppointmentAccess()) { showNoPermissionMessage(); return; }
    setActiveSidebarButton(ui->appointmentManagerButton);
    ui->mainStack->setCurrentWidget(ui->page_appointment);

    AppointmentManager::getInstance().removeDuplicates();

    // Reset sort về ID Ascending khi chuyển sang trang appointment
    currentAppointmentSortMode = AppointmentSortMode::BY_ID_ASC;
    ui->btnSortAZAppointment->setStyleSheet("");
    ui->btnSortZAAppointment->setStyleSheet("");
    ui->txtSearchAppointment->clear();

    setupAppointmentTable();
    loadAppointmentData(1, "");
}

void AdminWindow::on_patientManagerButton_clicked()
{
    if (!hasPatientAccess()) { showNoPermissionMessage(); return; }
    setActiveSidebarButton(ui->patientManagerButton);
    ui->mainStack->setCurrentWidget(ui->page_patient);

    currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    ui->btnSortAZPatient->setStyleSheet("");
    ui->btnSortZAPatient->setStyleSheet("");
    ui->txtSearchPatient->clear();

    setupPatientTable();
    loadPatientData(1, "");

    qDebug() << "[INFO] Switched to Patient Manager, sorted by ID ascending";
}

void AdminWindow::on_doctorManagerButton_clicked()
{
    if (!hasDoctorAccess()) { showNoPermissionMessage(); return; }
    setActiveSidebarButton(ui->doctorManagerButton);
    ui->mainStack->setCurrentWidget(ui->page_doctor);

    currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;
    ui->btnSortAZDoctor->setStyleSheet("");
    ui->btnSortZADoctor->setStyleSheet("");
    ui->txtSearchDoctor->clear();

    setupDoctorTable();
    loadDoctorData(1, "");

    qDebug() << "[INFO] Switched to Doctor Manager, sorted by ID ascending";
}


void AdminWindow::on_userManagerButton_clicked()
{
    if (!hasUserAccess()) { showNoPermissionMessage(); return; }
    setActiveSidebarButton(ui->userManagerButton);
    ui->mainStack->setCurrentWidget(ui->page_user);

    currentUserSortMode = UserSortMode::BY_ID_ASC;
    ui->btnSortAZUser->setStyleSheet("");
    ui->btnSortZAUser->setStyleSheet("");
    ui->txtSearchUser->clear();

    setupUserTable();
    loadUserData(1, "");

    qDebug() << "[INFO] Switched to User Manager, sorted by ID ascending";
}

void AdminWindow::on_medicalRecordButton_clicked()
{
    if (!hasMedicalRecordAccess()) { showNoPermissionMessage(); return; }
    setActiveSidebarButton(ui->medicalRecordButton);
    ui->mainStack->setCurrentWidget(ui->page_medicalRecord);

    currentMedicalRecordSortMode = MedicalRecordSortMode::BY_ID_ASC;
    ui->btnSortAZMedicalRecord->setStyleSheet("");
    ui->btnSortZAMedicalRecord->setStyleSheet("");
    ui->txtSearchMedicalRecord->clear();

    setupMedicalRecordTable();
    loadMedicalRecordData(1, "");
}

void AdminWindow::on_logoutButton_clicked()
{
    Core::saveAll();
    this->hide();
    if (parentWidget()) {
        parentWidget()->show();
    }
}

void AdminWindow::on_btnAddAppointment_clicked() {
    if (!hasAppointmentAccess(true)) {
        showNoPermissionMessage();
        return;
    }

    qDebug() << "Thêm cuộc hẹn mới clicked.";

    AddAppointmentDialog addDialog(this);

    if (addDialog.exec() == QDialog::Accepted) {
        try {
            Appointment newAppt = addDialog.getAppointmentData();

            // Kiểm tra lại trước khi thêm
            if (newAppt.getDoctorID() <= 0 || newAppt.getPatientID() <= 0) {
                QMessageBox::critical(this, "Lỗi", "Dữ liệu cuộc hẹn không hợp lệ!");
                return;
            }

            // Kiểm tra bác sĩ và bệnh nhân tồn tại
            try {
                DoctorManager::getInstance().getDoctorByID(newAppt.getDoctorID());
                PatientManager::getInstance().getPatientByID(newAppt.getPatientID());
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Lỗi",
                                      QString("Không tìm thấy bác sĩ hoặc bệnh nhân: %1").arg(e.what()));
                return;
            }

            // Thêm appointment
            AppointmentManager::getInstance().addAppointment(newAppt);

            // Lưu file
            AppointmentManager::getInstance().saveToFile(Config::APPOINTMENT_PATH);

            QMessageBox::information(this, "Thành công",
                                     QString("Đã thêm cuộc hẹn mới:\n"
                                             "ID: %1\n"
                                             "Bệnh nhân: %2\n"
                                             "Bác sĩ: %3\n"
                                             "Ngày: %4\n"
                                             "Giờ: %5 - %6")
                                         .arg(newAppt.getID())
                                         .arg(newAppt.getPatientID())
                                         .arg(newAppt.getDoctorID())
                                         .arg(QString::fromStdString(newAppt.getDate().toString()))
                                         .arg(QString::fromStdString(newAppt.getStartTime()))
                                         .arg(QString::fromStdString(newAppt.getEndTime())));

            // Tải lại dữ liệu
            loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Cuộc Hẹn",
                                  QString("Không thể thêm cuộc hẹn:\n%1").arg(e.what()));
            qDebug() << "Error adding appointment: " << e.what();
        }
    } else {
        qDebug() << "Thêm cuộc hẹn đã bị hủy.";
    }
}

void AdminWindow::on_btnSearchAppointment_clicked()
{
    if (!hasAppointmentAccess(true)) { showNoPermissionMessage(); return; }
    qDebug() << "page_appointment: TÌM KIẾM clicked.";
    QString searchText = ui->txtSearchAppointment->text().trimmed();
    currentAppointmentPage = 1;
    loadAppointmentData(currentAppointmentPage, searchText);
}

void AdminWindow::on_btnRemoveAppointment_clicked() {
    if (!hasAppointmentAccess(true)) { showNoPermissionMessage(); return; }

    // Lấy appointment ID từ hàng được chọn
    if (!ui->tableAppointment->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một cuộc hẹn để xóa.");
        return;
    }

    int selectedRow = ui->tableAppointment->selectionModel()->selectedRows().first().row();
    int appointmentID = ui->tableAppointment->item(selectedRow, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xóa",
                                  "Bạn có chắc chắn muốn xóa cuộc hẹn ID: " + QString::number(appointmentID) + " không?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            AppointmentManager::getInstance().removeAppointment(appointmentID);
            QMessageBox::information(this, "Thành công", "Đã xóa cuộc hẹn ID: " + QString::number(appointmentID));

            loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Xóa Cuộc Hẹn", QString::fromStdString(e.what()));
        }
    }
}

void AdminWindow::on_btnEditAppointment_clicked() {
    if (!hasAppointmentAccess(true)) { showNoPermissionMessage(); return; }

    if (!ui->tableAppointment->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một cuộc hẹn để chỉnh sửa.");
        return;
    }

    int selectedRow = ui->tableAppointment->selectionModel()->selectedRows().first().row();
    int appointmentID = ui->tableAppointment->item(selectedRow, 0)->text().toInt();

    try {
        const Appointment& appointmentToEdit = AppointmentManager::getInstance().getAppointmentByID(appointmentID);

        AddAppointmentDialog editDialog(appointmentToEdit, this);
        editDialog.setEditMode(true);

        if (editDialog.exec() == QDialog::Accepted) {
            Appointment updatedAppt = editDialog.getAppointmentData();

            // ✅ Ensure ID hasn't changed
            if (updatedAppt.getID() != appointmentID) {
                qWarning() << "[WARNING] Appointment ID changed during edit! Restoring original ID.";
                updatedAppt.setID(appointmentID);
            }

            // Update appointment
            AppointmentManager::getInstance().editAppointment(appointmentID, updatedAppt);
            AppointmentManager::getInstance().saveToFile(Config::APPOINTMENT_PATH);

            QMessageBox::information(this, "Thành công",
                                     QString("Đã cập nhật cuộc hẹn ID: %1").arg(appointmentID));

            loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

void AdminWindow::on_btnNextPage_Appointment_clicked()
{
    if (!hasAppointmentAccess(true)) { showNoPermissionMessage(); return; }
    if (currentAppointmentPage < totalAppointmentPages) {
        currentAppointmentPage++;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Appointment_clicked()
{
    if (!hasPatientAccess(true)) { showNoPermissionMessage(); return; }
    if (currentAppointmentPage > 1) {
        currentAppointmentPage--;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_1_clicked()
{
    if (!hasPatientAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Appointment_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_2_clicked()
{
    if (!hasPatientAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Appointment_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_3_clicked()
{
    if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Appointment_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnSortAZAppointment_clicked() {
    if (!hasAppointmentAccess()) { showNoPermissionMessage(); return; }
    qDebug() << "[SORT] Appointment Button A-Z clicked";

    currentAppointmentSortMode = AppointmentSortMode::BY_NAME_ASC;

    ui->btnSortAZAppointment->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortZAAppointment->setStyleSheet("");

    loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
}

void AdminWindow::on_btnSortZAAppointment_clicked() {
    if (!hasAppointmentAccess()) { showNoPermissionMessage(); return; }
    qDebug() << "[SORT] Appointment Button Z-A clicked";

    currentAppointmentSortMode = AppointmentSortMode::BY_NAME_DESC;

    ui->btnSortZAAppointment->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortAZAppointment->setStyleSheet("");

    loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
}

void AdminWindow::on_btnViewAppointmentDetail_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) {
        qWarning() << "[ERROR] Could not get sender button";
        return;
    }

    int appointmentID = btn->property("appointmentID").toInt();
    qDebug() << "[VIEW DETAIL] Appointment ID:" << appointmentID;

    try {
        const Appointment& appointment = AppointmentManager::getInstance().getAppointmentByID(appointmentID);

        // ✅ Show detail dialog first
        AppointmentDetailDialog detailDialog(appointment, this);
        int result = detailDialog.exec();
        
        qDebug() << "[DETAIL DIALOG] Result:" << result << "| Edit requested:" << detailDialog.shouldEdit();

        // ✅ Check if user clicked "Chỉnh sửa" button
        if (result == QDialog::Accepted && detailDialog.shouldEdit()) {
            qDebug() << "[OPENING EDIT DIALOG] For appointment ID:" << appointmentID;
            
            // ✅ Check write permission before allowing edit
            if (!hasAppointmentAccess(true)) {
                showNoPermissionMessage();
                return;
            }

            // ✅ Open edit dialog with current appointment data
            AddAppointmentDialog editDialog(appointment, this);
            editDialog.setEditMode(true);
            editDialog.setWindowTitle(QString("Chỉnh Sửa Cuộc Hẹn - ID: %1").arg(appointmentID));

            if (editDialog.exec() == QDialog::Accepted) {
                try {
                    Appointment updatedAppt = editDialog.getAppointmentData();

                    qDebug() << "[DEBUG] Appointment data from edit dialog:";
                    qDebug() << "  ID:" << updatedAppt.getID();
                    qDebug() << "  Patient ID:" << updatedAppt.getPatientID();
                    qDebug() << "  Doctor ID:" << updatedAppt.getDoctorID();
                    qDebug() << "  Date:" << QString::fromStdString(updatedAppt.getDate().toString());
                    qDebug() << "  Start Time:" << QString::fromStdString(updatedAppt.getStartTime());
                    qDebug() << "  End Time:" << QString::fromStdString(updatedAppt.getEndTime());
                    qDebug() << "  Room:" << QString::fromStdString(updatedAppt.getRoom());
                    qDebug() << "  Status:" << QString::fromStdString(Appointment::statusToString(updatedAppt.getStatus()));

                    // ✅ CRITICAL: Preserve the original ID
                    if (updatedAppt.getID() != appointmentID) {
                        qWarning() << "[WARNING] Appointment ID changed during edit! Restoring original ID.";
                        qWarning() << "  Original ID:" << appointmentID;
                        qWarning() << "  New ID:" << updatedAppt.getID();
                        // Force restore the correct ID
                        // Adjust the constructor arguments to match the Appointment class definition
                        updatedAppt.setID(appointmentID);
                        qDebug() << "[DEBUG] After ID restoration:";
                        qDebug() << "  ID:" << updatedAppt.getID();
                        qDebug() << "  Start Time:" << QString::fromStdString(updatedAppt.getStartTime());
                        qDebug() << "  End Time:" << QString::fromStdString(updatedAppt.getEndTime());
                    }

                    // ✅ Validate doctor and patient exist
                    try {
                        DoctorManager::getInstance().getDoctorByID(updatedAppt.getDoctorID());
                        PatientManager::getInstance().getPatientByID(updatedAppt.getPatientID());
                    } catch (const std::exception& e) {
                        QMessageBox::critical(this, "Lỗi",
                            QString("Không thể cập nhật: %1").arg(e.what()));
                        return;
                    }

                    // ✅ Update appointment in manager
                    AppointmentManager::getInstance().editAppointment(appointmentID, updatedAppt);
                    
                    // ✅ Save to file
                    AppointmentManager::getInstance().saveToFile(Config::APPOINTMENT_PATH);

                    QMessageBox::information(this, "Thành công",
                        QString("Đã cập nhật cuộc hẹn ID: %1\n"
                                "Bệnh nhân: %2\n"
                                "Bác sĩ: %3\n"
                                "Ngày: %4\n"
                                "Giờ: %5 - %6")
                            .arg(appointmentID)
                            .arg(updatedAppt.getPatientID())
                            .arg(updatedAppt.getDoctorID())
                            .arg(QString::fromStdString(updatedAppt.getDate().toString()))
                            .arg(QString::fromStdString(updatedAppt.getStartTime()))
                            .arg(QString::fromStdString(updatedAppt.getEndTime())));

                    // ✅ Reload current page data
                    loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());

                } catch (const std::exception& e) {
                    QMessageBox::critical(this, "Lỗi Cập Nhật",
                        QString("Không thể cập nhật cuộc hẹn:\n%1").arg(e.what()));
                    qDebug() << "[ERROR] Failed to update appointment:" << e.what();
                }
            } else {
                qDebug() << "[INFO] Edit dialog cancelled";
            }
        } else {
            qDebug() << "[INFO] Detail dialog closed without edit request";
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", 
            QString("Không thể xem chi tiết cuộc hẹn:\n%1").arg(e.what()));
        qDebug() << "[ERROR] Failed to load appointment:" << e.what();
    }
}

void AdminWindow::on_btnAddPatient_clicked() {
    qDebug() << "Dialog opened";
    AddEditPatientDialog dialog(this);
    dialog.setDialogTitle("Thêm Bệnh Nhân Mới");

    if (dialog.exec() == QDialog::Accepted) {
        try {
            qDebug() << "Dialog accepted, getting data...";
            Patient newPatient = dialog.getPatientData();

            PatientManager::getInstance().addPatient(newPatient);

            QMessageBox::information(this, "Thành công", QString("Đã thêm bệnh nhân mới với ID: %1").arg(newPatient.getID()));

            loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Bệnh Nhân",
                                  QString("Không thể thêm: %1").arg(e.what()));
            qDebug() << "Error adding patient: " << e.what();
        }
    }
}

void AdminWindow::on_btnEditPatient_clicked() {
    if (!ui->tablePatient->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một bệnh nhân để chỉnh sửa.");
        return;
    }

    int selectedRow = ui->tablePatient->selectionModel()->selectedRows().first().row();
    int patientID = ui->tablePatient->item(selectedRow, 0)->text().toInt();

    try {
        const Patient& patientToEdit = PatientManager::getInstance().getPatientByID(patientID);

        AddEditPatientDialog dialog(this, patientToEdit);

        if (dialog.exec() == QDialog::Accepted) {
            Patient updatedPatient = dialog.getPatientData();

            PatientManager::getInstance().editPatient(updatedPatient.getID(), updatedPatient);

            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin bệnh nhân.");

            loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi Chỉnh Sửa", QString::fromStdString(e.what()));
    }
}


void AdminWindow::on_btnViewPatientDetail_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int patientID = btn->property("patientID").toInt();
    qDebug() << "[VIEW DETAIL] Patient ID:" << patientID;

    try {
        const Patient& patient = PatientManager::getInstance().getPatientByID(patientID);

        // ✅ Show detail dialog first
        PatientDetailDialog detailDialog(patient, this);
        int result = detailDialog.exec();

        qDebug() << "[DETAIL DIALOG] Result:" << result << "| Edit requested:" << detailDialog.shouldEdit();

        // ✅ Check if user clicked "Chỉnh sửa" button
        if (result == QDialog::Accepted && detailDialog.shouldEdit()) {
            qDebug() << "[OPENING EDIT DIALOG] For patient ID:" << patientID;

            // ✅ Check write permission before allowing edit
            if (!hasPatientAccess(true)) {
                showNoPermissionMessage();
                return;
            }

            // ✅ Open edit dialog with current patient data
            AddEditPatientDialog editDialog(this, patient);
            editDialog.setDialogTitle(QString("Chỉnh Sửa Bệnh Nhân - ID: %1").arg(patientID));

            if (editDialog.exec() == QDialog::Accepted) {
                try {
                    Patient updatedPatient = editDialog.getPatientData();

                    // ✅ Ensure ID hasn't changed
                    if (updatedPatient.getID() != patientID) {
                        qWarning() << "[WARNING] Patient ID changed during edit! Restoring original ID.";
                        // Patient class might need a setID method or recreate with correct ID
                    }

                    // ✅ Update patient in manager
                    PatientManager::getInstance().editPatient(patientID, updatedPatient);

                    // ✅ Save to file
                    PatientManager::getInstance().saveToFile(Config::PATIENT_PATH);

                    QMessageBox::information(this, "Thành công",
                                             QString("Đã cập nhật thông tin bệnh nhân ID: %1\n"
                                                     "Họ tên: %2")
                                                 .arg(patientID)
                                                 .arg(QString::fromStdString(updatedPatient.getName())));

                    // ✅ Reload current page data
                    loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());

                } catch (const std::exception& e) {
                    QMessageBox::critical(this, "Lỗi Cập Nhật",
                                          QString("Không thể cập nhật bệnh nhân:\n%1").arg(e.what()));
                    qDebug() << "[ERROR] Failed to update patient:" << e.what();
                }
            } else {
                qDebug() << "[INFO] Edit dialog cancelled";
            }
        } else {
            qDebug() << "[INFO] Detail dialog closed without edit request";
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi",
                              QString("Không thể xem chi tiết bệnh nhân:\n%1").arg(e.what()));
        qDebug() << "[ERROR] Failed to load patient:" << e.what();
    }
}

void AdminWindow::on_btnRemovePatient_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int patientID = 0;

    if (btn) {
        // Xóa từ button trong bảng
        patientID = btn->property("patientID").toInt();
    } else if (ui->tablePatient->selectionModel()->hasSelection()) {
        // Xóa từ selection
        int selectedRow = ui->tablePatient->selectionModel()->selectedRows().first().row();
        patientID = ui->tablePatient->item(selectedRow, 0)->text().toInt();
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một bệnh nhân để xóa.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xóa",
                                  "Bạn có chắc chắn muốn xóa bệnh nhân ID: " + QString::number(patientID) + " không?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            PatientManager::getInstance().removePatient(patientID);
            QMessageBox::information(this, "Thành công", "Đã xóa bệnh nhân ID: " + QString::number(patientID));

            loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Xóa Bệnh Nhân", QString::fromStdString(e.what()));
        }
    }
}

void AdminWindow::on_btnSearchPatient_clicked() {
    qDebug() << "page_patient: TÌM KIẾM clicked.";

    QString searchText = ui->txtSearchPatient->text().trimmed();
    currentPatientPage = 1;
    loadPatientData(currentPatientPage, searchText);
}

void AdminWindow::on_btnNextPage_Patient_clicked()
{
    if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
    if (currentPatientPage < totalPatientPages) {
        currentPatientPage++;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Patient_clicked()
{
    if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
    if (currentPatientPage > 1) {
        currentPatientPage--;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_1_clicked()
{
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Patient_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalPatientPages) {
        currentPatientPage = pageNum;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_2_clicked()
{
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Patient_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalPatientPages) {
        currentPatientPage = pageNum;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_3_clicked()
{
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    int pageNum = ui->btnPage_Patient_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalPatientPages) {
        currentPatientPage = pageNum;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnSortAZPatient_clicked() {
    qDebug() << "[SORT] Button A-Z clicked";
    currentPatientSortMode = PatientSortMode::BY_NAME_ASC;
    ui->btnSortAZPatient->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortZAPatient->setStyleSheet("");
    loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
}

void AdminWindow::on_btnSortZAPatient_clicked() {
    qDebug() << "[SORT] Button Z-A clicked";
    currentPatientSortMode = PatientSortMode::BY_NAME_DESC;
    ui->btnSortZAPatient->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortAZPatient->setStyleSheet("");
    loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
}

void AdminWindow::on_btnAddDoctor_clicked() {
    if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
    qDebug() << "Dialog add doctor opened";
    AddEditDoctorDialog dialog(this);
    dialog.setDialogTitle("Thêm Bác Sĩ Mới");
    if (dialog.exec() == QDialog::Accepted) {
        try {
            qDebug() << "Dialog accepted, getting data...";
            Doctor newDoctor = dialog.getDoctorData();

            qDebug() << "Doctor data retrieved, ID:" << newDoctor.getID();

            DoctorManager::getInstance().addDoctor(newDoctor);

            QMessageBox::information(this, "Thành công",
                                     QString("Đã thêm bác sĩ mới với ID: %1\nFile đã được lưu tại: %2")
                                         .arg(newDoctor.getID())
                                         .arg(Config::DOCTOR_PATH));

            loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Bác Sĩ",
                                  QString("Không thể thêm: %1").arg(e.what()));
            qDebug() << "Error adding doctor: " << e.what();
        }
    }
}

void AdminWindow::on_btnRemoveDoctor_clicked() {
    if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int doctorID = 0;

    if (btn) {
        // Xóa từ button trong bảng
        doctorID = btn->property("doctorID").toInt();
    } else if (ui->tableDoctor->selectionModel()->hasSelection()) {
        // Xóa từ selection
        int selectedRow = ui->tableDoctor->selectionModel()->selectedRows().first().row();
        doctorID = ui->tableDoctor->item(selectedRow, 0)->text().toInt();
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một bác sĩ để xóa.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xóa",
                                  "Bạn có chắc chắn muốn xóa bác sĩ ID: " + QString::number(doctorID) + " không?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            DoctorManager::getInstance().removeDoctor(doctorID);
            QMessageBox::information(this, "Thành công", "Đã xóa bác sĩ ID: " + QString::number(doctorID));

            loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Xóa Bác Sĩ", QString::fromStdString(e.what()));
        }
    }
}

// void AdminWindow::on_btnEditDoctor_clicked() {
//     if (!hasDoctorAccess(true)) { showNoPermissionMessage(); return; }
//     QMessageBox::information(this, "Thông báo", "Chức năng Sửa Lịch Hẹn chưa được triển khai.");
// }

// Thêm include
#include "doctordetaildialog.h"

void AdminWindow::on_btnViewDoctorDetail_clicked() {
    if (!hasDoctorAccess()) { showNoPermissionMessage(); return; }

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int doctorID = btn->property("doctorID").toInt();
    qDebug() << "[VIEW DETAIL] Doctor ID:" << doctorID;

    try {
        const Doctor& doctor = DoctorManager::getInstance().getDoctorByID(doctorID);

        // ✅ Show detail dialog first
        DoctorDetailDialog detailDialog(doctor, this);
        int result = detailDialog.exec();

        qDebug() << "[DETAIL DIALOG] Result:" << result << "| Edit requested:" << detailDialog.shouldEdit();

        // ✅ Check if user clicked "Chỉnh sửa" button
        if (result == QDialog::Accepted && detailDialog.shouldEdit()) {
            qDebug() << "[OPENING EDIT DIALOG] For doctor ID:" << doctorID;

            // ✅ Check write permission before allowing edit
            if (!hasDoctorAccess(true)) {
                showNoPermissionMessage();
                return;
            }

            // ✅ Open edit dialog with current doctor data
            AddEditDoctorDialog editDialog(this, doctor);
            editDialog.setDialogTitle(QString("Chỉnh Sửa Bác Sĩ - ID: %1").arg(doctorID));

            if (editDialog.exec() == QDialog::Accepted) {
                try {
                    Doctor updatedDoctor = editDialog.getDoctorData();

                    // ✅ Ensure ID hasn't changed
                    if (updatedDoctor.getID() != doctorID) {
                        qWarning() << "[WARNING] Doctor ID changed during edit! Restoring original ID.";
                        // Doctor class might need a setID method or recreate with correct ID
                    }

                    // ✅ Update doctor in manager
                    DoctorManager::getInstance().editDoctor(doctorID, updatedDoctor);

                    // ✅ Save to file
                    DoctorManager::getInstance().saveToFile(Config::DOCTOR_PATH);

                    QMessageBox::information(this, "Thành công",
                                             QString("Đã cập nhật thông tin bác sĩ ID: %1\n"
                                                     "Họ tên: %2\n"
                                                     "Chuyên khoa: %3")
                                                 .arg(doctorID)
                                                 .arg(QString::fromStdString(updatedDoctor.getName()))
                                                 .arg(QString::fromStdString(updatedDoctor.getSpecialization())));

                    // ✅ Reload current page data
                    loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());

                } catch (const std::exception& e) {
                    QMessageBox::critical(this, "Lỗi Cập Nhật",
                                          QString("Không thể cập nhật bác sĩ:\n%1").arg(e.what()));
                    qDebug() << "[ERROR] Failed to update doctor:" << e.what();
                }
            } else {
                qDebug() << "[INFO] Edit dialog cancelled";
            }
        } else {
            qDebug() << "[INFO] Detail dialog closed without edit request";
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi",
                              QString("Không thể xem chi tiết bác sĩ:\n%1").arg(e.what()));
        qDebug() << "[ERROR] Failed to load doctor:" << e.what();
    }
}

void AdminWindow::on_btnSearchDoctor_clicked() {
    if (!hasDoctorAccess()) { showNoPermissionMessage(); return; }
    qDebug() << "page_doctor: TÌM KIẾM clicked.";

    QString searchText = ui->txtSearchDoctor->text().trimmed();
    currentDoctorPage = 1;
    loadDoctorData(currentDoctorPage, searchText);
}

void AdminWindow::on_btnNextPage_Doctor_clicked()
{
    if (!hasUserAccess(true)) { showNoPermissionMessage(); return; }
    if (currentDoctorPage < totalDoctorPages) {
        currentDoctorPage++;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Doctor_clicked()
{
    if (!hasUserAccess(true)) { showNoPermissionMessage(); return; }
    if (currentDoctorPage > 1) {
        currentDoctorPage--;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Doctor_1_clicked()
{
    int pageNum = ui->btnPage_Doctor_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalDoctorPages) {
        currentDoctorPage = pageNum;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Doctor_2_clicked()
{
    int pageNum = ui->btnPage_Doctor_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalDoctorPages) {
        currentDoctorPage = pageNum;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Doctor_3_clicked()
{
    int pageNum = ui->btnPage_Doctor_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalDoctorPages) {
        currentDoctorPage = pageNum;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnSortAZDoctor_clicked() {
    qDebug() << "[SORT] Button A-Z clicked";

    currentDoctorSortMode = DoctorSortMode::BY_NAME_ASC;

    ui->btnSortAZDoctor->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortZADoctor->setStyleSheet("");

    loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
}

void AdminWindow::on_btnSortZADoctor_clicked() {
    qDebug() << "[SORT] Button Z-A clicked";

    currentDoctorSortMode = DoctorSortMode::BY_NAME_DESC;

    ui->btnSortZADoctor->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortAZDoctor->setStyleSheet("");

    loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
}

void AdminWindow::on_btnAddMedicalRecord_clicked() {
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    qDebug() << "page_medicalRecord: + Thêm hồ sơ bệnh án mới clicked.";

    AddEditMedicalRecordDialog addDialog(this);

    if (addDialog.exec() == QDialog::Accepted) {
        try {
            MedicalRecord newRecord = addDialog.getMedicalRecordData();
            
            // ✅ Kiểm tra Patient và Doctor tồn tại TRƯỚC khi add
            int patientID = newRecord.getPatientID();
            int doctorID = newRecord.getDoctorID();
            
            bool patientExists = false;
            bool doctorExists = false;
            
            try {
                PatientManager::getInstance().getPatientByID(patientID);
                patientExists = true;
                qDebug() << "[INFO] Patient ID" << patientID << "exists";
            } catch (...) { 
                patientExists = false;
                qWarning() << "[WARNING] Patient ID" << patientID << "not found";
            }

            try {
                DoctorManager::getInstance().getDoctorByID(doctorID);
                doctorExists = true;
                qDebug() << "[INFO] Doctor ID" << doctorID << "exists";
            } catch (...) { 
                doctorExists = false;
                qWarning() << "[WARNING] Doctor ID" << doctorID << "not found";
            }

            if (!patientExists) {
                QMessageBox::warning(this, "Lỗi", 
                    QString("Bệnh nhân ID: %1 không tồn tại!").arg(patientID));
                return;
            }
            
            if (!doctorExists) {
                QMessageBox::warning(this, "Lỗi", 
                    QString("Bác sĩ ID: %1 không tồn tại!").arg(doctorID));
                return;
            }

            MedicalRecordManager::getInstance().addMedicalRecord(newRecord);

            QMessageBox::information(this, "Thành công", 
                "Hồ sơ bệnh án đã được thêm và lưu.");
            loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Hồ Sơ", 
                QString("Không thể thêm hồ sơ: %1").arg(e.what()));
            qDebug() << "Error adding medical record: " << e.what();
        }
    } else {
        qDebug() << "Thêm hồ sơ bệnh án đã bị hủy.";
    }
}

void AdminWindow::on_btnEditMedicalRecord_clicked() {
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    QMessageBox::information(this, "Thông báo", "Chức năng Sửa Hồ Sơ Bệnh Án chưa được triển khai.");
}

void AdminWindow::on_btnRemoveMedicalRecord_clicked() {
    if (!hasMedicalRecordAccess(true)) { showNoPermissionMessage(); return; }
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int recordID = 0;

    if (btn) {
        recordID = btn->property("recordID").toInt();
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một hồ sơ để xóa.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xóa",
                                  "Bạn có chắc chắn muốn xóa hồ sơ ID: " + QString::number(recordID) + " không?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            MedicalRecordManager::getInstance().removeMedicalRecord(recordID);
            QMessageBox::information(this, "Thành công", "Đã xóa hồ sơ ID: " + QString::number(recordID));

            loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Xóa Hồ Sơ", QString::fromStdString(e.what()));
        }
    }
}

void AdminWindow::on_btnSearchMedicalRecord_clicked() {
    if (!hasMedicalRecordAccess()) { showNoPermissionMessage(); return; }
    qDebug() << "page_medicalRecord: TÌM KIẾM clicked.";

    QString searchText = ui->txtSearchMedicalRecord->text().trimmed();
    currentMedicalRecordPage = 1;
    loadMedicalRecordData(currentMedicalRecordPage, searchText);
}

void AdminWindow::on_btnViewMedicalRecordDetail_clicked() {
    if (!hasMedicalRecordAccess()) {
        showNoPermissionMessage();
        return;
    }

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int recordID = btn->property("recordID").toInt();
    qDebug() << "[VIEW DETAIL] Medical Record ID:" << recordID;

    try {
        const MedicalRecord& record = MedicalRecordManager::getInstance().getMedicalRecordByID(recordID);

        // ✅ Show detail dialog first
        MedicalRecordDetailDialog detailDialog(record, this);
        int result = detailDialog.exec();

        qDebug() << "[DETAIL DIALOG] Result:" << result << "| Edit requested:" << detailDialog.shouldEdit();

        // ✅ Check if user clicked "Chỉnh sửa" button
        if (result == QDialog::Accepted && detailDialog.shouldEdit()) {
            qDebug() << "[OPENING EDIT DIALOG] For medical record ID:" << recordID;

            // ✅ Check write permission before allowing edit
            if (!hasMedicalRecordAccess(true)) {
                showNoPermissionMessage();
                return;
            }

            // ✅ Open edit dialog with current medical record data
            AddEditMedicalRecordDialog editDialog(this, record);
            editDialog.setWindowTitle(QString("Chỉnh Sửa Hồ Sơ Bệnh Án - ID: %1").arg(recordID));

            if (editDialog.exec() == QDialog::Accepted) {
                try {
                    MedicalRecord updatedRecord = editDialog.getMedicalRecordData();

                    qDebug() << "[DEBUG] Medical record data from edit dialog:";
                    qDebug() << "  ID:" << updatedRecord.getID();
                    qDebug() << "  Patient ID:" << updatedRecord.getPatientID();
                    qDebug() << "  Doctor ID:" << updatedRecord.getDoctorID();
                    qDebug() << "  Diagnosis:" << QString::fromStdString(updatedRecord.getDiagnosis());

                    // ✅ Ensure ID hasn't changed
                    if (updatedRecord.getID() != recordID) {
                        qWarning() << "[WARNING] Medical Record ID changed during edit! Restoring original ID.";
                        // MedicalRecord class might need a setID method
                    }

                    // ✅ Validate patient and doctor exist
                    try {
                        PatientManager::getInstance().getPatientByID(updatedRecord.getPatientID());
                        DoctorManager::getInstance().getDoctorByID(updatedRecord.getDoctorID());
                    } catch (const std::exception& e) {
                        QMessageBox::critical(this, "Lỗi",
                                              QString("Không thể cập nhật: %1").arg(e.what()));
                        return;
                    }

                    // ✅ Update medical record in manager
                    MedicalRecordManager::getInstance().editMedicalRecord(recordID, updatedRecord);

                    // ✅ Save to file
                    MedicalRecordManager::getInstance().saveToFile(Config::MEDICAL_RECORD_PATH);

                    QMessageBox::information(this, "Thành công",
                                             QString("Đã cập nhật hồ sơ bệnh án ID: %1\n"
                                                     "Bệnh nhân ID: %2\n"
                                                     "Bác sĩ ID: %3")
                                                 .arg(recordID)
                                                 .arg(updatedRecord.getPatientID())
                                                 .arg(updatedRecord.getDoctorID()));

                    // ✅ Reload current page data
                    loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());

                } catch (const std::exception& e) {
                    QMessageBox::critical(this, "Lỗi Cập Nhật",
                                          QString("Không thể cập nhật hồ sơ bệnh án:\n%1").arg(e.what()));
                    qDebug() << "[ERROR] Failed to update medical record:" << e.what();
                }
            } else {
                qDebug() << "[INFO] Edit dialog cancelled";
            }
        } else {
            qDebug() << "[INFO] Detail dialog closed without edit request";
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi",
                              QString("Không thể xem chi tiết hồ sơ bệnh án:\n%1").arg(e.what()));
        qDebug() << "[ERROR] Failed to load medical record:" << e.what();
    }
}

void AdminWindow::on_btnSortAZMedicalRecord_clicked() {
    qDebug() << "[SORT] Button A-Z clicked";

    currentMedicalRecordSortMode = MedicalRecordSortMode::BY_PATIENT_NAME_ASC;

    ui->btnSortAZMedicalRecord->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortZAMedicalRecord->setStyleSheet("");

    loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
}

void AdminWindow::on_btnSortZAMedicalRecord_clicked() {
    qDebug() << "[SORT] Button Z-A clicked";

    currentMedicalRecordSortMode = MedicalRecordSortMode::BY_PATIENT_NAME_DESC;

    ui->btnSortZAMedicalRecord->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortAZMedicalRecord->setStyleSheet("");

    loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
}

void AdminWindow::on_btnPrevPage_MedicalRecord_clicked() {
    if (currentMedicalRecordPage > 1) {
        currentMedicalRecordPage--;
        loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
    }
}

void AdminWindow::on_btnNextPage_MedicalRecord_clicked() {
    if (currentMedicalRecordPage < totalMedicalRecordPages) {
        currentMedicalRecordPage++;
        loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
    }
}

void AdminWindow::on_btnPage_MedicalRecord_1_clicked() {
    int pageNum = ui->btnPage_MedicalRecord_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalMedicalRecordPages) {
        currentMedicalRecordPage = pageNum;
        loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
    }
}

void AdminWindow::on_btnPage_MedicalRecord_2_clicked() {
    int pageNum = ui->btnPage_MedicalRecord_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalMedicalRecordPages) {
        currentMedicalRecordPage = pageNum;
        loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
    }
}

void AdminWindow::on_btnPage_MedicalRecord_3_clicked() {
    int pageNum = ui->btnPage_MedicalRecord_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalMedicalRecordPages) {
        currentMedicalRecordPage = pageNum;
        loadMedicalRecordData(currentMedicalRecordPage, ui->txtSearchMedicalRecord->text().trimmed());
    }
}

void AdminWindow::on_btnAddUser_clicked() {
    qDebug() << "Dialog add user opened";
    AddEditUserDialog dialog(this);
    dialog.setDialogTitle("Thêm Người Dùng Mới");

    if (dialog.exec() == QDialog::Accepted) {
        try {
            qDebug() << "Dialog accepted, getting data...";
            User newUser = dialog.getUserData();

            // ✅ VERIFY dữ liệu trước khi add
            qDebug() << "[VERIFY] User data before adding:";
            qDebug() << "  - ID:" << newUser.getID();
            qDebug() << "  - Username:" << QString::fromStdString(newUser.getUsername());
            qDebug() << "  - FullName:" << QString::fromStdString(newUser.getFullName());
            qDebug() << "  - CCCD:" << QString::fromStdString(newUser.getCCCD());
            qDebug() << "  - Phone:" << QString::fromStdString(newUser.getPhoneNumber());
            qDebug() << "  - Birthday:" << QString::fromStdString(newUser.getBirthday());

            UserManager::getInstance().addUser(newUser);

            // ✅ VERIFY sau khi add
            const User& addedUser = UserManager::getInstance().getUserByID(newUser.getID());
            qDebug() << "[VERIFY] User data after adding from manager:";
            qDebug() << "  - FullName:" << QString::fromStdString(addedUser.getFullName());
            qDebug() << "  - CCCD:" << QString::fromStdString(addedUser.getCCCD());

            UserManager::getInstance().saveToFile(Config::USER_PATH);

            QMessageBox::information(this, "Thành công",
                                     QString("Đã thêm người dùng mới với ID: %1\nFile đã được lưu tại: %2")
                                         .arg(newUser.getID())
                                         .arg(Config::USER_PATH));

            loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Người Dùng",
                                  QString("Không thể thêm: %1").arg(e.what()));
            qDebug() << "Error adding user: " << e.what();
        }
    }
}
void AdminWindow::on_btnRemoveUser_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int userID = 0;

    if (btn) {
        userID = btn->property("userID").toInt();
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn một người dùng để xoá.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận xoá",
                                  "Bạn có chắc chắn muốn xoá người dùng ID: " + QString::number(userID) + " không?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            UserManager::getInstance().removeUser(userID);
            QMessageBox::information(this, "Thành công", "Đã xoá người dùng ID: " + QString::number(userID));

            loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Xoá Người Dùng", QString::fromStdString(e.what()));
        }
    }
}

void AdminWindow::on_btnSearchUser_clicked() {
    qDebug() << "page_user: TÌM KIẾM clicked.";

    QString searchText = ui->txtSearchUser->text().trimmed();
    currentUserPage = 1;
    loadUserData(currentUserPage, searchText);
}
void AdminWindow::on_btnViewUserDetail_clicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int userID = btn->property("userID").toInt();
    qDebug() << "[VIEW DETAIL] User ID:" << userID;

    try {
        const User& user = UserManager::getInstance().getUserByID(userID);

        // ✅ Show detail dialog first
        UserDetailDialog detailDialog(user, this);
        int result = detailDialog.exec();

        qDebug() << "[DETAIL DIALOG] Result:" << result << "| Edit requested:" << detailDialog.shouldEdit();

        // ✅ Check if user clicked "Chỉnh sửa" button
        if (result == QDialog::Accepted && detailDialog.shouldEdit()) {
            qDebug() << "[OPENING EDIT DIALOG] For user ID:" << userID;

            // ✅ Check write permission before allowing edit
            if (!hasUserAccess(true)) {
                showNoPermissionMessage();
                return;
            }

            // ✅ Open edit dialog with current user data
            AddEditUserDialog editDialog(this, user);
            editDialog.setDialogTitle(QString("Chỉnh Sửa Người Dùng - ID: %1").arg(userID));

            if (editDialog.exec() == QDialog::Accepted) {
                try {
                    User updatedUser = editDialog.getUserData();

                    // ✅ Ensure ID hasn't changed
                    if (updatedUser.getID() != userID) {
                        qWarning() << "[WARNING] User ID changed during edit! Restoring original ID.";
                        // User class might need a setID method or recreate with correct ID
                    }

                    // ✅ Update user in manager
                    UserManager::getInstance().editUser(userID, updatedUser);

                    // ✅ Save to file
                    UserManager::getInstance().saveToFile(Config::USER_PATH);

                    QMessageBox::information(this, "Thành công",
                                             QString("Đã cập nhật thông tin người dùng ID: %1\n"
                                                     "Username: %2")
                                                 .arg(userID)
                                                 .arg(QString::fromStdString(updatedUser.getUsername())));

                    // ✅ Reload current page data
                    loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());

                } catch (const std::exception& e) {
                    QMessageBox::critical(this, "Lỗi Cập Nhật",
                                          QString("Không thể cập nhật người dùng:\n%1").arg(e.what()));
                    qDebug() << "[ERROR] Failed to update user:" << e.what();
                }
            } else {
                qDebug() << "[INFO] Edit dialog cancelled";
            }
        } else {
            qDebug() << "[INFO] Detail dialog closed without edit request";
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi",
                              QString("Không thể xem chi tiết người dùng:\n%1").arg(e.what()));
        qDebug() << "[ERROR] Failed to load user:" << e.what();
    }
}

void AdminWindow::on_btnSortAZUser_clicked() {
    qDebug() << "[SORT] Button A-Z clicked";

    currentUserSortMode = UserSortMode::BY_USERNAME_ASC;

    ui->btnSortAZUser->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortZAUser->setStyleSheet("");

    loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
}

void AdminWindow::on_btnSortZAUser_clicked() {
    qDebug() << "[SORT] Button Z-A clicked";

    currentUserSortMode = UserSortMode::BY_USERNAME_DESC;

    ui->btnSortZAUser->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    ui->btnSortAZUser->setStyleSheet("");

    loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
}

void AdminWindow::on_btnPrevPage_User_clicked()
{
    if (currentUserPage > 1) {
        currentUserPage--;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::on_btnNextPage_User_clicked()
{
    if (currentUserPage < totalUserPages) {
        currentUserPage++;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::on_btnPage_User_1_clicked()
{
    int pageNum = ui->btnPage_User_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::on_btnPage_User_2_clicked()
{
    int pageNum = ui->btnPage_User_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}
//
void AdminWindow::on_btnPage_User_3_clicked()
{
    int pageNum = ui->btnPage_User_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::setActiveSidebarButton(QPushButton* activeButton)
{
    // Reset tất cả button sidebar về trạng thái không active
    QList<QPushButton*> sidebarButtons = {
        ui->appointmentManagerButton,
        ui->doctorManagerButton,
        ui->patientManagerButton,
        ui->userManagerButton,
        ui->medicalRecordButton
    };

    for (QPushButton* btn : sidebarButtons) {
        if (btn == activeButton) {
            btn->setProperty("active", true);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        } else {
            btn->setProperty("active", false);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        }
    }
}

void AdminWindow::applyModernStyles()
{
    // ✨💫 PREMIUM GRADIENT THEME - Sinh động, Sang trọng, Hiện đại

    // Main Window - Gradient nền tinh tế
    this->setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #f5f5f0, stop:0.5 #faf9f7, stop:1 #f0f0eb);
        }
    )");

    // 🎨✨ STUNNING SIDEBAR - Multi-layer gradients
    ui->sidebarWidget->setStyleSheet(R"(
        QWidget#sidebarWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2530, stop:0.3 #2c3e50, stop:0.7 #34495e, stop:1 #2c3e50);
            border-right: 2px solid qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(52, 152, 219, 0.3), stop:1 rgba(142, 68, 173, 0.2));
        }

        QLabel#lblSystemTitle {
            color: #ecf0f1;
            font-size: 14pt;
            font-weight: 600;
            padding: 22px 15px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.15),
                stop:0.5 rgba(142, 68, 173, 0.1),
                stop:1 rgba(52, 152, 219, 0.15));
            border-bottom: 2px solid qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.4), stop:1 rgba(142, 68, 173, 0.3));
            letter-spacing: 0.8px;
        }

        QPushButton {
            color: #bdc3c7;
            background: transparent;
            border: none;
            padding: 15px 20px;
            text-align: left;
            font-size: 10.5pt;
            font-weight: 500;
            margin: 4px 12px;
            border-radius: 10px;
            letter-spacing: 0.4px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.2),
                stop:0.5 rgba(155, 89, 182, 0.15),
                stop:1 rgba(52, 152, 219, 0.1));
            color: #ecf0f1;
            border-left: 3px solid rgba(52, 152, 219, 0.5);
        }

        QPushButton[active="true"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.3),
                stop:0.5 rgba(155, 89, 182, 0.2),
                stop:1 rgba(52, 152, 219, 0.25));
            color: #5dade2;
            border-left: 4px solid qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3498db, stop:1 #9b59b6);
            font-weight: 600;
        }

        QPushButton#logoutButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #c0392b, stop:0.5 #e74c3c, stop:1 #c0392b);
            color: white;
            margin: 20px 12px 10px 12px;
            text-align: center;
            font-weight: 600;
            border: 1px solid rgba(255, 255, 255, 0.1);
        }

        QPushButton#logoutButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #a93226, stop:0.5 #d62c1a, stop:1 #a93226);
        }
    )");

    // 🎯✨ DYNAMIC CONTENT AREA - Glass morphism & Gradients
    ui->contentAreaWidget->setStyleSheet(R"(
        QWidget#contentAreaWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 255, 255, 0.95),
                stop:0.5 rgba(253, 253, 251, 0.98),
                stop:1 rgba(255, 255, 255, 0.95));
            border-radius: 15px;
            margin: 12px;
            border: 2px solid qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(52, 152, 219, 0.2),
                stop:0.5 rgba(155, 89, 182, 0.15),
                stop:1 rgba(52, 152, 219, 0.2));
        }

        /* 💎 STUNNING PAGE HEADERS - Multi-gradient */
        QLabel[styleSheet*="font-size: 18pt"],
        QLabel[styleSheet*="font-size: 24pt"] {
            color: transparent;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(248, 249, 250, 0.8),
                stop:0.2 rgba(52, 152, 219, 0.05),
                stop:0.5 rgba(155, 89, 182, 0.03),
                stop:0.8 rgba(52, 152, 219, 0.05),
                stop:1 rgba(248, 249, 250, 0.8));
            background-clip: text;
            -webkit-background-clip: text;
            font-size: 19pt;
            font-weight: 600;
            padding: 20px 22px;
            border-radius: 12px;
            border: 2px solid transparent;
            border-image: linear-gradient(90deg,
                rgba(52, 152, 219, 0.3),
                rgba(155, 89, 182, 0.2),
                rgba(52, 152, 219, 0.3)) 1;
            margin: 12px;
            letter-spacing: 0.6px;
        }

        /* 🎨 GRADIENT BUTTONS - Vibrant & Premium */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #7f8c8d, stop:0.5 #95a5a6, stop:1 #7f8c8d);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.2);
            padding: 12px 24px;
            border-radius: 10px;
            font-weight: 600;
            font-size: 9.5pt;
            min-height: 40px;
            min-width: 120px;
            letter-spacing: 0.4px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #6c7a7b, stop:0.5 #7f8c8d, stop:1 #6c7a7b);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #5a6869, stop:0.5 #6c7a7b, stop:1 #5a6869);
        }

        /* 🌿 Nút THÊM - Emerald gradient */
        QPushButton[text*="THÊM"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1abc9c, stop:0.3 #16a085, stop:0.7 #27ae60, stop:1 #229954);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.25);
        }

        QPushButton[text*="THÊM"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #16a085, stop:0.3 #138d75, stop:0.7 #229954, stop:1 #1e8449);
            border: 1px solid rgba(255, 255, 255, 0.35);
        }

        /* 🔥 Nút TÌM KIẾM - Sunset gradient */
        QPushButton[text*="TÌM KIẾM"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #e67e22, stop:0.3 #d35400, stop:0.7 #e74c3c, stop:1 #c0392b);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.25);
        }

        QPushButton[text*="TÌM KIẾM"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #d35400, stop:0.3 #ba4a00, stop:0.7 #c0392b, stop:1 #a93226);
            border: 1px solid rgba(255, 255, 255, 0.35);
        }

        /* 💜 Nút SẮP XẾP - Purple haze gradient */
        QPushButton[text*="SẮP XẾP"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #9b59b6, stop:0.3 #8e44ad, stop:0.7 #7d3c98, stop:1 #6c3483);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.25);
        }

        QPushButton[text*="SẮP XẾP"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #8e44ad, stop:0.3 #7d3c98, stop:0.7 #6c3483, stop:1 #5b2c6f);
            border: 1px solid rgba(255, 255, 255, 0.35);
        }

        /* 🔷 Nút icon sắp xếp - Blue steel gradient */
        QPushButton[text*="🔼"],
        QPushButton[text*="🔽"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #5d6d7e, stop:0.5 #4a5568, stop:1 #34495e);
            color: white !important;
            font-size: 10.5pt;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }

        QPushButton[text*="🔼"]:hover,
        QPushButton[text*="🔽"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #4a5568, stop:0.5 #3a4556, stop:1 #2c3e50);
            border: 1px solid rgba(255, 255, 255, 0.3);
        }

        /* 📄 Nút phân trang - Silver gradient */
        QPushButton[text*="Trang"],
        QPushButton[text*="<"],
        QPushButton[text*=">"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #95a5a6, stop:0.5 #7f8c8d, stop:1 #95a5a6);
            color: white !important;
            min-width: 100px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }

        QPushButton[text*="Trang"]:hover,
        QPushButton[text*="<"]:hover,
        QPushButton[text*=">"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #7f8c8d, stop:0.5 #6c7a7b, stop:1 #7f8c8d);
            border: 1px solid rgba(255, 255, 255, 0.3);
        }

        /* 🔢 Nút số trang - Glass morphism */
        QPushButton:has-text[0-9] {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(236, 240, 241, 0.9),
                stop:1 rgba(255, 255, 255, 0.8));
            color: #2c3e50 !important;
            min-width: 40px;
            font-weight: 500;
            border: 1.5px solid rgba(52, 152, 219, 0.2);
        }

        QPushButton:has-text[0-9]:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(213, 219, 219, 0.95),
                stop:1 rgba(236, 240, 241, 0.9));
            border: 1.5px solid rgba(52, 152, 219, 0.4);
        }

        /* ⭐ Nút số trang active - Ocean gradient */
        QPushButton[active="true"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #3498db, stop:0.3 #2980b9, stop:0.7 #1abc9c, stop:1 #16a085) !important;
            color: white !important;
            font-weight: 600;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }

        /* 📝 LineEdit - Glass input with gradient border */
        QLineEdit {
            padding: 12px 18px;
            border: 2px solid transparent;
            border-image: linear-gradient(135deg,
                rgba(52, 152, 219, 0.3),
                rgba(155, 89, 182, 0.2),
                rgba(52, 152, 219, 0.3)) 1;
            border-radius: 10px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 255, 255, 0.95),
                stop:1 rgba(248, 249, 250, 0.9));
            font-size: 10pt;
            color: #2c3e50;
            min-height: 40px;
            selection-background-color: #3498db;
            selection-color: white;
        }

        QLineEdit:focus {
            border-image: linear-gradient(135deg,
                rgba(52, 152, 219, 0.6),
                rgba(155, 89, 182, 0.4),
                rgba(52, 152, 219, 0.6)) 1;
            background: rgba(255, 255, 255, 1);
        }

        QLineEdit::placeholder {
            color: #95a5a6;
            font-style: italic;
        }

        /* 📊 Table - Premium data display */
        QTableWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(255, 255, 255, 0.98),
                stop:1 rgba(248, 249, 250, 0.95));
            border: 2px solid qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.2),
                stop:0.5 rgba(155, 89, 182, 0.15),
                stop:1 rgba(52, 152, 219, 0.2));
            border-radius: 12px;
            gridline-color: rgba(236, 240, 241, 0.5);
            selection-background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.15),
                stop:1 rgba(155, 89, 182, 0.1));
            selection-color: #2c3e50;
            color: #34495e;
            font-size: 9.5pt;
            alternate-background-color: rgba(250, 250, 250, 0.5);
        }

        QTableWidget::item {
            padding: 11px 7px;
            border-bottom: 1px solid rgba(236, 240, 241, 0.3);
        }

        QTableWidget::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.18),
                stop:0.5 rgba(155, 89, 182, 0.12),
                stop:1 rgba(52, 152, 219, 0.18));
            color: #2c3e50;
            font-weight: 500;
        }

        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #2c3e50, stop:0.3 #34495e, stop:0.7 #2c3e50, stop:1 #1a252f);
            color: #ecf0f1;
            padding: 14px 7px;
            border: none;
            font-weight: 600;
            font-size: 9.5pt;
            border-right: 1px solid rgba(236, 240, 241, 0.15);
            letter-spacing: 0.4px;
        }

        QHeaderView::section:last {
            border-right: none;
        }

        QHeaderView::section:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a252f, stop:0.3 #2c3e50, stop:0.7 #34495e, stop:1 #2c3e50);
        }

        /* 🏷️ Labels - Gradient text */
        QLabel {
            color: #5d6d7e;
            background: transparent;
            font-size: 10pt;
            font-weight: 500;
        }

        QLabel[text*="TextLabel"] {
            color: #34495e;
            font-weight: 600;
            font-size: 10.5pt;
        }

        /* 🛠️ Toolbar - Glass effect */
        QWidget[toolbar] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 255, 255, 0.7),
                stop:0.5 rgba(253, 253, 251, 0.8),
                stop:1 rgba(255, 255, 255, 0.7));
            border-radius: 12px;
            padding: 14px;
            border: 1.5px solid qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(52, 152, 219, 0.15),
                stop:0.5 rgba(155, 89, 182, 0.1),
                stop:1 rgba(52, 152, 219, 0.15));
            margin: 10px;
        }
    )");

    // 📄 Pagination buttons - Ocean wave gradient
    QString paginationStyle = R"(
        QPushButton {
            min-width: 40px;
            min-height: 40px;
            border-radius: 10px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #95a5a6, stop:0.5 #7f8c8d, stop:1 #95a5a6);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.2);
            font-weight: 600;
            font-size: 9.5pt;
            margin: 3px;
        }

        QPushButton:hover:enabled {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #7f8c8d, stop:0.5 #6c7a7b, stop:1 #7f8c8d);
            color: white !important;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }

        QPushButton:disabled {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #d5dbdb, stop:1 #bdc3c7);
            color: #95a5a6 !important;
            border: 1px solid rgba(149, 165, 166, 0.2);
        }

        QPushButton[active="true"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #3498db, stop:0.3 #2980b9, stop:0.7 #1abc9c, stop:1 #16a085) !important;
            color: white !important;
            font-weight: 600;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
    )";

    QStringList paginationButtons = {
        "btnPrevPage_Appointment", "btnNextPage_Appointment", "btnPage_Appointment_1", "btnPage_Appointment_2", "btnPage_Appointment_3",
        "btnPrevPage_Patient", "btnNextPage_Patient", "btnPage_Patient_1", "btnPage_Patient_2", "btnPage_Patient_3",
        "btnPrevPage_Doctor", "btnNextPage_Doctor", "btnPage_Doctor_1", "btnPage_Doctor_2", "btnPage_Doctor_3",
        "btnPrevPage_MedicalRecord", "btnNextPage_MedicalRecord", "btnPage_MedicalRecord_1", "btnPage_MedicalRecord_2", "btnPage_MedicalRecord_3",
        "btnPrevPage_User", "btnNextPage_User", "btnPage_User_1", "btnPage_User_2", "btnPage_User_3"
    };

    for (const QString& btnName : paginationButtons) {
        QPushButton* btn = findChild<QPushButton*>(btnName);
        if (btn) {
            btn->setStyleSheet(paginationStyle);
        }
    }

    setActiveSidebarButton(ui->appointmentManagerButton);
}
