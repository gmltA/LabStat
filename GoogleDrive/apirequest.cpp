#include "apirequest.h"

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
}

QByteArray GoogleAPIRequest::getRequestData() const
{
    return requestData;
}

void GoogleAPIRequest::setRequestData(const QByteArray& value)
{
    requestData = value;
}

