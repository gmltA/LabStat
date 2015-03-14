#ifndef GOOGLEAPIREQUEST_H
#define GOOGLEAPIREQUEST_H

#include "drivefile.h"

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class GoogleAPIRequest : public QNetworkRequest
{
    public:
        GoogleAPIRequest(QUrl _requestURL, QString _authToken)
            : QNetworkRequest(_requestURL), token(_authToken)
        {
            setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());
        }
        ~GoogleAPIRequest() {}

    protected:
        QString token;

    signals:

    public slots:
};

class UserInfoRequest : public GoogleAPIRequest
{
    public:
        UserInfoRequest(QString _authToken)
            : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), _authToken)
        {}
};

class InsertFileRequest : public GoogleAPIRequest
{
    public:
        InsertFileRequest(QUrl _requestUrl, QString _authToken, QString _boundary, DriveFile* _file);

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

    private:
        QString requestBoundary;
        QByteArray requestData;
        DriveFile* file;
};


#endif // GOOGLEAPIREQUEST_H
