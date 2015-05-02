#include "datasheet.h"
#include <QMetaEnum>

DataSheet::DataSheet(QString _fileName, QObject *parent)
    : QObject(parent), id(0), fileName(_fileName), groupId(0), subject(Subject::None)
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

DataSheet::Subject DataSheet::getSubject() const
{
    return subject;
}

void DataSheet::setSubject(const Subject& value)
{
    subject = value;
}

QString DataSheet::toString() const
{
    // NIY
    return "a \t b \t c \nd \t e \t n";
}

QStringList DataSheet::getGroupList() const
{
    return groups;
}

void DataSheet::setGroupList(const QStringList& value)
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

uint DataSheet::getGroupId() const
{
    return groupId;
}

void DataSheet::setGroupId(const uint& value)
{
    groupId = value;
}

const char* DataSheet::subjectString(DataSheet::Subject subject)
{
    const QMetaObject &mo = DataSheet::staticMetaObject;
    int index = mo.indexOfEnumerator("Subject");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.valueToKey(subject);
}
