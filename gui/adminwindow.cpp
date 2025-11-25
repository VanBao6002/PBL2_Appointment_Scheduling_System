#include "adminwindow.h"
#include "addeditpatientdialog.h"
#include "addeditdoctordialog.h"
#include "addedituserdialog.h"
#include "gui/ui_addedituserdialog.h"
#include "gui/ui_adminwindow.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <algorithm>
#include <QDir>
#include "appointmentManager.h"
#include "patientManager.h"
#include "doctorManager.h"
#include "userManager.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    this->showFullScreen();
    ui->tableAppointment->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    setupPatientTable();
    setupDoctorTable();

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

    currentUserPage = 1;
    totalUserPages = 1;

    itemsPerPage = 10;

    currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;
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

    //User
    connect(ui->btnSortAZUser, &QPushButton::clicked, this, &AdminWindow::on_btnSortAZUser_clicked);
    connect(ui->btnSortZAUser, &QPushButton::clicked, this, &AdminWindow::on_btnSortZAUser_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &AdminWindow::on_btnPrevPage_User_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &AdminWindow::on_btnNextPage_User_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &AdminWindow::on_btnPage_User_1_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &AdminWindow::on_btnPage_User_2_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &AdminWindow::on_btnPage_User_3_clicked);

    ui->btnSortAZUser->setText("🔼 A → Z");
    ui->btnSortZAUser->setText("🔽 Z → A");

    ui->mainStack->setCurrentWidget(ui->page_appointment);
    loadAppointmentData(currentAppointmentPage);

    qDebug() << "[INFO] AdminWindow initialized successfully";
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::loadAppointmentData(int page, const QString& searchText)
{
    qDebug() << "Loading appointment data for page" << page << "with search text:" << searchText;

    // Lấy dữ liệu từ Singleton
    std::vector<Appointment> allAppointments = AppointmentManager::getInstance().getAllAppointmentsAsVector();
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
                    << QString::fromStdString(appt.getTime())
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

    // Tính toán phân trang
    int totalItems = filteredAppointments.size();
    totalAppointmentPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

    if (page < 1) page = 1;
    if (page > totalAppointmentPages) page = totalAppointmentPages;
    currentAppointmentPage = page;

    // Đổ dữ liệu vào bảng
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

        ui->tableAppointment->setItem(rowIdx, 0, new QTableWidgetItem(QString::number(appt.getID())));
        ui->tableAppointment->setItem(rowIdx, 1, new QTableWidgetItem(patientName));
        ui->tableAppointment->setItem(rowIdx, 2, new QTableWidgetItem(doctorName));
        ui->tableAppointment->setItem(rowIdx, 3, new QTableWidgetItem(QString::fromStdString(appt.getDate().toString())));
        ui->tableAppointment->setItem(rowIdx, 4, new QTableWidgetItem(QString::fromStdString(appt.getTime())));
        ui->tableAppointment->setItem(rowIdx, 5, new QTableWidgetItem(QString::fromStdString(Appointment::statusToString(appt.getStatus()))));
    }
}

void AdminWindow::setupPatientTable() {
    // ✅ Chỉ còn 7 cột (bỏ Email)
    ui->tablePatient->setColumnCount(7);

    // ✅ Đặt tên các cột (không có Email)
    QStringList headers;
    headers << "ID Bệnh nhân" << "Họ tên" << "Giới tính" << "Ngày sinh"
            << "Nhóm máu" << "SĐT" << "Tuỳ chọn";
    ui->tablePatient->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tablePatient->horizontalHeader();

    // Cột 0: ID (Fixed)
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(0, 100);

    // Cột 1: Họ tên (Stretch)
    header->setSectionResizeMode(1, QHeaderView::Stretch);

    // Cột 2: Giới tính (Fixed)
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(2, 80);

    // Cột 3: Ngày sinh (Fixed)
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(3, 120);

    // Cột 4: Nhóm máu (Fixed)
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(4, 100);

    // Cột 5: SĐT (Fixed)
    header->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(5, 120);

    // ✅ Cột 6: Tuỳ chọn (Fixed - đủ cho 2 button)
    header->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tablePatient->setColumnWidth(6, 200);

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
    // ✅ Đặt 9 cột (thêm cột Ngày sinh)
    ui->tableDoctor->setColumnCount(9);

    // ✅ Đặt tên các cột
    QStringList headers;
    headers << "ID Bác sĩ" << "Họ tên" << "Giới tính" << "Ngày sinh"
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

    // Cột 4: Chuyên khoa (Stretch)
    header->setSectionResizeMode(4, QHeaderView::Stretch);

    // Cột 5: Email (Stretch)
    header->setSectionResizeMode(5, QHeaderView::Stretch);

    // Cột 6: SĐT (Fixed)
    header->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(6, 110);

    // Cột 7: Trạng thái (Fixed)
    header->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(7, 100);

    // Cột 8: Tuỳ chọn (Fixed)
    header->setSectionResizeMode(8, QHeaderView::Fixed);
    ui->tableDoctor->setColumnWidth(8, 200);

    // Các thiết lập khác
    ui->tableDoctor->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableDoctor->verticalHeader()->setDefaultSectionSize(75);
    ui->tableDoctor->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableDoctor->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableDoctor->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableDoctor->setAlternatingRowColors(true);
    ui->tableDoctor->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] Doctor table configured with Birthday column";
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

