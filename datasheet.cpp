#include "datasheet.h"
#include <QMetaEnum>

DataSheet::DataSheet(QObject *parent) : QObject(parent), id(0), subject(Subject::None)
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

QString DataSheet::getTitle() const
{
    if (subject != Subject::None)
        return QString("%1-%2").arg(id).arg(DataSheet::subjectString(subject));
    else
        return QString("%1").arg(id);
}

const char* DataSheet::subjectString(DataSheet::Subject subject)
{
    const QMetaObject &mo = DataSheet::staticMetaObject;
    int index = mo.indexOfEnumerator("Subject");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.valueToKey(subject);
}
