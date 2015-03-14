#include "drivefile.h"

DriveFile::DriveFile(QString _title, QString _parentId, QString _mimeType, QObject *parent)
    : QObject(parent), IDataItem(_title),
      parentId(_parentId), mimeType(_mimeType)
{
}

DriveFile::DriveFile(QString _id, QString _title, QString _parentId, QString _mimeType, QObject *parent)
    : QObject(parent), IDataItem(_id, _title),
      parentId(_parentId), mimeType(_mimeType)
{
}

DriveFile::~DriveFile()
{

}
QString DriveFile::getParentId() const
{
    return parentId;
}

void DriveFile::setParentId(const QString& value)
{
    parentId = value;
}
QString DriveFile::getMimeType() const
{
    return mimeType;
}

void DriveFile::setMimeType(const QString& value)
{
    mimeType = value;
}



