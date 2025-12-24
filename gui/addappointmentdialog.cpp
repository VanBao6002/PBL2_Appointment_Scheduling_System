#include "addappointmentdialog.h"
#include "gui/ui_addappointmentdialog.h"
#include "addeditpatientdialog.h"
#include <QStringList>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QLayoutItem>
#include <QLayout>
#include <Qt>
#include <QButtonGroup>
#include <QCalendarWidget>
#include <QListWidget>
#include <qtimer.h>
#include "doctorManager.h"
#include "patientManager.h"
#include "appointmentManager.h"
#include "timeInterval.h"
#include "IDHandler.h"
#include "core.h"
#include "config.h"

AddAppointmentDialog::AddAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog),
    isNewPatient(false),
    selectedDoctorID(-1),
    selectedPatientID(-1),
    cmbStatus(nullptr),
    editMode(false),
    appointmentID(-1),
    lastSelectedDate(QDate())

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    qDebug() << "[DEBUG] AddAppointmentDialog created. Initial doctor ID:" << selectedDoctorID;

    applyStyle();

    setupConnections();

    setDialogTitle("Thêm Cuộc Hẹn Mới");

    setupStatusComboBox();

    setupSpecializationComboBox();

    populateDoctorCards();
}

AddAppointmentDialog::AddAppointmentDialog(const Appointment& appointment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAppointmentDialog),
    isNewPatient(false),
    selectedDoctorID(-1),
    selectedPatientID(-1),
    cmbStatus(nullptr),
    editMode(true),
    appointmentID(appointment.getID()),
    lastSelectedDate(QDate())
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    applyStyle();

    setupConnections();

    loadAppointmentData(appointment);

    setDialogTitle("Chỉnh Sửa Cuộc Hẹn - ID: " + QString::number(appointmentID));
}

void AddAppointmentDialog::loadAppointmentData(const Appointment& appointment) {
    selectedPatientID = appointment.getPatientID();
    selectedDoctorID = appointment.getDoctorID();

    try {
        Patient patient = PatientManager::getInstance().getPatientByID(selectedPatientID);
        ui->txtCCCD->setText(QString::fromStdString(patient.getCCCD()));
        ui->txtCCCD->setEnabled(false);
    } catch (...) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thông tin bệnh nhân!");
    }

    ui->stackedWidget->setCurrentIndex(1);

    // Cập nhật danh sách bác sĩ và chọn bác sĩ hiện tại
    populateDoctorCards();

    // Chọn bác sĩ trong danh sách
    QTimer::singleShot(100, this, [this, appointment]() {
        // Tìm và chọn bác sĩ trong danh sách
        if (doctorButtonGroup) {
            QAbstractButton* doctorBtn = doctorButtonGroup->button(appointment.getDoctorID());
            if (doctorBtn) {
                doctorBtn->click();

                // Cập nhật calendar với ngày đã chọn
                QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
                if (page2) {
                    QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
                    if (calendar) {
                        QDate apptDate(appointment.getDate().getYear(),
                                       appointment.getDate().getMonth(),
                                       appointment.getDate().getDay());
                        calendar->setSelectedDate(apptDate);

                        // Cập nhật time slot
                        updateAvailableCalendarDaysAndTimeSlots(selectedDoctorID);

                        // Chọn time slot trong danh sách
                        QTimer::singleShot(200, this, [this, appointment]() {
                            QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
                            if (page2) {
                                QListWidget* listTimeSlots = page2->findChild<QListWidget*>("listTimeSlots");
                                if (listTimeSlots) {
                                    QString targetSlot = QString::fromStdString(appointment.getStartTime() + " - " + appointment.getEndTime());
                                    for (int i = 0; i < listTimeSlots->count(); ++i) {
                                        if (listTimeSlots->item(i)->text() == targetSlot) {
                                            listTimeSlots->setCurrentRow(i);
                                            break;
                                        }
                                    }
                                }

                                // Cập nhật trạng thái
                                QComboBox* statusCombo = page2->findChild<QComboBox*>("cmbStatus");
                                if (statusCombo) {
                                    QString statusStr = QString::fromStdString(Appointment::statusToString(appointment.getStatus()));
                                    int index = statusCombo->findText(statusStr, Qt::MatchFixedString);
                                    if (index >= 0) {
                                        statusCombo->setCurrentIndex(index);
                                    }
                                }
                            }
                        });
                    }
                }
            }
        }
    });
}

