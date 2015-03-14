#ifndef GOOGLEDRIVEENUMS
#define GOOGLEDRIVEENUMS

#include <QMimeData>
#include <QString>


struct DriveFile
{
    //QString id;
    QString title;
    QString parent;
    QString mimeType;

    DriveFile(QString _title, QString _parent, QString _mimeType)
        : title(_title), parent(_parent), mimeType(_mimeType)
    {
    }
};

#endif // GOOGLEDRIVEENUMS

