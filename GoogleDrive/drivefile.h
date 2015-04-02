#ifndef DRIVEFILE_H
#define DRIVEFILE_H

#include "../interface.dataitem.h"

#include <QJsonObject>
#include <datasheet.h>

class DriveFile : public IDataItem
{
        Q_INTERFACES(IDataItem)

    public:
        DriveFile() {}
        DriveFile(DataSheet* dataSheet);
        DriveFile(QJsonObject object);
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

    protected:
        QString parentId;
        QString mimeType;

        QString content;
};

Q_DECLARE_METATYPE(DriveFile)

#endif // DRIVEFILE_H
