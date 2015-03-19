#include "driveapi.h"
#include "apirequest.h"
#include "googleauthclient.h"

#include <QBuffer>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

void GoogleDriveAPI::test()
{
    UserInfoRequest request(token);

    QNetworkReply* reply = network->sendCustomRequest(request, request.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray());
    connect(reply, &QNetworkReply::finished, this, &GoogleDriveAPI::onAPITestFinished);
}

void GoogleDriveAPI::createFile()
{
    DriveFile* file = new DriveFile("Test", "root", "text/plain");

    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");
    InsertFileRequest request(url, token, file);

    //todo: delete buffer later
    QByteArray array = request.getRequestData();
    QBuffer* buffer = new QBuffer;
    buffer->setData(array);
    QNetworkReply* reply = network->sendCustomRequest(request, request.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray(), buffer);

    reply->setProperty("result", QVariant::fromValue<GoogleAPIRequestResult*>(request.getResultPointer()));

    connect(reply, &QNetworkReply::finished, this, &GoogleDriveAPI::onInsertFinished);
}

QString GoogleDriveAPI::getToken() const
{
    return token;
}

void GoogleDriveAPI::setToken(const QString& value)
{
    token = value;
}

bool GoogleDriveAPI::checkAuth(QNetworkReply* reply, std::function<void ()> callback)
{
    if (reply->error() == QNetworkReply::AuthenticationRequiredError)
    {
        GoogleAuthClient::getInstance().processAuth();
        connect(&GoogleAuthClient::getInstance(), &GoogleAuthClient::authCompleted, [callback](){
            callback();
        });
        return false;
    }
    return true;
}

void GoogleDriveAPI::onAPITestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!checkAuth(reply, [this](){ this->test(); }))
        return;

    QString json = reply->readAll();
    qDebug() << json;
}

void GoogleDriveAPI::onInsertFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!checkAuth(reply, [this](){ this->createFile(); }))
        return;

    GoogleAPIRequestResult* result = reply->property("result").value<GoogleAPIRequestResult*>();
    result->handleReply(reply);
}
