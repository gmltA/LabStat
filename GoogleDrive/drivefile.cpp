#include "drivefile.h"

DriveFile::DriveFile(QJsonObject object)
    : IDataItem(object["id"].toString(), object["title"].toString())
{

}

DriveFile::DriveFile(QString _title, QString _parentId, QString _mimeType)
    : IDataItem(_title),
      parentId(_parentId), mimeType(_mimeType)
{
}

DriveFile::DriveFile(QString _id, QString _title, QString _parentId, QString _mimeType)
    : IDataItem(_id, _title),
      parentId(_parentId), mimeType(_mimeType)
{
}

DriveFile::DriveFile(const DriveFile& other)
    : IDataItem(other.id, other.title)
{
    parentId = other.parentId;
    mimeType = other.mimeType;
}

void DriveFile::fill(const DriveFile& other)
{
    setId(other.id);
    setTitle(other.title);
    setMimeType(other.mimeType);
    setParentId(other.parentId);
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

QString DriveFile::buildSearchQuery()
{
    return QString("mimeType = '%1' and trashed = false and title = '%3'")
                            .arg(getMimeType())
                            .arg(getTitle());
}
