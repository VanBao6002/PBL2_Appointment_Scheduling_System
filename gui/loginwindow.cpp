#include "loginwindow.h"
#include "gui/ui_loginwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QResizeEvent>

loginwindow::loginwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);

    // load resource vào m_bgPixmap
    m_bgPixmap.load(":/LoginWindow/loginwindow_background.jpg");
    if (m_bgPixmap.isNull()) {
        qDebug() << "Không load được ảnh từ resource!";
    } else {
        // hiển thị lần đầu
        ui->labelBackground->setPixmap(m_bgPixmap.scaled(ui->labelBackground->size(),
                                                         Qt::KeepAspectRatioByExpanding,
                                                         Qt::SmoothTransformation));
        ui->labelBackground->lower(); // luôn nằm dưới các widget khác
    }
}


void loginwindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!m_bgPixmap.isNull()) {
        ui->labelBackground->setPixmap(m_bgPixmap.scaled(ui->labelBackground->size(),
                                                         Qt::KeepAspectRatioByExpanding,
                                                         Qt::SmoothTransformation));
    }
}


loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_loginButton_clicked()
{
    QString username = ui->UserName->text();
    QString password = ui->PassWord->text();

    if (username == "admin" && password == "123") {
    } else {
        QMessageBox::warning(this, "Login Failed", "Username or password incorrect!");
    }
}

void loginwindow::on_cancelButton_clicked()
{
    this->close();
}
