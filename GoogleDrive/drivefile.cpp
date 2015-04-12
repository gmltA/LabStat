#include "drivefile.h"
#include <QDebug>

QDataStream& operator<<(QDataStream &out, const DriveFileInfo &obj)
{
    out << obj.id << obj.modifiedDate;
    return out;
}

QDataStream& operator>>(QDataStream &in, DriveFileInfo &obj)
{
    in >> obj.id;
    in >> obj.modifiedDate;
    return in;
}

DriveFile::DriveFile(DataSheet* dataSheet)
    : IDataItem(dataSheet->getTitle()), mimeType("text/tsv"), content(dataSheet->toString())
{
}

DriveFile::DriveFile(QJsonObject object)
    : IDataItem(object["id"].toString(), object["title"].toString())
{

}

DriveFile::DriveFile(QDomNode node)
{
    id = node.firstChildElement("id").text();
    title = node.firstChildElement("title").text();

    modifiedDate = QDateTime::fromString(node.firstChildElement("updated").text(), Qt::ISODate);
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

QString DriveFile::getContent() const
{
    return content;
}

void DriveFile::setContent(const QString& value)
{
    content = value;
}

QDateTime DriveFile::getModifiedDate() const
{
    return modifiedDate;
}

void DriveFile::setModifiedDate(const QDateTime& value)
{
    modifiedDate = value;
}
