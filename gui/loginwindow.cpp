#include "loginwindow.h"
#include "gui/ui_loginwindow.h"
#include "adminwindow.h"
#include "assistantwindow.h"
#include "userManager.h"
#include "config.h"
#include "utils.h"

#include <QPainter>
#include <QResource>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

loginwindow::loginwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);

    if (!m_bgPixmap.load(Config::LOGIN_IMAGE_PATH)) {
        qDebug() << "Failed to load background image.";
    }
    ui->passWord->setEchoMode(QLineEdit::Password);
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_loginButton_clicked()
{
    QString username = ui->userName->text();
    QString password = ui->passWord->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Lỗi Đăng Nhập", "Tên đăng nhập và mật khẩu không được để trống.");
        return;
    }
    try {
        const auto& allUsers = UserManager::getInstance().getAllUsers();
        bool loginSuccess = false;
        User loggedInUser;

        for (const auto& pair : allUsers) {
            const User& user = pair.second;
            if (user.getUsername() == username.toStdString() &&
                user.getPassword() == password.toStdString())
            {
                loginSuccess = true;
                loggedInUser = user;
                break;
            }
        }

        if (loginSuccess) {
            this->hide();
            if (loggedInUser.getRole() == User::Role::ADMIN) {
                AdminWindow *adminWindow = new AdminWindow(this); 
                adminWindow->show();
                this->hide();
                qDebug() << "Admin login successful.";
            } else if(loggedInUser.getRole() == User::Role::ASSISTANT){
                AssistantWindow *assistantWindow = new AssistantWindow(this);
                assistantWindow->show();
                this->hide();
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
    // if (ui->labelBackground) {
    //     ui->labelBackground->setGeometry(0, 0, width(), height());
    // }
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

