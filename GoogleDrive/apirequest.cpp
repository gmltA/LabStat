#include "apirequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

InsertFileRequest::InsertFileRequest(QUrl _requestUrl, QString _authToken, DriveFile* _file)
    : GoogleAPIRequest(_requestUrl, _authToken, "POST")
{
    QString metadata = QString("{"
                                   "\"title\": \"%1\","
                                   "\"parents\": ["
                                   "{ \"id\": \"%2\"}"
                                   "]"
                                   "}").arg(_file->getTitle()).arg(_file->getParentId());

    setRawHeader("Content-Type", QString("multipart/related; boundary=\"%1\"").arg(requestBoundary).toLatin1());

    requestData = QString("--" + requestBoundary + "\n").toLatin1();
    requestData += QString("Content-Type: application/json; charset=UTF-8\n\n").toLatin1();
    requestData += QString(metadata + "\n\n").toLatin1();
    requestData += QString("--" + requestBoundary + "\n").toLatin1();
    requestData += QString("Content-Type: %1\n\n").arg(_file->getMimeType()).toLatin1();
    requestData += "";
    requestData += QString("\n--" + requestBoundary + "--").toLatin1();


    setRawHeader("Content-Length", QString::number(requestData.size()).toLatin1());

    result = new InsertFileRequestResult(_file);
}

GoogleAPIRequest::~GoogleAPIRequest()
{
    //todo: uncomment later
    //delete result;
}

QByteArray GoogleAPIRequest::getRequestData() const
{
    return requestData;
}

void GoogleAPIRequest::setRequestData(const QByteArray& value)
{
    requestData = value;
}

GoogleAPIRequestResult* GoogleAPIRequest::getResultPointer() const
{
    return result;
}

InsertFileRequestResult* InsertFileRequest::getResultPointer() const
{
    return static_cast<InsertFileRequestResult*>(GoogleAPIRequest::getResultPointer());
}


UserInfoRequest::UserInfoRequest(QString _authToken)
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), _authToken, "GET")
{
    result = new UserInfoRequestResult();
}
