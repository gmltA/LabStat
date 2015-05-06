#include "datasheet.h"
#include <QMetaEnum>
#include <QDebug>

DataSheet::DataSheet(QString _fileName, QObject *parent)
    : QObject(parent), id(0), fileName(_fileName)
{
}

DataSheet::~DataSheet()
{
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

QList<Student> DataSheet::getStudentList() const
{
    return students;
}

void DataSheet::setStudentList(const QList<Student>& value)
{
    students = value;
}

QList<TimetableEntry> DataSheet::getTimeTable() const
{
    return timeTable;
}

void DataSheet::setTimeTable(const QList<TimetableEntry>& value)
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

TimeTableModel::TimeTableModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

void TimeTableModel::addEntry(const TimetableEntry& entry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    timeTable << entry;
    endInsertRows();
}

int TimeTableModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return timeTable.count();
}

QVariant TimeTableModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= timeTable.count())
        return QVariant();

    const TimetableEntry& entry = timeTable[index.row()];
    switch (role)
    {
        case DateRole:
            return entry.dateTime.date();
        case TimeRole:
            return entry.dateTime.time();
        case GroupRole:
            return entry.subgroup;
        case StudentsRole:
        {
            QVariant var;
            var.setValue(entry.students);
            return var;
        }
    }
    return QVariant();
}

int TimeTableModel::subGroupIdForIndex(int index)
{
    return timeTable[index].subgroup;
}

QHash<int, QByteArray> TimeTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[TimeRole] = "time";
    roles[GroupRole] = "group";
    roles[StudentsRole] = "students";
    return roles;
}
