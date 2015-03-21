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
        GoogleAPIRequest(QUrl _requestURL, QString _authToken, QByteArray _verb, QByteArray _data = 0)
            : QNetworkRequest(_requestURL), requestData(_data), token(_authToken)
        {
            setAttribute(QNetworkRequest::CustomVerbAttribute, _verb);
            setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());
        }
        ~GoogleAPIRequest();

        QByteArray getRequestData() const;
        void setRequestData(const QByteArray& value);

        GoogleAPIRequestResult* getResultPointer() const;

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
        UserInfoRequest(QString _authToken);
};

class InsertFileRequest : public GoogleAPIRequest
{
    public:
        InsertFileRequest(QUrl _requestUrl, QString _authToken, DriveFile* _file);

        InsertFileRequestResult* getResultPointer() const;

    private:
        const QString requestBoundary = "ls_delim_boundary";
};


#endif // GOOGLEAPIREQUEST_H
