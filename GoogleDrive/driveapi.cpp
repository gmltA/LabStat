#include "driveapi.h"

#include <QBuffer>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>

GoogleDriveAPI::GoogleDriveAPI(QString _rootFolderName, QObject* parent)
 : QObject(parent)
{
    qRegisterMetaType<DriveFileInfo>("DriveFileInfo");
    qRegisterMetaTypeStreamOperators<DriveFileInfo>("DriveFileInfo");

    appRootDir = new DriveFile(_rootFolderName, "root", "application/vnd.google-apps.folder");
}

GoogleDriveAPI::~GoogleDriveAPI()
{
}

void GoogleDriveAPI::init()
{
    auto list = listFiles(appRootDir);
    if (!list.isEmpty())
        appRootDir->fill(list.first());
    else
        createFile(appRootDir);
}

void GoogleDriveAPI::getFile(DriveFile* file)
{
    GetFileRequest* request = new GetFileRequest(file);
    //GetFileRequestResult* result = sendSyncRequest<GetFileRequestResult*>(request);
}

QVector<DriveFile> GoogleDriveAPI::listFiles(DriveFile* templateFile)
{
    if (!templateFile)
        return QVector<DriveFile>();

    return listFiles(templateFile->buildSearchQuery());
}

QVector<DriveFile> GoogleDriveAPI::listFiles(QString searchQuery)
{
    ListFilesRequest* request = new ListFilesRequest(searchQuery);
    ListFilesRequestResult* result = sendRequest<ListFilesRequestResult*>(request);

    return result->getFileList();
}

void GoogleDriveAPI::createFile(DriveFile* file)
{
    InsertFileRequest* request = new InsertFileRequest(QUrl("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true"), file);
    sendRequest(request);
}

void GoogleDriveAPI::updateFile(DriveFile* file)
{
    UpdateFileRequest* request = new UpdateFileRequest("https://www.googleapis.com/upload/drive/v2/files/" + file->getId() + "?uploadType=multipart", file);
    sendRequest(request);
}

template<class T>
T GoogleDriveAPI::sendRequest(GoogleAPIRequest* request)
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
