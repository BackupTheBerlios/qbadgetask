#include "badgewindow.h"
#include "ui_badgewindow.h"

#include "dialogday.h"
#include <QDebug>
#include "badgedata.h"
#include "dialogsearch.h"
#include <QMessageBox>
#include "dialogconfigure.h"
#include "dialogstatistics.h"


BadgeWindow::BadgeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BadgeWindow)
{
    ui->setupUi(this);
    ui->menuBar->addAction(ui->actionSearch);
    ui->menuBar->addAction(ui->actionInfo);
    ui->menuBar->addAction(ui->actionConfigure);
#ifndef Q_WS_HILDON
    ui->menuBar->addAction(ui->actionClose);
#endif

    connect(ui->actionSearch, SIGNAL(triggered()), this, SLOT(search()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(info()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(configure()));

    QSettings settings("Niqt", "qbadgetask");
    if ((days = settings.value("days", 0).toInt()) != 0) {
        workingTime =  settings.value("workingtime").toTime();

    }


    //showMaximized();
}



void BadgeWindow::selectedDay(QDate date)
{
    DialogDay day;
    BadgeData data;

    TimeTable tt;
    tt.day = date;
    data.getTimetable(tt);
    tt = day.getTimes(tt);
    if (tt.wrong() != TimeTable::WRONG) {
        BadgeData data;
        data.setTimetable(tt);
    }

}


void BadgeWindow::configure()
{
    DialogConfigure configure;



    if (configure.openConfigure(days, workingTime)) {
        QSettings settings("Niqt", "qbadgetask");
        settings.setValue("days", days);
        settings.setValue("workingtime", workingTime);


    }

}

void BadgeWindow::search()
{
    DialogSearch searchDialog;
    int total;
    QString message;
    QString hours;
    QString minutes;
    QDate begin;
    QDate end;
    bool ok;

    //total = searchDialog.totalHours(ok);
    if (searchDialog.range(begin, end)) {
        QTime workingTime;
        DialogStatistics statistics;
        int overTime;
        int days;
        BadgeData data;
        QMap <QString, QTime> activities;
        QSqlQueryModel *model;

        model = new QSqlQueryModel(this);

        ok = true;
        total = data.totalTime(begin, end, overTime, /*activities,*/ workingTime, days);

        statistics.showStatistics(begin, end, total, overTime, model);

        delete model;
/*
        hours.setNum(total.hour());
        minutes.setNum(total.minute());
        message = "Total time is: " + hours + " hours and " + minutes + " minutes";
        QMessageBox::information(this, tr("Badge"), tr(message.toLatin1()));*/

    }

}

void BadgeWindow::info()
{
    QMessageBox::aboutQt(this);
    QMessageBox::about(this, tr("Badge"), tr("(c) 2009 Nicola De Filippo - nicola@nicoladefilippo.it\n"
                         "This program is licensed to you under terms of the GNU General Public\n"
                        "License Version 2 as published by Free Software Foundation. This gives\n"
                        "you legal permission to copy, distribute and/or modify this software under\n"
                        "certain conditions. For details, see the file 'COPYING' that came with this\n"
                        "software distribution. If you did not get the file, send email to author.\n\n"
                        "The program is provided AS IS with No WARRANTY OF ANY KIND,\n"
                        "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND\n"
                        "FITNESS FOR PARTICULAR PURPOSE"));
}

BadgeWindow::~BadgeWindow()
{
    delete ui;
}
