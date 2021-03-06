#include "apirequest.h"

GoogleAPIRequest::GoogleAPIRequest(QUrl _requestURL, QByteArray _verb, QByteArray _data)
    : QNetworkRequest(_requestURL), requestData(_data)
{
    setAttribute(QNetworkRequest::CustomVerbAttribute, _verb);
}

InsertFileRequest::InsertFileRequest(DriveFile* _file)
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true"), "POST")
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
}

UpdateFileRequest::UpdateFileRequest(DriveFile* _file)
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/upload/drive/v2/files/" + _file->getId() + "?uploadType=multipart"), "PUT")
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
        requestData += _file->getContent();
    }
    requestData += QString("\n--" + requestBoundary + "--").toLatin1();


    setRawHeader("Content-Length", QString::number(requestData.size()).toLatin1());
}

QByteArray GoogleAPIRequest::getRequestData() const
{
    return requestData;
}

void GoogleAPIRequest::setRequestData(const QByteArray& value)
{
    requestData = value;
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

UserInfoRequest::UserInfoRequest()
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/oauth2/v2/userinfo"), "GET")
{
}

ListFilesRequest::ListFilesRequest()
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/drive/v2/files"), "GET")
{
}

ListFilesRequest::ListFilesRequest(QString searchQuery)
    : GoogleAPIRequest(QUrl("https://www.googleapis.com/drive/v2/files"), "GET")
{
    if (!searchQuery.isEmpty())
    {
        QString query = "?q=" + QUrl::toPercentEncoding(searchQuery);
        setUrl(url().toString() + query);
    }
}

GetFileRequest::GetFileRequest(DriveFile* _file)
    : GoogleAPIRequest("https://www.googleapis.com/drive/v2/files/"+_file->getId(), "GET")
{
}
