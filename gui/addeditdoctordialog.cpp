#include "addeditdoctordialog.h"
#include "gui/ui_addeditdoctordialog.h"
#include "doctor.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QInputDialog>

// ============================================
// CONSTRUCTOR - THÊM MỚI
// ============================================
AddEditDoctorDialog::AddEditDoctorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditDoctorDialog),
    isEditMode(false),
    editingDoctorID(-1)
{
    ui->setupUi(this);
    setWindowTitle("Thêm Bác Sĩ Mới");

    setupComboBoxes();
    setupValidators();
    setupWorkingScheduleTable();

    // Connect working schedule buttons
    connect(ui->pushButton_3, &QPushButton::clicked, this, &AddEditDoctorDialog::onAddWorkingScheduleClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &AddEditDoctorDialog::onDeleteWorkingScheduleClicked);

    qDebug() << "[DIALOG] AddEditDoctorDialog opened in ADD mode";
}


// ============================================
// CONSTRUCTOR - CHỈNH SỬA
// ============================================
AddEditDoctorDialog::AddEditDoctorDialog(QWidget *parent, const Doctor& doctorToEdit) :
    QDialog(parent),
    ui(new Ui::AddEditDoctorDialog),
    isEditMode(true),
    editingDoctorID(doctorToEdit.getID())
{
    ui->setupUi(this);
    setWindowTitle("Chỉnh Sửa Thông Tin Bác Sĩ");

    setupComboBoxes();
    setupValidators();
    setupWorkingScheduleTable();
    loadDoctorData(doctorToEdit);

    // Connect working schedule buttons
    connect(ui->pushButton_3, &QPushButton::clicked, this, &AddEditDoctorDialog::onAddWorkingScheduleClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &AddEditDoctorDialog::onDeleteWorkingScheduleClicked);

    qDebug() << "[DIALOG] AddEditDoctorDialog opened in EDIT mode for ID:" << editingDoctorID;
}
// ============================================
// WORKING SCHEDULE BUTTON SLOTS
// ============================================
void AddEditDoctorDialog::onAddWorkingScheduleClicked() {
    int row = ui->tableWorkingSchedule->rowCount();
    ui->tableWorkingSchedule->insertRow(row);

    // Day combo box with placeholder
    QComboBox* dayCombo = new QComboBox();
    dayCombo->setMinimumWidth(120);
    dayCombo->addItem("Chọn ngày...");
    dayCombo->addItems({"Thứ 2", "Thứ 3", "Thứ 4", "Thứ 5", "Thứ 6", "Thứ 7", "Chủ nhật"});
    dayCombo->setCurrentIndex(0);
    ui->tableWorkingSchedule->setCellWidget(row, 0, dayCombo);


    // Use QTimeEdit for time selection (picker wheel style)
    QTimeEdit* beginTimeEdit = new QTimeEdit();
    beginTimeEdit->setDisplayFormat("HH:mm");
    beginTimeEdit->setTime(QTime(8, 0)); // default start time
    beginTimeEdit->setMinimumWidth(100);
    beginTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    beginTimeEdit->setAlignment(Qt::AlignCenter);
    beginTimeEdit->setKeyboardTracking(false);
    beginTimeEdit->setWrapping(true);
    // Snap to 30-minute intervals
    QObject::connect(beginTimeEdit, &QTimeEdit::timeChanged, beginTimeEdit, [beginTimeEdit](const QTime &t) {
        int minutes = t.minute();
        int snapped = (minutes < 15) ? 0 : (minutes < 45 ? 30 : 0);
        int hour = t.hour();
        if (minutes >= 45) hour = (hour + 1) % 24;
        QTime snappedTime(hour, snapped);
        if (t != snappedTime) beginTimeEdit->setTime(snappedTime);
    });
    ui->tableWorkingSchedule->setCellWidget(row, 1, beginTimeEdit);

    QTimeEdit* endTimeEdit = new QTimeEdit();
    endTimeEdit->setDisplayFormat("HH:mm");
    endTimeEdit->setTime(QTime(17, 0)); // default end time
    endTimeEdit->setMinimumWidth(100);
    endTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    endTimeEdit->setAlignment(Qt::AlignCenter);
    endTimeEdit->setKeyboardTracking(false);
    endTimeEdit->setWrapping(true);
    // Snap to 30-minute intervals
    QObject::connect(endTimeEdit, &QTimeEdit::timeChanged, endTimeEdit, [endTimeEdit](const QTime &t) {
        int minutes = t.minute();
        int snapped = (minutes < 15) ? 0 : (minutes < 45 ? 30 : 0);
        int hour = t.hour();
        if (minutes >= 45) hour = (hour + 1) % 24;
        QTime snappedTime(hour, snapped);
        if (t != snappedTime) endTimeEdit->setTime(snappedTime);
    });
    ui->tableWorkingSchedule->setCellWidget(row, 2, endTimeEdit);

    // Make sure the table is not editable by typing
    ui->tableWorkingSchedule->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Visually highlight/select the new row
    ui->tableWorkingSchedule->selectRow(row);
    dayCombo->setFocus();
}

