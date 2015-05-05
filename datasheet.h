#ifndef DATASHEET_H
#define DATASHEET_H

#include <QDateTime>
#include <QObject>
#include <QStringList>
#include "student.h"

struct TimetableEntry
{
        TimetableEntry(int _id, QDate date, QTime time, int _group, int _subgroup = 0)
            : id(_id), group(_group), subgroup(_subgroup)
        {
            dateTime.setDate(date);
            dateTime.setTime(time);
        }

        int id;

        QDateTime dateTime;
        int group;
        int subgroup;
};

class TimeTableModel : public QAbstractListModel
{
    Q_OBJECT
    public:
        enum TimeTableDataRoles {
            DateRole = Qt::UserRole + 1,
            TimeRole,
            GroupRole
        };

        TimeTableModel(QObject *parent = 0);

        void addEntry(const TimetableEntry& entry);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        Q_INVOKABLE int subGroupIdForIndex(int index);

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<TimetableEntry> timeTable;
};

class DataSheet : public QObject
{
        Q_OBJECT

    public:
        explicit DataSheet(QString _fileName = "", QObject* parent = 0);
        ~DataSheet();

        QString toString() const;

        uint getId() const;
        void setId(const uint& value);

        QList<int> getGroupList() const;
        void setGroupList(const QList<int>& value);

        //todo: move build* functions back to sync processor
        void buildGroupList(QByteArray rawData);

        QString getFileName() const;
        void setFileName(const QString& value);

        QList<Student> getStudentList() const;
        void setStudentList(const QList<Student>& value);
        void buildStudentList(QByteArray rawData);

        QList<TimetableEntry> getTimeTable() const;
        void setTimeTable(const QList<TimetableEntry>& value);
        void buildTimeTable(QByteArray rawData);

        QDateTime getLastSyncTime() const;
        int getLastSyncProcessorId() const;

        void synced(int processorId);

    private:
        uint id;
        QString fileName;
        int lastSyncProcessorId;
        QDateTime lastSyncTime;

        QList<int> groups;
        QList<Student> students;
        QList<TimetableEntry> timeTable;

    signals:
        void groupListChanged(QList<int> groups);
};

#endif // DATASHEET_H
