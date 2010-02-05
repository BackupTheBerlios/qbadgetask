#include "badgewindow.h"
#include "ui_badgewindow.h"

#include "dialogday.h"
#include <QDebug>
#include "badgedata.h"
#include "dialogsearch.h"
#include <QMessageBox>
#include "dialogconfigure.h"
#include "dialogstatistics.h"
#include "dialogtimer.h"
#include "dialogtask.h"

BadgeWindow::BadgeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BadgeWindow)
{
    ui->setupUi(this);
    ui->menuBar->addAction(ui->actionSearch);
    ui->menuBar->addAction(ui->actionTimer);
    ui->menuBar->addAction(ui->actionInfo);
    ui->menuBar->addAction(ui->actionConfigure);
#ifndef Q_WS_HILDON
    ui->menuBar->addAction(ui->actionClose);
#endif

    connect(ui->actionSearch, SIGNAL(triggered()), this, SLOT(search()));
    connect(ui->actionTimer, SIGNAL(triggered()), this, SLOT(timer()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(info()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(configure()));

    QSettings settings("Niqt", "qbadgetask");
    if ((days = settings.value("days", 0).toInt()) != 0) {
        workingTime =  settings.value("workingtime").toTime();

    }
    else {
        days = 127;
        workingTime.setHMS(8, 0, 0);
    }


    //showMaximized();
}



void BadgeWindow::selectedDay(QDate date)
{
    DialogDay day;
    BadgeData data;
    bool dayExist;
    TimeTable tt;

    tt.day = date;
    dayExist = data.getTimetable(tt);
    day.getTimes(tt, dayExist);
    //tt = day.getTimes(tt, dayExist);
    /*if (tt.wrong() != TimeTable::WRONG) {
        BadgeData data;
        data.setTimetable(tt);
    }*/

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
    QString own;

    //total = searchDialog.totalHours(ok);
    if (searchDialog.range(begin, end, own)) {

        DialogStatistics statistics;
        int overTime;

        BadgeData data;
        QMap <QString, QTime> activities;
        QSqlQueryModel *model;
        QString query;
        model = new QSqlQueryModel();

        ok = true;
        total = data.totalTime(begin, end, overTime, /*activities,*/ workingTime, days);

        query = "select attivita.attivita, (sum((strftime('%H',time) * 3600) + (strftime('%M',time) * 60)) / 3600),   ((sum((strftime('%H',time) * 3600) + (strftime('%M',time) * 60)) % 3600) /60) from attivita join task on (attivita.id=task.how) where task.day >= '"+ begin.toString("yyyy-MM-dd") + "' AND task.day <= '" + end.toString("yyyy-MM-dd") + "'";

        if (own.size() > 0)
             query += " AND task.own='" + own + "'";

        query += " group by attivita.attivita";

        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, tr("Task"));
        model->setHeaderData(1, Qt::Horizontal, tr("Hours"));
        model->setHeaderData(2, Qt::Horizontal, tr("Minutes"));



        statistics.showStatistics(begin, end, total, overTime, model);



        delete model;
/*
        hours.setNum(total.hour());
        minutes.setNum(total.minute());
        message = "Total time is: " + hours + " hours and " + minutes + " minutes";
        QMessageBox::information(this, tr("Badge"), tr(message.toLatin1()));*/

    }

}

void BadgeWindow::timer()
{
    DialogTimer dialogTimer;

    bool saveRequest;
    QString taskName;
    QTime elapsedTime;

    saveRequest = dialogTimer.getTaskInfo(taskName, elapsedTime);

    if (saveRequest) {
        DialogTask dialog;
        QSqlRelationalTableModel *taskModel;

        taskModel = new QSqlRelationalTableModel();

        taskModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        taskModel->setTable("task");
        qDebug() << "Day" << QDate::currentDate().toString("yyyy-MM-dd") << endl;
        taskModel->setFilter("day='" + QDate::currentDate().toString("yyyy-MM-dd") + "'");
        //taskModel.setEditStrategy(QSqlTableModel::OnFieldChange);
        taskModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        taskModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
        taskModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Elapsed"));
        //taskModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Attivita'"));
        taskModel->setRelation(3, QSqlRelation("attivita", "id", "attivita"));

        taskModel->select();

        // TODO: if current date don't exist create it


        dialog.openTask("Task", "Elapsed", taskModel, QDate::currentDate(), "task", elapsedTime, taskName);

        delete taskModel;
    }
}

void BadgeWindow::info()
{
    QMessageBox::aboutQt(this);
    QMessageBox::about(this, tr("Badge"), tr("(c) 2009 - 2010 Nicola De Filippo - nicola@nicoladefilippo.it\n"
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
