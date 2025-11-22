#ifndef ADDEDITDOCTORDIALOG_H
#define ADDEDITDOCTORDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStringList>
#include "doctor.h"

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

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditDoctorDialog *ui;

    bool isEditMode;
    int editingDoctorID;

    // Setup UI components
    void setupComboBoxes();
    void setupValidators();

    // Load specializations từ JSON file
    QStringList loadSpecializationsFromJson();

    // Load dữ liệu khi edit
    void loadDoctorData(const Doctor& doctor);

    // Validate form
    bool validateForm();
};

#endif // ADDEDITDOCTORDIALOG_H
