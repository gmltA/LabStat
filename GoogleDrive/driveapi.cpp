#include "driveapi.h"
#include "apirequest.h"

#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

void GoogleDriveAPI::test()
{
    UserInfoRequest request(token);

    QNetworkReply* reply = network->get(request.build());
    connect(reply, SIGNAL(finished()), this, SLOT(onAPITestFinished()));
}

void GoogleDriveAPI::createFile()
{
    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");

    DriveFile file("Test", "root", "text/plain");
    InsertFileRequest request(url, token, "ls_delim_boundary", file);

    QNetworkReply* reply = network->post(request.build(), request.getRequestData());
    connect(reply, SIGNAL(finished()), this, SLOT(onInsertFinished()));
}

GoogleDriveAPI::~GoogleDriveAPI()
{

}
QString GoogleDriveAPI::getToken() const
{
    return token;
}

void GoogleDriveAPI::setToken(const QString& value)
{
    token = value;
}


void GoogleDriveAPI::onAPITestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString json = reply->readAll();

    qDebug() << json;
}

void GoogleDriveAPI::onInsertFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyData = reply->readAll();

    qDebug() << replyData;
}
