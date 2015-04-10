#include "apirequestresult.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QXmlSimpleReader>

void InsertFileRequestResult::handleReply(QNetworkReply* reply)
{
    QString jsonData = reply->readAll();
    qDebug() << jsonData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject fileObject = jsonDoc.object();

    file->setId(fileObject["id"].toString());
}

DriveFile* InsertFileRequestResult::getFile() const
{
    return file;
}

DriveFile* UpdateFileRequestResult::getFile() const
{
    return file;
}

void UserInfoRequestResult::handleReply(QNetworkReply* reply)
{
    QString replyData = reply->readAll();
    qDebug() << replyData;
}

ListFilesRequestResult::~ListFilesRequestResult()
{
}

void ListFilesRequestResult::handleReply(QNetworkReply* reply)
{
    QString replyData = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.toUtf8());
    QJsonObject jsonResult = jsonDoc.object();

    for (auto item: jsonResult["items"].toArray())
        fileList.push_back(DriveFile(item.toObject()));
}

QVector<DriveFile> ListFilesRequestResult::getFileList() const
{
    return fileList;
}

void UpdateFileRequestResult::handleReply(QNetworkReply* reply)
{
    QString jsonData = reply->readAll();
    qDebug() << jsonData;
}

void GetFileRequestResult::handleReply(QNetworkReply* reply)
{
    QString replyData = reply->readAll();
    qDebug() << replyData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.toUtf8());
    QJsonObject fileObject = jsonDoc.object();

    QDateTime date = QDateTime::fromString(fileObject["modifiedDate"].toString(), Qt::ISODate);
    file->setModifiedDate(date);
}
