#ifndef DATASHEET_H
#define DATASHEET_H

#include <QDateTime>
#include <QObject>

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
        explicit DataSheet(QString fileName = "", int id = 0, QObject* parent = 0);
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

        TimeTable getTimeTable() const;
        void setTimeTable(const TimeTable& value);

        StatTable getStatTable() const;
        void setStatTable(const StatTable& value);

        QDateTime getLastSyncTime() const;
        int getLastSyncProcessorId() const;

        void synced(int processorId);

        TimeTableModel* getTimeTableModel(int groupId = 0);

        int getTotalLabCount() const;
        void setTotalLabCount(int value);

    private:
        uint id;
        QString fileName;
        int lastSyncProcessorId;
        QDateTime lastSyncTime;

        int totalLabCount;
        QList<int> groups;
        StudentList students;
        TimeTable timeTable;
        StatTable stats;
        TimeTableModel* timeTableModel;

    signals:
        void groupListChanged(QList<int> groups);

    public slots:
        void statEntryAdded(StatTableEntry* entry);
};

#endif // DATASHEET_H
