#include "addeditpatientdialog.h"
#include "gui/ui_addeditpatientdialog.h"
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include "IDHandler.h"
#include "patient.h"
#include "utils.h"

AddEditPatientDialog::AddEditPatientDialog(QWidget *parent, const Patient& patient) :
    QDialog(parent),
    ui(new Ui::AddEditPatientDialog),
    currentPatient(patient)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setupDatePicker();
    populateUI(patient);
}

AddEditPatientDialog::~AddEditPatientDialog()
{
    delete ui;
}

void AddEditPatientDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

bool AddEditPatientDialog::validateForm() {
    // Kiểm tra họ tên
    QString name = ui->txtName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên bệnh nhân!");
        ui->txtName->setFocus();
        return false;
    }
    if (name.length() < 2) {
        QMessageBox::warning(this, "Lỗi", "Họ tên phải có ít nhất 2 ký tự!");
        ui->txtName->setFocus();
        return false;
    }

    // Kiểm tra giới tính
    QString gender = ui->comboGender->currentText().trimmed();
    if (gender.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn giới tính!");
        ui->comboGender->setFocus();
        return false;
    }

    // Kiểm tra nhóm máu
    QString bloodType = ui->txtBloodType->text().trimmed();
    if (bloodType.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập nhóm máu!");
        ui->txtBloodType->setFocus();
        return false;
    }
    QRegularExpression bloodTypeRegex("^(A|B|AB|O)[+-]$");
    if (!bloodTypeRegex.match(bloodType).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Nhóm máu không hợp lệ! VD: A+, O-, AB+");
        ui->txtBloodType->setFocus();
        return false;
    }

    // Kiểm tra ngày sinh
    QDate birthday = ui->dateEditBirthday->date();
    QDate today = QDate::currentDate();
    int age = today.year() - birthday.year();
    if (birthday > today.addYears(-age)) age--;
    if (age < 0) {
        QMessageBox::warning(this, "Lỗi", "Ngày sinh không thể trong tương lai!");
        ui->dateEditBirthday->setFocus();
        return false;
    }
    if (age > 150) {
        QMessageBox::warning(this, "Lỗi", "Ngày sinh không hợp lệ (quá 150 tuổi)!");
        ui->dateEditBirthday->setFocus();
        return false;
    }

    // Kiểm tra số điện thoại bệnh nhân
    QString phone = ui->txtPhoneNumber->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại bệnh nhân!");
        ui->txtPhoneNumber->setFocus();
        return false;
    }
    QRegularExpression phoneRegex("^[0-9]{10,11}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Số điện thoại phải có 10-11 chữ số!");
        ui->txtPhoneNumber->setFocus();
        return false;
    }

    // Kiểm tra số điện thoại mẹ (không bắt buộc)
    QString phoneMother = ui->txtPhoneMother->text().trimmed();
    if (!phoneMother.isEmpty()) {
        QRegularExpression phoneRegex("^[0-9]{10,11}$");
        if (!phoneRegex.match(phoneMother).hasMatch()) {
            QMessageBox::warning(this, "Lỗi", "Số điện thoại mẹ phải có 10-11 chữ số!");
            ui->txtPhoneMother->setFocus();
            return false;
        }
    }

    // Kiểm tra số điện thoại cha (không bắt buộc)
    QString phoneFather = ui->txtPhoneFather->text().trimmed();
    if (!phoneFather.isEmpty()) {
        QRegularExpression phoneRegex("^[0-9]{10,11}$");
        if (!phoneRegex.match(phoneFather).hasMatch()) {
            QMessageBox::warning(this, "Lỗi", "Số điện thoại cha phải có 10-11 chữ số!");
            ui->txtPhoneFather->setFocus();
            return false;
        }
    }

    // Kiểm tra email (không bắt buộc)
    QString email = ui->txtEmail->text().trimmed();
    if (!email.isEmpty()) {
        QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!emailRegex.match(email).hasMatch()) {
            QMessageBox::warning(this, "Lỗi", "Email không đúng định dạng!\nVD: benhnhan@gmail.com");
            ui->txtEmail->setFocus();
            return false;
        }
    }

    // Kiểm tra CCCD
    QString cccd = ui->txtCCCD->text().trimmed();
    if (cccd.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số CCCD!");
        ui->txtCCCD->setFocus();
        return false;
    }
    QRegularExpression cccdRegex("^[0-9]{12}$");
    if (!cccdRegex.match(cccd).hasMatch()) {
        QMessageBox::warning(this, "Lỗi", "Số CCCD phải có đúng 12 chữ số!");
        ui->txtCCCD->setFocus();
        return false;
    }

    return true;
}


