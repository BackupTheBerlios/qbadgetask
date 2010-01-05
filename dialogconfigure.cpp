#include "dialogconfigure.h"
#include "ui_dialogconfigure.h"

DialogConfigure::DialogConfigure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfigure)
{
    ui->setupUi(this);
}


bool DialogConfigure::openConfigure(unsigned int &days, QTime &time)
{
    if (days & 1)
        ui->checkBoxMon->setChecked(true);
    if (days & 2)
        ui->checkBoxTue->setChecked(true);
    if (days & 4)
        ui->checkBoxWed->setChecked(true);
    if (days & 8)
        ui->checkBoxThu->setChecked(true);
    if (days & 16)
        ui->checkBoxFri->setChecked(true);
    if (days & 32)
        ui->checkBoxSat->setChecked(true);
    if (days & 64)
        ui->checkBoxSun->setChecked(true);

    ui->timeEdit->setTime(time);

    if (this->exec() == QDialog::Accepted) {
        time = ui->timeEdit->time();

        days = 0;


        if (ui->checkBoxMon->isChecked())
            days += 1;
        if (ui->checkBoxTue->isChecked())
            days += 2;
        if (ui->checkBoxWed->isChecked())
            days += 4;
        if (ui->checkBoxThu->isChecked())
            days += 8;
        if (ui->checkBoxFri->isChecked())
            days += 16;
        if (ui->checkBoxSat->isChecked())
            days += 32;
        if (ui->checkBoxSun->isChecked())
            days += 64;


        return true;
    }
    return false;
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
