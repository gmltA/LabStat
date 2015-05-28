#ifndef STATTABLEENTRY_H
#define STATTABLEENTRY_H

#include <QList>

class StatTableEntry
{
    public:
        int id;
        int timeTableId;
        int studentId;
        bool attended;
};

typedef QList<StatTableEntry> StatTable;

#endif // STATTABLEENTRY_H
