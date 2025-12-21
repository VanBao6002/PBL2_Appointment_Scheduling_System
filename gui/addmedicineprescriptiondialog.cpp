#include "addmedicineprescriptiondialog.h"
#include "ui_addmedicineprescriptiondialog.h"
#include "config.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

AddMedicinePrescriptionDialog::AddMedicinePrescriptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMedicinePrescriptionDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("Thêm Thuốc Vào Đơn");
    setupUI();
    loadMedicinesFromJson();
    
    qDebug() << "[DIALOG] AddMedicinePrescriptionDialog opened";
}

AddMedicinePrescriptionDialog::~AddMedicinePrescriptionDialog()
{
    delete ui;
}

void AddMedicinePrescriptionDialog::setupUI()
{
    // Đặt placeholder
    ui->txtDosage->setPlaceholderText("VD: 500mg");
    ui->txtInstruction->setPlaceholderText("VD: Uống sau ăn");
    
    // Đặt giới hạn cho spin box
    ui->spinFrequency->setRange(1, 10);
    ui->spinFrequency->setValue(3);
    
    ui->spinDuration->setRange(1, 90);
    ui->spinDuration->setValue(7);
    
    // XÓA hoặc comment các dòng kết nối này vì đã có auto-connect
    // connect(ui->btnSearchMedicine, &QPushButton::clicked,
    //         this, &AddMedicinePrescriptionDialog::on_btnSearchMedicine_clicked);
    // connect(ui->cmbMedicine, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this, &AddMedicinePrescriptionDialog::on_cmbMedicine_currentIndexChanged);
    
}

void AddMedicinePrescriptionDialog::loadMedicinesFromJson()
{
    QFile file(Config::MEDICINES_PATH);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open medicines.json file";
        
        // Tạo danh sách mặc định nếu file không tồn tại
        MedicineInfo defaultMed;
        defaultMed.id = 0;
        defaultMed.name = "Paracetamol";
        defaultMed.category = "Giảm đau, hạ sốt";
        defaultMed.commonDosages = {"500mg"};
        allMedicines.append(defaultMed);
        
        ui->cmbMedicine->addItem("Paracetamol");
        return;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON format in medicines.json";
        return;
    }
    
    QJsonObject root = doc.object();
    QJsonArray medicinesArray = root["medicines"].toArray();
    
    allMedicines.clear();
    ui->cmbMedicine->clear();
    
    for (const QJsonValue& value : medicinesArray) {
        QJsonObject medObj = value.toObject();
        
        MedicineInfo med;
        med.id = medObj["id"].toInt();
        med.name = medObj["name"].toString();
        med.category = medObj["category"].toString();
        
        // Load common names
        QJsonArray commonNamesArray = medObj["common_names"].toArray();
        for (const QJsonValue& name : commonNamesArray) {
            med.commonNames.append(name.toString());
        }
        
        // Load common dosages
        QJsonArray dosagesArray = medObj["common_dosages"].toArray();
        for (const QJsonValue& dosage : dosagesArray) {
            med.commonDosages.append(dosage.toString());
        }
        
        allMedicines.append(med);
        ui->cmbMedicine->addItem(med.name);
    }
    
    filteredMedicines = allMedicines;
    qDebug() << "[MEDICINES] Loaded" << allMedicines.size() << "medicines";
}

void AddMedicinePrescriptionDialog::filterMedicines(const QString& searchText)
{
    filteredMedicines.clear();
    ui->cmbMedicine->clear();
    
    if (searchText.isEmpty()) {
        filteredMedicines = allMedicines;
    } else {
        QString lowerSearch = searchText.toLower();
        
        for (const MedicineInfo& med : allMedicines) {
            // Tìm theo tên chính
            if (med.name.toLower().contains(lowerSearch)) {
                filteredMedicines.append(med);
                continue;
            }
            
            // Tìm theo tên thông dụng
            bool foundInCommon = false;
            for (const QString& commonName : med.commonNames) {
                if (commonName.toLower().contains(lowerSearch)) {
                    foundInCommon = true;
                    break;
                }
            }
            
            // Tìm theo danh mục
            if (med.category.toLower().contains(lowerSearch)) {
                foundInCommon = true;
            }
            
            if (foundInCommon) {
                filteredMedicines.append(med);
            }
        }
    }
    
    // Thêm vào combobox
    for (const MedicineInfo& med : filteredMedicines) {
        ui->cmbMedicine->addItem(med.name);
    }
    
    if (filteredMedicines.isEmpty()) {
        ui->cmbMedicine->addItem("Không tìm thấy thuốc phù hợp");
        ui->cmbMedicine->setEnabled(false);
    } else {
        ui->cmbMedicine->setEnabled(true);
    }
}

