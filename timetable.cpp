#include "timetable.h"



QDataStream &operator<<(QDataStream &out, const TimeTable &timetable)
{
    out << timetable.day << timetable.entrance << timetable.firstPause << timetable.endFirstPause << timetable.secondPause
            << timetable.endSecondPause << timetable.exit;

    return out;
}

QDataStream &operator>>(QDataStream &in, TimeTable &timetable)
{
    in >> timetable.day >> timetable.entrance >> timetable.firstPause >> timetable.endFirstPause >> timetable.secondPause
            >> timetable.endSecondPause >> timetable.exit;
    return in;
}

int TimeTable::wrong()
{
    if (isWrong)
        return WRONG;
    else
        return NO_WRONG;
}

void TimeTable::setWrong(bool wrong)
{
    isWrong = wrong;
}
