#include "driveapi.h"

#include <QBuffer>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>

#include "googleauthclient.h"

GoogleDriveAPI::GoogleDriveAPI(IAuthClient* _authClient, QString _rootFolderName, QObject* parent)
    : QObject(parent), IDataStore(IDataStore::OriginOnline), authClient(_authClient)
{
    network = new QNetworkAccessManager();
    appRootDir = new DriveFile(_rootFolderName, "root", "application/vnd.google-apps.folder");

    connect(this, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), this, SLOT(setToken(QString)));

    loadFileTable();
}

GoogleDriveAPI::~GoogleDriveAPI()
{
    network->deleteLater();
}

void GoogleDriveAPI::init()
{
    auto list = listFilesSync(appRootDir);
    if (!list.isEmpty())
        appRootDir->fill(list.first());
    else
        createFileSync(appRootDir);
}

void GoogleDriveAPI::syncFile(DataSheet* dataFile)
{
    DriveFile* file = new DriveFile(dataFile);
    file->setParentId(appRootDir->getId());

    if (fileTable.contains(dataFile->getId()))
    {
        //check modify date
        file->setId(fileTable[dataFile->getId()]);
        getFileSync(file);
    }
    else
    {
        createFileSync(file);
        fileTable[dataFile->getId()] = file->getId();

        storeFileTable();
    }

    delete file;
}

void GoogleDriveAPI::getFileSync(DriveFile* file)
{
    QNetworkAccessManager* mgr = new QNetworkAccessManager();

    GetFileRequest* request = new GetFileRequest(file);

    QEventLoop* loop = new QEventLoop();
    connect(this, &GoogleDriveAPI::workDone, loop, &QEventLoop::quit);
    connect(this, &GoogleDriveAPI::workDone, mgr, &QNetworkAccessManager::deleteLater);
    sendRequest(request, mgr);
    loop->exec();

    GetFileRequestResult* result = static_cast<GetFileRequestResult*>(request->getResultPointer());
}

QVector<DriveFile> GoogleDriveAPI::listFilesSync(DriveFile* templateFile)
{
    if (!templateFile)
        return QVector<DriveFile>();

    return listFilesSync(templateFile->buildSearchQuery());
}

QVector<DriveFile> GoogleDriveAPI::listFilesSync(QString searchQuery)
{
    QNetworkAccessManager* mgr = new QNetworkAccessManager();

    ListFilesRequest* request = new ListFilesRequest(searchQuery);

    QEventLoop* loop = new QEventLoop();
    connect(this, &GoogleDriveAPI::workDone, loop, &QEventLoop::quit);
    connect(this, &GoogleDriveAPI::workDone, mgr, &QNetworkAccessManager::deleteLater);
    sendRequest(request, mgr);
    loop->exec();

    ListFilesRequestResult* result = static_cast<ListFilesRequestResult*>(request->getResultPointer());
    return result->getFileList();
}

void GoogleDriveAPI::createFileSync(DriveFile* file)
{
    QNetworkAccessManager* mgr = new QNetworkAccessManager();

    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");

    QEventLoop* loop = new QEventLoop();
    InsertFileRequest* request = new InsertFileRequest(url, file);
    connect(this, &GoogleDriveAPI::workDone, loop, &QEventLoop::quit);
    connect(this, &GoogleDriveAPI::workDone, mgr, &QNetworkAccessManager::deleteLater);
    sendRequest(request, mgr);
    loop->exec();

    //return file;
}

void GoogleDriveAPI::updateFileSync(DriveFile* file)
{
    QNetworkAccessManager* mgr = new QNetworkAccessManager();

    QEventLoop* loop = new QEventLoop();
    UpdateFileRequest* request = new UpdateFileRequest("https://www.googleapis.com/upload/drive/v2/files/" + file->getId() + "?uploadType=multipart", file);
    connect(this, &GoogleDriveAPI::workDone, loop, &QEventLoop::quit);
    connect(this, &GoogleDriveAPI::workDone, mgr, &QNetworkAccessManager::deleteLater);
    sendRequest(request, mgr);
    loop->exec();

    //return file;
}

void GoogleDriveAPI::test()
{
    ListFilesRequest* request = new ListFilesRequest();

    connect(this, &GoogleDriveAPI::workDone, [](){qDebug() << "empty";});
    sendRequest(request);
}

void GoogleDriveAPI::createFile()
{
    DriveFile* file = new DriveFile("Test", "root", "text/plain");

    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");
    InsertFileRequest* request = new InsertFileRequest(url, file);

    sendRequest(request);
}

void GoogleDriveAPI::sendRequest(GoogleAPIRequest* request, QNetworkAccessManager* manager)
{
    if (!manager)
        manager = network;

    request->setToken(token);

    QByteArray array = request->getRequestData();
    QBuffer* buffer = new QBuffer();
    buffer->setData(array);

    QNetworkReply* reply = manager->sendCustomRequest(*request, request->attribute(QNetworkRequest::CustomVerbAttribute).toByteArray(), buffer);
    request->getResultPointer()->setCallback([=](){
        //todo: manager is in different thread!
        this->sendRequest(request, manager);
    });

    reply->setProperty("result", QVariant::fromValue<GoogleAPIRequestResult*>(request->getResultPointer()));

    connect(reply, &QNetworkReply::finished, this, &GoogleDriveAPI::onRequestFinished);
    connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
}

QString GoogleDriveAPI::getToken() const
{
    return token;
}

void GoogleDriveAPI::setToken(const QString& value)
{
    qDebug() << "token set " << value;
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
    emit workDone();
}

void GoogleDriveAPI::loadFileTable()
{
    QSettings settings;
    QVariantHash storeHash = settings.value("DriveAPIFileTable").toHash();
    for (auto iter = storeHash.begin(); iter != storeHash.end(); iter++)
        fileTable[iter.key().toUInt()] = iter.value().toString();

    qDebug() << "FileTable entries: " << fileTable.size();
}

void GoogleDriveAPI::storeFileTable()
{
    QSettings settings;
    QVariantHash storeHash;
    for (auto iter = fileTable.begin(); iter != fileTable.end(); iter++)
        storeHash[QString::number(iter.key())] = iter.value();
    settings.setValue("DriveAPIFileTable", storeHash);
}
