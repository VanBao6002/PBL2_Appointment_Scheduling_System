#ifndef USERDETAILDIALOG_H
#define USERDETAILDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "user.h"

namespace Ui {
class UserDetailDialog;
}

class UserDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDetailDialog(const User& user, QWidget *parent = nullptr);
    ~UserDetailDialog();

    bool shouldEdit() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_btnClose_clicked();
    void on_btnEdit_clicked();

private:
    Ui::UserDetailDialog *ui;
    User m_user;
    bool m_dragging;
    QPoint m_dragPosition;
    bool editRequested;

    void populateData();
};

#endif // USERDETAILDIALOG_H
