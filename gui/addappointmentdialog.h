#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include <string>
#include <QPushButton>
#include <QList>
#include <QString>
#include "appointment.h"
#include "doctorManager.h"
#include "patientManager.h"
#include "patient.h"
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
    void on_searchButton_clicked();
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();

private:
    bool isDoctorValid(int doctorID) const;
    bool isPatientValid(int patientID) const;
    bool isPatientExisted(const std::string& CCCD);
    void updateAvailableTimeSlot(const QStringList& allSlots, const QSet<QString>& occupiedSlots);
    void updateAvailableCalendarDaysAndTimeSlots(int doctorID);
    void populateDoctorCards();

private:
    Ui::AddAppointmentDialog *ui;
    Patient selectedPatient;
    QList<QPushButton*> timeSlotButtons;
    QString selectedTimeSlot;
    QButtonGroup* doctorButtonGroup = nullptr;
    int selectedDoctorID = -1;

};

#endif // ADDAPPOINTMENTDIALOG_H
