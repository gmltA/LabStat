#include "student.h"

#include <QStringList>

Student::Student(int _id, QString _surname, QString _name, QString _patronymic, QString _note)
    : QObject(), id(_id), name(_name), surname(_surname),  patronymic(_patronymic), note(_note)
{
}

Student::Student(int _id, QDomNode studentNode) : QObject(), id(_id)
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
    emit dataChanged(this);
}

QString Student::getSurname() const
{
    return surname;
}

void Student::setSurname(const QString& value)
{
    surname = value;
    emit dataChanged(this);
}

QString Student::getPatronymic() const
{
    return patronymic;
}

void Student::setPatronymic(const QString& value)
{
    patronymic = value;
    emit dataChanged(this);
}

int Student::getGroup() const
{
    return group;
}

void Student::setGroup(const int& value)
{
    group = value;
    emit dataChanged(this);
}

int Student::getSubgroup() const
{
    return subgroup;
}

void Student::setSubgroup(const int& value)
{
    subgroup = value;
    emit dataChanged(this);
}

QString Student::getNote() const
{
    return note;
}

void Student::setNote(const QString& value)
{
    note = value;
    emit dataChanged(this);
}

int Student::getId() const
{
    return id;
}

void Student::setId(int value)
{
    id = value;
    emit dataChanged(this);
}
