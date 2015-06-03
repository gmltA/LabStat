#ifndef DRIVEFILE_H
#define DRIVEFILE_H

#include "../../interface.dataitem.h"

#include <QDataStream>
#include <QDateTime>
#include <QJsonObject>
#include <QDomDocument>
#include "../../datasheet.h"

/*!
 * \brief The DriveFile class is a basic entry for file on Drive
 *
 * Class has set of constructors to extract data from different formats of API results.
 */
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

/*!
 * \brief The WorkSheet class represents Spreadsheet page called Worksheet
 */
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

/*!
 * \brief The SpreadSheet class is an specific kind of Drive file compatible with Sheets API
 */
class SpreadSheet : public DriveFile
{
    public:
        SpreadSheet();
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
