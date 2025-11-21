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

// Đường dẫn file người dùng
constexpr const char* USER_FILE_PATH = "data/user.json"; // ✅ Sửa path cho đúng với Config

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
    void paintEvent(QPaintEvent *event) override;

private:
    void loadUsers();
    Ui::loginwindow *ui;
    QPixmap m_bgPixmap;
};

#endif // LOGINWINDOW_H
