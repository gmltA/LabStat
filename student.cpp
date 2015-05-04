#include "student.h"

Student::Student(int _id, QDomNode studentNode) : id(_id)
{
    QStringList nameData = studentNode.firstChildElement("gsx:фио").text().split(" ");
    surname = nameData[0];
    name = nameData[1];
    patronymic = nameData.size() < 3 ? "" : nameData[2];

    QStringList groupData = studentNode.firstChildElement("gsx:группа").text().split("-");
    group = groupData[0].toInt();
    subgroup = groupData.size() < 2 ? 0 : groupData[1].toInt();

    note = studentNode.firstChildElement("gsx:заметки").text();
}

QString Student::getName() const
{
    return name;
}

void Student::setName(const QString& value)
{
    name = value;
}

QString Student::getSurname() const
{
    return surname;
}

void Student::setSurname(const QString& value)
{
    surname = value;
}

QString Student::getPatronymic() const
{
    return patronymic;
}

void Student::setPatronymic(const QString& value)
{
    patronymic = value;
}

int Student::getGroup() const
{
    return group;
}

void Student::setGroup(const int& value)
{
    group = value;
}

int Student::getSubgroup() const
{
    return subgroup;
}

void Student::setSubgroup(const int& value)
{
    subgroup = value;
}

QString Student::getNote() const
{
    return note;
}

void Student::setNote(const QString& value)
{
    note = value;
}

int Student::getId() const
{
    return id;
}

void Student::setId(int value)
{
    id = value;
}

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
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> StudentListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SurnameRole] = "surname";
    roles[NoteRole] = "note";
    roles[SubGroupRole] = "subgroup";
    return roles;
}
