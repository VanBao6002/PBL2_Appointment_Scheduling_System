#ifndef APPOINTMENTDETAILDIALOG_H
#define APPOINTMENTDETAILDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "appointment.h"

namespace Ui {
class AppointmentDetailDialog;
}

class AppointmentDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppointmentDetailDialog(const Appointment& appointment, QWidget *parent = nullptr);
    ~AppointmentDetailDialog();

    // ✅ Method to check if edit was requested
    bool shouldEdit() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnClose_clicked();
    void on_btnEdit_clicked();

private:
    Ui::AppointmentDetailDialog *ui;
    Appointment m_appointment;
    int appointmentID;
    bool editRequested;
    
    // For window dragging
    bool m_dragging = false;
    QPoint m_dragPosition;

    void populateData();
    void applyStyles();
};

#endif // APPOINTMENTDETAILDIALOG_H