void AddEditDoctorDialog::onDeleteWorkingScheduleClicked() {
    auto selected = ui->tableWorkingSchedule->selectionModel()->selectedRows();
    for (const QModelIndex &index : selected) {
        ui->tableWorkingSchedule->removeRow(index.row());
    }
}

// ============================================
// SETUP WORKING SCHEDULE TABLE
// ============================================
void AddEditDoctorDialog::setupWorkingScheduleTable() {
    ui->tableWorkingSchedule->setColumnCount(3);
    QStringList headers;
    headers << "Ngày" << "Giờ bắt đầu" << "Giờ kết thúc";
    ui->tableWorkingSchedule->setHorizontalHeaderLabels(headers);
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWorkingSchedule->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWorkingSchedule->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// ============================================
// LOAD WORKING SCHEDULE TO TABLE
// ============================================
void AddEditDoctorDialog::loadWorkingSchedule(const WorkingSchedule& schedule) {
    ui->tableWorkingSchedule->setRowCount(0);
    int row = 0;

    for (auto it = schedule.schedule.begin(); it != schedule.schedule.end(); ++it) {
        const std::string& day = it->first;
        const std::vector<std::pair<std::string, std::string>>& timeSlots = it->second;
        for (size_t i = 0; i < timeSlots.size(); ++i) {
            const std::pair<std::string, std::string>& slot = timeSlots[i];
            ui->tableWorkingSchedule->insertRow(row);
            ui->tableWorkingSchedule->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(day)));
            ui->tableWorkingSchedule->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(slot.first)));
            ui->tableWorkingSchedule->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(slot.second)));
            ++row;
        }
    }
}


// ============================================
// EXTRACT WORKING SCHEDULE FROM TABLE
// ============================================
WorkingSchedule AddEditDoctorDialog::getWorkingScheduleFromTable() const {
    WorkingSchedule schedule;
    int rowCount = ui->tableWorkingSchedule->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        // Day
        QString day;
        QWidget* dayWidget = ui->tableWorkingSchedule->cellWidget(row, 0);
        if (auto combo = qobject_cast<QComboBox*>(dayWidget)) {
            day = combo->currentText();
        } else if (ui->tableWorkingSchedule->item(row, 0)) {
            day = ui->tableWorkingSchedule->item(row, 0)->text();
        }

        // Begin time
        QString start;
        QWidget* beginWidget = ui->tableWorkingSchedule->cellWidget(row, 1);
        if (auto timeEdit = qobject_cast<QTimeEdit*>(beginWidget)) {
            start = timeEdit->time().toString("HH:mm");
        } else if (ui->tableWorkingSchedule->item(row, 1)) {
            start = ui->tableWorkingSchedule->item(row, 1)->text();
        }

        // End time
        QString end;
        QWidget* endWidget = ui->tableWorkingSchedule->cellWidget(row, 2);
        if (auto timeEdit = qobject_cast<QTimeEdit*>(endWidget)) {
            end = timeEdit->time().toString("HH:mm");
        } else if (ui->tableWorkingSchedule->item(row, 2)) {
            end = ui->tableWorkingSchedule->item(row, 2)->text();
        }

        if (!day.isEmpty() && !start.isEmpty() && !end.isEmpty() && day != "Chọn ngày..." && start != "Chọn giờ bắt đầu..." && end != "Chọn giờ kết thúc...") {
            schedule.schedule[day.toStdString()].emplace_back(start.toStdString(), end.toStdString());
        }
    }
    return schedule;

}

AddEditDoctorDialog::~AddEditDoctorDialog()
{
    delete ui;
}


