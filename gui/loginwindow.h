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
    Ui::loginwindow *ui;
    QPixmap m_bgPixmap;
};

#endif // LOGINWINDOW_H