void AdminWindow::loadDoctorData(int page, const QString& searchText) {
    qDebug() << "Loading doctor data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allDoctors = DoctorManager::getInstance().getAllDoctors();
    std::vector<Doctor> filteredDoctors;

    // 2. Lọc CHỈ theo ID
    if (searchText.isEmpty()) {
        for (const auto& pair : allDoctors) {
            filteredDoctors.push_back(pair.second);
        }
    } else {
        bool isNumber = false;
        int searchID = searchText.toInt(&isNumber);

        if (!isNumber) {
            QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập ID (số nguyên) để tìm kiếm!");
            for (const auto& pair : allDoctors) {
                filteredDoctors.push_back(pair.second);
            }
        } else {
            for (const auto& pair : allDoctors) {
                if (pair.second.getID() == searchID) {
                    filteredDoctors.push_back(pair.second);
                }
            }

            if (filteredDoctors.empty()) {
                QMessageBox::information(this, "Kết quả tìm kiếm",
                                         QString("Không tìm thấy bác sĩ có ID: %1").arg(searchID));
                for (const auto& pair : allDoctors) {
                    filteredDoctors.push_back(pair.second);
                }
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

        // ✅ Cột 0: ID (căn giữa)
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(doctor.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 0, idItem);

        // ✅ Cột 1: Họ tên (căn trái)
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(doctor.getName()));
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 1, nameItem);

        // ✅ Cột 2: Giới tính (hiển thị "Nam/Nữ/Khác")
        QString genderDisplay;
        char gender = doctor.getGender();
        if (gender == 'M') genderDisplay = "Nam";
        else if (gender == 'F') genderDisplay = "Nữ";
        else genderDisplay = "Khác";

        QTableWidgetItem* genderItem = new QTableWidgetItem(genderDisplay);
        genderItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 2, genderItem);

        // ✅ Cột 3: Ngày sinh (căn giữa)
        QTableWidgetItem* birthdayItem = new QTableWidgetItem(QString::fromStdString(doctor.getBirthday().toString()));
        birthdayItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 3, birthdayItem);

        // ✅ Cột 4: Chuyên khoa (căn trái)
        QTableWidgetItem* specializationItem = new QTableWidgetItem(QString::fromStdString(doctor.getSpecialization()));
        specializationItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 4, specializationItem);

        // ✅ Cột 5: Email (căn trái)
        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(doctor.getEmail()));
        emailItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableDoctor->setItem(row, 5, emailItem);

        // ✅ Cột 6: SĐT (căn giữa)
        QString phoneDisplay = QString::fromStdString(doctor.getPhoneNumber());
        if (phoneDisplay.isEmpty()) {
            phoneDisplay = "None";
        }
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem(phoneDisplay);
        phoneNumberItem->setTextAlignment(Qt::AlignCenter);
        ui->tableDoctor->setItem(row, 6, phoneNumberItem);

        // ✅ Cột 7: Trạng thái (căn giữa)
        QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(Doctor::statusToString(doctor.getStatus())));
        statusItem->setTextAlignment(Qt::AlignCenter);

        // ✅ Tô màu theo trạng thái
        if (doctor.getStatus() == Doctor::Status::Active) {
            statusItem->setBackground(QBrush(QColor(76, 175, 80, 50)));
        } else if (doctor.getStatus() == Doctor::Status::OnLeave) {
            statusItem->setBackground(QBrush(QColor(255, 193, 7, 50)));
        } else if (doctor.getStatus() == Doctor::Status::Retired) {
            statusItem->setBackground(QBrush(QColor(158, 158, 158, 50)));
        }

        ui->tableDoctor->setItem(row, 7, statusItem);

        // ✅ Cột 8: Tuỳ chọn - Thêm 2 nút "Xem chi tiết" và "Xoá"
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        // Button "Xem chi tiết"
        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("doctorID", doctor.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background-color: #2196F3;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #1976D2;
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewDoctorDetail_clicked);

        // Button "Xoá"
        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("doctorID", doctor.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background-color: #f44336;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #d32f2f;
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveDoctor_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableDoctor->setCellWidget(row, 8, actionWidget);
    }

    // 6. Cập nhật UI phân trang
    updateDoctorPaginationUI();
}

