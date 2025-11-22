#ifndef ADDEDITPATIENTDIALOG_H
#define ADDEDITPATIENTDIALOG_H

#include <QDialog>
#include "patient.h"

namespace Ui {
class AddEditPatientDialog;
}

class AddEditPatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditPatientDialog(QWidget *parent = nullptr, const Patient& patient = Patient());
    ~AddEditPatientDialog();
    void setDialogTitle(const QString& title);
    Patient getPatientData() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditPatientDialog *ui;
    Patient currentPatient;
    void populateUI(const Patient& patient);
    void setupDatePicker();
};

#endif // ADDEDITPATIENTDIALOG_H
