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
        DriveFile(QDomNode node);
        DriveFile(QString _title, QString _parentId, QString _mimeType);
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

        QString content;
};

Q_DECLARE_METATYPE(DriveFile)

#endif // DRIVEFILE_H
