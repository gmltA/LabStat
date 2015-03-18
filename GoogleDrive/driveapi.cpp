#include "driveapi.h"
#include "apirequest.h"

#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

void GoogleDriveAPI::test()
{
    UserInfoRequest request(token);

    QNetworkReply* reply = network->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onAPITestFinished()));
}

void GoogleDriveAPI::createFile()
{
    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");

    DriveFile file("Test", "root", "text/plain");
    InsertFileRequest request(url, token, "ls_delim_boundary", &file);

    QNetworkReply* reply = network->post(request, request.getRequestData());

    reply->setProperty("file", QVariant::fromValue(file));

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

    DriveFile file = reply->property("file").value<DriveFile>();
    qDebug() << file.getTitle();

    QString replyData = reply->readAll();
    qDebug() << replyData;
}
