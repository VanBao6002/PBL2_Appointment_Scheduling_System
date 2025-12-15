#include "addedituserdialog.h"
#include "gui/ui_addedituserdialog.h"
#include <QMouseEvent>
#include <QDate>
#include "utils.h"

AddEditUserDialog::AddEditUserDialog(QWidget *parent, const User& user) :
    QDialog(parent),
    ui(new Ui::AddEditUserDialog),
    currentUser(user),
    m_dragging(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    setupComboBoxes();
    setupDatePicker();
    populateUI(user);

    if (user.getID() > 0) {
        setWindowTitle("Chỉnh Sửa Người Dùng - ID: " + QString::number(user.getID()));
    } else {
        setWindowTitle("Thêm Người Dùng Mới");
    }

    qDebug() << "[DIALOG] AddEditUserDialog opened";
}

AddEditUserDialog::~AddEditUserDialog()
{
    delete ui;
}

void AddEditUserDialog::setupComboBoxes() {
    ui->cmbRole->clear();
    ui->cmbRole->addItem("ADMIN", "ADMIN");
    ui->cmbRole->addItem("ASSISTANT", "ASSISTANT");
}

void AddEditUserDialog::setupDatePicker() {
    // Ngày sinh
    ui->dateEditBirthday->setDisplayFormat("dd/MM/yyyy");
    ui->dateEditBirthday->setCalendarPopup(true);

    QDate defaultBirthday = QDate::currentDate().addYears(-20);
    ui->dateEditBirthday->setDate(defaultBirthday);

    QDate minBirthday = QDate::currentDate().addYears(-100);
    QDate maxBirthday = QDate::currentDate().addYears(-10);
    ui->dateEditBirthday->setMinimumDate(minBirthday);
    ui->dateEditBirthday->setMaximumDate(maxBirthday);

    // Ngày tạo - luôn là ngày hiện tại
    ui->dateEditCreation->setDisplayFormat("dd/MM/yyyy");
    ui->dateEditCreation->setCalendarPopup(true);
    ui->dateEditCreation->setDate(QDate::currentDate());

    // Giới hạn ngày tạo (không quá ngày hiện tại)
    ui->dateEditCreation->setMaximumDate(QDate::currentDate());
}

void AddEditUserDialog::populateUI(const User& user) {
    if (user.getID() > 0) {
        // Chế độ Sửa
        ui->txtFullName->setText(QString::fromStdString(user.getFullName()));
        ui->txtCCCD->setText(QString::fromStdString(user.getCCCD()));
        ui->txtPhone->setText(QString::fromStdString(user.getPhoneNumber()));

        // Thiết lập ngày sinh
        std::string birthday = user.getBirthday();
        if (!birthday.empty()) {
            QStringList parts = QString::fromStdString(birthday).split("/");
            if (parts.size() == 3) {
                QDate qDate(parts[2].toInt(), parts[1].toInt(), parts[0].toInt());
                if (qDate.isValid()) {
                    ui->dateEditBirthday->setDate(qDate);
                }
            }
        }

        // Thiết lập ngày tạo
        Date creationDate = user.getCreationDate();
        QDate qCreationDate(creationDate.getYear(), creationDate.getMonth(), creationDate.getDay());
        if (qCreationDate.isValid()) {
            ui->dateEditCreation->setDate(qCreationDate);
        }

        // Thông tin tài khoản
        ui->txtUsername->setText(QString::fromStdString(user.getUsername()));

        // Vai trò
        QString roleStr = QString::fromStdString(User::roleToString(user.getRole()));
        int roleIndex = ui->cmbRole->findText(roleStr);
        if (roleIndex != -1) {
            ui->cmbRole->setCurrentIndex(roleIndex);
        }

        // Mật khẩu (chỉ hiển thị placeholder)
        ui->txtPassword->setPlaceholderText("Nhập mật khẩu mới nếu muốn thay đổi");
        ui->txtConfirmPassword->setPlaceholderText("Xác nhận mật khẩu mới");
    } else {
        // Chế độ Thêm mới - ngày tạo luôn là ngày hiện tại
        ui->dateEditCreation->setDate(QDate::currentDate());
        ui->txtPassword->setPlaceholderText("Tối thiểu 8 ký tự");
        ui->txtConfirmPassword->setPlaceholderText("Nhập lại mật khẩu");
    }
}

