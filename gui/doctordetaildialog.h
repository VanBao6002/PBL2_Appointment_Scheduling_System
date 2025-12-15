#ifndef DOCTORDETAILDIALOG_H
#define DOCTORDETAILDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "doctor.h"

namespace Ui {
class DoctorDetailDialog;
}

class DoctorDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorDetailDialog(const Doctor& doctor, QWidget *parent = nullptr);
    ~DoctorDetailDialog();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnClose_clicked();

private:
    Ui::DoctorDetailDialog *ui;
    Doctor m_doctor;
    bool m_dragging;
    QPoint m_dragPosition;

    void setupWorkingScheduleTable();
    void populateData();
    void loadWorkingSchedule();
};

#endif // DOCTORDETAILDIALOG_H
