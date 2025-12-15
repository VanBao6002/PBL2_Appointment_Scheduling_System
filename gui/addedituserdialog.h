#ifndef ADDEDITUSERDIALOG_H
#define ADDEDITUSERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include <QDate>
#include "user.h"

namespace Ui {
class AddEditUserDialog;
}

class AddEditUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditUserDialog(QWidget *parent = nullptr, const User& user = User());
    ~AddEditUserDialog();

    User getUserData() const;
    void setDialogTitle(const QString& title);
    void populateUI(const User& user); // ✅ THÊM HÀN

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditUserDialog *ui;
    User currentUser; // ✅ THÊM BIẾN ĐỂ LƯU USER HIỆN TẠI
    bool m_dragging = false;
    QPoint m_dragPosition;

    void setupComboBoxes();
    void setupDatePicker(); // ✅ THÊM HÀM
    bool validateForm();
};

#endif // ADDEDITUSERDIALOG_H
