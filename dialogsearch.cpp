#include "dialogsearch.h"
#include "ui_dialogsearch.h"
#include "dialogcalendar.h"
#include <QMessageBox>
#include "badgedata.h"

DialogSearch::DialogSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);
    //move(, 0);
    //showMaximized();
}

DialogSearch::~DialogSearch()
{
    delete ui;
}

void DialogSearch::changeEvent(QEvent *e)
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

void DialogSearch::start()
{
    DialogCalendar cal;
    ui->dateEditStart->setDate(cal.getDate(ui->dateEditStart->date()));
}

void DialogSearch::end()
{
    DialogCalendar cal;
    ui->dateEditEnd->setDate(cal.getDate(ui->dateEditEnd->date()));
}

/*QTime DialogSearch::totalHours(bool &ok)
{

    if (this->exec() == QDialog::Accepted) {
        QTime workingTime;
        QTime overTime;
        int days;
        BadgeData data;
        QMap <QString, QTime> activities;
        ok = true;
        return data.totalTime(ui->dateEditStart->date(), ui->dateEditEnd->date(), overTime, activities, workingTime, days);
    }
    ok = false;


    return QTime(0, 0, 0, 0);
}*/

bool DialogSearch::range(QDate &begin, QDate &end)
{
    QDate start;

    // Default search from 1 of the current month to today

    start.setYMD(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    ui->dateEditStart->setDate(start);
    ui->dateEditEnd->setDate(QDate::currentDate());

    if (this->exec() == QDialog::Accepted) {
        /*QTime workingTime;
        QTime overTime;
        int days;
        BadgeData data;
        QMap <QString, QTime> activities;
        ok = true;*/
        begin = ui->dateEditStart->date();
        end = ui->dateEditEnd->date();
        return true;
    }
    return false;



}

void DialogSearch::accept()
{
    if (ui->dateEditEnd->date() < ui->dateEditStart->date()) {
        QMessageBox::warning(this, tr("Badge"),
                                        tr("End date must to be major or equal of start date.\n"));
    }
    else
        QDialog::accept();
}
