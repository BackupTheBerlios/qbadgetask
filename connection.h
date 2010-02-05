
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/


static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");



    db.setDatabaseName(QDir::homePath() + QDir::separator() + "badgetask");


    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This software needs SQLite support. Please"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }



    QSqlQuery query;

    query.exec("create table IF NOT EXISTS attivita (id int primary key, attivita varchar(30))");
    query.exec("create table IF NOT EXISTS days (daywork DATE primary key, enter DATETIME, exit DATETIME, beginfirst DATETIME, endfirst DATETIME,"
                "beginsecond DATETIME, endsecond DATETIME)");
    query.exec("create table IF NOT EXISTS todo (own varchar(30), id int primary key, day DATE, time DATETIME, how int, FOREIGN KEY(how) REFERENCES attivita(id))");
    query.exec("create table IF NOT EXISTS todonote (id int primary key, note blob, todoid int, FOREIGN KEY(todoid) REFERENCES todo(id))");
    query.exec("create table IF NOT EXISTS task (own varchar(30), id int primary key, day DATE, time DATETIME, how int)");
    query.exec("create table IF NOT EXISTS tasknote (id int primary key, note blob, taskid int, FOREIGN KEY(taskid) REFERENCES task(id))");

    return true;
}



#endif // CONNECTION_H
