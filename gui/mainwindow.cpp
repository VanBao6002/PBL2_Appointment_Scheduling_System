#include "mainwindow.h"
#include "gui/ui_mainwindow.h"
#include "loginwindow.h"
#include "config.h"

#include <QPainter>
#include <QResource>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Center the window on the screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    if (!pixmap.load(Config::MAIN_SCREEN_IMAGE_PATH)) {
        qDebug() << "Failed to load background image.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()
{
    loginwindow *lgwd = new loginwindow;
    lgwd->show();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    if (!pixmap.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(rect(), pixmap);
    }
}

