#include "addeditmedicalrecorddialog.h"
#include "gui/ui_addeditmedicalrecorddialog.h"
#include <QRegularExpressionValidator>
#include "addmedicineprescriptiondialog.h"

AddEditMedicalRecordDialog::AddEditMedicalRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditMedicalRecordDialog),
    isEditMode(false),
    editingRecordID(-1)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setupUI();
    setupValidators();
    populatePatientCombo();
    populateDoctorCombo();
    setupConnections();
    qDebug() << "[DIALOG] AddEditMedicalRecordDialog opened in ADD mode";
}

AddEditMedicalRecordDialog::AddEditMedicalRecordDialog(QWidget *parent, const MedicalRecord& recordToEdit) :
    QDialog(parent),
    ui(new Ui::AddEditMedicalRecordDialog),
    isEditMode(true),
    editingRecordID(recordToEdit.getID())
{
    ui->setupUi(this);
    setWindowTitle("Chỉnh Sửa Hồ Sơ Bệnh Án");
    setupUI();
    setupValidators();
    populatePatientCombo();
    populateDoctorCombo();
    setupConnections();
    loadMedicalRecordData(recordToEdit);
    qDebug() << "[DIALOG] AddEditMedicalRecordDialog opened in EDIT mode for ID:" << editingRecordID;
}

AddEditMedicalRecordDialog::~AddEditMedicalRecordDialog()
{
    delete ui;
}

void AddEditMedicalRecordDialog::setupUI() {
    // Setup DateEdit
    ui->dateCreation->setDisplayFormat("dd/MM/yyyy");
    ui->dateCreation->setCalendarPopup(true);
    ui->dateCreation->setDate(QDate::currentDate());

    ui->dateLastUpdated->setDisplayFormat("dd/MM/yyyy");
    ui->dateLastUpdated->setCalendarPopup(true);
    ui->dateLastUpdated->setDate(QDate::currentDate());

    ui->dateFollowUp->setDisplayFormat("dd/MM/yyyy");
    ui->dateFollowUp->setCalendarPopup(true);
    ui->dateFollowUp->setDate(QDate::currentDate());

    // Set proper labels
    ui->label_9->setText("Nhịp tim:");
    ui->label_10->setText("Nhiệt độ:");

    // Configure spin boxes
    ui->spinHeartRate->setRange(30, 220);
    ui->spinHeartRate->setValue(72);
    ui->spinHeartRate->setSuffix(" bpm");

    ui->spinBodyTemp->setRange(30.0, 45.0);
    ui->spinBodyTemp->setValue(36.5);
    ui->spinBodyTemp->setDecimals(1);
    ui->spinBodyTemp->setSuffix(" °C");

    // Placeholders
    ui->txtDiagnosis->setPlaceholderText("VD: Viêm đường hô hấp trên");
    ui->txtSymptoms->setPlaceholderText("VD: Ho, sốt, đau đầu");
    ui->txtTestResults->setPlaceholderText("VD: XN máu: WBC 12000");
    ui->txtBloodPressure->setPlaceholderText("VD: 120/80");
    ui->txtTreatment->setPlaceholderText("VD: Nghỉ ngơi, uống thuốc theo đơn");
    ui->txtDoctorNotes->setPlaceholderText("Ghi chú của bác sĩ...");
}

void AddEditMedicalRecordDialog::setupConnections() {
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddEditMedicalRecordDialog::on_buttonBox_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddEditMedicalRecordDialog::on_buttonBox_rejected);
}

void AddEditMedicalRecordDialog::setupValidators() {
    // Blood pressure validator: XXX/YYY
    QRegularExpression bpRegex("^\\d{2,3}/\\d{2,3}$");
    QRegularExpressionValidator* bpValidator = new QRegularExpressionValidator(bpRegex, this);
    ui->txtBloodPressure->setValidator(bpValidator);
}

void AddEditMedicalRecordDialog::populatePatientCombo() {
    ui->cmbPatient->clear();
    const auto& patients = PatientManager::getInstance().getAllPatientsTable();

    for (const auto& pair : patients) {
        const Patient& p = pair.second;
        QString display = QString("ID: %1 - %2")
                              .arg(p.getID())
                              .arg(QString::fromStdString(p.getName()));
        ui->cmbPatient->addItem(display, p.getID());
    }

    if (ui->cmbPatient->count() > 0) {
        ui->cmbPatient->setCurrentIndex(0);
    }
}

