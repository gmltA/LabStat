#include "apirequestresult.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

std::function<void ()> GoogleAPIRequestResult::getCallback() const
{
    return callback;
}

void GoogleAPIRequestResult::setCallback(const std::function<void ()>& value)
{
    callback = value;
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
