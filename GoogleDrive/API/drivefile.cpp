#include "drivefile.h"
#include <QRegularExpression>

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
    : IDataItem(dataSheet->getFileName()), mimeType("text/tsv"), content(dataSheet->toString())
{
}

DriveFile::DriveFile(QJsonObject object)
    : IDataItem(object["id"].toString(), object["title"].toString())
{
}

DriveFile::DriveFile(QString _title, QString _mimeType, QString _parentId)
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

SpreadSheet::SpreadSheet() :
    DriveFile(QString(), "application/vnd.google-apps.spreadsheet")
{
}

SpreadSheet::SpreadSheet(QString _id) :
    DriveFile(_id, "application/vnd.google-apps.spreadsheet")
{
}

SpreadSheet::SpreadSheet(QDomNode node)
{
    QRegularExpression regex("(?<=full/)(.+)");
    id = regex.match(node.firstChildElement("id").text()).captured(0);

    title = node.firstChildElement("title").text();
    mimeType = "application/vnd.google-apps.spreadsheet";
    modifiedDate = QDateTime::fromString(node.firstChildElement("updated").text(), Qt::ISODate);
}

QList<WorkSheet> SpreadSheet::getWorkSheets() const
{
    return workSheets;
}

WorkSheet SpreadSheet::getWorkSheet(QString title) const
{
    for (auto sheet: workSheets)
        if (sheet.getTitle() == title)
            return sheet;

    return WorkSheet();
}

void SpreadSheet::setWorkSheets(const QList<WorkSheet>& value)
{
    workSheets = value;
}

WorkSheet::WorkSheet()
{
}

WorkSheet::WorkSheet(QDomNode node)
{
    QRegularExpression regex("(?<=full/)(.+)");
    id = regex.match(node.firstChildElement("id").text()).captured(0);

    title = node.firstChildElement("title").text();

    QDomNodeList sheetNodes = node.childNodes();
    for (int j = 0; j < sheetNodes.size(); j++)
    {
        if (sheetNodes.item(j).nodeName() == "link")
        {
            QDomElement element = sheetNodes.item(j).toElement();
            if (element.attribute("rel") == "http://schemas.google.com/spreadsheets/2006#listfeed")
                listFeedURL = element.attribute("href");
        }
    }
}

QString WorkSheet::getId() const
{
    return id;
}

void WorkSheet::setId(const QString& value)
{
    id = value;
}

QString WorkSheet::getTitle() const
{
    return title;
}

void WorkSheet::setTitle(const QString& value)
{
    title = value;
}

QString WorkSheet::getListFeedURL() const
{
    return listFeedURL;
}

void WorkSheet::setListFeedURL(const QString& value)
{
    listFeedURL = value;
}
