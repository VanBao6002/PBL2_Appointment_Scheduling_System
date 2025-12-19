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
#include <QButtonGroup>
#include <QCalendarWidget>
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

    // Connect searchButton
    QPushButton* searchButton = ui->stackedWidget->findChild<QPushButton*>("searchButton");
    if (searchButton) {
        connect(searchButton, &QPushButton::clicked, this, &AddAppointmentDialog::on_searchButton_clicked);
    }

    // Connect backToPage0Button (on page_1)
    QPushButton* backToPage0Button = ui->stackedWidget->findChild<QPushButton*>("backToPage0Button");
    if (backToPage0Button) {
        connect(backToPage0Button, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentIndex(0);
        });
    }

    // Connect backToPage1button (on page_2)
    QPushButton* backToPage1button = ui->stackedWidget->findChild<QPushButton*>("backToPage1button");
    if (backToPage1button) {
        connect(backToPage1button, &QPushButton::clicked, this, [this]() {
            ui->stackedWidget->setCurrentIndex(1);
        });
    }

    // Connect cancelButton (on dialog)
    QPushButton* cancelButton = this->findChild<QPushButton*>("cancelButton");
    if (cancelButton) {
        connect(cancelButton, &QPushButton::clicked, this, &AddAppointmentDialog::reject);
    }

    // --- Specialization ComboBox setup ---
    QWidget* page1 = ui->stackedWidget->findChild<QWidget*>("page_1");
    QComboBox* cmbSpecialization = page1 ? page1->findChild<QComboBox*>("cmbSpecialization") : nullptr;
    if (cmbSpecialization) {
        cmbSpecialization->clear();
        QSet<QString> specializations;
        auto& doctorManager = DoctorManager::getInstance();
        for (const auto& pair : doctorManager.getAllDoctors()) {
            specializations.insert(QString::fromStdString(pair.second.getSpecialization()));
        }
        cmbSpecialization->addItems(specializations.values());
        connect(cmbSpecialization, &QComboBox::currentTextChanged, this, [this]() {
            populateDoctorCards();
        });
        if (cmbSpecialization->count() > 0)
            cmbSpecialization->setCurrentIndex(0);
    }
    populateDoctorCards();

    ui->stackedWidget->setCurrentIndex(0);
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
    cmbStatus->addItem("Occupied");
    cmbStatus->addItem("Scheduled");
    cmbStatus->addItem("Cancelled");
}

void AddAppointmentDialog::updateAvailableTimeSlot(const QStringList& inputSlots, const QSet<QString>& occupiedSlots) {
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    if (!page2) return;

    QCalendarWidget* calendar = page2->findChild<QCalendarWidget*>("calendarWidget");
    if (!calendar) return;

    QDate selectedDate = calendar->selectedDate();
    if (!selectedDate.isValid() || selectedDoctorID < 0) return;

    Doctor doctor;
    try {
        doctor = DoctorManager::getInstance().getDoctorByID(selectedDoctorID);
    } catch (...) {
        return;
    }

    // Get weekday name
    QString weekdayName = QLocale().dayName(selectedDate.dayOfWeek(), QLocale::LongFormat);

    // Get booked slots for this doctor and date (implement this as needed)
    std::vector<std::pair<std::string, std::string>> bookedSlots;
    // Example: bookedSlots = AppointmentManager::getInstance().getBookedSlotsForDoctorDate(selectedDoctorID, selectedDate);

    // Get all intervals for the day
    auto intervals = doctor.getWorkingSchedule().getIntervalsWithBooking(
        weekdayName.toStdString(), bookedSlots
    );

    // Prepare available slots
    QStringList allSlots;
    for (const auto& [start, end, isBooked] : intervals) {
        if (!isBooked)
            allSlots.append(QString::fromStdString(start + " - " + end));
    }

    // Update your time slot selection widget here
    // Example: ui->cmbTimeSlots->clear(); ui->cmbTimeSlots->addItems(allSlots);
    // Replace with your actual widget update logic
    QComboBox* cmbTimeSlots = page2->findChild<QComboBox*>("cmbTimeSlots");
    if (cmbTimeSlots) {
        cmbTimeSlots->clear();
        cmbTimeSlots->addItems(allSlots);
    }
}

