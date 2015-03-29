#include "driveapi.h"

#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "googleauthclient.h"

GoogleDriveAPI::GoogleDriveAPI(IAuthClient* _authClient, QObject* parent)
    : QObject(parent), IDataStore(IDataStore::OriginOnline), authClient(_authClient)
{
    network = new QNetworkAccessManager();

    connect(this, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), this, SLOT(setToken(QString)));
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
    emit authRecovered();
}

bool GoogleDriveAPI::checkAuth(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::AuthenticationRequiredError)
    {
        emit authRequired();

        GoogleAPIRequestResult* result = reply->property("result").value<GoogleAPIRequestResult*>();
        connect(this, &GoogleDriveAPI::authRecovered, [=](){
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
