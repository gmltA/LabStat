#ifndef DATASHEET_H
#define DATASHEET_H

#include <QDateTime>
#include <QObject>
#include <QStringList>
#include "student.h"

struct TimetableEntry
{
        TimetableEntry(int _id, QDateTime _dateTime, int _group, int _subgroup = 0)
            : id(_id), group(_group), subgroup(_subgroup)
        {
            dateTime = _dateTime;
            students = new StudentListModel();
        }

        TimetableEntry(int _id, QDate date, QTime time, int _group, int _subgroup = 0)
            : id(_id), group(_group), subgroup(_subgroup)
        {
            dateTime.setDate(date);
            dateTime.setTime(time);
            students = new StudentListModel();
        }

        ~TimetableEntry()
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

class TimeTableModel : public QAbstractListModel
{
    Q_OBJECT
    public:
        enum TimeTableDataRoles {
            DateRole = Qt::UserRole + 1,
            TimeRole,
            GroupRole,
            StudentsRole
        };

        TimeTableModel(int groupId, QObject *parent = 0);

        void addEntry(const TimetableEntry& entry);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

        int getGroupId() const;

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        int groupId;
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

        QString getFileName() const;
        void setFileName(const QString& value);

        QList<Student> getStudentList() const;
        void setStudentList(const QList<Student>& value);

        QList<TimetableEntry> getTimeTable() const;
        void setTimeTable(const QList<TimetableEntry>& value);

        QDateTime getLastSyncTime() const;
        int getLastSyncProcessorId() const;

        void synced(int processorId);

        TimeTableModel* getTimeTableModel(int groupId = 0);

    private:
        uint id;
        QString fileName;
        int lastSyncProcessorId;
        QDateTime lastSyncTime;

        QList<int> groups;
        QList<Student> students;
        QList<TimetableEntry> timeTable;
        TimeTableModel* timeTableModel;

    signals:
        void groupListChanged(QList<int> groups);
};

#endif // DATASHEET_H
