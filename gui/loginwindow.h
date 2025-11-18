#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QResizeEvent>
#include <QMessageBox>
#include "userManager.h"

namespace Ui {
class loginwindow;
}

// Thêm: Định nghĩa đường dẫn file người dùng
constexpr const char* USER_FILE_PATH = "user.json";

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);
    ~loginwindow();

private slots:
    void on_loginButton_clicked();
    void on_cancelButton_clicked();

    void on_showPasswordButton_toggled(bool checked);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // Thêm: Để vẽ nền

private:
    void loadUsers(); // Thêm: Hàm để load người dùng từ file

    Ui::loginwindow *ui;
    QPixmap m_bgPixmap; // lưu ảnh gốc
    // std::vector<User> m_users; // Xóa: Không cần thiết nữa, dùng UserManager
    UserManager m_userManager; // Thêm: Đối tượng quản lý người dùng
};

#endif // LOGINWINDOW_H