void AddEditMedicalRecordDialog::populateDoctorCombo() {
    ui->cmbDoctor->clear();
    const auto& doctors = DoctorManager::getInstance().getAllDoctors();

    for (const auto& pair : doctors) {
        const Doctor& d = pair.second;
        QString display = QString("ID: %1 - %2 (%3)")
                              .arg(d.getID())
                              .arg(QString::fromStdString(d.getName()))
                              .arg(QString::fromStdString(d.getSpecialization()));
        ui->cmbDoctor->addItem(display, d.getID());
    }

    if (ui->cmbDoctor->count() > 0) {
        ui->cmbDoctor->setCurrentIndex(0);
    }
}

void AddEditMedicalRecordDialog::loadMedicalRecordData(const MedicalRecord& record) {
    // Patient & Doctor
    int patientIdx = ui->cmbPatient->findData(record.getPatientID());
    if (patientIdx >= 0) ui->cmbPatient->setCurrentIndex(patientIdx);

    int doctorIdx = ui->cmbDoctor->findData(record.getDoctorID());
    if (doctorIdx >= 0) ui->cmbDoctor->setCurrentIndex(doctorIdx);

    // Dates
    Date cDate = record.getCreationDate();
    ui->dateCreation->setDate(QDate(cDate.getYear(), cDate.getMonth(), cDate.getDay()));

    Date lDate = record.getLastUpdated();
    ui->dateLastUpdated->setDate(QDate(lDate.getYear(), lDate.getMonth(), lDate.getDay()));

    // Medical info
    ui->txtDiagnosis->setText(QString::fromStdString(record.getDiagnosis()));
    ui->txtSymptoms->setPlainText(QString::fromStdString(record.getSymptoms()));
    ui->txtTestResults->setPlainText(QString::fromStdString(record.getTestResults()));
    ui->txtBloodPressure->setText(QString::fromStdString(record.getBloodPressure()));
    ui->spinHeartRate->setValue(record.getHeartRate());
    ui->spinBodyTemp->setValue(record.getBodyTemperature());
    ui->txtTreatment->setPlainText(QString::fromStdString(record.getTreatment()));
    ui->txtDoctorNotes->setPlainText(QString::fromStdString(record.getDoctorNotes()));

    // Follow-up dates
    ui->listFollowUp->clear();
    for (const auto& date : record.getFollowUpDates()) {
        ui->listFollowUp->addItem(QString::fromStdString(date.toString()));
    }

    // Prescriptions
    ui->listPrescriptions->clear();
    medicines.clear();

    if (!record.getPrescriptions().empty()) {
        // Lấy prescription đầu tiên
        const Prescription& firstPrescription = record.getPrescriptions()[0];
        
        // Lấy danh sách thuốc từ prescription
        medicines = firstPrescription.getMedicines();
        
        // Hiển thị trong list
        for (const auto& medicine : medicines) {
            QString medInfo = QString("%1 - %2 (%3 lần/ngày, %4 ngày)")
                                .arg(QString::fromStdString(medicine.name))
                                .arg(QString::fromStdString(medicine.dosage))
                                .arg(medicine.frequency)
                                .arg(medicine.duration);
            ui->listPrescriptions->addItem(medInfo);
        }
    }
}

bool AddEditMedicalRecordDialog::validateForm() {
    // Check diagnosis
    QString diagnosis = ui->txtDiagnosis->text().trimmed();
    if (diagnosis.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập chẩn đoán!");
        ui->txtDiagnosis->setFocus();
        return false;
    }

    // Check blood pressure format
    QString bp = ui->txtBloodPressure->text().trimmed();
    if (!bp.isEmpty()) {
        QRegularExpression bpRegex("^\\d{2,3}/\\d{2,3}$");
        if (!bpRegex.match(bp).hasMatch()) {
            QMessageBox::warning(this, "Lỗi", "Huyết áp không đúng định dạng (VD: 120/80)");
            ui->txtBloodPressure->setFocus();
            return false;
        }
    }

    // Check dates
    QDate creation = ui->dateCreation->date();
    QDate updated = ui->dateLastUpdated->date();

    if (updated < creation) {
        QMessageBox::warning(this, "Lỗi", "Ngày cập nhật không thể trước ngày tạo!");
        return false;
    }

    return true;
}

