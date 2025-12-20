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

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnClose_clicked();

private:
    Ui::AppointmentDetailDialog *ui;
    const Appointment& m_appointment;
    bool m_dragging = false;
    QPoint m_dragPosition;

    void populateData();
};

#endif // APPOINTMENTDETAILDIALOG_H
