#ifndef STATTABLEENTRY_H
#define STATTABLEENTRY_H

#include <QList>
#include <QVariantList>

class StatTableEntry
{
    public:
        int id;
        int timeTableId;
        int studentId;
        bool attended;
        QVariantList labWorks;
};

typedef QList<StatTableEntry*> StatTable;

#endif // STATTABLEENTRY_H
