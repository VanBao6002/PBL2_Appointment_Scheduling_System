#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_Login_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;
    bool centered = false;

    void centerWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // MAINWINDOW_H
