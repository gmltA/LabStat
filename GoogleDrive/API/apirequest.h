#ifndef GOOGLEAPIREQUEST_H
#define GOOGLEAPIREQUEST_H

#include "drivefile.h"

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

/*!
 * \brief The GoogleAPIRequest is a basic class for every reqeust executed on drive service
 */
class GoogleAPIRequest : public QNetworkRequest
{
    public:
        GoogleAPIRequest(QUrl _requestURL, QByteArray _verb, QByteArray _data = 0);

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

        QString getToken() const;
        void setToken(const QString& value);

    protected:
        QByteArray requestData;
        QString token;

    signals:

    public slots:
};

class UserInfoRequest : public GoogleAPIRequest
{
    public:
        UserInfoRequest();
};

class UpdateFileRequest : public GoogleAPIRequest
{
    public:
        UpdateFileRequest(DriveFile* _file);

    private:
        const QString requestBoundary = "ls_delim_boundary";
};

class InsertFileRequest : public GoogleAPIRequest
{
    public:
        InsertFileRequest(DriveFile* _file);

    private:
        const QString requestBoundary = "ls_delim_boundary";
};

class GetFileRequest : public GoogleAPIRequest
{
    public:
        GetFileRequest(DriveFile* _file);
};

class ListFilesRequest : public GoogleAPIRequest
{
    public:
        ListFilesRequest();
        ListFilesRequest(QString searchQuery);
};


#endif // GOOGLEAPIREQUEST_H
