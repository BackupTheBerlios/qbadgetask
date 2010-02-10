#include "dialogstatistics.h"
#include "ui_dialogstatistics.h"
#include "dialogdetail.h"
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QSqlQuery>

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

    _begin = begin.toString("yyyy-MM-dd");
    _end = end.toString("yyyy-MM-dd");


    ui->tableView->setModel(model);
    ui->labelPeriod->setText("From " + begin.toString("yyyy-MM-dd") + " to " + end.toString("yyyy-MM-dd"));
    ui->label->setText("Total working time is " + hours + " hours and " + minutes + " minutes");
    ui->label_2->setText("Total overtime is " + hoursOvertime + " hours and " + minutesOvertime + " minutes");

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();

    if (this->exec() == QDialog::Accepted) {

    }
}

void DialogStatistics::details(QModelIndex index)
{
    DialogDetail detail;
    //QSqlRelationalTableModelQSqlQueryModel *model;
    QString day;


    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("select task.own, task.day, task.time, tasknote.note from attivita join task on (attivita.id=task.how) "
                    "left join tasknote on (tasknote.taskid=task.id) where task.day >='" + _begin + "' AND task.day <= '" + _end + "' AND attivita='" + index.data().toString() + "'");



    model->setHeaderData(0, Qt::Horizontal, tr("Project"));
    model->setHeaderData(1, Qt::Horizontal, tr("Day"));
    model->setHeaderData(2, Qt::Horizontal, tr("Time"));
    model->setHeaderData(3, Qt::Horizontal, tr("Note"));

    detail.showDetails(model, day);

    delete model;
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
