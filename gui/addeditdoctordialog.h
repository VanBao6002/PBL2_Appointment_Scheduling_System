#ifndef ADDEDITDOCTORDIALOG_H
#define ADDEDITDOCTORDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStringList>
#include "doctor.h"
#include "core.h"
namespace Ui {
class AddEditDoctorDialog;
}

class AddEditDoctorDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THÊM MỚI
    explicit AddEditDoctorDialog(QWidget *parent = nullptr);

    // Constructor cho CHỈNH SỬA (truyền vào doctor cần edit)
    explicit AddEditDoctorDialog(QWidget *parent, const Doctor& doctorToEdit);

    ~AddEditDoctorDialog();

    // Lấy dữ liệu từ form
    Doctor getDoctorData() const;

    // Đặt tiêu đề dialog
    void setDialogTitle(const QString& title);

    WorkingSchedule getWorkingScheduleFromTable() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
     void onAddWorkingScheduleClicked();
     void onDeleteWorkingScheduleClicked();

private:
    Ui::AddEditDoctorDialog *ui;

    bool isEditMode;
    int editingDoctorID;

    // Setup UI components
    void setupComboBoxes();
    void setupValidators();
    void setupWorkingScheduleTable();
    // Load dữ liệu khi edit
    void loadDoctorData(const Doctor& doctor);
    void loadWorkingSchedule(const WorkingSchedule& schedule);
    // Validate form
    bool validateForm();
};

#endif // ADDEDITDOCTORDIALOG_H