MedicalRecord AddEditMedicalRecordDialog::getMedicalRecordData() const {
    int patientID = ui->cmbPatient->currentData().toInt();
    int doctorID = ui->cmbDoctor->currentData().toInt();

    qDebug() << "[DEBUG] Selected Patient ID:" << patientID;
    qDebug() << "[DEBUG] Selected Doctor ID:" << doctorID;

    QDate qCreation = ui->dateCreation->date();
    std::string creationDate = QString("%1/%2/%3")
                                   .arg(qCreation.day())
                                   .arg(qCreation.month())
                                   .arg(qCreation.year())
                                   .toStdString();

    QDate qUpdated = ui->dateLastUpdated->date();
    std::string lastUpdated = QString("%1/%2/%3")
                                  .arg(qUpdated.day())
                                  .arg(qUpdated.month())
                                  .arg(qUpdated.year())
                                  .toStdString();

    std::string diagnosis = ui->txtDiagnosis->text().trimmed().toStdString();
    std::string symptoms = ui->txtSymptoms->toPlainText().trimmed().toStdString();
    std::string testResults = ui->txtTestResults->toPlainText().trimmed().toStdString();
    std::string bloodPressure = ui->txtBloodPressure->text().trimmed().toStdString();
    int heartRate = ui->spinHeartRate->value();
    float bodyTemp = static_cast<float>(ui->spinBodyTemp->value());
    std::string treatment = ui->txtTreatment->toPlainText().trimmed().toStdString();
    std::string doctorNotes = ui->txtDoctorNotes->toPlainText().trimmed().toStdString();

    // Follow-up dates
    std::vector<Date> followUpDates;
    for (int i = 0; i < ui->listFollowUp->count(); ++i) {
        QString dateStr = ui->listFollowUp->item(i)->text();
        followUpDates.push_back(Date::fromString(dateStr.toStdString()));
    }

    // Prescriptions (empty for now, will be added separately)
    std::vector<Prescription> prescriptions;
    if (!medicines.empty()) {
        // Tạo prescription mới với danh sách thuốc
        Prescription prescription;

        // ✅ SỬA: Không set medicalRecordID ở đây, để MedicalRecordManager xử lý
        // MedicalRecordManager sẽ cập nhật ID sau khi tạo medical record

        prescription.setDate(lastUpdated);

        // Thêm tất cả thuốc vào prescription
        for (const auto& medicine : medicines) {
            prescription.addMedicine(
                medicine.name,
                medicine.dosage,
                medicine.frequency,
                medicine.duration,
                medicine.instruction
                );
        }

        // Thêm vào vector prescriptions
        prescriptions.push_back(prescription);
    }

    std::string changeHistory = lastUpdated;

    MedicalRecord record(patientID, doctorID, creationDate, lastUpdated,
                         diagnosis, symptoms, testResults, bloodPressure,
                         heartRate, bodyTemp, treatment, doctorNotes,
                         followUpDates, prescriptions, changeHistory);

    if (isEditMode) {
        record.setID(editingRecordID);
        qDebug() << "[DIALOG] getMedicalRecordData() - EDIT mode, ID:" << editingRecordID;
    } else {
        qDebug() << "[DIALOG] getMedicalRecordData() - ADD mode, new ID:" << record.getID();
    }

    return record;
}

void AddEditMedicalRecordDialog::setDialogTitle(const QString& title) {
    setWindowTitle(title);
}

// SLOTS
void AddEditMedicalRecordDialog::on_buttonBox_accepted() {
    if (validateForm()) {
        qDebug() << "[DIALOG] Form validated, accepting dialog";
        accept();
    }
}

void AddEditMedicalRecordDialog::on_buttonBox_rejected() {
    qDebug() << "[DIALOG] Dialog rejected/cancelled";
    reject();
}

void AddEditMedicalRecordDialog::on_btnAddFollowUp_clicked() {
    QDate selectedDate = ui->dateFollowUp->date();
    QString dateStr = QString("%1/%2/%3")
                          .arg(selectedDate.day())
                          .arg(selectedDate.month())
                          .arg(selectedDate.year());

    // Check duplicate
    for (int i = 0; i < ui->listFollowUp->count(); ++i) {
        if (ui->listFollowUp->item(i)->text() == dateStr) {
            QMessageBox::warning(this, "Cảnh báo", "Ngày tái khám này đã tồn tại!");
            return;
        }
    }

    ui->listFollowUp->addItem(dateStr);
    qDebug() << "[DIALOG] Added follow-up date:" << dateStr;
}

void AddEditMedicalRecordDialog::on_btnRemoveFollowUp_clicked() {
    QListWidgetItem* selected = ui->listFollowUp->currentItem();
    if (selected) {
        QString dateStr = selected->text();
        delete ui->listFollowUp->takeItem(ui->listFollowUp->row(selected));
        qDebug() << "[DIALOG] Removed follow-up date:" << dateStr;
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn ngày tái khám cần xóa!");
    }
}

