#ifndef DRIVEFILE_H
#define DRIVEFILE_H

#include "../interface.dataitem.h"

#include <QDataStream>
#include <QDateTime>
#include <QJsonObject>
#include <QDomDocument>
#include "../datasheet.h"

struct DriveFileInfo
{
        QString id;
        QDateTime modifiedDate;

        friend QDataStream & operator<< (QDataStream &, const DriveFileInfo &);
        friend QDataStream& operator>> (QDataStream &, DriveFileInfo &);
};

Q_DECLARE_METATYPE(DriveFileInfo)

class DriveFile : public IDataItem
{
        Q_INTERFACES(IDataItem)

    public:
        DriveFile() {}
        DriveFile(DataSheet* dataSheet);
        DriveFile(QJsonObject object);
        DriveFile(QString _title, QString _mimeType, QString _parentId = "");
        DriveFile(QString _id, QString _title, QString _parentId, QString _mimeType);
        DriveFile(const DriveFile &other);
        ~DriveFile() {}

        void fill(const DriveFile &other);

        QString getParentId() const;
        void setParentId(const QString& value);

        QString getMimeType() const;
        void setMimeType(const QString& value);

        QString buildSearchQuery();

        QString getContent() const;
        void setContent(const QString& value);

        QDateTime getModifiedDate() const;
        void setModifiedDate(const QDateTime& value);

    protected:
        QString parentId;
        QString mimeType;
        QDateTime modifiedDate;

    private:
        QString content;
};

class WorkSheet
{
    public:
        WorkSheet();
        WorkSheet(QString _id);
        WorkSheet(QDomNode node);

        QString getId() const;
        void setId(const QString& value);

        QString getTitle() const;
        void setTitle(const QString& value);

        QString getListFeedURL() const;
        void setListFeedURL(const QString& value);

    private:
        QString id;
        QString title;
        QString listFeedURL;
};

class SpreadSheet : public DriveFile
{
    public:
        SpreadSheet(QString _id);
        SpreadSheet(QDomNode node);

        QList<WorkSheet> getWorkSheets() const;
        WorkSheet getWorkSheet(QString title) const;
        void setWorkSheets(const QList<WorkSheet>& value);

    private:
        QList<WorkSheet> workSheets;
};

Q_DECLARE_METATYPE(DriveFile)

#endif // DRIVEFILE_H
