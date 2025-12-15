#include "doctordetaildialog.h"
#include "gui/ui_doctordetaildialog.h"
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include <QHeaderView>
#include "doctorManager.h"
#include "utils.h"

DoctorDetailDialog::DoctorDetailDialog(const Doctor& doctor, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DoctorDetailDialog)
    , m_doctor(doctor)
    , m_dragging(false)
{
    ui->setupUi(this);

    // Set window flags để có thể di chuyển như PatientDetailDialog
    setWindowFlags(Qt::FramelessWindowHint);

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // Setup working schedule table
    setupWorkingScheduleTable();

    // Populate data
    populateData();

    // Connect close button
    connect(ui->btnClose, &QPushButton::clicked, this, &DoctorDetailDialog::on_btnClose_clicked);
}

DoctorDetailDialog::~DoctorDetailDialog()
{
    delete ui;
}

void DoctorDetailDialog::setupWorkingScheduleTable()
{
    // Setup table properties
    ui->tableWorkingSchedule->setColumnCount(3);
    QStringList headers;
    headers << "Ngày" << "Giờ bắt đầu" << "Giờ kết thúc";
    ui->tableWorkingSchedule->setHorizontalHeaderLabels(headers);

    // Set column resize modes
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWorkingSchedule->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    // Set selection behavior
    ui->tableWorkingSchedule->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWorkingSchedule->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Enable alternating row colors for better readability
    ui->tableWorkingSchedule->setAlternatingRowColors(true);

    // Apply stylesheet for better visibility
    ui->tableWorkingSchedule->setStyleSheet(
        "QTableWidget {"
        "   background-color: #ffffff;"
        "   alternate-background-color: #f5f5f5;"
        "   border: 2px solid #CCCCCC;"
        "   gridline-color: #d0d0d0;"
        "   border-radius: 5px;"
        "}"
        "QTableWidget::item {"
        "   color: #2c3e50;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "   border-bottom: 1px solid #e0e0e0;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "}"
        "QTableWidget::item:hover {"
        "   background-color: #e8f5e9;"
        "}"
        "QHeaderView::section {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   padding: 10px;"
        "   border: 1px solid #45A049;"
        "   font-weight: bold;"
        "   font-size: 11pt;"
        "}"
        "QTableWidget QTableCornerButton::section {"
        "   background-color: #4CAF50;"
        "   border: 1px solid #45A049;"
        "}"
        );

    qDebug() << "[DETAIL] Working schedule table setup completed";
}

void DoctorDetailDialog::populateData()
{
    // Set title with doctor name
    QString title = QString("CHI TIẾT BÁC SĨ - ID: %1 - %2")
                        .arg(m_doctor.getID())
                        .arg(QString::fromStdString(m_doctor.getName()));
    ui->lblMainTitle->setText(title);

    // Basic Info
    ui->lblIDValue->setText(QString::number(m_doctor.getID()));
    ui->lblNameValue->setText(QString::fromStdString(m_doctor.getName()));

    QString genderText;
    char gender = m_doctor.getGender();
    if (gender == 'M') genderText = "Nam";
    else if (gender == 'F') genderText = "Nữ";
    else genderText = "Khác";

    ui->lblGenderValue->setText(genderText);
    ui->lblBirthdayValue->setText(QString::fromStdString(m_doctor.getBirthday().toString()));

    // Professional Info
    ui->lblSpecializationValue->setText(QString::fromStdString(m_doctor.getSpecialization()));

    // Phòng làm việc
    QString roomText = QString::fromStdString(m_doctor.getRoom());
    if (roomText.isEmpty()) {
        roomText = "Chưa xác định";
    }
    ui->lblRoomValue->setText(roomText);

    QString statusText = QString::fromStdString(Doctor::statusToString(m_doctor.getStatus()));
    ui->lblStatusValue->setText(statusText);

    // Tô màu cho trạng thái
    QString statusStyle;
    if (m_doctor.getStatus() == Doctor::Status::Active) {
        statusStyle = "background-color: #4CAF50; color: white;";
    } else if (m_doctor.getStatus() == Doctor::Status::OnLeave) {
        statusStyle = "background-color: #FFC107; color: black;";
    } else if (m_doctor.getStatus() == Doctor::Status::Retired) {
        statusStyle = "background-color: #9E9E9E; color: white;";
    }
    ui->lblStatusValue->setStyleSheet(statusStyle + " padding: 5px; border-radius: 3px;");

    // Contact Info
    ui->lblPhoneNumberValue->setText(QString::fromStdString(m_doctor.getPhoneNumber()));
    ui->lblEmailValue->setText(QString::fromStdString(m_doctor.getEmail()));
    ui->lblCCCDValue->setText(QString::fromStdString(m_doctor.getCCCD()));

    // Lịch làm việc
    loadWorkingSchedule();

    // Đặt title cho window với ID
    setWindowTitle(QString("Chi Tiết Bác Sĩ - ID: %1").arg(m_doctor.getID()));
}

