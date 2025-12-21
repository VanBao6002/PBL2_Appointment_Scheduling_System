#include "patientdetaildialog.h"
#include "gui/ui_patientdetaildialog.h"
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include "patientManager.h"
#include "utils.h"

PatientDetailDialog::PatientDetailDialog(const Patient& patient, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PatientDetailDialog)
    , m_patient(patient)
    , patientID(patient.getID())
    , editRequested(false)
    , m_dragging(false)
{
    ui->setupUi(this);

    // Set window flags để có thể di chuyển như AddEditMedicalRecordDialog
    setWindowFlags(Qt::FramelessWindowHint);

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    // ✅ Check if buttons exist in UI, if not create them
    QPushButton* btnEdit = findChild<QPushButton*>("btnEdit");
    QPushButton* btnClose = findChild<QPushButton*>("btnClose");

    if (!btnEdit) {
        // Create Edit button dynamically if not in UI
        QWidget* bottomWidget = findChild<QWidget*>("widgetBottom");
        if (bottomWidget && bottomWidget->layout()) {
            QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(bottomWidget->layout());
            if (layout) {
                btnEdit = new QPushButton("Chỉnh sửa");
                btnEdit->setObjectName("btnEdit");
                btnEdit->setMinimumSize(100, 40);
                btnEdit->setStyleSheet(R"(
                    QPushButton {
                        background-color: #f39c12;
                        color: white;
                        border: none;
                        padding: 8px 25px;
                        border-radius: 3px;
                        font-weight: bold;
                        font-size: 14px;
                        min-width: 100px;
                    }
                    QPushButton:hover {
                        background-color: #e67e22;
                    }
                    QPushButton:pressed {
                        background-color: #d35400;
                    }
                )");

                // Insert before close button (at index 1, after first spacer)
                layout->insertWidget(1, btnEdit);
            }
        }
    }

    // Connect signals
    if (btnEdit) {
        connect(btnEdit, &QPushButton::clicked, this, &PatientDetailDialog::on_btnEdit_clicked);
    }
    if (btnClose) {
        connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
    }

    // Populate data
    populateData();
}

PatientDetailDialog::~PatientDetailDialog()
{
    delete ui;
}

void PatientDetailDialog::populateData()
{
    // Set title with patient name
    QString title = QString("CHI TIẾT BỆNH NHÂN - ID: %1 - %2")
                        .arg(m_patient.getID())
                        .arg(QString::fromStdString(m_patient.getName()));
    ui->lblMainTitle->setText(title);

    // Basic Info
    ui->lblIDValue->setText(QString::number(m_patient.getID()));
    ui->lblNameValue->setText(QString::fromStdString(m_patient.getName()));

    QString genderText;
    char gender = m_patient.getGender();
    if (gender == 'M') genderText = "Nam";
    else if (gender == 'F') genderText = "Nữ";
    else genderText = "Khác";

    ui->lblGenderValue->setText(genderText);
    ui->lblBirthdayValue->setText(QString::fromStdString(m_patient.getBirthday().toString()));

    // Medical Info
    ui->lblBloodTypeValue->setText(QString::fromStdString(m_patient.getBloodType()));
    ui->lblInsuranceIDValue->setText(QString::fromStdString(m_patient.getInsuranceID()));

    std::vector<std::string> allergies = m_patient.getAllergies();
    QString allergiesText = allergies.empty() ? "Không có" :
                                QString::fromStdString(Utils::join(allergies, ", "));
    ui->lblAllergiesValue->setText(allergiesText);
    ui->lblAllergiesValue->setWordWrap(true);

    std::vector<std::string> diseases = m_patient.getChronicDiseases();
    QString diseasesText = diseases.empty() ? "Không có" :
                               QString::fromStdString(Utils::join(diseases, ", "));
    ui->lblChronicDiseasesValue->setText(diseasesText);
    ui->lblChronicDiseasesValue->setWordWrap(true);

    // Contact Info
    ui->lblPhoneNumberValue->setText(QString::fromStdString(m_patient.getPhoneNumber()));
    ui->lblEmailValue->setText(QString::fromStdString(m_patient.getEmail()));
    ui->lblCCCDValue->setText(QString::fromStdString(m_patient.getCCCD()));

    // Family Info
    ui->lblNameMotherValue->setText(QString::fromStdString(m_patient.getNameMother()));
    ui->lblNameFatherValue->setText(QString::fromStdString(m_patient.getNameFather()));
    ui->lblPhoneMotherValue->setText(QString::fromStdString(m_patient.getPhoneMother()));
    ui->lblPhoneFatherValue->setText(QString::fromStdString(m_patient.getPhoneFather()));

    // Đặt title cho window với ID
    setWindowTitle(QString("Chi Tiết Bệnh Nhân - ID: %1").arg(m_patient.getID()));
}

void PatientDetailDialog::on_btnClose_clicked()
{
    reject();  // Close without edit
}

void PatientDetailDialog::on_btnEdit_clicked()
{
    qDebug() << "[EDIT CLICKED] User requested to edit patient ID:" << patientID;
    editRequested = true;
    accept();  // Close and signal edit request
}

bool PatientDetailDialog::shouldEdit() const
{
    return editRequested;
}

void PatientDetailDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void PatientDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void PatientDetailDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
