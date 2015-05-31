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
            extractLabStatData(statData);
        }

        QString labStatsToString()
        {
            QString strLabStatData;
            if (!attended)
            {
                strLabStatData = "н";
            }
            else
            {
                QStringList dataList;
                for (QMap<int, bool>::iterator iter = labWorks.begin(); iter != labWorks.end(); iter++)
                {
                    if (iter.value())
                        dataList.append(QString::number(iter.key()));
                }
                strLabStatData = dataList.join(",");
            }
            return strLabStatData;
        }

        int id;
        int timeTableId;
        int studentId;
        bool attended;
        QMap<int, bool> labWorks;

    private:
        void extractLabStatData(QString statData)
        {
            if (statData.toLower() == "н")
                attended = false;
            else
            {
                attended = true;
                QStringList dataList = statData.split(",");
                foreach (QString labData, dataList)
                {
                    labWorks[labData.toInt()] = true;
                }
            }
        }
};

typedef QList<StatTableEntry*> StatTable;

#endif // STATTABLEENTRY_H
