#include "addeditdoctordialog.h"
#include "gui/ui_addeditdoctordialog.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    loadDoctorData(doctorToEdit);

    qDebug() << "[DIALOG] AddEditDoctorDialog opened in EDIT mode for ID:" << editingDoctorID;
}

AddEditDoctorDialog::~AddEditDoctorDialog()
{
    delete ui;
}

// ============================================
// LOAD SPECIALIZATIONS TỪ FILE JSON
// ============================================
QStringList AddEditDoctorDialog::loadSpecializationsFromJson() {
    QStringList specializations;

    QString filePath = "data/doctorSpecializations.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[WARNING] Cannot open specializations file:" << filePath;
        specializations << "Cardiology" << "Neurology" << "Pediatrics"
                        << "Orthopedics" << "Dermatology";
        return specializations;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "[WARNING] Invalid JSON format in specializations file";
        specializations << "Cardiology" << "Neurology" << "Pediatrics";
        return specializations;
    }

    QJsonObject obj = doc.object();
    if (obj.contains("specializations") && obj["specializations"].isArray()) {
        QJsonArray arr = obj["specializations"].toArray();
        for (const QJsonValue& val : arr) {
            if (val.isString()) {
                specializations << val.toString();
            }
        }
    }

    qDebug() << "[DIALOG] Loaded" << specializations.size() << "specializations from JSON";
    return specializations;
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
    QStringList specializations = loadSpecializationsFromJson();
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

    // ✅ Dùng txtPhoneNumber và txtEmail
    std::string phoneNumber = ui->txtPhoneNumber->text().trimmed().toStdString();
    std::string email = ui->txtEmail->text().trimmed().toStdString();
    std::string specialization = ui->cmbSpecialization->currentText().trimmed().toStdString();
    std::string status = ui->cmbStatus->currentData().toString().toStdString();

    if (isEditMode) {
        Doctor doctor(name, gender, birthday, phoneNumber, email, specialization, status);
        qDebug() << "[DIALOG] getDoctorData() - EDIT mode, original ID:" << editingDoctorID;
        return doctor;
    } else {
        Doctor doctor(name, gender, birthday, phoneNumber, email, specialization, status);
        qDebug() << "[DIALOG] getDoctorData() - ADD mode, new ID:" << doctor.getID();
        return doctor;
    }
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