void AddAppointmentDialog::applyStyle() {
    this->setStyleSheet(R"(
        /* ===== DIALOG BACKGROUND ===== */
        QDialog {
            background-color: #f5f5f5;
        }

        /* ===== LABELS - ĐẢM BẢO HIỂN THỊ RÕ RÀNG ===== */
        QLabel {
            color: #2c3e50;
            background-color: transparent;
            border: none;
            font-weight: 600;
            font-size: 11pt;
            padding: 2px;
        }

        /* ===== TITLE LABELS ===== */
        QLabel[text*="TÌM KIẾM"],
        QLabel[text*="CHỌN BÁC SĨ"],
        QLabel[text*="CHỌN NGÀY"],
        QLabel[text*="XÁC NHẬN"] {
            color: #1a252f;
            font-size: 18pt;
            font-weight: bold;
            background: white;
            border: 2px solid #3498db;
            border-radius: 8px;
            padding: 15px;
        }

        /* ===== GROUPBOX ===== */
        QGroupBox {
            font-weight: bold;
            font-size: 11pt;
            color: #2c3e50;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 15px;
            padding-top: 15px;
            background-color: white;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 5px 10px;
            background-color: white;
            color: #2c3e50;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
        }

        /* ===== BUTTONS ===== */
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 6px;
            font-weight: bold;
            font-size: 10pt;
            min-width: 100px;
            min-height: 35px;
        }

        QPushButton:hover {
            background-color: #2980b9;
        }

        QPushButton:pressed {
            background-color: #1f6391;
        }

        QPushButton:disabled {
            background-color: #bdc3c7;
            color: #7f8c8d;
        }

        /* ===== SPECIFIC BUTTONS ===== */
        QPushButton[text*="Tìm kiếm"],
        QPushButton[text*="Chọn bác sĩ"] {
            background-color: #27ae60;
        }

        QPushButton[text*="Tìm kiếm"]:hover,
        QPushButton[text*="Chọn bác sĩ"]:hover {
            background-color: #229954;
        }

        QPushButton[text*="Quay lại"] {
            background-color: #95a5a6;
        }

        QPushButton[text*="Quay lại"]:hover {
            background-color: #7f8c8d;
        }

        QPushButton[text*="Hủy"],
        QPushButton[text*="Cancel"] {
            background-color: #e74c3c;
        }

        QPushButton[text*="Hủy"]:hover,
        QPushButton[text*="Cancel"]:hover {
            background-color: #c0392b;
        }

        QPushButton[text*="Xác nhận"],
        QPushButton[text*="Tiếp theo"] {
            background-color: #27ae60;
            font-size: 11pt;
            padding: 12px 30px;
        }

        QPushButton[text*="Xác nhận"]:hover,
        QPushButton[text*="Tiếp theo"]:hover {
            background-color: #229954;
        }

        /* ===== LINE EDIT ===== */
        QLineEdit {
            border: 2px solid #bdc3c7;
            border-radius: 5px;
            padding: 10px;
            font-size: 11pt;
            background-color: white;
            color: #2c3e50;
        }

        QLineEdit:focus {
            border: 2px solid #3498db;
            background-color: #ffffff;
        }

        QLineEdit::placeholder {
            color: #95a5a6;
            font-style: italic;
        }

        /* ===== COMBOBOX ===== */
        QComboBox {
            border: 2px solid #bdc3c7;
            border-radius: 5px;
            padding: 8px;
            background-color: white;
            font-size: 11pt;
            color: #2c3e50;
            min-height: 30px;
        }

        QComboBox:focus {
            border: 2px solid #3498db;
        }

        QComboBox::drop-down {
            border: none;
            width: 30px;
        }

        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 8px solid #2c3e50;
            margin-right: 5px;
        }

        QComboBox QAbstractItemView {
            background-color: white;
            color: #2c3e50;
            selection-background-color: #3498db;
            selection-color: white;
            border: 1px solid #bdc3c7;
        }

        /* ===== CALENDAR WIDGET - IMPROVED ===== */
        QCalendarWidget {
            background-color: white;
            border: 2px solid #3498db;
            border-radius: 8px;
        }

        /* Calendar Navigation Bar */
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: #3498db;
            min-height: 60px;
        }

        /* Month/Year buttons */
        QCalendarWidget QToolButton {
            background-color: #2980b9;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px;
            font-weight: bold;
            font-size: 11pt;
            margin: 5px;
        }

        QCalendarWidget QToolButton:hover {
            background-color: #1f6391;
        }

        QCalendarWidget QToolButton::menu-indicator {
            image: none;
        }

        /* Previous/Next Month Arrows */
        QCalendarWidget QToolButton#qt_calendar_prevmonth,
        QCalendarWidget QToolButton#qt_calendar_nextmonth {
            background-color: transparent;
            qproperty-icon: none;
            min-width: 40px;
            max-width: 40px;
        }

        QCalendarWidget QToolButton#qt_calendar_prevmonth {
            qproperty-text: "◀";
        }

        QCalendarWidget QToolButton#qt_calendar_nextmonth {
            qproperty-text: "▶";
        }

        /* Header (Days of week) */
        QCalendarWidget QWidget {
            alternate-background-color: white;
        }

        QCalendarWidget QAbstractItemView:enabled {
            background-color: white;
            color: #2c3e50;
            font-size: 11pt;
            selection-background-color: #3498db;
            selection-color: white;
        }

        /* Weekend days */
        QCalendarWidget QAbstractItemView:enabled[class="weekend"] {
            color: #e74c3c;
        }

        /* Month dropdown menu */
        QCalendarWidget QMenu {
            background-color: white;
            border: 1px solid #bdc3c7;
        }

        QCalendarWidget QSpinBox {
            background-color: white;
            color: #2c3e50;
            border: 1px solid #bdc3c7;
            border-radius: 3px;
            padding: 5px;
            font-size: 11pt;
        }

        /* ===== LIST WIDGET (Time Slots) ===== */
        QListWidget {
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            background-color: white;
            font-size: 11pt;
            padding: 5px;
        }

        QListWidget::item {
            padding: 12px;
            border-bottom: 1px solid #ecf0f1;
            color: #2c3e50;
            background-color: white;
        }

        QListWidget::item:hover {
            background-color: #e8f5e9;
        }

        QListWidget::item:selected {
            background-color: #27ae60;
            color: white;
            font-weight: bold;
            border-left: 4px solid #229954;
        }

        QListWidget::item:disabled {
            color: #bdc3c7;
            background-color: #f5f5f5;
            text-decoration: line-through;
        }

        /* ===== SCROLL AREA ===== */
        QScrollArea {
            border: none;
            background-color: transparent;
        }

        /* ===== SCROLL BARS ===== */
        QScrollBar:vertical {
            border: none;
            background: #ecf0f1;
            width: 12px;
            margin: 0px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: #95a5a6;
            border-radius: 6px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: #7f8c8d;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #ecf0f1;
            height: 12px;
            margin: 0px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background: #95a5a6;
            border-radius: 6px;
            min-width: 30px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #7f8c8d;
        }
    )");
}

