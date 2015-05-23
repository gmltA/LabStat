#include "datasheet.h"
#include <QMetaEnum>
#include <QDebug>

DataSheet::DataSheet(QString _fileName, QObject *parent)
    : QObject(parent), id(0), fileName(_fileName), timeTableModel(nullptr)
{
}

DataSheet::~DataSheet()
{
    if (timeTableModel)
        timeTableModel->deleteLater();
}

uint DataSheet::getId() const
{
    return id;
}

void DataSheet::setId(const uint& value)
{
    id = value;
}

QString DataSheet::toString() const
{
    // NIY
    return "a \t b \t c \nd \t e \t n";
}

QList<int> DataSheet::getGroupList() const
{
    return groups;
}

void DataSheet::setGroupList(const QList<int>& value)
{
    groups = value;
    emit groupListChanged(groups);
}

QString DataSheet::getFileName() const
{
    return fileName;
}

void DataSheet::setFileName(const QString& value)
{
    fileName = value;
}

StudentList DataSheet::getStudentList() const
{
    return students;
}

void DataSheet::setStudentList(const StudentList& value)
{
    students = value;
}

QList<TimeTableEntry> DataSheet::getTimeTable() const
{
    return timeTable;
}

void DataSheet::setTimeTable(const QList<TimeTableEntry>& value)
{
    timeTable = value;
}

QDateTime DataSheet::getLastSyncTime() const
{
    return lastSyncTime;
}

int DataSheet::getLastSyncProcessorId() const
{
    return lastSyncProcessorId;
}

void DataSheet::synced(int processorId)
{
    lastSyncProcessorId = processorId;
    lastSyncTime = QDateTime::currentDateTime();
}

TimeTableModel* DataSheet::getTimeTableModel(int groupId)
{
    if (!groupId)
        return timeTableModel;

    if (!timeTableModel || timeTableModel->getGroupId() != groupId)
    {
        if (timeTableModel)
            timeTableModel->deleteLater();

        timeTableModel = new TimeTableModel(groupId);
        foreach (TimeTableEntry timeTableEntry, timeTable)
        {
            if (groupId != timeTableEntry.group)
                continue;

            foreach (Student person, students)
            {
                if (person.getGroup() == groupId && (person.getSubgroup() == timeTableEntry.subgroup
                        || timeTableEntry.subgroup == 0 || person.getSubgroup() == 0))
                {
                    timeTableEntry.students->addStudent(person);

                    foreach (StatTableEntry entry, statTable)
                    {
                        if (entry.studentId == person.getId() && entry.timeTableId == timeTableEntry.id)
                            timeTableEntry.students->addStatEntry(entry);
                    }
                }
            }
            timeTableModel->addEntry(timeTableEntry);
        }
    }
    return timeTableModel;
}

QList<StatTableEntry> DataSheet::getStatTable() const
{
    return statTable;
}

void DataSheet::setStatTable(const QList<StatTableEntry>& value)
{
    statTable = value;
}