void DoctorDetailDialog::loadWorkingSchedule()
{
    ui->tableWorkingSchedule->setRowCount(0);
    const WorkingSchedule& ws = m_doctor.getWorkingSchedule();

    qDebug() << "[DETAIL] Loading working schedule for doctor ID:" << m_doctor.getID();
    qDebug() << "[DETAIL] Schedule map size:" << ws.schedule.size();

    if (ws.schedule.empty()) {
        qDebug() << "[DETAIL] No working schedule found for this doctor";
        // Thêm một dòng thông báo không có lịch
        ui->tableWorkingSchedule->insertRow(0);
        QTableWidgetItem* emptyItem = new QTableWidgetItem("Chưa có lịch làm việc");
        emptyItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWorkingSchedule->setItem(0, 0, emptyItem);
        ui->tableWorkingSchedule->setSpan(0, 0, 1, 3);
        return;
    }

    int row = 0;
    // Sắp xếp các ngày trong tuần theo thứ tự
    QStringList dayOrder = {"Thứ 2", "Thứ 3", "Thứ 4", "Thứ 5", "Thứ 6", "Thứ 7", "Chủ nhật"};

    for (const QString& dayName : dayOrder) {
        std::string dayStd = dayName.toStdString();
        auto it = ws.schedule.find(dayStd);

        if (it != ws.schedule.end()) {
            const std::vector<std::pair<std::string, std::string>>& timeSlots = it->second;

            qDebug() << "[DETAIL] Day:" << dayName << "has" << timeSlots.size() << "time slots";

            for (const auto& slot : timeSlots) {
                ui->tableWorkingSchedule->insertRow(row);

                QTableWidgetItem* dayItem = new QTableWidgetItem(dayName);
                QTableWidgetItem* startItem = new QTableWidgetItem(QString::fromStdString(slot.first));
                QTableWidgetItem* endItem = new QTableWidgetItem(QString::fromStdString(slot.second));

                // Center align all items
                dayItem->setTextAlignment(Qt::AlignCenter);
                startItem->setTextAlignment(Qt::AlignCenter);
                endItem->setTextAlignment(Qt::AlignCenter);

                ui->tableWorkingSchedule->setItem(row, 0, dayItem);
                ui->tableWorkingSchedule->setItem(row, 1, startItem);
                ui->tableWorkingSchedule->setItem(row, 2, endItem);

                qDebug() << "[DETAIL] Added row" << row << ":" << dayName << slot.first.c_str() << "-" << slot.second.c_str();

                row++;
            }
        }
    }

    qDebug() << "[DETAIL] Total rows added:" << row;
}

void DoctorDetailDialog::on_btnClose_clicked()
{
    accept();
}

void DoctorDetailDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void DoctorDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void DoctorDetailDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
