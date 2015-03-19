#ifndef DRIVEFILE_H
#define DRIVEFILE_H

#include "interface.dataitem.h"

class DriveFile : public IDataItem
{
        Q_INTERFACES(IDataItem)

    public:
        DriveFile() {}
        DriveFile(QString _title, QString _parentId, QString _mimeType);
        DriveFile(QString _id, QString _title, QString _parentId, QString _mimeType);
        DriveFile(const DriveFile &other);
        ~DriveFile() {}

        QString getParentId() const;
        void setParentId(const QString& value);

        QString getMimeType() const;
        void setMimeType(const QString& value);

    protected:
        QString parentId;
        QString mimeType;
};

Q_DECLARE_METATYPE(DriveFile)

#endif // DRIVEFILE_H
