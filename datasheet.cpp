#include "datasheet.h"

DataSheet::DataSheet(QString _fileName, int _id, QObject *parent)
    : QObject(parent), id(_id), fileName(_fileName), timeTableModel(nullptr)
{
}

DataSheet::~DataSheet()
{
    if (timeTableModel)
        timeTableModel->deleteLater();

    foreach (TimeTableEntry* entry, timeTable)
        entry->deleteLater();

    foreach (StatTableEntry* entry, stats)
        delete entry;

    foreach (Student* student, students)
        delete student;
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

TimeTable DataSheet::getTimeTable() const
{
    return timeTable;
}

void DataSheet::setTimeTable(const TimeTable& value)
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
    lastSyncTime = QDateTime::currentDateTimeUtc();
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
        foreach (TimeTableEntry* timeTableEntry, timeTable)
        {
            if (groupId != timeTableEntry->group)
                continue;

            foreach (Student* person, students)
            {
                if (person->getGroup() == groupId && (person->getSubgroup() == timeTableEntry->subgroup
                        || timeTableEntry->subgroup == 0 || person->getSubgroup() == 0))
                {
                    timeTableEntry->students->addStudent(person);

                    foreach (StatTableEntry* entry, stats)
                    {
                        if (entry->studentId == person->getId() && entry->timeTableId == timeTableEntry->id)
                            timeTableEntry->students->addStatEntry(entry);
                    }
                }
            }
            timeTableEntry->students->setTotalLabCount(totalLabCount);
            connect(timeTableEntry->students, &StudentListModel::statEntryAdded, this, &DataSheet::statEntryAdded);
            timeTableModel->addEntry(timeTableEntry);
        }
    }
    return timeTableModel;
}

int DataSheet::getTotalLabCount() const
{
    return totalLabCount;
}

void DataSheet::setTotalLabCount(int value)
{
    totalLabCount = value;
}

void DataSheet::statEntryAdded(StatTableEntry* entry)
{
    if (stats.count() > 0)
        entry->id = stats.last()->id + 1;

    stats.append(entry);
}

StatTable DataSheet::getStatTable() const
{
    return stats;
}

void DataSheet::setStatTable(const StatTable& value)
{
    stats = value;
}
