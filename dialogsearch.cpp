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

QTime DialogSearch::totalHours(bool &ok)
{

    if (this->exec() == QDialog::Accepted) {
        QTime workingTime;
        QTime overTime;
        int days;
        BadgeData data;

        ok = true;
        return data.totalTime(ui->dateEditStart->date(), ui->dateEditEnd->date(), overTime, workingTime, days);
    }
    ok = false;


    return QTime(0, 0, 0, 0);
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
