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
    QTime totalTime(QDate begin, QDate end);
};

#endif // BADGEDATA_H
