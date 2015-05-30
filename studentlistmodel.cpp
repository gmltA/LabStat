#include "studentlistmodel.h"
#include "timetableentry.h"

StudentListModel::StudentListModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

void StudentListModel::addStudent(Student* student)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    students << student;
    endInsertRows();
}

void StudentListModel::addStatEntry(StatTableEntry* entry)
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

    Student* student = students[index.row()];
    switch (role)
    {
        case NameRole:
            return student->getName();
        case SurnameRole:
            return student->getSurname();
        case NoteRole:
            return student->getNote();
        case SubGroupRole:
            return student->getSubgroup();
        case AttendenceRole:
            if (StatTableEntry* statEntry = statEntryForStudent(student->getId()))
            {
                return statEntry->attended;
            }
            return false;
        case LabWorksRole:
        {
            if (StatTableEntry* statEntry = statEntryForStudent(student->getId()))
            {
                QVariantList labWorks = statEntry->labWorks;
                return labWorks;
            }
            break;
        }
    }
    return QVariant();
}

bool StudentListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() < 0 || index.row() >= students.count())
        return false;

    Student* student = students[index.row()];
    switch (role)
    {
        case NameRole:
            student->setName(value.toString());
            emit dataChanged(index, index);
            break;
        case NoteRole:
            student->setNote(value.toString());
            emit dataChanged(index, index);
            break;
        case AttendenceRole:
        {
            StatTableEntry* operatingEntry = statEntryForStudent(student->getId());
            if (!operatingEntry)
            {
                operatingEntry = new StatTableEntry;
                operatingEntry->studentId = student->getId();
                operatingEntry->timeTableId = dynamic_cast<TimeTableEntry*>(parent())->id;
                stats.append(operatingEntry);
                emit statEntryAdded(operatingEntry);
            }

            operatingEntry->attended = value.toBool();
            emit dataChanged(index, index);
            return true;
            break;
        }
        case LabWorksRole:
        {
            if (StatTableEntry* statEntry = statEntryForStudent(student->getId()))
            {
                statEntry->labWorks = value.toList();
                emit dataChanged(index, index);
                return true;
            }
            break;
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
    roles[LabWorksRole] = "labWorks";
    return roles;
}

StatTableEntry* StudentListModel::statEntryForStudent(int studentId) const
{
    StatTableEntry* result = nullptr;
    for (StatTableEntry* entry: stats)
    {
        if (entry->studentId == studentId)
        {
            result = entry;
        }
    }
    return result;
}
