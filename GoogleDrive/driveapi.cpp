#include "driveapi.h"
#include "../googleauthclient.h"

#include <QBuffer>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

GoogleDriveAPI::GoogleDriveAPI(QObject* parent)
    : QObject(parent), IDataStore(IDataStore::OriginOnline)
{
    network = new QNetworkAccessManager();
}

GoogleDriveAPI::~GoogleDriveAPI()
{
    network->deleteLater();
}

void GoogleDriveAPI::test()
{
    ListFilesRequest* request = new ListFilesRequest();
    connect(static_cast<ListFilesRequestResult*>(request->getResultPointer()), &ListFilesRequestResult::emptyResult, [](){qDebug() << "empty";});
    sendRequest(request);
}

void GoogleDriveAPI::createFile()
{
    DriveFile* file = new DriveFile("Test", "root", "text/plain");

    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");
    InsertFileRequest* request = new InsertFileRequest(url, file);

    sendRequest(request);
}

void GoogleDriveAPI::sendRequest(GoogleAPIRequest* request)
{
    request->setToken(token);

    //todo: delete buffer later
    QByteArray array = request->getRequestData();
    QBuffer* buffer = new QBuffer;
    buffer->setData(array);

    QNetworkReply* reply = network->sendCustomRequest(*request, request->attribute(QNetworkRequest::CustomVerbAttribute).toByteArray(), buffer);
    request->getResultPointer()->setCallback([this, request](){
        this->sendRequest(request);
    });

    reply->setProperty("result", QVariant::fromValue<GoogleAPIRequestResult*>(request->getResultPointer()));

    connect(reply, &QNetworkReply::finished, this, &GoogleDriveAPI::onRequestFinished);
}

QString GoogleDriveAPI::getToken() const
{
    return token;
}

void GoogleDriveAPI::setToken(const QString& value)
{
    token = value;
}

bool GoogleDriveAPI::checkAuth(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::AuthenticationRequiredError)
    {
        GoogleAuthClient::getInstance().processAuth();

        GoogleAPIRequestResult* result = reply->property("result").value<GoogleAPIRequestResult*>();
        connect(&GoogleAuthClient::getInstance(), &GoogleAuthClient::authCompleted, [=](QString token){
            this->setToken(token);

            auto callbackFunction = result->getCallback();
            callbackFunction();
        });
        return false;
    }
    return true;
}

void GoogleDriveAPI::onRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!checkAuth(reply))
        return;

    GoogleAPIRequestResult* result = reply->property("result").value<GoogleAPIRequestResult*>();
    result->handleReply(reply);
}
