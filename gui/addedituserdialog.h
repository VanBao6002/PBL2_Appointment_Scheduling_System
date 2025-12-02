#ifndef ADDEDITUSERDIALOG_H
#define ADDEDITUSERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>  // Thêm include này
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

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddEditUserDialog *ui;
    bool m_dragging = false;
    QPoint m_dragPosition;

    void setupComboBoxes();
    bool validateForm();
};

#endif // ADDEDITUSERDIALOG_H