// ============================================
// SETUP COMBOBOXES
// ============================================
void AddEditDoctorDialog::setupComboBoxes() {
    // ComboBox Giới tính
    ui->cmbGender->clear();
    ui->cmbGender->addItem("Nam", QChar('M'));
    ui->cmbGender->addItem("Nữ", QChar('F'));
    ui->cmbGender->addItem("Khác", QChar('O'));

    // ComboBox Chuyên khoa - Load từ JSON
    ui->cmbSpecialization->clear();
    auto specs = Core::loadSpecializations();
    QStringList specializations;
    for (const auto& s : specs) {
        specializations << QString::fromStdString(s);
    }
    for (const QString& spec : specializations) {
        ui->cmbSpecialization->addItem(spec);
    }
    ui->cmbSpecialization->setEditable(true);

    // ComboBox Trạng thái
    ui->cmbStatus->clear();
    ui->cmbStatus->addItem("Đang làm việc", "Active");
    ui->cmbStatus->addItem("Đang nghỉ phép", "OnLeave");
    ui->cmbStatus->addItem("Đã nghỉ hưu", "Retired");

    // Setup DateEdit cho ngày sinh
    ui->dateBirthday->setDisplayFormat("dd/MM/yyyy");
    ui->dateBirthday->setCalendarPopup(true);
    ui->dateBirthday->setDate(QDate(1980, 1, 1));
    ui->dateBirthday->setMaximumDate(QDate::currentDate().addYears(-22));
    ui->dateBirthday->setMinimumDate(QDate(1940, 1, 1));
}

// ============================================
// SETUP VALIDATORS
// ============================================
void AddEditDoctorDialog::setupValidators() {
    // ✅ KHÔNG dùng validator cho SĐT - chỉ dùng placeholder
    // Validator regex chặn người dùng nhập, gây khó chịu
    ui->txtPhoneNumber->setPlaceholderText("VD: 0901234567");
    ui->txtPhoneNumber->setMaxLength(11); // Giới hạn 11 ký tự

    // ✅ KHÔNG dùng validator cho Email - validate khi submit
    ui->txtEmail->setPlaceholderText("VD: bacsi@gmail.com");

    // Placeholder cho tên
    ui->txtName->setPlaceholderText("VD: Nguyễn Văn A");
}

// ============================================
// LOAD DỮ LIỆU KHI EDIT
// ============================================
void AddEditDoctorDialog::loadDoctorData(const Doctor& doctor) {
    // Họ tên
    ui->txtName->setText(QString::fromStdString(doctor.getName()));

    // Giới tính
    char gender = doctor.getGender();
    int genderIndex = 0;
    if (gender == 'M') genderIndex = 0;
    else if (gender == 'F') genderIndex = 1;
    else genderIndex = 2;
    ui->cmbGender->setCurrentIndex(genderIndex);

    // Ngày sinh
    Date birthday = doctor.getBirthday();
    ui->dateBirthday->setDate(QDate(birthday.getYear(), birthday.getMonth(), birthday.getDay()));

    // Chuyên khoa
    QString specialization = QString::fromStdString(doctor.getSpecialization());
    int specIndex = ui->cmbSpecialization->findText(specialization);
    if (specIndex >= 0) {
        ui->cmbSpecialization->setCurrentIndex(specIndex);
    } else {
        ui->cmbSpecialization->setCurrentText(specialization);
    }

    // ✅ Email - dùng txtEmail (QLineEdit)
    ui->txtEmail->setText(QString::fromStdString(doctor.getEmail()));

    // ✅ SĐT - dùng txtPhoneNumber (QLineEdit)
    ui->txtPhoneNumber->setText(QString::fromStdString(doctor.getPhoneNumber()));

    // Trạng thái
    QString statusStr = QString::fromStdString(Doctor::statusToString(doctor.getStatus()));
    int statusIndex = ui->cmbStatus->findData(statusStr);
    if (statusIndex >= 0) {
        ui->cmbStatus->setCurrentIndex(statusIndex);
    }

    // Lịch làm việc
    loadWorkingSchedule(doctor.getWorkingSchedule());

    qDebug() << "[DIALOG] Loaded doctor data for ID:" << doctor.getID();
}