AddAppointmentDialog::~AddAppointmentDialog()
{
    delete ui;
}

void AddAppointmentDialog::setupStatusComboBox() {
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    if (!page2) return;
    QComboBox* cmbStatus = page2->findChild<QComboBox*>("cmbStatus");
    if (!cmbStatus) return;

    cmbStatus->clear();
    cmbStatus->addItem("Scheduled");
    cmbStatus->addItem("Occupied");
    cmbStatus->addItem("Cancelled");
}

void AddAppointmentDialog::updateAvailableTimeSlot(const QStringList& availableSlots, const QSet<QString>& occupiedSlots) {
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    if (!page2) return;

    QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
    if (!calendar) return;

    QDate selectedDate = calendar->selectedDate();
    if (!selectedDate.isValid()) return;

    // Get weekday name in 'Thứ N' format to match JSON
    QString weekdayName = "Thứ " + QString::number(selectedDate.dayOfWeek());

    // ✅ FIX: Get FRESH booked slots for this exact date
    std::vector<std::pair<std::string, std::string>> bookedSlots;
    bookedSlots = AppointmentManager::getInstance().getBookedSlotsForDoctorDate(
        selectedDoctorID, selectedDate.toString("dd-MM-yyyy").toStdString());

    qDebug() << "[DEBUG] Date:" << selectedDate.toString("dd-MM-yyyy") 
             << "| Booked slots count:" << bookedSlots.size();
    
    // Get all intervals for the day with booking status
    auto intervals = TimeInterval::getIntervalsWithBooking(
        weekdayName.toStdString(), 
        DoctorManager::getInstance().getDoctorByID(selectedDoctorID).getWorkingSchedule(), 
        bookedSlots
    );

    // Populate list widget with ALL intervals, marking booked ones as disabled
    QListWidget* listTimeSlots = page2->findChild<QListWidget*>("listTimeSlots");
    if (listTimeSlots) {
        listTimeSlots->blockSignals(true);
        listTimeSlots->clear();
        
        int firstAvailableRow = -1;
        int rowCount = 0;
        
        for (const auto& [start, end, isBooked] : intervals) {
            QString slotText = QString::fromStdString(start + " - " + end);
            QListWidgetItem* item = new QListWidgetItem(slotText);
            
            if (isBooked) {
                // ✅ Mark booked slots as disabled
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
                item->setForeground(QBrush(QColor(189, 195, 199))); // Gray
                item->setBackground(QBrush(QColor(245, 245, 245))); // Light gray bg
                
                qDebug() << "[BOOKED]" << slotText;
            } else {
                // ✅ Available slot
                if (firstAvailableRow == -1) {
                    firstAvailableRow = rowCount;
                }
                qDebug() << "[AVAILABLE]" << slotText;
            }
            
            listTimeSlots->addItem(item);
            rowCount++;
        }
        
        listTimeSlots->blockSignals(false);
        
        // ✅ Select first available slot
        if (firstAvailableRow != -1) {
            selectedTimeSlot = listTimeSlots->item(firstAvailableRow)->text();
            listTimeSlots->setCurrentRow(firstAvailableRow);
            qDebug() << "[SELECTED]" << selectedTimeSlot;
        } else {
            selectedTimeSlot.clear();
            qDebug() << "[NO SLOTS] No available slots for this date";
        }
        
        // ✅ Connect signal to update selectedTimeSlot when user changes selection
        disconnect(listTimeSlots, &QListWidget::currentTextChanged, nullptr, nullptr);
        connect(listTimeSlots, &QListWidget::currentTextChanged, this, [this](const QString& text) {
            selectedTimeSlot = text;
            qDebug() << "[USER SELECTED]" << selectedTimeSlot;
        });
    }
}

