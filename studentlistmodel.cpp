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
            return true;
        case LabWorksRole:
        {
            QMap<int,bool> stats;
            if (StatTableEntry* statEntry = statEntryForStudent(student->getId()))
            {
                stats = statEntry->labWorks;
            }

            QVariantList labWorks;
            for (int index = 0; index < totalLabCount; index++)
            {
                if (stats.contains(index))
                    labWorks << stats[index];
                else
                    labWorks << false;
            }
            return labWorks;
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
            StatTableEntry* entry = getOrCreateStatEntryForStudent(student->getId());
            entry->attended = value.toBool();
            emit dataChanged(index, index);
            return true;
            break;
        }
        case LabWorksRole:
        {
            StatTableEntry* entry = getOrCreateStatEntryForStudent(student->getId());
            entry->attended = true;
            entry->labWorks = convertListToMap(value);
            emit dataChanged(index, index);
            return true;
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

int StudentListModel::getTotalLabCount() const
{
    return totalLabCount;
}

void StudentListModel::setTotalLabCount(int value)
{
    totalLabCount = value;
}

QMap<int, bool> StudentListModel::convertListToMap(const QVariant& value)
{
    QMap<int,bool> updatedStats;
    QVariantList labWorks = value.toList();
    for (int listIndex = 0; listIndex < labWorks.size(); listIndex++)
        updatedStats[listIndex] = labWorks[listIndex].toBool();

    return updatedStats;
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

StatTableEntry* StudentListModel::getOrCreateStatEntryForStudent(int studentId)
{
    StatTableEntry* statEntry = statEntryForStudent(studentId);
    if (!statEntry)
    {
        statEntry = new StatTableEntry;
        statEntry->studentId = studentId;
        statEntry->timeTableId = dynamic_cast<TimeTableEntry*>(parent())->id;
        stats.append(statEntry);
        emit statEntryAdded(statEntry);
    }
    return statEntry;
}
