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
    int totalTime(QDate begin, QDate end, int &overTime, /*QMap<QString, QTime> &activities,*/ QTime workingTime, int days = 127);
    void dayRemain(QDate day, QTime &total, QTime &remain);
};

#endif // BADGEDATA_H
