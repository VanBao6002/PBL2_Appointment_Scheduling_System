#ifndef ADDEDITMEDICALRECORDDIALOG_H
#define ADDEDITMEDICALRECORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QListWidgetItem>
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

    void setupUI();
    void setupValidators();
    void loadMedicalRecordData(const MedicalRecord& record);
    bool validateForm();
    void populatePatientCombo();
    void populateDoctorCombo();
};

#endif // ADDEDITMEDICALRECORDDIALOG_H
