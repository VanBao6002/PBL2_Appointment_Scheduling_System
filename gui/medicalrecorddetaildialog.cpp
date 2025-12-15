#include "medicalrecorddetaildialog.h"
#include "ui_medicalrecorddetaildialog.h"

medicalrecorddetaildialog::medicalrecorddetaildialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::medicalrecorddetaildialog)
{
    ui->setupUi(this);
}

medicalrecorddetaildialog::~medicalrecorddetaildialog()
{
    delete ui;
}