bool AddEditUserDialog::validateForm() {
    // Kiểm tra thông tin cá nhân
    QString fullName = ui->txtFullName->text().trimmed();
    if (fullName.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ và tên!");
        ui->txtFullName->setFocus();
        return false;
    }

    QString cccd = ui->txtCCCD->text().trimmed();
    if (cccd.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số CCCD!");
        ui->txtCCCD->setFocus();
        return false;
    }

    QString phone = ui->txtPhone->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại!");
        ui->txtPhone->setFocus();
        return false;
    }

    // Kiểm tra ngày sinh
    QDate birthday = ui->dateEditBirthday->date();
    if (!birthday.isValid()) {
        QMessageBox::warning(this, "Lỗi", "Ngày sinh không hợp lệ!");
        return false;
    }

    // Kiểm tra ngày tạo
    QDate creationDate = ui->dateEditCreation->date();
    if (!creationDate.isValid()) {
        QMessageBox::warning(this, "Lỗi", "Ngày tạo không hợp lệ!");
        return false;
    }

    // Ngày tạo không được lớn hơn ngày hiện tại
    if (creationDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Lỗi", "Ngày tạo không thể lớn hơn ngày hiện tại!");
        return false;
    }

    // Kiểm tra username
    QString username = ui->txtUsername->text().trimmed();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập tên người dùng!");
        ui->txtUsername->setFocus();
        return false;
    }
    if (username.length() < 4 || username.length() > 20) {
        QMessageBox::warning(this, "Lỗi", "Tên người dùng phải có từ 4-20 ký tự!");
        ui->txtUsername->setFocus();
        return false;
    }

    // Kiểm tra password (chỉ bắt buộc khi thêm mới hoặc khi muốn đổi mật khẩu)
    if (currentUser.getID() == 0) { // Thêm mới
        QString password = ui->txtPassword->text();
        if (password.isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập mật khẩu!");
            ui->txtPassword->setFocus();
            return false;
        }
        if (password.length() < 8 || password.length() > 64) {
            QMessageBox::warning(this, "Lỗi", "Mật khẩu phải có từ 8-64 ký tự!");
            ui->txtPassword->setFocus();
            return false;
        }

        // Kiểm tra confirm password
        QString confirmPassword = ui->txtConfirmPassword->text();
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Lỗi", "Mật khẩu xác nhận không khớp!");
            ui->txtConfirmPassword->setFocus();
            return false;
        }
    } else if (!ui->txtPassword->text().isEmpty()) { // Sửa và có nhập mật khẩu mới
        QString password = ui->txtPassword->text();
        if (password.length() < 8 || password.length() > 64) {
            QMessageBox::warning(this, "Lỗi", "Mật khẩu phải có từ 8-64 ký tự!");
            ui->txtPassword->setFocus();
            return false;
        }

        QString confirmPassword = ui->txtConfirmPassword->text();
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Lỗi", "Mật khẩu xác nhận không khớp!");
            ui->txtConfirmPassword->setFocus();
            return false;
        }
    }

    // Validation bằng Utils
    try {
        Utils::validName(fullName.toStdString());
        Utils::validCCCD(cccd.toStdString());
        Utils::validPhoneNumber(phone.toStdString());

        // Kiểm tra tuổi từ ngày sinh
        QDate today = QDate::currentDate();
        int age = today.year() - birthday.year();
        if (birthday > today.addYears(-age)) age--;

        if (age < 10) {
            throw std::invalid_argument("Người dùng phải từ 10 tuổi trở lên!");
        }

    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Lỗi Nhập Liệu", QString::fromStdString(e.what()));
        return false;
    }

    return true;
}

User AddEditUserDialog::getUserData() const {
    std::string role = ui->cmbRole->currentText().toStdString();
    std::string username = ui->txtUsername->text().trimmed().toStdString();
    std::string fullName = ui->txtFullName->text().trimmed().toStdString();
    std::string cccd = ui->txtCCCD->text().trimmed().toStdString();
    std::string phone = ui->txtPhone->text().trimmed().toStdString();
    QDate qBirthday = ui->dateEditBirthday->date();
    std::string birthday = QString("%1/%2/%3")
                               .arg(qBirthday.day())
                               .arg(qBirthday.month())
                               .arg(qBirthday.year())
                               .toStdString();

    User user;

    if (currentUser.getID() > 0) {
        // Chế độ SỬA
        user = currentUser;

        // Chỉ cập nhật mật khẩu nếu có nhập mới
        std::string password = ui->txtPassword->text().toStdString();
        if (!password.empty()) {
            user.setPassword(password);
        }

        // Cập nhật vai trò và username
        user.setRole(role);
        user.setUsername(username);
    } else {
        // Chế độ THÊM MỚI
        std::string password = ui->txtPassword->text().toStdString();
        user = User(role, username, password, fullName, cccd, phone, birthday);
    }

    // Ngày tạo
    QDate qCreationDate = ui->dateEditCreation->date();
    Date creationDate(qCreationDate.day(), qCreationDate.month(), qCreationDate.year());
    user.setCreationDate(creationDate);

    qDebug() << "[DIALOG] getUserData() - ID:" << user.getID();
    qDebug() << "[DIALOG] Creation Date:" << QString::fromStdString(creationDate.toString());

    return user;
}

void AddEditUserDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

void AddEditUserDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddEditUserDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AddEditUserDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}

void AddEditUserDialog::on_buttonBox_accepted() {
    if (validateForm()) {
        qDebug() << "[DIALOG] Form validated, accepting dialog";
        accept();
    }
}

void AddEditUserDialog::on_buttonBox_rejected() {
    qDebug() << "[DIALOG] Dialog rejected/cancelled";
    reject();
}
