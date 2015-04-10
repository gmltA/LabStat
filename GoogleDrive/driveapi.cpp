#include "driveapi.h"

#include <QBuffer>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>

#include "../googleauthclient.h"

GoogleDriveAPI::GoogleDriveAPI(IAuthClient* _authClient, QString _rootFolderName, QObject* parent)
    : QObject(parent), IDataStore(IDataStore::OriginOnline), authClient(_authClient)
{
    qRegisterMetaType<DriveFileInfo>("DriveFileInfo");
    qRegisterMetaTypeStreamOperators<DriveFileInfo>("DriveFileInfo");

    appRootDir = new DriveFile(_rootFolderName, "root", "application/vnd.google-apps.folder");

    connect(this, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), this, SLOT(setToken(QString)));

    loadFileTable();
}

GoogleDriveAPI::~GoogleDriveAPI()
{
}

void GoogleDriveAPI::init()
{
    auto list = listFilesSync(appRootDir);
    if (!list.isEmpty())
        appRootDir->fill(list.first());
    else
        createFileSync(appRootDir);
    emit syncDone();
}

void GoogleDriveAPI::syncFile(DataSheet* dataFile)
{
    DriveFile* file = new DriveFile(dataFile);
    file->setParentId(appRootDir->getId());

    if (fileTable.contains(dataFile->getId()))
    {
        //check modify date
        file->setId(fileTable[dataFile->getId()].id);
        getFileSync(file);
    }
    else
    {
        createFileSync(file);
        DriveFileInfo fileInfo;
        fileInfo.id = file->getId();
        fileInfo.modifiedDate = QDateTime::currentDateTime();

        fileTable[dataFile->getId()] = fileInfo;

        storeFileTable();
    }

    delete file;
}

void GoogleDriveAPI::getFileSync(DriveFile* file)
{
    GetFileRequest* request = new GetFileRequest(file);
    //GetFileRequestResult* result = sendSyncRequest<GetFileRequestResult*>(request);
}

QVector<DriveFile> GoogleDriveAPI::listFilesSync(DriveFile* templateFile)
{
    if (!templateFile)
        return QVector<DriveFile>();

    return listFilesSync(templateFile->buildSearchQuery());
}

QVector<DriveFile> GoogleDriveAPI::listFilesSync(QString searchQuery)
{
    ListFilesRequest* request = new ListFilesRequest(searchQuery);
    ListFilesRequestResult* result = sendSyncRequest<ListFilesRequestResult*>(request);

    return result->getFileList();
}

void GoogleDriveAPI::createFileSync(DriveFile* file)
{
    InsertFileRequest* request = new InsertFileRequest(QUrl("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true"), file);
    sendSyncRequest(request);
}

void GoogleDriveAPI::updateFileSync(DriveFile* file)
{
    UpdateFileRequest* request = new UpdateFileRequest("https://www.googleapis.com/upload/drive/v2/files/" + file->getId() + "?uploadType=multipart", file);
    sendSyncRequest(request);
}

template<class T>
T GoogleDriveAPI::sendSyncRequest(GoogleAPIRequest* request)
{
    QNetworkReply* reply;
    do
    {
        request->setToken(token);
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QEventLoop* loop = new QEventLoop();

        QBuffer* buffer = new QBuffer();
        buffer->setData(request->getRequestData());

        reply = manager->sendCustomRequest(*request, request->attribute(QNetworkRequest::CustomVerbAttribute).toByteArray(), buffer);

        connect(reply, &QNetworkReply::finished, loop, &QEventLoop::quit);
        connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
        connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);

        loop->exec();
    }
    // todo: add condition when auth fail multiple times
    while (!checkAuth(reply));
    request->getResultPointer()->handleReply(reply);

    return static_cast<T>(request->getResultPointer());
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
        QEventLoop* loop = new QEventLoop();
        connect(this, &GoogleDriveAPI::authRecovered, loop, &QEventLoop::quit);
        emit authRequired();

        loop->exec();
        return false;
    }
    else
        return true;
}

void GoogleDriveAPI::loadFileTable()
{
    QSettings settings;
    QVariantHash storeHash = settings.value("DriveAPIFileTable").toHash();
    for (auto iter = storeHash.begin(); iter != storeHash.end(); iter++)
    {
        fileTable[iter.key().toUInt()] = iter.value().value<DriveFileInfo>();
        qDebug() << fileTable[iter.key().toUInt()].id  << " " << fileTable[iter.key().toUInt()].modifiedDate;
    }

    qDebug() << "FileTable entries: " << fileTable.size();
}

void GoogleDriveAPI::storeFileTable()
{
    QSettings settings;
    QVariantHash storeHash;
    for (auto iter = fileTable.begin(); iter != fileTable.end(); iter++)
        storeHash[QString::number(iter.key())] = QVariant::fromValue<DriveFileInfo>(iter.value());
    settings.setValue("DriveAPIFileTable", storeHash);
}
