#ifndef ADDEDITMEDICALRECORDDIALOG_H
#define ADDEDITMEDICALRECORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QListWidgetItem>
#include <vector>
#include <string>
#include <QMouseEvent>
#include "medicalRecord.h"
#include "patient.h"
#include "doctor.h"
#include "patientManager.h"
#include "doctorManager.h"

namespace Ui {
class AddEditMedicalRecordDialog;
}

class AddEditMedicalRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditMedicalRecordDialog(QWidget *parent = nullptr);
    explicit AddEditMedicalRecordDialog(QWidget *parent, const MedicalRecord& recordToEdit);
    ~AddEditMedicalRecordDialog();

    MedicalRecord getMedicalRecordData() const;
    void setDialogTitle(const QString& title);
        // Getter cho danh sách thuốc
    const std::vector<Prescription::Medicine>& getMedicines() const { return medicines; }
    
    // Setter cho danh sách thuốc (nếu cần)
    void setMedicines(const std::vector<Prescription::Medicine>& meds) { medicines = meds; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_btnAddFollowUp_clicked();
    void on_btnRemoveFollowUp_clicked();
    void on_btnAddPrescription_clicked();
    void on_btnRemovePrescription_clicked();
    void on_btnSearchPatient_clicked();
    void on_btnSearchDoctor_clicked();

private:
    Ui::AddEditMedicalRecordDialog *ui;
    bool isEditMode;
    int editingRecordID;
    bool m_dragging;
    QPoint m_dragPosition;
    std::vector<Prescription::Medicine> medicines;

    void setupUI();
    void setupValidators();
    void loadMedicalRecordData(const MedicalRecord& record);
    bool validateForm();
    void populatePatientCombo();
    void populateDoctorCombo();
    void setupConnections();
};

#endif // ADDEDITMEDICALRECORDDIALOG_H
