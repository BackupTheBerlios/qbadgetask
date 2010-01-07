#include "badgedata.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <math.h>

BadgeData::BadgeData()
{

}

bool BadgeData::setTimetable(TimeTable tt)
{
#ifdef ONLY_BADGE
    QFile data;
    TimeTable dum;
    qint64 pos = 0;

    data.setFileName(QDir::homePath() + QDir::separator() + "badge.data");
    data.open(QIODevice::ReadWrite);
    QDataStream out(&data);
    while (!data.atEnd()) {
        out >> dum;
        if (dum.day == tt.day) {
            data.seek(pos);

            out << tt;
            data.close();
            return true;
        }
         pos = data.pos();
    }
    out << tt;
    data.close();
    return true;
#else
    QSqlQuery query;
    QVariant vday(tt.day);
    QVariant venter(tt.entrance);
    QVariant vexit(tt.exit);
    QVariant vbeginFirst(tt.firstPause);
    QVariant vendFirst(tt.endFirstPause);
    QVariant vbeginSecond(tt.secondPause);
    QVariant vendSecond(tt.endSecondPause);

    //QVariant vdefinition (definition);

    query.prepare("select count(*)  from days where daywork = ?");
    query.addBindValue(vday);
    query.exec();
    query.next();

    if(query.value(0).toInt() > 0)
    {

        query.prepare("update days set enter = ? , exit = ? , beginfirst = ? , endfirst = ? , beginsecond = ? , endsecond = ? where daywork = ?");

        query.addBindValue(venter);
        query.addBindValue(vexit);
        query.addBindValue(vbeginFirst);
        query.addBindValue(vendFirst);
        query.addBindValue(vbeginSecond);
        query.addBindValue(vendSecond);
        query.addBindValue(vday);
        if(query.exec())
            qDebug() << "UPDATE OK\n";

    }
    else
    {
        query.prepare("insert into days values(?,?, ?, ?, ?, ?, ?)");
        query.addBindValue(vday);
        query.addBindValue(venter);
        query.addBindValue(vexit);
        query.addBindValue(vbeginFirst);
        query.addBindValue(vendFirst);
        query.addBindValue(vbeginSecond);
        query.addBindValue(vendSecond);

        query.exec();

    }
#endif
    return false;
}

bool BadgeData::getTimetable(TimeTable &tt)
{
#ifdef ONLY_BADGE
    QFile data;
    TimeTable dum;
    data.setFileName(QDir::homePath() + QDir::separator() + "badge.data");
    data.open(QIODevice::ReadOnly);
    QDataStream out(&data);



    while (!data.atEnd()) {
        out >> dum;
        if (dum.day == tt.day) {
            tt = dum;
            data.close();
            return true;
        }
    }
    data.close();
    return false;
#else
    QSqlQuery query;
    QVariant vday(tt.day);

    query.prepare("select *  from days where daywork = ?");
    query.addBindValue(vday);

    query.exec();


    if (query.next()) {

        tt.entrance = query.value(1).toTime();
        tt.exit = query.value(2).toTime();
        tt.firstPause  = query.value(3).toTime();
        tt.endFirstPause  = query.value(4).toTime();
        tt.secondPause  = query.value(5).toTime();
        tt.endSecondPause  = query.value(6).toTime();

        return true;
    }
#endif
    return false;
}

QTime BadgeData::totalTime(QDate begin, QDate end, QTime &overTime, QTime workingTime, int days)
{
     QTime total(0, 0, 0, 0);
     int totalInSeconds = 0;
#ifdef ONLY_BADGE
    QFile data;
    TimeTable dum;




    data.setFileName(QDir::homePath() + QDir::separator() + "badge.data");
    data.open(QIODevice::ReadOnly);
    QDataStream out(&data);
    while (!data.atEnd()) {
        out >> dum;
        if (dum.day >=  begin && dum.day <= end) {
            totalInSeconds += dum.entrance.secsTo(dum.exit);
            if (dum.firstPause != dum.endFirstPause) {
                totalInSeconds -= dum.firstPause.secsTo(dum.endFirstPause);
            }

            if (dum.secondPause != dum.endSecondPause) {
                totalInSeconds -= dum.secondPause.secsTo(dum.endSecondPause);
            }



        }

    }
    data.close();
    total.setHMS((int)(totalInSeconds / 3600), (int) ((totalInSeconds % 3600) / 60), 0);
#else
    QSqlQuery query;
    QVariant vbegin(begin);
    QVariant vend(end);

    // Working and pause Time

    QTime entrance;
    QTime exit;
    QTime beginFirst;
    QTime endFirst;
    QTime beginSecond;
    QTime endSecond;
    QDate day;
    QTime zeroTime;
    int dayHour = 0;
    int overTimeSeconds;

    query.prepare("select *  from days where daywork >= ? AND daywork <= ?");
    query.addBindValue(vbegin);
    query.addBindValue(vend);
    query.exec();


    while (query.next()) {
        day = query.value(0).toDate();
        entrance = query.value(1).toTime();
        exit = query.value(2).toTime();
        beginFirst  = query.value(3).toTime();
        endFirst  = query.value(4).toTime();
        beginSecond  = query.value(5).toTime();
        endSecond  = query.value(6).toTime();
        dayHour = 0;
        if (exit > entrance) {
            dayHour += entrance.secsTo(exit);
            if (beginFirst != endFirst) {
                dayHour -= beginFirst.secsTo(endFirst);
            }

            if (beginSecond != endSecond) {
                dayHour -= beginSecond.secsTo(endSecond);
            }
        }

        // Calculate overTime
        if ((dayHour - zeroTime.secsTo(workingTime)) > 0) {
            if (((int) pow(2, (day.dayOfWeek() - 1))) & days)
                overTimeSeconds = dayHour - zeroTime.secsTo(workingTime);
            else
                overTimeSeconds = dayHour;
        }

        totalInSeconds += dayHour;
    }
    total.setHMS((int)(totalInSeconds / 3600), (int) ((totalInSeconds % 3600) / 60), 0);
#endif
    return total;

}
