#ifndef ADDEDITPATIENTDIALOG_H
#define ADDEDITPATIENTDIALOG_H

#include <QDialog>
#include "patient.h"
#include <QMouseEvent>

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

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditPatientDialog *ui;
    Patient currentPatient;
    void populateUI(const Patient& patient);
    void setupDatePicker();
    bool m_dragging;
    QPoint m_dragPosition;
};

#endif // ADDEDITPATIENTDIALOG_H
