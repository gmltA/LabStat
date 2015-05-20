#ifndef TIMETABLEENTRY_H
#define TIMETABLEENTRY_H

#include "studentlistmodel.h"

#include <QDateTime>


class TimeTableEntry
{
    public:
        TimeTableEntry(int _id, QDateTime _dateTime, int _group, int _subgroup = 0)
            : id(_id), group(_group), subgroup(_subgroup)
        {
            dateTime = _dateTime;
            students = new StudentListModel();
        }

        TimeTableEntry(int _id, QDate date, QTime time, int _group, int _subgroup = 0)
            : id(_id), group(_group), subgroup(_subgroup)
        {
            dateTime.setDate(date);
            dateTime.setTime(time);
            students = new StudentListModel();
        }

        ~TimeTableEntry()
        {
            // BUG! Causes app crash
            //delete students;
        }

        int id;

        QDateTime dateTime;
        StudentListModel* students;
        int group;
        int subgroup;
};

#endif // TIMETABLEENTRY_H
