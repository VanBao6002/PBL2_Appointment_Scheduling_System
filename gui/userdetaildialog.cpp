#include "userdetaildialog.h"
#include "gui/ui_userdetaildialog.h"
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include "userManager.h"
#include "utils.h"

UserDetailDialog::UserDetailDialog(const User& user, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserDetailDialog)
    , m_user(user)
{
    ui->setupUi(this);

    // Set window flags để có thể di chuyển
    setWindowFlags(Qt::FramelessWindowHint);

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Populate data
    populateData();
}

UserDetailDialog::~UserDetailDialog()
{
    delete ui;
}

void UserDetailDialog::populateData()
{
    // Set title with user info
    QString title = QString("CHI TIẾT NGƯỜI DÙNG - ID: %1 - %2")
                        .arg(m_user.getID())
                        .arg(QString::fromStdString(m_user.getUsername()));
    ui->lblMainTitle->setText(title);

    // Basic Info
    ui->lblIDValue->setText(QString::number(m_user.getID()));
    ui->lblUsernameValue->setText(QString::fromStdString(m_user.getUsername()));
    ui->lblPasswordValue->setText(QString::fromStdString(m_user.getPlainPassword()));
    ui->lblRoleValue->setText(QString::fromStdString(User::roleToString(m_user.getRole())));

    // Tô màu vai trò
    if (m_user.getRole() == User::Role::ADMIN) {
        ui->lblRoleValue->setStyleSheet("QLabel { color: #E65100; background-color: #FFF3E0; padding: 5px; border: 1px solid #FFB74D; border-radius: 3px; font-weight: bold; }");
    } else if (m_user.getRole() == User::Role::ASSISTANT) {
        ui->lblRoleValue->setStyleSheet("QLabel { color: #2E7D32; background-color: #E8F5E9; padding: 5px; border: 1px solid #81C784; border-radius: 3px; font-weight: bold; }");
    } else {
        ui->lblRoleValue->setStyleSheet("QLabel { color: #1565C0; background-color: #E3F2FD; padding: 5px; border: 1px solid #64B5F6; border-radius: 3px; font-weight: bold; }");
    }

    // Creation Date
    QString creationDateStr = QString::fromStdString(m_user.getCreationDate().toString());
    ui->lblCreatedDateValue->setText(creationDateStr);

    // Personal Info
    QString fullName = QString::fromStdString(m_user.getFullName());
    QString cccd = QString::fromStdString(m_user.getCCCD());
    QString phone = QString::fromStdString(m_user.getPhoneNumber());
    QString birthday = QString::fromStdString(m_user.getBirthday());

    // Hiển thị thông tin cá nhân
    ui->lblFullNameValue->setText(!fullName.isEmpty() ? fullName : "Không có");
    ui->lblCCCDValue->setText(!cccd.isEmpty() ? cccd : "Không có");
    ui->lblPhoneValue->setText(!phone.isEmpty() ? phone : "Không có");
    ui->lblBirthdayValue->setText(!birthday.isEmpty() ? birthday : "Không có");

    // Đặt title cho window
    setWindowTitle(QString("Chi Tiết Người Dùng - ID: %1").arg(m_user.getID()));
}

void UserDetailDialog::on_btnClose_clicked()
{
    accept();
}

void UserDetailDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void UserDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void UserDetailDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
