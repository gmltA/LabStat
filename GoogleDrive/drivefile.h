#ifndef DRIVEFILE_H
#define DRIVEFILE_H

#include "interface.dataitem.h"

class DriveFile : public QObject, public IDataItem
{
        Q_OBJECT
        Q_INTERFACES(IDataItem)

    public:
        DriveFile(QString _title, QString _parentId, QString _mimeType, QObject *parent = 0);
        DriveFile(QString _id, QString _title, QString _parentId, QString _mimeType, QObject *parent = 0);
        ~DriveFile();

        QString getParentId() const;
        void setParentId(const QString& value);

        QString getMimeType() const;
        void setMimeType(const QString& value);

    protected:
        QString parentId;
        QString mimeType;
};

#endif // DRIVEFILE_H
