#include "apirequestresult.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

void InsertFileRequestResult::handleReply(QNetworkReply* reply)
{
    QString jsonData = reply->readAll();
    qDebug() << jsonData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject fileObject = jsonDoc.object();
    file->setId(fileObject["id"].toString());

    qDebug() << file->getId();
}

DriveFile* InsertFileRequestResult::getFile() const
{
    return file;
}