void AddAppointmentDialog::updateAvailableCalendarDaysAndTimeSlots(int doctorID) {
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    if (!page2) return;

    QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
    if (!calendar) return;

    Doctor doctor;
    try {
        doctor = DoctorManager::getInstance().getDoctorByID(doctorID);
    } catch (...) {
        return;
    }

    // ===== RESET TẤT CẢ FORMAT TRƯỚC =====
    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::white);
    defaultFormat.setForeground(Qt::black);

    QDate minDate = QDate::currentDate();
    QDate maxDate = minDate.addDays(Config::MAGIC_MARKED_AVAILABLE_WORKING_DAYS_NUMBER);

    for (QDate date = minDate; date <= maxDate; date = date.addDays(1)) {
        calendar->setDateTextFormat(date, defaultFormat);
    }

    // ===== TẠO FORMAT CHO WORKING DAYS =====
    QTextCharFormat workingDayFormat;
    workingDayFormat.setBackground(QColor(46, 204, 113, 100)); // Xanh lá nhạt
    workingDayFormat.setForeground(Qt::white);
    workingDayFormat.setFontWeight(QFont::Bold);

    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor(52, 152, 219)); // Xanh dương
    todayFormat.setForeground(Qt::white);
    todayFormat.setFontWeight(QFont::Bold);

    QTextCharFormat selectedFormat;
    selectedFormat.setBackground(QColor(231, 76, 60)); // Đỏ
    selectedFormat.setForeground(Qt::white);
    selectedFormat.setFontWeight(QFont::Bold);

    QSet<QDate> workingDays;
    const auto& daysVec = doctor.getWorkingSchedule().getDays();
    QDate today = QDate::currentDate();

    // ===== ĐÁNH DẤU WORKING DAYS =====
    for (int i = 0; i < Config::MAGIC_MARKED_AVAILABLE_WORKING_DAYS_NUMBER; ++i) {
        QDate date = today.addDays(i);
        QString weekdayName = "Thứ " + QString::number(date.dayOfWeek());

        for (const auto& dayStr : daysVec) {
            if (weekdayName.compare(QString::fromStdString(dayStr), Qt::CaseInsensitive) == 0) {
                workingDays.insert(date);

                if (date == today) {
                    calendar->setDateTextFormat(date, todayFormat);
                } else {
                    calendar->setDateTextFormat(date, workingDayFormat);
                }
                break;
            }
        }
    }

    // ===== THÊM LEGEND (HƯỚNG DẪN) =====
    QWidget* legendWidget = page2->findChild<QWidget*>("legendWidget");
    if (!legendWidget) {
        legendWidget = new QWidget(page2);
        legendWidget->setObjectName("legendWidget");

        QHBoxLayout* legendLayout = new QHBoxLayout(legendWidget);
        legendLayout->setContentsMargins(10, 5, 10, 5);

        // Ngày hôm nay
        QLabel* todayIcon = new QLabel("⬤");
        todayIcon->setStyleSheet("color: #3498db; font-size: 14pt;");
        QLabel* todayLabel = new QLabel("Hôm nay");
        todayLabel->setStyleSheet("color: #2c3e50; font-size: 10pt;");

        // Ngày làm việc
        QLabel* workingIcon = new QLabel("⬤");
        workingIcon->setStyleSheet("color: #2ecc71; font-size: 14pt;");
        QLabel* workingLabel = new QLabel("Ngày làm việc");
        workingLabel->setStyleSheet("color: #2c3e50; font-size: 10pt;");

        // Ngày đã chọn
        QLabel* selectedIcon = new QLabel("⬤");
        selectedIcon->setStyleSheet("color: #e74c3c; font-size: 14pt;");
        QLabel* selectedLabel = new QLabel("Đã chọn");
        selectedLabel->setStyleSheet("color: #2c3e50; font-size: 10pt;");

        legendLayout->addWidget(todayIcon);
        legendLayout->addWidget(todayLabel);
        legendLayout->addSpacing(20);
        legendLayout->addWidget(workingIcon);
        legendLayout->addWidget(workingLabel);
        legendLayout->addSpacing(20);
        legendLayout->addWidget(selectedIcon);
        legendLayout->addWidget(selectedLabel);
        legendLayout->addStretch();

        // Thêm vào layout của page2
        QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(page2->layout());
        if (mainLayout) {
            // Tìm vị trí của groupBoxDate để insert legend sau nó
            for (int i = 0; i < mainLayout->count(); ++i) {
                QLayoutItem* item = mainLayout->itemAt(i);
                if (item && item->layout()) {
                    QHBoxLayout* hbox = qobject_cast<QHBoxLayout*>(item->layout());
                    if (hbox) {
                        mainLayout->insertWidget(i + 1, legendWidget);
                        break;
                    }
                }
            }
        }
    }

    // ===== DISCONNECT PREVIOUS CONNECTIONS =====
    disconnect(calendar, nullptr, nullptr, nullptr);

    // In updateAvailableCalendarDaysAndTimeSlots function, replace the calendar selection changed connection:

connect(calendar, &QCalendarWidget::selectionChanged, this,
        [this, doctorID, calendar, daysVec, selectedFormat, today, todayFormat, workingDays, workingDayFormat]() {

            QDate selected = calendar->selectedDate();
            QString weekdayName = "Thứ " + QString::number(selected.dayOfWeek());

            // ✅ RESET NGÀY TRƯỚC VỀ FORMAT GỐC
            if (lastSelectedDate.isValid() && lastSelectedDate != selected) {
                if (lastSelectedDate == today) {
                    calendar->setDateTextFormat(lastSelectedDate, todayFormat);
                } else if (workingDays.contains(lastSelectedDate)) {
                    calendar->setDateTextFormat(lastSelectedDate, workingDayFormat);
                }
            }

            // Highlight ngày được chọn
            calendar->setDateTextFormat(selected, selectedFormat);
            lastSelectedDate = selected;

            // Kiểm tra ngày làm việc
            bool isWorkingDay = false;
            for (const auto& dayStr : daysVec) {
                if (weekdayName.compare(QString::fromStdString(dayStr), Qt::CaseInsensitive) == 0) {
                    isWorkingDay = true;
                    break;
                }
            }

            if (!isWorkingDay) {
                QMessageBox::information(this, "Thông báo",
                                         "Bác sĩ không làm việc vào " + weekdayName + " này.\n"
                                                                                  "Vui lòng chọn ngày làm việc khác (được đánh dấu màu xanh lá).");
                updateAvailableTimeSlot(QStringList(), QSet<QString>());
                return;
            }

            Doctor doctor;
            try {
                doctor = DoctorManager::getInstance().getDoctorByID(doctorID);
            } catch (...) {
                updateAvailableTimeSlot(QStringList(), QSet<QString>());
                return;
            }

            // ✅ FIX: Get booked slots using the correct date format
            std::vector<std::pair<std::string, std::string>> bookedSlots;
            bookedSlots = AppointmentManager::getInstance().getBookedSlotsForDoctorDate(
                doctorID, selected.toString("dd-MM-yyyy").toStdString());

            // ✅ FIX: Get intervals with proper booking status
            auto intervals = TimeInterval::getIntervalsWithBooking(
                weekdayName.toStdString(), doctor.getWorkingSchedule(), bookedSlots);

            // ✅ FIX: Prepare all slots and mark booked ones correctly
            QStringList availableSlots;
            for (const auto& [start, end, isBooked] : intervals) {
                if (!isBooked) {
                    availableSlots.append(QString::fromStdString(start + " - " + end));
                }
            }

            // ✅ Update the time slot list
            updateAvailableTimeSlot(availableSlots, QSet<QString>());
        });
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
    bool valid = IDHandler<Patient>::checkDuplicateID(static_cast<size_t>(patientID));
    return valid;
}

