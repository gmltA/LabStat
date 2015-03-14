#ifndef GOOGLEAPIREQUEST_H
#define GOOGLEAPIREQUEST_H

#include "enums.h"

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class GoogleAPIRequest : public QObject
{
        Q_OBJECT
    public:
        GoogleAPIRequest(QUrl _requestURL, QString _authToken, QObject* parent = 0)
            : QObject(parent), requestURL(_requestURL), token(_authToken)
        {}
        ~GoogleAPIRequest() {}

        virtual QNetworkRequest build();

        QUrl getRequestURL() const;
        void setRequestURL(const QUrl& value);

    protected:
        QUrl requestURL;
        QString token;

    signals:

    public slots:
};

class UserInfoRequest : public GoogleAPIRequest
{
        Q_OBJECT
    public:
        UserInfoRequest(QString _authToken, QObject* parent = 0)
            : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), _authToken, parent)
        {}
};

class InsertFileRequest : public GoogleAPIRequest
{
        Q_OBJECT
    public:
        InsertFileRequest(QUrl _requestUrl, QString _authToken, QString _boundary, DriveFile _file, QObject* parent = 0)
            : GoogleAPIRequest(_requestUrl, _authToken, parent),requestBoundary(_boundary), file(_file)
        {}

        QNetworkRequest build();

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

    private:
        QString requestBoundary;
        QByteArray requestData;
        DriveFile file;
};


#endif // GOOGLEAPIREQUEST_H
