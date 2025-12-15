#ifndef MEDICALRECORDDETAILDIALOG_H
#define MEDICALRECORDDETAILDIALOG_H

#include <QWidget>

namespace Ui {
class medicalrecorddetaildialog;
}

class medicalrecorddetaildialog : public QWidget
{
    Q_OBJECT

public:
    explicit medicalrecorddetaildialog(QWidget *parent = nullptr);
    ~medicalrecorddetaildialog();

private:
    Ui::medicalrecorddetaildialog *ui;
};

#endif // MEDICALRECORDDETAILDIALOG_H