Prescription::Medicine AddMedicinePrescriptionDialog::getMedicineData() const
{
    Prescription::Medicine medicine;
    
    if (ui->cmbMedicine->currentIndex() >= 0 && !filteredMedicines.isEmpty()) {
        int idx = ui->cmbMedicine->currentIndex();
        if (idx < filteredMedicines.size()) {
            medicine.name = filteredMedicines.at(idx).name.toStdString();
        }
    } else {
        medicine.name = ui->cmbMedicine->currentText().trimmed().toStdString();
    }
    
    medicine.dosage = ui->txtDosage->text().trimmed().toStdString();
    medicine.frequency = ui->spinFrequency->value();
    medicine.duration = ui->spinDuration->value();
    medicine.instruction = ui->txtInstruction->toPlainText().trimmed().toStdString();
    
    return medicine;
}

void AddMedicinePrescriptionDialog::setMedicineData(const Prescription::Medicine& medicine)
{
    // Tìm thuốc trong danh sách
    int index = -1;
    for (int i = 0; i < allMedicines.size(); ++i) {
        if (allMedicines.at(i).name.toStdString() == medicine.name) {
            index = i;
            break;
        }
    }
    
    if (index >= 0) {
        ui->cmbMedicine->setCurrentIndex(index);
    } else {
        ui->cmbMedicine->setCurrentText(QString::fromStdString(medicine.name));
    }
    
    ui->txtDosage->setText(QString::fromStdString(medicine.dosage));
    ui->spinFrequency->setValue(medicine.frequency);
    ui->spinDuration->setValue(medicine.duration);
    ui->txtInstruction->setPlainText(QString::fromStdString(medicine.instruction));
}

bool AddMedicinePrescriptionDialog::validateForm()
{
    QString medicineName = ui->cmbMedicine->currentText().trimmed();
    if (medicineName.isEmpty() || medicineName == "Không tìm thấy thuốc phù hợp") {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn hoặc nhập tên thuốc!");
        ui->cmbMedicine->setFocus();
        return false;
    }
    
    QString dosage = ui->txtDosage->text().trimmed();
    if (dosage.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập liều lượng!");
        ui->txtDosage->setFocus();
        return false;
    }
    
    return true;
}

void AddMedicinePrescriptionDialog::on_buttonBox_accepted()
{
    if (validateForm()) {
        qDebug() << "[DIALOG] Medicine form validated, accepting dialog";
        accept();
    }
}

void AddMedicinePrescriptionDialog::on_buttonBox_rejected()
{
    qDebug() << "[DIALOG] Medicine dialog rejected/cancelled";
    reject();
}

void AddMedicinePrescriptionDialog::on_btnSearchMedicine_clicked()
{
    QString searchText = ui->txtSearchMedicine->text().trimmed();
    filterMedicines(searchText);
    
    // Hiển thị thông tin nếu có kết quả
    if (!filteredMedicines.isEmpty() && ui->cmbMedicine->currentIndex() >= 0) {
        int idx = ui->cmbMedicine->currentIndex();
        const MedicineInfo& med = filteredMedicines.at(idx);
        
        // Tự động điền liều lượng phổ biến nếu có
        if (!med.commonDosages.isEmpty() && ui->txtDosage->text().isEmpty()) {
            ui->txtDosage->setText(med.commonDosages.first());
        }
    }
}

void AddMedicinePrescriptionDialog::on_cmbMedicine_currentIndexChanged(int index)
{
    if (index >= 0 && index < filteredMedicines.size()) {
        const MedicineInfo& med = filteredMedicines.at(index);
        
        // Hiển thị thông tin về thuốc
        QString info = QString("Danh mục: %1\nTên khác: %2")
                           .arg(med.category)
                           .arg(med.commonNames.join(", "));
        
        // Tự động điền liều lượng phổ biến nếu có
        if (!med.commonDosages.isEmpty() && ui->txtDosage->text().isEmpty()) {
            ui->txtDosage->setText(med.commonDosages.first());
        }
        
        qDebug() << "[MEDICINE] Selected:" << med.name;
    }
}

void AddMedicinePrescriptionDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AddMedicinePrescriptionDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AddMedicinePrescriptionDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}
