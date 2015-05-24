#include "timetablemodel.h"

TimeTableModel::TimeTableModel(int _groupId, QObject* parent)
    : QAbstractListModel(parent), groupId(_groupId)
{

}

void TimeTableModel::addEntry(const TimeTableEntry& entry)
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

    const TimeTableEntry& entry = timeTable[index.row()];
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

QHash<int, QByteArray> TimeTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[TimeRole] = "time";
    roles[GroupRole] = "group";
    roles[StudentsRole] = "students";
    return roles;
}

int TimeTableModel::getGroupId() const
{
    return groupId;
}

int TimeTableModel::getClosestEntryIndex()
{
    QDateTime now = QDateTime::currentDateTime();

    int closest = 0;

    for (int i = 0; i < timeTable.size(); i++)
    {
        if (i != closest)
        {
            TimeTableEntry entry = timeTable.at(i);

            if (abs(entry.dateTime.secsTo(now)) < abs(timeTable.at(closest).dateTime.secsTo(now)))
                closest = i;
        }
    }

    return closest;

}
