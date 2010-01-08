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
    QString hours;
    QString minutes;
    QString hoursOvertime;
    QString minutesOvertime;

    hours.setNum((total / 3600));
    minutes.setNum((total % 3600) / 60);
    hoursOvertime.setNum((overTime / 3600));
    minutesOvertime.setNum((overTime % 3600) / 60);

    ui->tableView->setModel(model);
    ui->labelPeriod->setText("From " + begin.toString("yyyy-MM-dd") + " to " + end.toString("yyyy-MM-dd"));
    ui->label->setText("Total working time is " + hours + " hours and " + minutes + " minutes");
    ui->label_2->setText("Total overtime is " + hoursOvertime + " hours and " + minutesOvertime + " minutes");

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