void AddEditMedicalRecordDialog::on_btnAddPrescription_clicked() {
    // Mở dialog thêm thuốc
    AddMedicinePrescriptionDialog dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Prescription::Medicine newMedicine = dialog.getMedicineData();
            
            // Kiểm tra trùng tên thuốc
            for (const auto& medicine : medicines) {
                if (medicine.name == newMedicine.name) {
                    QMessageBox::warning(this, "Cảnh báo", 
                        QString("Thuốc '%1' đã có trong đơn!").arg(QString::fromStdString(newMedicine.name)));
                    return;
                }
            }
            
            // Thêm vào danh sách
            medicines.push_back(newMedicine);
            
            // Hiển thị trong list
            QString medInfo = QString("%1 - %2 (%3 lần/ngày, %4 ngày)")
                                  .arg(QString::fromStdString(newMedicine.name))
                                  .arg(QString::fromStdString(newMedicine.dosage))
                                  .arg(newMedicine.frequency)
                                  .arg(newMedicine.duration);
            ui->listPrescriptions->addItem(medInfo);
            
            qDebug() << "[DIALOG] Added medicine:" << QString::fromStdString(newMedicine.name);
            
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Lỗi", QString("Không thể thêm thuốc: %1").arg(e.what()));
        }
    }
}

void AddEditMedicalRecordDialog::on_btnRemovePrescription_clicked() {
    QListWidgetItem* selected = ui->listPrescriptions->currentItem();
    if (selected) {
        // Lấy tên thuốc từ item text (phần đầu trước dấu "-")
        QString itemText = selected->text();
        QString medicineName = itemText.split(" - ").first();

        // Xóa khỏi danh sách medicines
        medicines.erase(std::remove_if(medicines.begin(), medicines.end(),
                                       [&medicineName](const Prescription::Medicine& med) {
                                           return QString::fromStdString(med.name) == medicineName;
                                       }), medicines.end());

        // Xóa khỏi list widget
        delete ui->listPrescriptions->takeItem(ui->listPrescriptions->row(selected));

        qDebug() << "[DIALOG] Removed medicine:" << medicineName;
    } else {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn thuốc cần xóa!");
    }
}
void AddEditMedicalRecordDialog::on_btnSearchPatient_clicked() {
    QString searchText = ui->txtSearchPatient->text().trimmed();
    if (searchText.isEmpty()) {
        populatePatientCombo();
        return;
    }

    bool isNumber = false;
    int searchID = searchText.toInt(&isNumber);

    ui->cmbPatient->clear();
    const auto& patients = PatientManager::getInstance().getAllPatientsTable();

    for (const auto& pair : patients) {
        const Patient& p = pair.second;
        QString name = QString::fromStdString(p.getName()).toLower();

        bool match = false;
        if (isNumber && p.getID() == searchID) {
            match = true;
        } else if (!isNumber && name.contains(searchText.toLower())) {
            match = true;
        }

        if (match) {
            QString display = QString("ID: %1 - %2")
            .arg(p.getID())
                .arg(QString::fromStdString(p.getName()));
            ui->cmbPatient->addItem(display, p.getID());
        }
    }

    if (ui->cmbPatient->count() == 0) {
        QMessageBox::information(this, "Kết quả", "Không tìm thấy bệnh nhân phù hợp!");
        populatePatientCombo();
    }
}

void AddEditMedicalRecordDialog::on_btnSearchDoctor_clicked() {
    QString searchText = ui->txtSearchDoctor->text().trimmed();
    if (searchText.isEmpty()) {
        populateDoctorCombo();
        return;
    }

    bool isNumber = false;
    int searchID = searchText.toInt(&isNumber);

    ui->cmbDoctor->clear();
    const auto& doctors = DoctorManager::getInstance().getAllDoctors();

    for (const auto& pair : doctors) {
        const Doctor& d = pair.second;
        QString name = QString::fromStdString(d.getName()).toLower();
        QString spec = QString::fromStdString(d.getSpecialization()).toLower();

        bool match = false;
        if (isNumber && d.getID() == searchID) {
            match = true;
        } else if (!isNumber && (name.contains(searchText.toLower()) || spec.contains(searchText.toLower()))) {
            match = true;
        }

        if (match) {
            QString display = QString("ID: %1 - %2 (%3)")
            .arg(d.getID())
                .arg(QString::fromStdString(d.getName()))
                .arg(QString::fromStdString(d.getSpecialization()));
            ui->cmbDoctor->addItem(display, d.getID());
        }
    }

    if (ui->cmbDoctor->count() == 0) {
        QMessageBox::information(this, "Kết quả", "Không tìm thấy bác sĩ phù hợp!");
        populateDoctorCombo();
    }
}

void AddEditMedicalRecordDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddEditMedicalRecordDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AddEditMedicalRecordDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