bool AddAppointmentDialog::isPatientExisted(const std::string& cccd) {
    bool patientExists = true;
    try {
        PatientManager::getInstance().getPatientByCCCD(cccd);
    } catch (...) {
        patientExists = false;
    }
    return patientExists;
}

void AddAppointmentDialog::populateDoctorCards() {
    QWidget* doctorListWidget = ui->stackedWidget->findChild<QWidget*>("doctorListWidget");
    if (!doctorListWidget) {
        qDebug() << "[ERROR] doctorListWidget not found!";
        return;
    }

    // Clear old layout
    QLayout* oldLayout = doctorListWidget->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout* layout = new QVBoxLayout(doctorListWidget);
    doctorButtonGroup = new QButtonGroup(this);
    doctorButtonGroup->setExclusive(true);

    auto& doctorManager = DoctorManager::getInstance();
    QWidget* page1 = ui->stackedWidget->findChild<QWidget*>("page_1");
    QComboBox* cmbSpecialization = page1 ? page1->findChild<QComboBox*>("cmbSpecialization") : nullptr;
    QString selectedSpecialization = cmbSpecialization ? cmbSpecialization->currentText() : QString();

    qDebug() << "[DEBUG] Selected specialization:" << selectedSpecialization;

    bool showAll = selectedSpecialization.isEmpty() ||
                   selectedSpecialization == "-- Tất cả chuyên khoa --";

    int doctorCount = 0;

    // In danh sách tất cả bác sĩ để debug
    qDebug() << "[DEBUG] All doctors in manager:";
    for (const auto& pair : doctorManager.getAllDoctors()) {
        const Doctor& doc = pair.second;
        qDebug() << "  ID:" << doc.getID()
                 << ", Name:" << QString::fromStdString(doc.getName())
                 << ", Status:" << static_cast<int>(doc.getStatus())
                 << ", Specialization:" << QString::fromStdString(doc.getSpecialization());
    }

    for (const auto& pair : doctorManager.getAllDoctors()) {
        const Doctor& doctor = pair.second;

        // Chỉ filter theo status và specialization
        if (doctor.getStatus() != Doctor::Status::Active) {
            qDebug() << "[DEBUG] Skipping doctor ID" << doctor.getID() << "- Not Active";
            continue;
        }

        if (!showAll && QString::fromStdString(doctor.getSpecialization()) != selectedSpecialization) {
            qDebug() << "[DEBUG] Skipping doctor ID" << doctor.getID() << "- Specialization mismatch";
            continue;
        }

        doctorCount++;
        qDebug() << "[DEBUG] Adding doctor to UI:" << doctor.getID()
                 << "-" << QString::fromStdString(doctor.getName());

        // Tạo card cho bác sĩ
        QWidget* card = new QWidget;
        card->setStyleSheet(R"(
            QWidget {
                background-color: white;
                border: 2px solid #e0e0e0;
                border-radius: 10px;
                padding: 15px;
                margin: 5px;
            }
            QWidget:hover {
                border: 2px solid #4CAF50;
            }
        )");

        QVBoxLayout* vbox = new QVBoxLayout(card);

        // Tên bác sĩ
        QLabel* name = new QLabel(QString::fromStdString(doctor.getName()));
        name->setStyleSheet("font-size: 14pt; font-weight: bold; color: #2c3e50;");

        // Chuyên khoa
        QLabel* spec = new QLabel("Chuyên khoa: " + QString::fromStdString(doctor.getSpecialization()));
        spec->setStyleSheet("font-size: 11pt; color: #7f8c8d;");

        // Phòng
        QLabel* room = new QLabel("Phòng: " + QString::fromStdString(doctor.getRoom()));
        room->setStyleSheet("font-size: 10pt; color: #95a5a6;");

        // Mô tả
        QString desc = QString::fromStdString(doctor.getDescription());
        if (!desc.isEmpty()) {
            QLabel* description = new QLabel(desc);
            description->setWordWrap(true);
            description->setStyleSheet("font-size: 10pt; color: #7f8c8d; margin-top: 5px;");
            vbox->addWidget(description);
        }

        // Button chọn
        QPushButton* selectBtn = new QPushButton("Chọn bác sĩ này");
        selectBtn->setCheckable(true);
        selectBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 5px;
                font-weight: bold;
                margin-top: 10px;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QPushButton:checked {
                background-color: #2196F3;
            }
        )");

        // Lưu doctor ID vào property của button
        selectBtn->setProperty("doctorID", doctor.getID());

        vbox->addWidget(name);
        vbox->addWidget(spec);
        vbox->addWidget(room);
        vbox->addWidget(selectBtn);

        doctorButtonGroup->addButton(selectBtn, doctor.getID());
        layout->addWidget(card);

        // SỬA LỖI: Sử dụng doctor object từ capture list
        connect(selectBtn, &QPushButton::clicked, this, [this, doctor]() {  // Capture doctor object
            selectedDoctorID = doctor.getID();  // Lấy ID trực tiếp
            qDebug() << "[DEBUG] Selected doctor ID:" << selectedDoctorID;

            // Chuyển sang trang chọn ngày giờ
            ui->stackedWidget->setCurrentIndex(2);

            // Khởi tạo calendar và time slots
            updateAvailableCalendarDaysAndTimeSlots(selectedDoctorID);
        });
    }

    layout->addStretch(); // Đẩy các card lên trên

    qDebug() << "[DEBUG] Total doctors displayed:" << doctorCount;

    if (doctorCount == 0) {
        QLabel* noDataLabel = new QLabel("Không có bác sĩ nào khả dụng với chuyên khoa này.");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setStyleSheet("font-size: 12pt; color: #e74c3c; padding: 50px;");
        layout->addWidget(noDataLabel);
    }
}

