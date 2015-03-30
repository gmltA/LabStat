#include "apirequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

GoogleAPIRequest::GoogleAPIRequest(QUrl _requestURL, QByteArray _verb, QByteArray _data)
    : QNetworkRequest(_requestURL), requestData(_data)
{
    setAttribute(QNetworkRequest::CustomVerbAttribute, _verb);
}

InsertFileRequest::InsertFileRequest(QUrl _requestUrl, DriveFile* _file)
    : GoogleAPIRequest(_requestUrl, "POST")
{
    QString metadata = QString("{"
                                   "\"title\": \"%1\","
                                   "\"parents\": ["
                                   "{ \"id\": \"%2\"}"
                                   "],"
                                   "\"mimeType\": \"%3\""
                                   "}").arg(_file->getTitle()).arg(_file->getParentId(), _file->getMimeType());

    setRawHeader("Content-Type", QString("multipart/related; boundary=\"%1\"").arg(requestBoundary).toLatin1());

    requestData = QString("--" + requestBoundary + "\n").toLatin1();
    requestData += QString("Content-Type: application/json; charset=UTF-8\n\n").toLatin1();
    requestData += QString(metadata + "\n\n").toLatin1();

    // file content (excluded for folders)
    // @todo: extract fileType property
    if (_file->getMimeType() != "application/vnd.google-apps.folder")
    {
        requestData += QString("--" + requestBoundary + "\n").toLatin1();
        requestData += QString("Content-Type: %1\n\n").arg(_file->getMimeType()).toLatin1();
        requestData += "";
    }
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

QString GoogleAPIRequest::getToken() const
{
    return token;
}

void GoogleAPIRequest::setToken(const QString& value)
{
    token = value;
    setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());
}

InsertFileRequestResult* InsertFileRequest::getResultPointer() const
{
    return static_cast<InsertFileRequestResult*>(GoogleAPIRequest::getResultPointer());
}

UserInfoRequest::UserInfoRequest()
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), "GET")
{
    result = new UserInfoRequestResult();
}

ListFilesRequest::ListFilesRequest()
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/drive/v2/files?q=mimeType+%3D+%27application%2Fvnd.google-apps.folder%27+and+title+%3D+%27LabStat%27+and+trashed+%3D+false"), "GET")
{
    result = new ListFilesRequestResult();
}
