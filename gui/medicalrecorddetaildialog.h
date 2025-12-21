#ifndef MEDICALRECORDDETAILDIALOG_H
#define MEDICALRECORDDETAILDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "medicalRecord.h"

namespace Ui {
class MedicalRecordDetailDialog;
}

class MedicalRecordDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedicalRecordDetailDialog(const MedicalRecord& record, QWidget *parent = nullptr);
    ~MedicalRecordDetailDialog();

    bool shouldEdit() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnClose_clicked();
    void on_btnEdit_clicked();

private:
    Ui::MedicalRecordDetailDialog *ui;
    MedicalRecord m_record;
    bool m_dragging;
    QPoint m_dragPosition;
    bool editRequested;

    void populateData();
};

#endif // MEDICALRECORDDETAILDIALOG_H
