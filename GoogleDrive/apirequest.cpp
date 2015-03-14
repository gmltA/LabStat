#include "apirequest.h"

QNetworkRequest GoogleAPIRequest::build()
{
    QNetworkRequest request;

    request.setUrl(requestURL);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());

    return request;
}

QUrl GoogleAPIRequest::getRequestURL() const
{
    return requestURL;
}

void GoogleAPIRequest::setRequestURL(const QUrl& value)
{
    requestURL = value;
}

QNetworkRequest InsertFileRequest::build()
{
    QNetworkRequest request = GoogleAPIRequest::build();

    QString metadata = QString("{"
                                   "\"title\": \"%1\","
                                   "\"parents\": ["
                                   "{ \"id\": \"%2\"}"
                                   "]"
                                   "}").arg(file->getTitle()).arg(file->getParentId());

    request.setRawHeader("Content-Type", QString("multipart/related; boundary=\"%1\"").arg(requestBoundary).toLatin1());

    requestData = QString("--" + requestBoundary + "\n").toLatin1();
    requestData += QString("Content-Type: application/json; charset=UTF-8\n\n").toLatin1();
    requestData += QString(metadata + "\n\n").toLatin1();
    requestData += QString("--" + requestBoundary + "\n").toLatin1();
    requestData += QString("Content-Type: %1\n\n").arg(file->getMimeType()).toLatin1();
    requestData += "";
    requestData += QString("\n--" + requestBoundary + "--").toLatin1();


    request.setRawHeader("Content-Length", QString::number(requestData.size()).toLatin1());
    return request;
}

QByteArray InsertFileRequest::getRequestData() const
{
    return requestData;
}

void InsertFileRequest::setRequestData(const QByteArray& value)
{
    requestData = value;
}

