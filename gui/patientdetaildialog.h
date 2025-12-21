#ifndef PATIENTDETAILDIALOG_H
#define PATIENTDETAILDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "patient.h"

namespace Ui {
class PatientDetailDialog;
}

class PatientDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientDetailDialog(const Patient& patient, QWidget *parent = nullptr);
    ~PatientDetailDialog();

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
    Ui::PatientDetailDialog *ui;
    Patient m_patient;
    int patientID;
    bool editRequested;

    // For window dragging
    bool m_dragging;
    QPoint m_dragPosition;

    void populateData();
};

#endif // PATIENTDETAILDIALOG_H
