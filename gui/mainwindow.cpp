#include "mainwindow.h"
#include "gui/ui_mainwindow.h"
#include "loginwindow.h"
#include "config.h"

#include <QPainter>
#include <QResource>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QStyle>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(710, 550);
    if (!pixmap.load(Config::MAIN_SCREEN_IMAGE_PATH)) {
        qDebug() << "Failed to load background image.";
    }
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if (!centered) {
        centered = true;
        QTimer::singleShot(0, this, &MainWindow::centerWindow);
    }
}

void MainWindow::centerWindow()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect availableGeometry = screen->availableGeometry();
    QSize size = this->size();
    QRect centeredRect = QStyle::alignedRect(
        Qt::LeftToRight, Qt::AlignCenter, size, availableGeometry);
    this->setGeometry(centeredRect);
    this->activateWindow();
    this->raise();
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

