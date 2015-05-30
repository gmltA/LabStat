#ifndef DATASHEET_H
#define DATASHEET_H

#include <QDateTime>
#include <QObject>
#include <QStringList>

#include "student.h"
#include "studentlistmodel.h"
#include "timetablemodel.h"

/*!
 * \brief The DataSheet class contains all valueble data related to particular subject
 *
 * Object contains data that can be extracted or modifed e.g. student list, timetable, stats, etc.
 */
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

        StudentList getStudentList() const;
        void setStudentList(const StudentList& value);

        QList<TimeTableEntry> getTimeTable() const;
        void setTimeTable(const QList<TimeTableEntry>& value);

        QList<StatTableEntry> getStatTable() const;
        void setStatTable(const QList<StatTableEntry>& value);

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
        StudentList students;
        QList<TimeTableEntry> timeTable;
        QList<StatTableEntry> statTable;
        TimeTableModel* timeTableModel;

    signals:
        void groupListChanged(QList<int> groups);
};

#endif // DATASHEET_H
