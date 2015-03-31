#ifndef GOOGLEAPIREQUEST_H
#define GOOGLEAPIREQUEST_H

#include "drivefile.h"
#include "apirequestresult.h"

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class GoogleAPIRequest : public QNetworkRequest
{
    public:
        GoogleAPIRequest(QUrl _requestURL, QByteArray _verb, QByteArray _data = 0);
        ~GoogleAPIRequest();

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

        GoogleAPIRequestResult* getResultPointer() const;

        QString getToken() const;
        void setToken(const QString& value);

    protected:
        GoogleAPIRequestResult* result;
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

class InsertFileRequest : public GoogleAPIRequest
{
    public:
        // todo: move URL into private methods of request
        InsertFileRequest(QUrl _requestUrl, DriveFile* _file);

        InsertFileRequestResult* getResultPointer() const;

    private:
        const QString requestBoundary = "ls_delim_boundary";
};

class ListFilesRequest : public GoogleAPIRequest
{
    public:
        ListFilesRequest();
        ListFilesRequest(QString searchQuery);
};


#endif // GOOGLEAPIREQUEST_H
