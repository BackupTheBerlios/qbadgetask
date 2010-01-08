#include "dialogstatistics.h"
#include "ui_dialogstatistics.h"

DialogStatistics::DialogStatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatistics)
{
    ui->setupUi(this);
}

void DialogStatistics::showStatistics(QDate begin, QDate end, int total, int overTime, QSqlQueryModel *model)
{
    if (this->exec() == QDialog::Accepted) {

    }
}

DialogStatistics::~DialogStatistics()
{
    delete ui;
}

void DialogStatistics::changeEvent(QEvent *e)
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
