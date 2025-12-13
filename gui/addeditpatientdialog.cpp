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

        // NEW: Set insuranceID field
        ui->txtInsuranceID->setText(QString::fromStdString(patient.getInsuranceID()));

        // ✅ Cập nhật cách thiết lập ngày sinh
        QDate qDate(patient.getBirthday().getYear(), patient.getBirthday().getMonth(), patient.getBirthday().getDay());
        ui->dateEditBirthday->setDate(qDate);

        setWindowTitle("Chỉnh Sửa Thông Tin Bệnh Nhân - ID: " + QString::number(patient.getID()));
    } else {
        // Chế độ Thêm mới
        setWindowTitle("Thêm Bệnh Nhân Mới");
        int defaultGenderIndex = ui->comboGender->findText("M");
        if (defaultGenderIndex != -1) {
            ui->comboGender->setCurrentIndex(defaultGenderIndex);
        }

        // NEW: Clear insuranceID field
        ui->txtInsuranceID->clear();

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
    QString CCCD = ui->txtCCCD->text().trimmed(); // thêm vào trong QT Creator
    QString email = ui->txtEmail->text().trimmed(); // thêm vào trong QT Creator

    try {
        Utils::validName(name.toStdString());
        Utils::validPhoneNumber(phoneNumber.toStdString());
        Utils::validBloodType(bloodType.toStdString());
        Date dob(qDate.day(), qDate.month(), qDate.year());
        Utils::validDate(dob);
        if (!genderStr.isEmpty()) {
            Utils::validGender(genderStr.toStdString()[0]);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Validation failed in getPatientData: " + std::string(e.what()));
    }

    if (currentPatient.getID() > 0) {
        // Chế độ SỬA
        Patient updatedPatient = currentPatient;

        updatedPatient.setName(name.toStdString());
        if (!genderStr.isEmpty()) {
            updatedPatient.setGender(genderStr.toStdString()[0]);
        }
        Date newBirthday(qDate.day(), qDate.month(), qDate.year());
        updatedPatient.setBirthday(newBirthday);
        updatedPatient.setBloodType(bloodType.toStdString());
        updatedPatient.setAllergies(allergies.toStdString());
        updatedPatient.setChronicDiseases(chronicDiseases.toStdString());
        updatedPatient.setNameMother(nameMother.toStdString());
        updatedPatient.setNameFather(nameFather.toStdString());
        updatedPatient.setPhoneNumber(phoneNumber.toStdString());

        return updatedPatient;
    } else {
        // Chế độ THÊM MỚI
        std::string birthday =
            QString("%1/%2/%3")
                .arg(qDate.day())
                .arg(qDate.month())
                .arg(qDate.year())
                .toStdString();

        Patient newPatient(
            name.toStdString(),
            genderStr.isEmpty() ? 'M' : genderStr.toStdString()[0],
            birthday,
            phoneNumber.toStdString(),
            CCCD.toStdString(),
            email.toStdString(),
            insuranceID.toStdString(),
            bloodType.toStdString(),
            allergies.toStdString(),
            chronicDiseases.toStdString(),
            nameMother.toStdString(),
            nameFather.toStdString()
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
    try {
        // 1. Kiểm tra tên
        QString nameText = ui->txtName->text().trimmed();
        if (nameText.isEmpty()) {
            throw std::invalid_argument("Tên không được để trống!");
        }
        Utils::validName(nameText.toStdString());

        // 2. Kiểm tra số điện thoại
        QString phoneText = ui->txtPhoneNumber->text().trimmed();
        if (phoneText.isEmpty()) {
            throw std::invalid_argument("Số điện thoại không được để trống!");
        }
        Utils::validPhoneNumber(phoneText.toStdString());

        // 3. Kiểm tra email (nếu có field email)
        QString emailText = ui->txtEmail->text().trimmed();
        if (!emailText.isEmpty()) {
            Utils::validEmail(emailText.toStdString());
        }

        // 4. Kiểm tra nhóm máu
        QString bloodTypeText = ui->txtBloodType->text().trimmed();
        if (bloodTypeText.isEmpty()) {
            throw std::invalid_argument("Nhóm máu không được để trống!");
        }
        Utils::validBloodType(bloodTypeText.toStdString());

        // ✅ 5. Kiểm tra ngày sinh (thêm kiểm tra tuổi hợp lệ)
        QDate qDate = ui->dateEditBirthday->date();

        // Kiểm tra ngày hợp lệ
        if (!qDate.isValid()) {
            throw std::invalid_argument("Ngày sinh không hợp lệ!");
        }

        // Kiểm tra tuổi (tối thiểu 0 tuổi, tối đa 150 tuổi)
        QDate today = QDate::currentDate();
        int age = today.year() - qDate.year();
        if (qDate > today.addYears(-age)) age--;

        if (age < 0) {
            throw std::invalid_argument("Ngày sinh không thể trong tương lai!");
        }

        if (age > 150) {
            throw std::invalid_argument("Ngày sinh không hợp lệ (quá 150 tuổi)!");
        }

        Date dob(qDate.day(), qDate.month(), qDate.year());
        Utils::validDate(dob);

        // 6. Kiểm tra giới tính
        QString genderText = ui->comboGender->currentText().trimmed();
        if (genderText.isEmpty()) {
            throw std::invalid_argument("Vui lòng chọn giới tính!");
        }
        Utils::validGender(genderText.toStdString()[0]);

        // ✅ TẤT CẢ VALIDATION PASS → CHỈ KHI ĐÓ MỚI ACCEPT
        accept();

    } catch (const std::invalid_argument& e) {
        // ❌ Hiển thị lỗi và KHÔNG accept dialog
        QMessageBox::warning(this, "Lỗi Nhập Liệu", QString::fromStdString(e.what()));
        // KHÔNG gọi accept() → Dialog vẫn mở, user có thể sửa

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

void AddEditPatientDialog::on_buttonBox_rejected()
{
    reject();  // Đóng dialog khi bấm Cancel
}

