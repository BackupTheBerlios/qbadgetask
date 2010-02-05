#include "dialogday.h"
#include "ui_dialogday.h"
#include <QDebug>
#include "dialogtask.h"
#include "badgedata.h"
#include "timetable.h"
#include <QMessageBox>

DialogDay::DialogDay(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogDay)
{
    m_ui->setupUi(this);
    taskModel = new QSqlRelationalTableModel(this);
}

DialogDay::~DialogDay()
{
    delete m_ui;
}

void DialogDay::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogDay::enableFirst(bool enable)
{
    m_ui->timeEditFBegin->setEnabled(enable);
    m_ui->timeEditEndF->setEnabled(enable);
}

void DialogDay::enableSecond(bool enable)
{
    m_ui->timeEditSBegin->setEnabled(enable);
    m_ui->timeEditEndS->setEnabled(enable);
}


TimeTable DialogDay::getTimes(TimeTable timetable)
{
    day = timetable.day;
    m_ui->timeEditEntrance->setTime(timetable.entrance);
    m_ui->timeEditExit->setTime(timetable.exit);
    m_ui->checkBoxFirst->setChecked(false);
    m_ui->checkBoxSecond->setChecked(false);

    if (timetable.firstPause != timetable.endFirstPause) {
        m_ui->checkBoxFirst->setChecked(true);
        m_ui->timeEditFBegin->setTime(timetable.firstPause);
        m_ui->timeEditFBegin->setEnabled(true);
        m_ui->timeEditEndF->setTime(timetable.endFirstPause);
        m_ui->timeEditEndF->setEnabled(true);
    }
    if (timetable.secondPause != timetable.endSecondPause) {
        m_ui->checkBoxSecond->setChecked(true);
        m_ui->timeEditSBegin->setTime(timetable.secondPause);
        m_ui->timeEditSBegin->setEnabled(true);
        m_ui->timeEditEndS->setTime(timetable.endSecondPause);
        m_ui->timeEditEndS->setEnabled(true);
    }



    if (this->exec() == QDialog::Accepted) {

            timetable.entrance = m_ui->timeEditEntrance->time();
            timetable.exit = m_ui->timeEditExit->time();
        if (m_ui->checkBoxFirst->isChecked()) {
            timetable.firstPause = m_ui->timeEditFBegin->time();
            timetable.endFirstPause = m_ui->timeEditEndF->time();
        }
        if (m_ui->checkBoxSecond->isChecked()) {
            timetable.secondPause = m_ui->timeEditSBegin->time();
            timetable.endSecondPause = m_ui->timeEditEndS->time();
        }
        return timetable;
    }
    timetable.setWrong(true);
    return timetable;
}


void DialogDay::getTimes(TimeTable timetable, bool dayExist)
{
    day = timetable.day;
    m_ui->timeEditEntrance->setTime(timetable.entrance);
    m_ui->timeEditExit->setTime(timetable.exit);
    m_ui->checkBoxFirst->setChecked(false);
    m_ui->checkBoxSecond->setChecked(false);

    if (timetable.firstPause != timetable.endFirstPause) {
        m_ui->checkBoxFirst->setChecked(true);
        m_ui->timeEditFBegin->setTime(timetable.firstPause);
        m_ui->timeEditFBegin->setEnabled(true);
        m_ui->timeEditEndF->setTime(timetable.endFirstPause);
        m_ui->timeEditEndF->setEnabled(true);
    }
    if (timetable.secondPause != timetable.endSecondPause) {
        m_ui->checkBoxSecond->setChecked(true);
        m_ui->timeEditSBegin->setTime(timetable.secondPause);
        m_ui->timeEditSBegin->setEnabled(true);
        m_ui->timeEditEndS->setTime(timetable.endSecondPause);
        m_ui->timeEditEndS->setEnabled(true);
    }


     // if day don't exist you can' insert a task

    exist = dayExist;
    m_ui->pushButtonTask->setEnabled(dayExist);


    if (this->exec() == QDialog::Accepted) {


    }

}

void DialogDay::save()
{

    /*if (m_ui->timeEditEntrance->time() > m_ui->timeEditExit->time()) {
        QMessageBox::warning(this, "Error", "Exit Time must to be minor of Entrance Time");
    }
    else*/ {
        TimeTable timetable;
        timetable.day = day;
        timetable.entrance = m_ui->timeEditEntrance->time();
        timetable.exit = m_ui->timeEditExit->time();
        if (m_ui->checkBoxFirst->isChecked()) {
            timetable.firstPause = m_ui->timeEditFBegin->time();
            timetable.endFirstPause = m_ui->timeEditEndF->time();
        }
        if (m_ui->checkBoxSecond->isChecked()) {
            timetable.secondPause = m_ui->timeEditSBegin->time();
            timetable.endSecondPause = m_ui->timeEditEndS->time();
        }
        BadgeData data;
        data.setTimetable(timetable);
        //m_ui->pushButtonTodo->setEnabled(true);
        m_ui->pushButtonTask->setEnabled(true);
    }

}

void DialogDay::task()
{
    DialogTask dialog;
    QTime initialTime(0, 0, 0);
    taskModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    taskModel->setTable("task");
    qDebug() << "Day" << day.toString("yyyy-MM-dd") << endl;
    taskModel->setFilter("day='" + day.toString("yyyy-MM-dd") + "'");
    //taskModel.setEditStrategy(QSqlTableModel::OnFieldChange);
    taskModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Own"));
     taskModel->setHeaderData(1, Qt::Horizontal, QObject::tr("ID"));
    taskModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    taskModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Elapsed"));
    //taskModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Attivita'"));
    taskModel->setRelation(4, QSqlRelation("attivita", "id", "attivita"));

    taskModel->select();
    //QSqlRelationalTableModel todoModel;

    dialog.openTask("Task", "Elapsed", taskModel, day, "task", initialTime);
}

void DialogDay::todo()
{
    if ( exist == false) {
        save();
    }
    DialogTask dialog;
    QTime initialTime(0, 0, 0);
    taskModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    taskModel->setTable("todo");
    qDebug() << "Day" << day.toString("yyyy-MM-dd") << endl;
    taskModel->setFilter("day='" + day.toString("yyyy-MM-dd") + "'");
    //taskModel.setEditStrategy(QSqlTableModel::OnFieldChange);
    taskModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    taskModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    taskModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Expected"));
    //taskModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Attivita'"));
    taskModel->setRelation(3, QSqlRelation("attivita", "id", "attivita"));

    taskModel->select();
    //QSqlRelationalTableModel todoModel;
    dialog.openTask("ToDo", "Expected", taskModel, day, "todo", initialTime);
}
