#ifndef BADGEDATA_H
#define BADGEDATA_H

#include "timetable.h"
#include <QDate>

class BadgeData
{

public:
    BadgeData();
    bool setTimetable(TimeTable in);
    bool getTimetable(TimeTable &out);
    QTime totalTime(QDate begin, QDate end, QTime &overTime, QTime workingTime, int days = 127);
};

#endif // BADGEDATA_H
