#include "loginwindow.h"
#include "gui/ui_loginwindow.h"
#include "adminwindow.h"
#include "doctorwindow.h"
#include "assistantwindow.h"
#include <QPainter>
#include <QDebug>

loginwindow::loginwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);
    if (!m_bgPixmap.load(":/images/background.jpg")) {
        qDebug() << "Failed to load background image.";
    }
    loadUsers();
    ui->passWord->setEchoMode(QLineEdit::Password); // Ẩn mật khẩu
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::loadUsers()
{
    try {
        m_userManager.loadFromFile(USER_FILE_PATH);
        QMessageBox::information(this, "Thành công", "Đã tải dữ liệu người dùng thành công.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi Tải Dữ Liệu", QString("Không thể tải file %1: %2").arg(USER_FILE_PATH).arg(e.what()));
        qDebug() << "Error loading users: " << e.what();
    }
}

void loginwindow::on_loginButton_clicked()
{
    QString username = ui->userName->text();
    QString password = ui->passWord->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Lỗi Đăng Nhập", "Tên đăng nhập và mật khẩu không được để trống.");
        return;
    }

    // Tạm thời bỏ qua hash (cho mục đích bài tập)
    std::string passwordHash = password.toStdString();

    try {
        const auto& allUsers = m_userManager.getAllUsers();
        bool loginSuccess = false;
        User loggedInUser;

        for (const auto& pair : allUsers) {
            const User& user = pair.second;
            // So sánh trực tiếp chuỗi mật khẩu nhập vào với chuỗi đã lưu
            if (user.getUsername() == username.toStdString() &&
                user.getPassword() == passwordHash)
            {
                loginSuccess = true;
                loggedInUser = user;
                break;
            }
        }

        if (loginSuccess) {
            this->close();
            if (loggedInUser.getRole() == User::Role::ADMIN) {
                AdminWindow *adminWindow = new AdminWindow();
                adminWindow->show();
                qDebug() << "Admin login successful.";

            } else if (loggedInUser.getRole() == User::Role::DOCTOR) {
                DoctorWindow *doctorWindow = new DoctorWindow();
                doctorWindow->show();
                qDebug() << "Doctor login successful.";

            } else if(loggedInUser.getRole() == User::Role::ASSISTANT){
                AssistantWindow *assistantWindow = new AssistantWindow();
                assistantWindow->show();
                qDebug() << "Assistant login successful.";
            } else {
                QMessageBox::critical(nullptr, "Lỗi Vai Trò", "Vai trò người dùng không hợp lệ hoặc không được hỗ trợ.");
            }
        } else {
            QMessageBox::warning(this, "Lỗi Đăng Nhập", "Sai tên đăng nhập hoặc mật khẩu.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Lỗi Hệ Thống", QString("Lỗi trong quá trình kiểm tra: %1").arg(e.what()));
        qDebug() << "System error during login check: " << e.what();
    }
}

void loginwindow::on_cancelButton_clicked()
{
    QMessageBox::information(this, "Hủy Bỏ", "Đã hủy đăng nhập.");
    close();
}

void loginwindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    // Thay đổi kích thước QLabel labelBackground để nó chiếm toàn bộ cửa sổ
    if (ui->labelBackground) {
        ui->labelBackground->setGeometry(0, 0, width(), height());
    }
}

void loginwindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (!m_bgPixmap.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(rect(), m_bgPixmap);
    }
}

void loginwindow::on_showPasswordButton_toggled(bool checked)
{
    if (checked) {
        // Nút được nhấn -> Hiển thị mật khẩu
        ui->passWord->setEchoMode(QLineEdit::Normal);
        // Có thể thay đổi biểu tượng nút thành "👁️"
        ui->showPasswordButton->setText("Hide");
    } else {
        // Nút không được nhấn -> Ẩn mật khẩu
        ui->passWord->setEchoMode(QLineEdit::Password);
        // Có thể thay đổi biểu tượng nút thành "🙈"
        ui->showPasswordButton->setText("Show");
    }
}

