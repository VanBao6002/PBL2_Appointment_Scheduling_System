#ifndef ADDAPPOINTMENTDIALOG_H
#define ADDAPPOINTMENTDIALOG_H

#include <QDialog>
#include <qcombobox.h>
#include <string>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QMouseEvent>
#include <QDate>
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
    explicit AddAppointmentDialog(const Appointment& appointment, QWidget *parent = nullptr);
    ~AddAppointmentDialog();

    Appointment getAppointmentData() const;
    void setupStatusComboBox();
    void setDialogTitle(const QString& title);
    void setEditMode(bool isEdit);
    bool isEditMode() const { return editMode; }

private slots:
    void on_searchButton_clicked();
    void on_cancelButton_clicked();
    void on_confirmButton_clicked();
    void on_cmbStatus_currentTextChanged(const QString &text);

private:
    bool isDoctorValid(int doctorID) const;
    bool isPatientValid(int patientID) const;
    bool isPatientExisted(const std::string& CCCD);
    void updateAvailableTimeSlot(const QStringList& allSlots, const QSet<QString>& occupiedSlots);
    void updateAvailableCalendarDaysAndTimeSlots(int doctorID);
    void populateDoctorCards();
    void loadAppointmentData(const Appointment& appointment);
    void setupConnections();
    void applyStyle();
    void setupSpecializationComboBox();
    void updateConfirmationPage();

private:
    Ui::AddAppointmentDialog *ui;
    int selectedPatientID = -1;
    int selectedDoctorID = -1;
    QList<QPushButton*> timeSlotButtons;
    QString selectedTimeSlot;
    QButtonGroup* doctorButtonGroup = nullptr;
    bool isNewPatient;
    QString selectedStatus;
    QComboBox* cmbStatus;
    bool editMode = false;
    int appointmentID = -1;
    bool m_dragging;
    QPoint m_dragPosition;
    QDate lastSelectedDate;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // ADDAPPOINTMENTDIALOG_H
