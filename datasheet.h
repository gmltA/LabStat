#ifndef DATASHEET_H
#define DATASHEET_H

#include <QDateTime>
#include <QObject>
#include <QStringList>
#include "student.h"

struct TimetableEntry
{
        TimetableEntry(int _id, QDate date, QTime time, int _group, int _subgroup = 0)
            : id(_id), group(_group), subroup(_subgroup)
        {
            dateTime.setDate(date);
            dateTime.setTime(time);
        }

        int id;

        QDateTime dateTime;
        int group;
        int subroup;
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

        QStringList getGroupList() const;
        void setGroupList(const QStringList& value);
        void buildGroupList(QByteArray rawData);

        QString getFileName() const;
        void setFileName(const QString& value);

        QList<Student> getStudentList() const;
        void setStudentList(const QList<Student>& value);
        void buildStudentList(QByteArray rawData);

        QList<TimetableEntry> getTimeTable() const;
        void setTimeTable(const QList<TimetableEntry>& value);
        void buildTimeTable(QByteArray rawData);

    private:
        uint id;
        QString fileName;

        QStringList groups;
        QList<Student> students;
        QList<TimetableEntry> timeTable;

    signals:
        void groupListChanged(QStringList groups);
};

#endif // DATASHEET_H