void AdminWindow::updatePatientPaginationUI() {
    // Cập nhật label hiển thị trang hiện tại
    ui->lblCurrentPage_Appointment->setText(QString("Trang %1/%2").arg(currentPatientPage).arg(totalPatientPages));

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

void AdminWindow::loadPatientData(int page, const QString& searchText) {
    qDebug() << "Loading patient data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allPatients = PatientManager::getInstance().getAllPatientsTable();
    std::vector<Patient> filteredPatients;

    // 2. Lọc CHỈ theo ID
    if (searchText.isEmpty()) {
        for (const auto& pair : allPatients) {
            filteredPatients.push_back(pair.second);
        }
    } else {
        bool isNumber = false;
        int searchID = searchText.toInt(&isNumber);

        if (!isNumber) {
            QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập ID (số nguyên) để tìm kiếm!");
            for (const auto& pair : allPatients) {
                filteredPatients.push_back(pair.second);
            }
        } else {
            for (const auto& pair : allPatients) {
                if (pair.second.getID() == searchID) {
                    filteredPatients.push_back(pair.second);
                }
            }

            if (filteredPatients.empty()) {
                QMessageBox::information(this, "Kết quả tìm kiếm",
                                         QString("Không tìm thấy bệnh nhân có ID: %1").arg(searchID));
                for (const auto& pair : allPatients) {
                    filteredPatients.push_back(pair.second);
                }
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

        // ✅ Cột 4: Nhóm máu (căn giữa)
        QTableWidgetItem* bloodTypeItem = new QTableWidgetItem(QString::fromStdString(patient.getBloodType()));
        bloodTypeItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 4, bloodTypeItem);

        // ✅ Cột 5: SĐT (căn giữa)
        QTableWidgetItem* phoneNumberItem = new QTableWidgetItem(QString::fromStdString(patient.getPhoneNumber()));
        phoneNumberItem->setTextAlignment(Qt::AlignCenter);
        ui->tablePatient->setItem(row, 5, phoneNumberItem);

        // ✅ Cột 6: Tuỳ chọn - Thêm 2 nút "Xem chi tiết" và "Xoá"
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        // Button "Xem chi tiết"
        QPushButton* btnViewDetail = new QPushButton("Xem chi tiết");
        btnViewDetail->setProperty("patientID", patient.getID());
        btnViewDetail->setStyleSheet(R"(
            QPushButton {
                background-color: #2196F3;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #1976D2;
            }
        )");
        connect(btnViewDetail, &QPushButton::clicked, this, &AdminWindow::on_btnViewPatientDetail_clicked);

        // Button "Xoá"
        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("patientID", patient.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background-color: #f44336;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #d32f2f;
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemovePatient_clicked);

        layout->addWidget(btnViewDetail);
        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tablePatient->setCellWidget(row, 6, actionWidget);
    }

    updatePatientPaginationUI();
}

void AdminWindow::on_logoutButton_clicked()
{   
    Core::saveAll();
    this->hide();
    if (parentWidget()) {
        parentWidget()->show();
    }
}

void AdminWindow::on_appointmentManagerButton_clicked()
{
    ui->mainStack->setCurrentWidget(ui->page_appointment);
    loadAppointmentData(1, "");
}

void AdminWindow::on_doctorManagerButton_clicked()
{
    ui->mainStack->setCurrentWidget(ui->page_doctor);

    currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;

    ui->btnSortAZDoctor->setStyleSheet("");
    ui->btnSortZADoctor->setStyleSheet("");

    ui->txtSearchDoctor->clear();

    loadDoctorData(1, "");

    qDebug() << "[INFO] Switched to Doctor Manager, sorted by ID ascending";
}

void AdminWindow::on_patientManagerButton_clicked()
{
    ui->mainStack->setCurrentWidget(ui->page_patient);

    currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    ui->btnSortAZPatient->setStyleSheet("");
    ui->btnSortZAPatient->setStyleSheet("");
    ui->txtSearchPatient->clear();
    loadPatientData(1, "");

    qDebug() << "[INFO] Switched to Patient Manager, sorted by ID ascending";
}

void AdminWindow::on_userManagerButton_clicked()
{
    ui->mainStack->setCurrentWidget(ui->page_user);

    currentUserSortMode = UserSortMode::BY_ID_ASC;
    ui->btnSortAZUser->setStyleSheet("");
    ui->btnSortZAUser->setStyleSheet("");
    ui->txtSearchUser->clear();

    // Gọi setupUserTable() trước khi load data
    setupUserTable();

    loadUserData(1, "");

    qDebug() << "[INFO] Switched to User Manager, sorted by ID ascending";
}

void AdminWindow::on_medicalRecordButton_clicked()
{
    ui->mainStack->setCurrentWidget(ui->page_medicalRecord);
}

void AdminWindow::on_btnAddAppointment_clicked()
{
    qDebug() << "page_appointment: + Thêm cuộc hẹn mới clicked.";

    AddAppointmentDialog addDialog(this);

    if (addDialog.exec() == QDialog::Accepted) {
        try {
            Appointment newAppt = addDialog.getAppointmentData();
            bool doctorExists = true;
            bool patientExists = true;
            try {
                DoctorManager::getInstance().getDoctorByID(newAppt.getDoctorID());
            } catch (...) { doctorExists = false; }

            try {
                PatientManager::getInstance().getPatientByID(newAppt.getPatientID());
            } catch (...) { patientExists = false; }

            if (!doctorExists || !patientExists) {
                QMessageBox::warning(this, "Lỗi", "ID Bác sĩ hoặc Bệnh nhân không tồn tại!");
                return;
            }

            AppointmentManager::getInstance().addAppointment(newAppt);

            QMessageBox::information(this, "Thành công", "Cuộc hẹn đã được thêm và lưu.");
            loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi Thêm Cuộc Hẹn", QString("Không thể thêm cuộc hẹn: %1").arg(e.what()));
            qDebug() << "Error adding appointment: " << e.what();
        }
    } else {
        qDebug() << "Thêm cuộc hẹn đã bị hủy.";
    }
}

void AdminWindow::on_btnSearchAppointment_clicked()
{
    qDebug() << "page_appointment: TÌM KIẾM clicked.";
    QString searchText = ui->txtSearchAppointment->text().trimmed();
    currentAppointmentPage = 1;
    loadAppointmentData(currentAppointmentPage, searchText);
}

void AdminWindow::on_btnRemoveAppointment_clicked() {
    QMessageBox::information(this, "Thông báo", "Chức năng Xóa Lịch Hẹn chưa được triển khai.");
}

void AdminWindow::on_btnEditAppointment_clicked() {
    QMessageBox::information(this, "Thông báo", "Chức năng Sửa Lịch Hẹn chưa được triển khai.");
}

void AdminWindow::on_btnViewPatientDetail_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int patientID = btn->property("patientID").toInt();
    qDebug() << "[VIEW DETAIL] Patient ID:" << patientID;

    try {
        const Patient& patient = PatientManager::getInstance().getPatientByID(patientID);

        QString details = QString(
                              "=== THÔNG TIN BỆNH NHÂN ===\n\n"
                              "ID: %1\n"
                              "Họ tên: %2\n"
                              "Giới tính: %3\n"
                              "Ngày sinh: %4\n"
                              "Nhóm máu: %5\n"
                              "Số điện thoại: %6\n"
                              "Mẹ: %7\n"
                              "Cha: %8\n"
                              ).arg(patient.getID())
                              .arg(QString::fromStdString(patient.getName()))
                              .arg(QString(patient.getGender()))
                              .arg(QString::fromStdString(patient.getBirthday().toString()))
                              .arg(QString::fromStdString(patient.getBloodType()))
                              .arg(QString::fromStdString(patient.getPhoneNumber()))
                              .arg(QString::fromStdString(patient.getNameMother()))
                              .arg(QString::fromStdString(patient.getNameFather()));

        if (!patient.getAllergies().empty()) {
            details += "\nDị ứng: " + QString::fromStdString(Utils::join(patient.getAllergies(), ", "));
        }

        if (!patient.getChronicDiseases().empty()) {
            details += "\nBệnh nền: " + QString::fromStdString(Utils::join(patient.getChronicDiseases(), ", "));
        }

        QMessageBox::information(this, "Chi tiết Bệnh nhân", details);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", QString("Không thể xem chi tiết: %1").arg(e.what()));
    }
}

void AdminWindow::on_btnNextPage_Appointment_clicked()
{
    if (currentAppointmentPage < totalAppointmentPages) {
        currentAppointmentPage++;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Appointment_clicked()
{
    if (currentAppointmentPage > 1) {
        currentAppointmentPage--;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_1_clicked()
{
    int pageNum = ui->btnPage_Appointment_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_2_clicked()
{
    int pageNum = ui->btnPage_Appointment_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Appointment_3_clicked()
{
    int pageNum = ui->btnPage_Appointment_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalAppointmentPages) {
        currentAppointmentPage = pageNum;
        loadAppointmentData(currentAppointmentPage, ui->txtSearchAppointment->text().trimmed());
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

            qDebug() << "Patient data retrieved, ID:" << newPatient.getID();

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
    if (currentPatientPage < totalPatientPages) {
        currentPatientPage++;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Patient_clicked()
{
    if (currentPatientPage > 1) {
        currentPatientPage--;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_1_clicked()
{
    int pageNum = ui->btnPage_Patient_1->text().toInt();
    if (pageNum > 0 && pageNum <= totalPatientPages) {
        currentPatientPage = pageNum;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_2_clicked()
{
    int pageNum = ui->btnPage_Patient_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalPatientPages) {
        currentPatientPage = pageNum;
        loadPatientData(currentPatientPage, ui->txtSearchPatient->text().trimmed());
    }
}

void AdminWindow::on_btnPage_Patient_3_clicked()
{
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

void AdminWindow::on_btnEditDoctor_clicked() {
    QMessageBox::information(this, "Thông báo", "Chức năng Sửa Lịch Hẹn chưa được triển khai.");
}

void AdminWindow::on_btnViewDoctorDetail_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int doctorID = btn->property("doctorID").toInt();
    qDebug() << "[VIEW DETAIL] Doctor ID:" << doctorID;

    try {
        const Doctor& doctor = DoctorManager::getInstance().getDoctorByID(doctorID);

        QString details = QString(
                              "=== THÔNG TIN BÁC SĨ ===\n\n"
                              "ID: %1\n"
                              "Họ tên: %2\n"
                              "Giới tính: %3\n"
                              "Ngày sinh: %4\n"
                              "Chuyên khoa: %5\n"
                              "Email: %6\n"
                              "Số điện thoại: %7\n"
                              "Trạng thái: %8\n"
                              ).arg(doctor.getID())
                              .arg(QString::fromStdString(doctor.getName()))
                              .arg(QString(doctor.getGender()))
                              .arg(QString::fromStdString(doctor.getBirthday().toString()))
                              .arg(QString::fromStdString(doctor.getSpecialization()))
                              .arg(QString::fromStdString(doctor.getEmail()))
                              .arg(QString::fromStdString(doctor.getPhoneNumber()))
                              .arg(QString::fromStdString(Doctor::statusToString(doctor.getStatus())));

        // Thêm danh sách bệnh nhân
        if (!doctor.getPatientIDs().empty()) {
            details += "\nDanh sách bệnh nhân đang điều trị:\n";
            for (int patientID : doctor.getPatientIDs()) {
                try {
                    const Patient& patient = PatientManager::getInstance().getPatientByID(patientID);
                    details += QString("  - ID %1: %2\n")
                                   .arg(patientID)
                                   .arg(QString::fromStdString(patient.getName()));
                } catch (...) {
                    details += QString("  - ID %1: (không tìm thấy)\n").arg(patientID);
                }
            }
        } else {
            details += "\nChưa có bệnh nhân nào.";
        }

        QMessageBox::information(this, "Chi tiết Bác sĩ", details);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", QString("Không thể xem chi tiết: %1").arg(e.what()));
    }
}

void AdminWindow::on_btnSearchDoctor_clicked() {
    qDebug() << "page_doctor: TÌM KIẾM clicked.";

    QString searchText = ui->txtSearchDoctor->text().trimmed();
    currentDoctorPage = 1;
    loadDoctorData(currentDoctorPage, searchText);
}

void AdminWindow::on_btnNextPage_Doctor_clicked()
{
    if (currentDoctorPage < totalDoctorPages) {
        currentDoctorPage++;
        loadDoctorData(currentDoctorPage, ui->txtSearchDoctor->text().trimmed());
    }
}

void AdminWindow::on_btnPrevPage_Doctor_clicked()
{
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

void AdminWindow::setupUserTable() {
    ui->tableUser->setColumnCount(5);

    QStringList headers;
    headers << "ID" << "Vai trò" << "Tên người dùng" << "Mật khẩu" << "Tuỳ chọn";
    ui->tableUser->setHorizontalHeaderLabels(headers);

    QHeaderView* header = ui->tableUser->horizontalHeader();

    // Cột 0: ID (Fixed)
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(0, 80);

    // Cột 1: Vai trò (Fixed)
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(1, 150);

    // Cột 2: Tên người dùng (Stretch)
    header->setSectionResizeMode(2, QHeaderView::Stretch);

    // Cột 3: Mật khẩu (Fixed)
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(3, 150);

    // Cột 4: Tuỳ chọn (Fixed)
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableUser->setColumnWidth(4, 120);

    ui->tableUser->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableUser->verticalHeader()->setDefaultSectionSize(75);
    ui->tableUser->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUser->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableUser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUser->setAlternatingRowColors(true);
    ui->tableUser->verticalHeader()->setVisible(false);
    header->setDefaultAlignment(Qt::AlignCenter);

    qDebug() << "[TABLE SETUP] User table configured successfully";
}

void AdminWindow::updateUserPaginationUI() {
    ui->lblCurrentPage_User->setText(QString("Trang %1/%2").arg(currentUserPage).arg(totalUserPages));

    ui->pushButton_5->setEnabled(currentUserPage > 1);
    ui->pushButton_4->setEnabled(currentUserPage < totalUserPages);

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

    // Nút 1
    if (startPage <= totalUserPages) {
        ui->pushButton_3->setText(QString::number(startPage));
        ui->pushButton_3->setVisible(true);
        ui->pushButton_3->setEnabled(startPage != currentUserPage);
        if (startPage == currentUserPage) {
            ui->pushButton_3->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->pushButton_3->setStyleSheet("");
        }
    } else {
        ui->pushButton_3->setVisible(false);
    }

    // Nút 2
    if (startPage + 1 <= totalUserPages) {
        ui->pushButton_2->setText(QString::number(startPage + 1));
        ui->pushButton_2->setVisible(true);
        ui->pushButton_2->setEnabled(startPage + 1 != currentUserPage);
        if (startPage + 1 == currentUserPage) {
            ui->pushButton_2->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->pushButton_2->setStyleSheet("");
        }
    } else {
        ui->pushButton_2->setVisible(false);
    }

    // Nút 3
    if (startPage + 2 <= totalUserPages) {
        ui->pushButton->setText(QString::number(startPage + 2));
        ui->pushButton->setVisible(true);
        ui->pushButton->setEnabled(startPage + 2 != currentUserPage);
        if (startPage + 2 == currentUserPage) {
            ui->pushButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        } else {
            ui->pushButton->setStyleSheet("");
        }
    } else {
        ui->pushButton->setVisible(false);
    }

    qDebug() << "[USER PAGINATION] Current:" << currentUserPage
             << "| Total:" << totalUserPages
             << "| Buttons:" << startPage << startPage+1 << startPage+2;
}

void AdminWindow::loadUserData(int page, const QString& searchText) {
    qDebug() << "Loading user data for page" << page << "with search text:" << searchText;

    // 1. Lấy dữ liệu từ Singleton
    const auto& allUsers = UserManager::getInstance().getAllUsers();
    std::vector<User> filteredUsers;

    // 2. Lọc CHỈ theo ID
    if (searchText.isEmpty()) {
        for (const auto& pair : allUsers) {
            filteredUsers.push_back(pair.second);
        }
    } else {
        bool isNumber = false;
        int searchID = searchText.toInt(&isNumber);

        if (!isNumber) {
            QMessageBox::warning(this, "Cảnh báo", "Vui lòng nhập ID (số nguyên) để tìm kiếm!");
            for (const auto& pair : allUsers) {
                filteredUsers.push_back(pair.second);
            }
        } else {
            for (const auto& pair : allUsers) {
                if (pair.second.getID() == searchID) {
                    filteredUsers.push_back(pair.second);
                }
            }

            if (filteredUsers.empty()) {
                QMessageBox::information(this, "Kết quả tìm kiếm",
                                         QString("Không tìm thấy người dùng có ID: %1").arg(searchID));
                for (const auto& pair : allUsers) {
                    filteredUsers.push_back(pair.second);
                }
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

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(user.getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableUser->setItem(row, 0, idItem);

        QTableWidgetItem* roleItem = new QTableWidgetItem(QString::fromStdString(User::roleToString(user.getRole())));
        roleItem->setTextAlignment(Qt::AlignCenter);

        // Tô màu theo vai trò
        if (user.getRole() == User::Role::ADMIN) {
            roleItem->setBackground(QBrush(QColor(255, 87, 34, 50))); // Cam
 
        } else if (user.getRole() == User::Role::ASSISTANT) {
            roleItem->setBackground(QBrush(QColor(76, 175, 80, 50))); // Xanh lá
        }

        ui->tableUser->setItem(row, 1, roleItem);

        QTableWidgetItem* usernameItem = new QTableWidgetItem(QString::fromStdString(user.getUsername()));
        usernameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableUser->setItem(row, 2, usernameItem);

        QTableWidgetItem* passwordItem = new QTableWidgetItem(QString::fromStdString(user.getPassword()));
        passwordItem->setTextAlignment(Qt::AlignCenter);
        ui->tableUser->setItem(row, 3, passwordItem);

        QWidget* actionWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionWidget);
        layout->setContentsMargins(5, 2, 5, 2);
        layout->setSpacing(5);

        QPushButton* btnDelete = new QPushButton("Xoá");
        btnDelete->setProperty("userID", user.getID());
        btnDelete->setStyleSheet(R"(
            QPushButton {
                background-color: #f44336;
                color: white;
                border: none;
                padding: 5px 10px;
                border-radius: 3px;
            }
            QPushButton:hover {
                background-color: #d32f2f;
            }
        )");
        connect(btnDelete, &QPushButton::clicked, this, &AdminWindow::on_btnRemoveUser_clicked);

        layout->addWidget(btnDelete);
        actionWidget->setLayout(layout);

        ui->tableUser->setCellWidget(row, 4, actionWidget);
    }

    updateUserPaginationUI();
}
void AdminWindow::on_btnAddUser_clicked() {
    qDebug() << "Dialog add user opened";
    AddEditUserDialog dialog(this);
    dialog.setDialogTitle("Thêm Người Dùng Mới");

    if (dialog.exec() == QDialog::Accepted) {
        try {
            qDebug() << "Dialog accepted, getting data...";
            User newUser = dialog.getUserData();

            qDebug() << "User data retrieved, ID:" << newUser.getID();

            UserManager::getInstance().addUser(newUser);

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
void AdminWindow::on_btnViewUserDetail_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int userID = btn->property("userID").toInt();
    qDebug() << "[VIEW DETAIL] User ID:" << userID;

    try {
        const User& user = UserManager::getInstance().getUserByID(userID);

        QString details = QString(
                              "=== THÔNG TIN NGƯỜI DÙNG ===\n\n"
                              "ID: %1\n"
                              "Vai trò: %2\n"
                              "Tên người dùng: %3\n"
                              ).arg(user.getID())
                              .arg(QString::fromStdString(User::roleToString(user.getRole())))
                              .arg(QString::fromStdString(user.getUsername()));

        QMessageBox::information(this, "Chi tiết Người dùng", details);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", QString("Không thể xem chi tiết: %1").arg(e.what()));
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
    int pageNum = ui->pushButton_3->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::on_btnPage_User_2_clicked()
{
    int pageNum = ui->pushButton_2->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}

void AdminWindow::on_btnPage_User_3_clicked()
{
    int pageNum = ui->pushButton->text().toInt();
    if (pageNum > 0 && pageNum <= totalUserPages) {
        currentUserPage = pageNum;
        loadUserData(currentUserPage, ui->txtSearchUser->text().trimmed());
    }
}
