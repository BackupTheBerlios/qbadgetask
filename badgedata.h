#ifndef BADGEDATA_H
#define BADGEDATA_H

#include "timetable.h"
#include <QDate>
#include <QMap>

class BadgeData
{

public:
    BadgeData();
    bool setTimetable(TimeTable in);
    bool getTimetable(TimeTable &out);
    QTime totalTime(QDate begin, QDate end, QTime &overTime, QMap<QString, QTime> &activities, QTime workingTime, int days = 127);
};

#endif // BADGEDATA_H
