#ifndef DIALOGDAY_H
#define DIALOGDAY_H

#include <QtGui/QDialog>
#include "timetable.h"
#include <QDate>
#include <QtSql>

namespace Ui {
    class DialogDay;
}

class DialogDay : public QDialog {
    Q_OBJECT

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDay *m_ui;
    bool change;
    QDate day;
    QSqlRelationalTableModel *taskModel;
    bool exist;

public:
    DialogDay(QWidget *parent = 0);
    ~DialogDay();
    TimeTable getTimes(TimeTable timetable);
    void getTimes(TimeTable timetable, bool dayExist);

public slots:
    void enableFirst(bool);
    void enableSecond(bool);
    void task();
    void todo();
    void save();
};

#endif // DIALOGDAY_H
