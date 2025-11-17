#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPixmap>

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

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::loginwindow *ui;
    QPixmap m_bgPixmap; // lưu ảnh gốc
};

#endif // LOGINWINDOW_H
