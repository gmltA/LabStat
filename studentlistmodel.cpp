#include "studentlistmodel.h"

StudentListModel::StudentListModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

void StudentListModel::addStudent(const Student& student)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    students << student;
    endInsertRows();
}

void StudentListModel::addStatEntry(const StatTableEntry& entry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    stats << entry;
    endInsertRows();
}

int StudentListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return students.count();
}

QVariant StudentListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= students.count())
        return QVariant();

    const Student& student = students[index.row()];
    switch (role)
    {
        case NameRole:
            return student.getName();
        case SurnameRole:
            return student.getSurname();
        case NoteRole:
            return student.getNote();
        case SubGroupRole:
            return student.getSubgroup();
        case AttendenceRole:
            foreach (StatTableEntry entry, stats)
            {
                if (entry.studentId == student.getId())
                    return entry.attended;
            }
            return false;
        default:
            return QVariant();
    }
}

bool StudentListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() < 0 || index.row() >= students.count())
        return false;

    const Student& student = students[index.row()];
    switch (role)
    {
        case AttendenceRole:
            for (StatTableEntry& entry: stats)
            {
                if (entry.studentId == student.getId())
                {
                    entry.attended = value.toBool();

                    emit dataChanged(index, index);
                    return true;
                }
            }
    }
    return false;
}

QHash<int, QByteArray> StudentListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SurnameRole] = "surname";
    roles[NoteRole] = "note";
    roles[SubGroupRole] = "subgroup";
    roles[AttendenceRole] = "attendence";
    return roles;
}