Appointment AddAppointmentDialog::getAppointmentData() const {
     qDebug() << "[DEBUG][getAppointmentData] Entered function.";
    
    // Get data from UI
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    QDate qDate = page2 ? page2->findChild<QCalendarWidget*>("calendarWidget")->selectedDate() : QDate::currentDate();
    Date apptDate(qDate.day(), qDate.month(), qDate.year());
    QString qStartTime = selectedTimeSlot.section(" - ", 0, 0);
    QString qEndTime = selectedTimeSlot.section(" - ", 1, 1);
    
    // Get room from doctor
    std::string room = "";
    try {
        Doctor doctor = DoctorManager::getInstance().getDoctorByID(selectedDoctorID);
        room = doctor.getRoom();
    } catch (const std::exception& e) {
        room = "Chưa xác định";
    }
    
    // Get status
    std::string statusStr = selectedStatus.toStdString();
    if (statusStr.empty()) {
        statusStr = "Scheduled";
    }
    
    // Create appointment WITHOUT auto-generated ID
    Appointment appt(selectedDoctorID, selectedPatientID,
                     apptDate.toString(),
                     qStartTime.toStdString(),
                     qEndTime.toStdString(),
                     room,
                     statusStr);
    
    if (editMode && appointmentID > 0) {
        // ✅ EDIT MODE: Reuse existing ID
        appt.setID(appointmentID);
        qDebug() << "[EDIT MODE] Reusing appointment ID:" << appointmentID;
    } else {
        // ✅ ADD MODE: Generate new ID
        int newID = static_cast<int>(IDHandler<Appointment>::generateID());
        appt.setID(newID);
        IDHandler<Appointment>::registerID(newID);
        qDebug() << "[ADD MODE] Generated new appointment ID:" << newID;
    }
    
    return appt;
}

void AddAppointmentDialog::on_searchButton_clicked() {
    QString Qcccd = ui->txtCCCD->text().trimmed();
    std::string cccd = Qcccd.toStdString();

    if (cccd.empty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập CCCD!");
        return;
    }

    if (isPatientExisted(cccd)) {
        selectedPatientID = PatientManager::getInstance().getPatientByCCCD(cccd).getID();
        isNewPatient = false; // Bệnh nhân đã tồn tại
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        // Hỏi người dùng có muốn tạo bệnh nhân mới không
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Bệnh nhân không tồn tại",
                                      "Không tìm thấy bệnh nhân với CCCD này.\nBạn có muốn tạo bệnh nhân mới?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            AddEditPatientDialog dlg(this);
            if (dlg.exec() == QDialog::Accepted) {
                Patient newPatient = dlg.getPatientData();
                try {
                    // Kiểm tra xem đã tồn tại chưa
                    PatientManager::getInstance().getPatientByID(newPatient.getID());
                    QMessageBox::warning(this, "Lỗi", "Bệnh nhân đã tồn tại!");
                    return;
                } catch (...) {
                    // Thêm bệnh nhân mới
                    PatientManager::getInstance().addPatient(newPatient);
                    selectedPatientID = newPatient.getID();
                    isNewPatient = true; // Đánh dấu là bệnh nhân mới

                    // Cập nhật CCCD field
                    bool oldBlock = ui->txtCCCD->blockSignals(true);
                    ui->txtCCCD->setText(QString::fromStdString(newPatient.getCCCD()));
                    ui->txtCCCD->blockSignals(oldBlock);

                    ui->stackedWidget->setCurrentIndex(1);
                }
            }
        }
    }
}

void AddAppointmentDialog::on_cancelButton_clicked() {
    // Chỉ xóa bệnh nhân nếu là bệnh nhân mới tạo trong phiên này
    if (isNewPatient && selectedPatientID > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Xác nhận",
                                      "Bạn có chắc muốn hủy và xóa bệnh nhân mới vừa tạo?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            try {
                PatientManager::getInstance().removePatient(selectedPatientID);
                IDHandler<Patient>::unregisterID(selectedPatientID);
            } catch (...) {
                // Bỏ qua nếu không xóa được
            }
        }
    }
    reject();
}