void AddEditPatientDialog::populateUI(const Patient& patient) {
    if (patient.getID() > 0) {
        // Chế độ Sửa
        ui->txtName->setText(QString::fromStdString(patient.getName()));

        QString genderStr = QString(patient.getGender()).toUpper();
        int genderIndex = ui->comboGender->findText(genderStr);
        if (genderIndex != -1) {
            ui->comboGender->setCurrentIndex(genderIndex);
        }

        ui->txtBloodType->setText(QString::fromStdString(patient.getBloodType()));

        QString allergiesStr = QString::fromStdString(Utils::join(patient.getAllergies(), ", "));
        ui->txtAllergies->setText(allergiesStr);

        QString chronicStr = QString::fromStdString(Utils::join(patient.getChronicDiseases(), ", "));
        ui->txtChronicDiseases->setText(chronicStr);

        ui->txtNameMother->setText(QString::fromStdString(patient.getNameMother()));
        ui->txtNameFather->setText(QString::fromStdString(patient.getNameFather()));

        // Số điện thoại cha mẹ
        ui->txtPhoneMother->setText(QString::fromStdString(patient.getPhoneMother()));
        ui->txtPhoneFather->setText(QString::fromStdString(patient.getPhoneFather()));

        // Set insuranceID field
        ui->txtInsuranceID->setText(QString::fromStdString(patient.getInsuranceID()));

        // ✅ Cập nhật cách thiết lập ngày sinh
        QDate qDate(patient.getBirthday().getYear(), patient.getBirthday().getMonth(), patient.getBirthday().getDay());
        ui->dateEditBirthday->setDate(qDate);

        // Số điện thoại bệnh nhân
        ui->txtPhoneNumber->setText(QString::fromStdString(patient.getPhoneNumber()));

        // CCCD và Email
        ui->txtCCCD->setText(QString::fromStdString(patient.getCCCD()));
        ui->txtEmail->setText(QString::fromStdString(patient.getEmail()));

        setWindowTitle("Chỉnh Sửa Thông Tin Bệnh Nhân - ID: " + QString::number(patient.getID()));
    } else {
        // Chế độ Thêm mới
        setWindowTitle("Thêm Bệnh Nhân Mới");
        int defaultGenderIndex = ui->comboGender->findText("M");
        if (defaultGenderIndex != -1) {
            ui->comboGender->setCurrentIndex(defaultGenderIndex);
        }

        // Clear các field
        ui->txtInsuranceID->clear();
        ui->txtPhoneMother->clear();
        ui->txtPhoneFather->clear();
        ui->txtPhoneNumber->clear();
        ui->txtCCCD->clear();
        ui->txtEmail->clear();

        // ✅ Thiết lập ngày sinh mặc định
        ui->dateEditBirthday->setDate(QDate::currentDate().addYears(-10));
    }
}
Patient AddEditPatientDialog::getPatientData() const {
    // Lấy dữ liệu từ UI
    QString name = ui->txtName->text().trimmed();
    QString genderStr = ui->comboGender->currentText().trimmed();
    QDate qDate = ui->dateEditBirthday->date();
    QString bloodType = ui->txtBloodType->text().trimmed();
    QString allergies = ui->txtAllergies->text().trimmed();
    QString chronicDiseases = ui->txtChronicDiseases->text().trimmed();
    QString nameMother = ui->txtNameMother->text().trimmed();
    QString nameFather = ui->txtNameFather->text().trimmed();
    QString phoneNumber = ui->txtPhoneNumber->text().trimmed();
    QString insuranceID = ui->txtInsuranceID->text().trimmed();
    QString CCCD = ui->txtCCCD->text().trimmed();
    QString email = ui->txtEmail->text().trimmed();
    QString phoneMother = ui->txtPhoneMother->text().trimmed();
    QString phoneFather = ui->txtPhoneFather->text().trimmed();

    Date birthday(qDate.day(), qDate.month(), qDate.year());
    char gender = genderStr.isEmpty() ? 'M' : genderStr.toStdString()[0];

    if (currentPatient.getID() > 0) {
        // Chế độ SỬA
        Patient updatedPatient = currentPatient;
        updatedPatient.setName(name.toStdString());
        updatedPatient.setGender(gender);
        updatedPatient.setBirthday(birthday);
        updatedPatient.setBloodType(bloodType.toStdString());
        updatedPatient.setAllergies(allergies.toStdString());
        updatedPatient.setChronicDiseases(chronicDiseases.toStdString());
        updatedPatient.setNameMother(nameMother.toStdString());
        updatedPatient.setNameFather(nameFather.toStdString());
        updatedPatient.setPhoneNumber(phoneNumber.toStdString());
        updatedPatient.setInsuranceID(insuranceID.toStdString());
        updatedPatient.setCCCD(CCCD.toStdString());
        updatedPatient.setEmail(email.toStdString());
        updatedPatient.setPhoneMother(phoneMother.toStdString());
        updatedPatient.setPhoneFather(phoneFather.toStdString());
        return updatedPatient;
    } else {
        // Chế độ THÊM MỚI
        Patient newPatient(
            name.toStdString(),
            gender,
            QString("%1/%2/%3").arg(qDate.day()).arg(qDate.month()).arg(qDate.year()).toStdString(),
            phoneNumber.toStdString(),
            CCCD.toStdString(),
            email.toStdString(),
            insuranceID.toStdString(),
            bloodType.toStdString(),
            allergies.toStdString(),
            chronicDiseases.toStdString(),
            nameMother.toStdString(),
            nameFather.toStdString(),
            phoneMother.toStdString(),
            phoneFather.toStdString()
        );
        return newPatient;
    }
}

void AddEditPatientDialog::setupDatePicker() {
    // ✅ Setup QDateEdit để dễ dùng
    ui->dateEditBirthday->setDisplayFormat("dd/MM/yyyy");
    ui->dateEditBirthday->setCalendarPopup(true);

    // Đặt ngày mặc định là 10 năm trước
    QDate defaultDate = QDate::currentDate().addYears(-10);
    ui->dateEditBirthday->setDate(defaultDate);

    // Giới hạn ngày (không quá 120 tuổi, không quá ngày hôm nay)
    QDate minDate = QDate::currentDate().addYears(-120);
    QDate maxDate = QDate::currentDate();

    ui->dateEditBirthday->setMinimumDate(minDate);
    ui->dateEditBirthday->setMaximumDate(maxDate);

    qDebug() << "[DATE PICKER] Setup completed for patient birthday";
}

void AddEditPatientDialog::on_buttonBox_accepted()
{
    if (validateForm()) {
        qDebug() << "[DIALOG] Form validated, accepting dialog";
        accept();
    }
}

void AddEditPatientDialog::on_buttonBox_rejected()
{
    reject();
}

void AddEditPatientDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddEditPatientDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AddEditPatientDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
