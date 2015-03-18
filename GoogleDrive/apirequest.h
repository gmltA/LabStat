#ifndef GOOGLEAPIREQUEST_H
#define GOOGLEAPIREQUEST_H

#include "drivefile.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

class GoogleAPIRequest : public QNetworkRequest
{
    public:
        GoogleAPIRequest(QUrl _requestURL, QString _authToken, QByteArray _verb, QByteArray _data = 0)
            : QNetworkRequest(_requestURL), requestData(_data), token(_authToken)
        {
            setAttribute(QNetworkRequest::CustomVerbAttribute, _verb);
            setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());
        }
        ~GoogleAPIRequest() {}

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

        virtual void handleReply(QNetworkReply* reply) = 0;

    protected:
        QByteArray requestData;
        QString token;

    signals:

    public slots:
};

class UserInfoRequest : public GoogleAPIRequest
{
    public:
        UserInfoRequest(QString _authToken)
            : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), _authToken, "GET")
        {}

        void handleReply(QNetworkReply* reply) override
        {
            Q_UNUSED(reply)
        }
};

class InsertFileRequest : public GoogleAPIRequest
{
    public:
        InsertFileRequest(QUrl _requestUrl, QString _authToken, DriveFile* _file);


    private:
        const QString requestBoundary = "ls_delim_boundary";
        DriveFile* file;
};


#endif // GOOGLEAPIREQUEST_H
