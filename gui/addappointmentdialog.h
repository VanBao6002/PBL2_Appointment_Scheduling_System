#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include "appointment.h"
#include "doctorManager.h"
#include "patientManager.h"

namespace Ui {
class AddAppointmentDialog;
}

class AddAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAppointmentDialog(QWidget *parent = nullptr);
    ~AddAppointmentDialog();
    Appointment getAppointmentData() const;
    void setupStatusComboBox();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    bool isDoctorValid(int doctorID) const;
    bool isPatientValid(int patientID) const;

private:
    Ui::AddAppointmentDialog *ui;
};

#endif // ADDAPPOINTMENTDIALOG_H
