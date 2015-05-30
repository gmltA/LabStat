#ifndef STATTABLEENTRY_H
#define STATTABLEENTRY_H

#include <QList>
#include <QVariantList>

class StatTableEntry
{
    public:
        StatTableEntry() : id(0), timeTableId(0), studentId(0), attended(false)
        {
        }

        StatTableEntry(int _id, int _timeTableId, int _studentId, QString statData)
            : id(_id), timeTableId(_timeTableId), studentId(_studentId)
        {
            if (statData.toLower() == "н")
                attended = false;
            else
            {
                attended = true;
                extractLabStatData(statData);
            }
        }

        int id;
        int timeTableId;
        int studentId;
        bool attended;
        QMap<int, bool> labWorks;

    private:
        void extractLabStatData(QString statData)
        {
            QStringList dataList = statData.split(",");
            foreach (QString labData, dataList)
            {
                labWorks[labData.toInt()] = true;
            }
        }
};

typedef QList<StatTableEntry*> StatTable;

#endif // STATTABLEENTRY_H
