#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QDataStream>
#include <QDate>
#include <QTime>


class  TimeTable
{
    private:
        bool isWrong;
    public:
    enum {WRONG, NO_WRONG};
    QDate day;
    QTime entrance;
    QTime firstPause;
    QTime endFirstPause;
    QTime secondPause;
    QTime endSecondPause;
    QTime exit;


    TimeTable() {isWrong = false;}
    int wrong();
    void setWrong(bool wrong);

};

QDataStream &operator<<(QDataStream &out, const TimeTable &timetable);
QDataStream &operator>>(QDataStream &in, TimeTable &timetable);



#endif // TIMETABLE_H