void AddAppointmentDialog::updateAvailableCalendarDaysAndTimeSlots(int doctorID) {
    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    QCalendarWidget* calendar = page2 ? page2->findChild<QCalendarWidget*>("calendarWidget") : nullptr;
    if (!calendar) return;

    Doctor doctor;
    try {
        doctor = DoctorManager::getInstance().getDoctorByID(doctorID);
    } catch (...) {
        return;
    }

    QSet<QDate> workingDays;
    const auto& daysVec = doctor.getWorkingSchedule().getDays();
    QDate today = QDate::currentDate();

    for (int i = 0; i < Config::MAGIC_MARKED_AVAILABLE_WORKING_DAYS_NUMBER; ++i) {
        QDate date = today.addDays(i);
        QString weekdayName = QLocale().dayName(date.dayOfWeek(), QLocale::LongFormat);
        for (const auto& dayStr : daysVec) {
            if (weekdayName.compare(QString::fromStdString(dayStr), Qt::CaseInsensitive) == 0) {
                workingDays.insert(date);
                break;
            }
        }
    }

    // Mark working days on calendar (using QTextCharFormat)
    QTextCharFormat fmt;
    fmt.setBackground(Qt::green);
    for (const QDate& d : workingDays) {
        calendar->setDateTextFormat(d, fmt);
    }

    // Disconnect previous connections to avoid duplicate slots
    disconnect(calendar, nullptr, nullptr, nullptr);

    // Connect calendar selection to update time slots
    connect(calendar, &QCalendarWidget::selectionChanged, this, [this, doctorID, calendar, daysVec]() {
        QDate selected = calendar->selectedDate();
        QString weekdayName = QLocale().dayName(selected.dayOfWeek(), QLocale::LongFormat);

        // Check if selected day is a working day
        bool isWorkingDay = false;
        for (const auto& dayStr : daysVec) {
            if (weekdayName.compare(QString::fromStdString(dayStr), Qt::CaseInsensitive) == 0) {
                isWorkingDay = true;
                break;
            }
        }
        if (!isWorkingDay) {
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

        // Get booked slots for this doctor and date (implement as needed)
        std::vector<std::pair<std::string, std::string>> bookedSlots;
        // Example: bookedSlots = AppointmentManager::getInstance().getBookedSlotsForDoctorDate(doctorID, selected);

        // Get all intervals for the day
        auto intervals = doctor.getWorkingSchedule().getIntervalsWithBooking(
            weekdayName.toStdString(), bookedSlots
        );

        // Prepare available slots
        QStringList allSlots;
        for (const auto& [start, end, isBooked] : intervals) {
            if (!isBooked)
                allSlots.append(QString::fromStdString(start + " - " + end));
        }

        updateAvailableTimeSlot(allSlots, QSet<QString>());
    });

    // Initial trigger for current selection
    calendar->selectionChanged();
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
    if (!doctorListWidget) return;

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
    for (const auto& pair : doctorManager.getAllDoctors()) {
        const Doctor& doctor = pair.second;
        if (doctor.getStatus() != Doctor::Status::Active ||
            QString::fromStdString(doctor.getSpecialization()) != selectedSpecialization) {
            continue;
        }

        QWidget* card = new QWidget;
        QVBoxLayout* vbox = new QVBoxLayout(card);

        QLabel* name = new QLabel(QString::fromStdString(doctor.getName()));
        QLabel* desc = new QLabel(QString::fromStdString(doctor.getDescription())); 
        QPushButton* selectBtn = new QPushButton("Select");
        selectBtn->setCheckable(true);

        vbox->addWidget(name);
        vbox->addWidget(desc);
        vbox->addWidget(selectBtn);

        doctorButtonGroup->addButton(selectBtn, doctor.getID());
        layout->addWidget(card);

        connect(selectBtn, &QPushButton::clicked, this, [this, selectBtn]() {
            selectedDoctorID = doctorButtonGroup->id(selectBtn);
            // Move to the next page (e.g., time slot selection)
            ui->stackedWidget->setCurrentIndex(2);
        });
    }
}

Appointment AddAppointmentDialog::getAppointmentData() const {
    int doctorID = selectedDoctorID;
    int patientID = selectedPatient.getID();

    QWidget* page2 = ui->stackedWidget->findChild<QWidget*>("page_2");
    QDate qDate = page2 ? page2->findChild<QCalendarWidget*>("calendarWidget")->selectedDate() : QDate();
    Date apptDate(qDate.day(), qDate.month(), qDate.year());
    QString qStartTime = selectedTimeSlot.section(" - ", 0, 0);
    QString qEndTime = selectedTimeSlot.section(" - ", 1, 1);
    std::string room = DoctorManager::getInstance().getDoctorByID(selectedDoctorID).getRoom();
    std::string statusStr = ui->cmbStatus->currentText().toStdString();
    Appointment::Status status = Appointment::statusFromString(statusStr);
    return Appointment(doctorID, patientID, apptDate.toString(),
                      qStartTime.toStdString(), qEndTime.toStdString(),
                      room, statusStr);
}

void AddAppointmentDialog::on_searchButton_clicked() {
    QString Qcccd = ui->txtCCCD->text().trimmed();
    std::string cccd = Qcccd.toStdString();
    if (isPatientExisted(cccd)) {
        selectedPatient = PatientManager::getInstance().getPatientByCCCD(cccd);
        // Set patient data to the dialog fields as needed
        // ...
        ui->stackedWidget->setCurrentIndex(1); // Go to page 1
    } else {    
        AddEditPatientDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            selectedPatient = dlg.getPatientData();
            ui->stackedWidget->setCurrentIndex(1); // Go to page 1
        }
    }
}