// ============================================
// VALIDATE FORM
// ============================================
bool AddEditDoctorDialog::validateForm() {
    // Kiểm tra họ tên
    QString name = ui->txtName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên bác sĩ!");
        ui->txtName->setFocus();
        return false;
    }
    if (name.length() < 2) {
        QMessageBox::warning(this, "Lỗi", "Họ tên phải có ít nhất 2 ký tự!");
        ui->txtName->setFocus();
        return false;
    }

    // Kiểm tra chuyên khoa
    QString specialization = ui->cmbSpecialization->currentText().trimmed();
    if (specialization.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn hoặc nhập chuyên khoa!");
        ui->cmbSpecialization->setFocus();
        return false;
    }

    // ✅ Kiểm tra Email - dùng txtEmail
    QString email = ui->txtEmail->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập địa chỉ email!");
        ui->txtEmail->setFocus();
        return false;
    }
    // Kiểm tra định dạng email (cho phép @gmail.com và các domain khác)
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Email không đúng định dạng!\nVD: bacsi@gmail.com");
        ui->txtEmail->setFocus();
        return false;
    }

    // ✅ Kiểm tra SĐT - dùng txtPhoneNumber
    QString phone = ui->txtPhoneNumber->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại!");
        ui->txtPhoneNumber->setFocus();
        return false;
    }

    // ✅ Kiểm tra SĐT chỉ chứa số và có độ dài 10-11
    QRegularExpression phoneRegex("^[0-9]{10,11}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Số điện thoại phải có 10-11 chữ số!\nVD: 0901234567");
        ui->txtPhoneNumber->setFocus();
        return false;
    }

    // Kiểm tra ngày sinh
    QDate birthday = ui->dateBirthday->date();
    QDate today = QDate::currentDate();
    int age = today.year() - birthday.year();
    if (birthday > today.addYears(-age)) age--;

    if (age < 22) {
        QMessageBox::warning(this, "Lỗi", "Bác sĩ phải từ 22 tuổi trở lên!");
        ui->dateBirthday->setFocus();
        return false;
    }
    if (age > 100) {
        QMessageBox::warning(this, "Lỗi", "Ngày sinh không hợp lệ!");
        ui->dateBirthday->setFocus();
        return false;
    }

    return true;
}

// ============================================
// LẤY DỮ LIỆU TỪ FORM
// ============================================
Doctor AddEditDoctorDialog::getDoctorData() const {
    std::string name = ui->txtName->text().trimmed().toStdString();

    char gender = ui->cmbGender->currentData().toChar().toLatin1();

    QDate qBirthday = ui->dateBirthday->date();
    std::string birthday = QString("%1/%2/%3")
                               .arg(qBirthday.day())
                               .arg(qBirthday.month())
                               .arg(qBirthday.year())
                               .toStdString();


    std::string phoneNumber = ui->txtPhoneNumber->text().trimmed().toStdString();
    std::string CCCD = ui->txtCCCD->text().trimmed().toStdString();
    std::string email = ui->txtEmail->text().trimmed().toStdString();
    std::string specialization = ui->cmbSpecialization->currentText().trimmed().toStdString();
    std::string status = ui->cmbStatus->currentData().toString().toStdString();
    WorkingSchedule ws = getWorkingScheduleFromTable();

    Doctor doctor(name, gender, birthday, phoneNumber, CCCD, email, specialization, status, ws);
    // Debug output for working schedule
    qDebug() << "[DEBUG] WorkingSchedule for doctor:";
    for (const auto& day : ws.schedule) {
        QString dayStr = QString::fromStdString(day.first);
        for (const auto& slot : day.second) {
            QString startStr = QString::fromStdString(slot.first);
            QString endStr = QString::fromStdString(slot.second);
            qDebug() << "  " << dayStr << ":" << startStr << "-" << endStr;
        }
    }
    if (isEditMode) {
        qDebug() << "[DIALOG] getDoctorData() - EDIT mode, original ID:" << editingDoctorID;
    } else {
        qDebug() << "[DIALOG] getDoctorData() - ADD mode, new ID:" << doctor.getID();
    }
    return doctor;
}

void AddEditDoctorDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

// ============================================
// BUTTON SLOTS
// ============================================
void AddEditDoctorDialog::on_buttonBox_accepted() {
    if (validateForm()) {
        qDebug() << "[DIALOG] Form validated, accepting dialog";
        accept();
    }
}

void AddEditDoctorDialog::on_buttonBox_rejected() {
    qDebug() << "[DIALOG] Dialog rejected/cancelled";
    reject();
}