void AddAppointmentDialog::on_confirmButton_clicked() {
    // Basic checks
    if (selectedDoctorID < 0) {
        QMessageBox::warning(this, "Chưa chọn bác sĩ", "Vui lòng chọn bác sĩ!");
        return;
    }

    // Verify doctor exists
    try {
        DoctorManager::getInstance().getDoctorByID(selectedDoctorID);
        qDebug() << "[DEBUG] Doctor ID" << selectedDoctorID << "exists and is valid.";
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi",
                              QString("Không tìm thấy bác sĩ với ID: %1\nLỗi: %2").arg(selectedDoctorID).arg(e.what()));
        return;
    }

    if (selectedPatientID < 0) {
        QMessageBox::warning(this, "Chưa có bệnh nhân", "Vui lòng tìm kiếm bệnh nhân!");
        return;
    }

    // Get UI elements
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    if (!page2) {
        QMessageBox::warning(this, "Lỗi", "Không thể truy cập trang chọn ngày!");
        return;
    }

    QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
    QListWidget* listTimeSlots = page2->findChild<QListWidget*>("listTimeSlots");

    if (!calendar || !listTimeSlots) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy calendar hoặc danh sách khung giờ!");
        return;
    }

    QDate selectedDate = calendar->selectedDate();
    if (!selectedDate.isValid()) {
        QMessageBox::warning(this, "Chưa chọn ngày", "Vui lòng chọn ngày hẹn!");
        return;
    }

    if (selectedTimeSlot.isEmpty() || listTimeSlots->currentRow() < 0) {
        QMessageBox::warning(this, "Chưa chọn giờ", "Vui lòng chọn khung giờ hẹn!");
        return;
    }

    // ✅ CHECK 1: Verify time slot is enabled (not already booked)
    QListWidgetItem* currentItem = listTimeSlots->currentItem();
    if (!currentItem || !(currentItem->flags() & Qt::ItemIsEnabled)) {
        QMessageBox::warning(this, "Khung giờ đã đặt", 
            "Khung giờ này đã được đặt. Vui lòng chọn khung giờ khác!");
        return;
    }

    // Parse selected time
    std::string selectedDateStr = selectedDate.toString("dd-MM-yyyy").toStdString();
    std::string selectedStart = selectedTimeSlot.section(" - ", 0, 0).toStdString();
    std::string selectedEnd = selectedTimeSlot.section(" - ", 1, 1).toStdString();

    const auto& allAppointments = AppointmentManager::getInstance().getAllAppointmentsMap();

    // ✅ CHECK 2: Verify DOCTOR is available (no double-booking)
    for (const auto& pair : allAppointments) {
        const Appointment& apt = pair.second;
        
        // Skip if it's the same appointment we're editing
        if (editMode && apt.getID() == appointmentID) {
            continue;
        }
        
        // Check if DOCTOR is already booked at this time
        if (apt.getDoctorID() == selectedDoctorID &&
            apt.getDate().toString() == selectedDateStr) {
            
            // Check time overlap
            if (!(apt.getEndTime() <= selectedStart || apt.getStartTime() >= selectedEnd)) {
                try {
                    Patient conflictingPatient = PatientManager::getInstance().getPatientByID(apt.getPatientID());
                    QMessageBox::warning(this, "Bác sĩ đã có lịch hẹn",
                                         QString("Bác sĩ đã có cuộc hẹn với bệnh nhân khác:\n"
                                                 "Bệnh nhân: %1 (ID: %2)\n"
                                                 "Thời gian: %3 - %4\n"
                                                 "Trạng thái: %5\n\n"
                                                 "Vui lòng chọn khung giờ khác!")
                                             .arg(QString::fromStdString(conflictingPatient.getName()))
                                             .arg(apt.getPatientID())
                                             .arg(QString::fromStdString(apt.getStartTime()))
                                             .arg(QString::fromStdString(apt.getEndTime()))
                                             .arg(QString::fromStdString(Appointment::statusToString(apt.getStatus()))));
                } catch (...) {
                    QMessageBox::warning(this, "Bác sĩ đã có lịch hẹn",
                                         QString("Bác sĩ đã có cuộc hẹn khác:\n"
                                                 "Thời gian: %1 - %2\n\n"
                                                 "Vui lòng chọn khung giờ khác!")
                                             .arg(QString::fromStdString(apt.getStartTime()))
                                             .arg(QString::fromStdString(apt.getEndTime())));
                }
                return;
            }
        }
    }

    // ✅ CHECK 3: Verify PATIENT doesn't have conflicting appointments
    for (const auto& pair : allAppointments) {
        const Appointment& apt = pair.second;
        
        // Skip if it's the same appointment we're editing
        if (editMode && apt.getID() == appointmentID) {
            continue;
        }
        
        if (apt.getPatientID() == selectedPatientID &&
            apt.getDate().toString() == selectedDateStr) {

            // Check time overlap
            if (!(apt.getEndTime() <= selectedStart || apt.getStartTime() >= selectedEnd)) {
                try {
                    Doctor conflictingDoctor = DoctorManager::getInstance().getDoctorByID(apt.getDoctorID());
                    QMessageBox::warning(this, "Trùng lịch hẹn",
                                         QString("Bệnh nhân đã có cuộc hẹn khác:\n"
                                                 "Bác sĩ: %1 (ID: %2)\n"
                                                 "Thời gian: %3 - %4\n"
                                                 "Trạng thái: %5")
                                             .arg(QString::fromStdString(conflictingDoctor.getName()))
                                             .arg(apt.getDoctorID())
                                             .arg(QString::fromStdString(apt.getStartTime()))
                                             .arg(QString::fromStdString(apt.getEndTime()))
                                             .arg(QString::fromStdString(Appointment::statusToString(apt.getStatus()))));
                } catch (...) {
                    QMessageBox::warning(this, "Trùng lịch hẹn",
                                         QString("Bệnh nhân đã có cuộc hẹn khác:\n"
                                                 "Thời gian: %1 - %2")
                                             .arg(QString::fromStdString(apt.getStartTime()))
                                             .arg(QString::fromStdString(apt.getEndTime())));
                }
                return;
            }
        }
    }

    // All checks passed
    accept();
}

