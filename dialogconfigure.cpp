#include "dialogconfigure.h"
#include "ui_dialogconfigure.h"

DialogConfigure::DialogConfigure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfigure)
{
    ui->setupUi(this);
}

DialogConfigure::~DialogConfigure()
{
    delete ui;
}

void DialogConfigure::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
