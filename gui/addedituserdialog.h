#ifndef ADDEDITUSERDIALOG_H
#define ADDEDITUSERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include "user.h"

namespace Ui {
class AddEditUserDialog;
}

class AddEditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditUserDialog(QWidget *parent = nullptr);
    ~AddEditUserDialog();

    User getUserData() const;
    void setDialogTitle(const QString& title);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditUserDialog *ui;

    void setupComboBoxes();
    bool validateForm();
};

#endif // ADDEDITUSERDIALOG_H