void AddAppointmentDialog::on_cmbStatus_currentTextChanged(const QString &text) {
    selectedStatus = text;
}

void AddAppointmentDialog::setupConnections() {
    // Connect backToPage0Button
    QPushButton* backToPage0Button = ui->stackedWidget->findChild<QPushButton*>("backToPage0Button");
    if (backToPage0Button) {
        connect(backToPage0Button, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
    }

    // Connect backToPage1Button
    QPushButton* backToPage1Button = ui->stackedWidget->findChild<QPushButton*>("backToPage1Button");
    if (backToPage1Button) {
        connect(backToPage1Button, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentIndex(1);
        });
    }

    // Connect backToPage2Button
    QPushButton* backToPage2Button = ui->stackedWidget->findChild<QPushButton*>("backToPage2button");
    if (backToPage2Button) {
        connect(backToPage2Button, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentIndex(2);
        });
    }

    // Connect nextToPage3Button
    QPushButton* nextToPage3Button = ui->stackedWidget->findChild<QPushButton*>("nextToPage3Button");
    if (nextToPage3Button) {
        connect(nextToPage3Button, &QPushButton::clicked, this, [this]() {
            updateConfirmationPage();
            ui->stackedWidget->setCurrentIndex(3);
        });
    }
}

// Hàm set title
void AddAppointmentDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

// Hàm set edit mode
void AddAppointmentDialog::setEditMode(bool isEdit) {
    editMode = isEdit;
}

void AddAppointmentDialog::setupSpecializationComboBox() {
    QWidget* page1 = ui->stackedWidget->findChild<QWidget*>("page_1");
    if (!page1) return;

    QComboBox* cmbSpecialization = page1->findChild<QComboBox*>("cmbSpecialization");
    if (!cmbSpecialization) return;

    // Lấy danh sách chuyên khoa từ tất cả bác sĩ
    QSet<QString> specializations;
    auto& doctorManager = DoctorManager::getInstance();

    for (const auto& pair : doctorManager.getAllDoctors()) {
        const Doctor& doctor = pair.second;
        if (doctor.getStatus() == Doctor::Status::Active) {
            specializations.insert(QString::fromStdString(doctor.getSpecialization()));
        }
    }

    // Khởi tạo combobox
    cmbSpecialization->clear();
    cmbSpecialization->addItem("-- Tất cả chuyên khoa --"); // Option mặc định

    QStringList sortedSpecializations = specializations.values();
    sortedSpecializations.sort();
    cmbSpecialization->addItems(sortedSpecializations);

    // Kết nối signal để cập nhật danh sách bác sĩ khi chọn chuyên khoa khác
    disconnect(cmbSpecialization, nullptr, this, nullptr); // Xóa connection cũ
    connect(cmbSpecialization, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddAppointmentDialog::populateDoctorCards);
}

void AddAppointmentDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddAppointmentDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AddAppointmentDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}

void AddAppointmentDialog::updateConfirmationPage() {
    QWidget* page3 = ui->stackedWidget->findChild<QWidget*>("page_3");
    if (!page3) return;

    QLabel* lblPatientName = page3->findChild<QLabel*>("lblPatientName");
    QLabel* lblDoctorName = page3->findChild<QLabel*>("lblDoctorName");
    QLabel* lblDate = page3->findChild<QLabel*>("lblDate");
    QLabel* lblTime = page3->findChild<QLabel*>("lblTime");
    QLabel* lblStatus = page3->findChild<QLabel*>("lblStatus");

    if (lblPatientName) {
        try {
            Patient patient = PatientManager::getInstance().getPatientByID(selectedPatientID);
            lblPatientName->setText(QString::fromStdString(patient.getName() + " (ID: " + std::to_string(selectedPatientID) + ")"));
        } catch (...) {
            lblPatientName->setText("Không xác định");
        }
    }

    if (lblDoctorName) {
        try {
            Doctor doctor = DoctorManager::getInstance().getDoctorByID(selectedDoctorID);
            lblDoctorName->setText(QString::fromStdString(doctor.getName() + " - " + doctor.getSpecialization()));
        } catch (...) {
            lblDoctorName->setText("Không xác định");
        }
    }

    if (lblDate) {
        QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
        if (page2) {
            QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
            if (calendar) {
                lblDate->setText(calendar->selectedDate().toString("dd/MM/yyyy"));
            }
        }
    }

    if (lblTime) {
        lblTime->setText(selectedTimeSlot.isEmpty() ? "Chưa chọn" : selectedTimeSlot);
    }

    if (lblStatus) {
        lblStatus->setText(selectedStatus.isEmpty() ? "Scheduled" : selectedStatus);
    }
}

