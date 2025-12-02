#include "addedituserdialog.h"
#include "gui/ui_addedituserdialog.h"
#include <QMouseEvent>

AddEditUserDialog::AddEditUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditUserDialog),
    m_dragging(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    setWindowTitle("Thêm Người Dùng Mới");
    setupComboBoxes();

    qDebug() << "[DIALOG] AddEditUserDialog opened in ADD mode";
}

AddEditUserDialog::~AddEditUserDialog()
{
    delete ui;
}

void AddEditUserDialog::setupComboBoxes() {
    // ComboBox Vai trò
    ui->cmbRole->clear();
    ui->cmbRole->addItem("ADMIN", "ADMIN");
    ui->cmbRole->addItem("ASSISTANT", "ASSISTANT");

    // Set placeholder text
    ui->txtUsername->setPlaceholderText("VD: admin123");
    ui->txtPassword->setPlaceholderText("Tối thiểu 8 ký tự");
    ui->txtPassword->setEchoMode(QLineEdit::Password);
    ui->txtConfirmPassword->setEchoMode(QLineEdit::Password);
}

bool AddEditUserDialog::validateForm() {
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

    // Kiểm tra password
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

    return true;
}

User AddEditUserDialog::getUserData() const {
    std::string role = ui->cmbRole->currentData().toString().toStdString();
    std::string username = ui->txtUsername->text().trimmed().toStdString();
    std::string password = ui->txtPassword->text().toStdString();

    User user(role, username, password);

    qDebug() << "[DIALOG] getUserData() - new ID:" << user.getID();
    return user;
}

void AddEditUserDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

void AddEditUserDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddEditUserDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
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
