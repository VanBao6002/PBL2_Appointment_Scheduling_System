#pragma once

#include <QDialog>
#include "prescription.h"
#include <QMouseEvent>

namespace Ui {
class AddMedicinePrescriptionDialog;
}

class AddMedicinePrescriptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMedicinePrescriptionDialog(QWidget *parent = nullptr);
    ~AddMedicinePrescriptionDialog();

    Prescription::Medicine getMedicineData() const;
    void setMedicineData(const Prescription::Medicine& medicine);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_btnSearchMedicine_clicked();  // THÊM SLOT TÌM KIẾM
    void on_cmbMedicine_currentIndexChanged(int index);  // THÊM SLOT KHI CHỌN THUỐC

private:
    Ui::AddMedicinePrescriptionDialog *ui;
    void setupUI();
    bool validateForm();
    void loadMedicinesFromJson();  // THÊM HÀM LOAD THUỐC
    void filterMedicines(const QString& searchText);  // THÊM HÀM LỌC

    bool m_dragging = false;
    QPoint m_dragPosition;
    
    struct MedicineInfo {  // THÊM STRUCT ĐỂ LƯU THÔNG TIN THUỐC
        int id;
        QString name;
        QStringList commonNames;
        QString category;
        QStringList commonDosages;
    };
    
    QVector<MedicineInfo> allMedicines;  // DANH SÁCH TẤT CẢ THUỐC
    QVector<MedicineInfo> filteredMedicines;  // DANH SÁCH ĐÃ LỌC
};
