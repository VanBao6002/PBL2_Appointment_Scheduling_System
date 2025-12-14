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

    // Số điện thoại cha mẹ
    QString phoneMother = ui->txtPhoneMother->text().trimmed();
    QString phoneFather = ui->txtPhoneFather->text().trimmed();

    try {
        Utils::validName(name.toStdString());
        Utils::validPhoneNumber(phoneNumber.toStdString());
        Utils::validBloodType(bloodType.toStdString());
        Date dob(qDate.day(), qDate.month(), qDate.year());
        Utils::validDate(dob);
        if (!genderStr.isEmpty()) {
            Utils::validGender(genderStr.toStdString()[0]);
        }
        // Validation cho CCCD và Email nếu cần
        Utils::validCCCD(CCCD.toStdString());
        Utils::validEmail(email.toStdString());

        // Validation cho số điện thoại cha mẹ (không bắt buộc)
        if (!phoneMother.isEmpty()) {
            Utils::validPhoneNumber(phoneMother.toStdString());
        }
        if (!phoneFather.isEmpty()) {
            Utils::validPhoneNumber(phoneFather.toStdString());
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
        updatedPatient.setInsuranceID(insuranceID.toStdString());
        updatedPatient.setCCCD(CCCD.toStdString());
        updatedPatient.setEmail(email.toStdString());
        updatedPatient.setPhoneMother(phoneMother.toStdString());
        updatedPatient.setPhoneFather(phoneFather.toStdString());

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
            nameFather.toStdString(),
            phoneMother.toStdString(),    // Số điện thoại mẹ
            phoneFather.toStdString()     // Số điện thoại cha
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

        // 2. Kiểm tra số điện thoại bệnh nhân
        QString phoneText = ui->txtPhoneNumber->text().trimmed();
        if (phoneText.isEmpty()) {
            throw std::invalid_argument("Số điện thoại bệnh nhân không được để trống!");
        }
        Utils::validPhoneNumber(phoneText.toStdString());

        // 3. Kiểm tra số điện thoại mẹ (không bắt buộc)
        QString phoneMotherText = ui->txtPhoneMother->text().trimmed();
        if (!phoneMotherText.isEmpty()) {
            Utils::validPhoneNumber(phoneMotherText.toStdString());
        }

        // 4. Kiểm tra số điện thoại cha (không bắt buộc)
        QString phoneFatherText = ui->txtPhoneFather->text().trimmed();
        if (!phoneFatherText.isEmpty()) {
            Utils::validPhoneNumber(phoneFatherText.toStdString());
        }

        // 5. Kiểm tra email
        QString emailText = ui->txtEmail->text().trimmed();
        if (!emailText.isEmpty()) {
            Utils::validEmail(emailText.toStdString());
        }

        // 6. Kiểm tra nhóm máu
        QString bloodTypeText = ui->txtBloodType->text().trimmed();
        if (bloodTypeText.isEmpty()) {
            throw std::invalid_argument("Nhóm máu không được để trống!");
        }
        Utils::validBloodType(bloodTypeText.toStdString());

        // ✅ 7. Kiểm tra ngày sinh
        QDate qDate = ui->dateEditBirthday->date();
        if (!qDate.isValid()) {
            throw std::invalid_argument("Ngày sinh không hợp lệ!");
        }

        // Kiểm tra tuổi
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

        // 8. Kiểm tra giới tính
        QString genderText = ui->comboGender->currentText().trimmed();
        if (genderText.isEmpty()) {
            throw std::invalid_argument("Vui lòng chọn giới tính!");
        }
        Utils::validGender(genderText.toStdString()[0]);

        // 9. Kiểm tra CCCD
        QString cccdText = ui->txtCCCD->text().trimmed();
        if (cccdText.isEmpty()) {
            throw std::invalid_argument("Số CCCD không được để trống!");
        }
        Utils::validCCCD(cccdText.toStdString());

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
