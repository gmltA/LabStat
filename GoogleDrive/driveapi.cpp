#include "driveapi.h"
#include "sheetsapirequest.h"

#include <QBuffer>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkRequest>

GoogleDriveAPI::GoogleDriveAPI(QString _rootFolderName, QObject* parent)
 : QObject(parent), verboseOutput(false), Sheets(this)
{
    qRegisterMetaType<DriveFileInfo>("DriveFileInfo");
    qRegisterMetaTypeStreamOperators<DriveFileInfo>("DriveFileInfo");

    appRootDir = new DriveFile(_rootFolderName, "application/vnd.google-apps.folder", "root");
}

GoogleDriveAPI::~GoogleDriveAPI()
{
}

bool GoogleDriveAPI::init()
{
    auto list = listFiles(appRootDir);
    if (!list.isEmpty())
    {
        VERBOSE("folder found")
        appRootDir->fill(list.first());

        return true;
    }
    else
        return createFile(appRootDir);
}

void GoogleDriveAPI::getFile(DriveFile* file)
{
    QNetworkReply* reply = sendRequest(GetFileRequest(file));
    QString replyData = reply->readAll();
    VERBOSE(replyData)

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.toUtf8());
    QJsonObject fileObject = jsonDoc.object();

    QDateTime date = QDateTime::fromString(fileObject["modifiedDate"].toString(), Qt::ISODate);
    file->setModifiedDate(date);
}

QVector<DriveFile> GoogleDriveAPI::listFiles(DriveFile* templateFile)
{
    if (!templateFile)
        return QVector<DriveFile>();

    return listFiles(templateFile->buildSearchQuery());
}

QVector<DriveFile> GoogleDriveAPI::listFiles(QString searchQuery)
{
    QVector<DriveFile> fileList;

    QNetworkReply* reply = sendRequest(ListFilesRequest(searchQuery));
    QString replyData = reply->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.toUtf8());
    QJsonObject jsonResult = jsonDoc.object();

    for (auto item: jsonResult["items"].toArray())
        fileList.push_back(DriveFile(item.toObject()));

    return fileList;
}

bool GoogleDriveAPI::createFile(DriveFile* file)
{
    QNetworkReply* reply = sendRequest(InsertFileRequest(file));
    QString replyData = reply->readAll();
    VERBOSE(replyData)

    QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.toUtf8());
    QJsonObject fileObject = jsonDoc.object();
    if (fileObject.contains("id"))
    {
        file->setId(fileObject["id"].toString());
        return true;
    }
    return false;
}

bool GoogleDriveAPI::updateFile(DriveFile* file)
{
    QNetworkReply* reply = sendRequest(UpdateFileRequest(file));
    VERBOSE(reply->readAll())
    return true;
}

QNetworkReply* GoogleDriveAPI::sendRequest(GoogleAPIRequest request)
{
    QNetworkReply* reply;
    do
    {
        request.setToken(token);
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        QEventLoop* loop = new QEventLoop();

        QBuffer* buffer = new QBuffer();
        buffer->setData(request.getRequestData());

        reply = manager->sendCustomRequest(request, request.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray(), buffer);

        connect(reply, &QNetworkReply::finished, loop, &QEventLoop::quit);
        connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
        connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);

        loop->exec();
    }
    // todo: add condition when auth fail multiple times
    while (!checkAuth(reply));

    return reply;
}

QString GoogleDriveAPI::getToken() const
{
    return token;
}

void GoogleDriveAPI::setToken(const QString& value)
{
    VERBOSE("token set " + value)
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
bool GoogleDriveAPI::getVerboseOutput() const
{
    return verboseOutput;
}

void GoogleDriveAPI::setVerboseOutput(bool value)
{
    verboseOutput = value;
}

QList<SpreadSheet> GoogleDriveAPI::SheetsAPI::listFiles()
{
    QList<SpreadSheet> fileList;

    QNetworkReply* reply = drive->sendRequest(Sheets::ListFilesRequest());
    QDomDocument doc;
    if (doc.setContent(reply->readAll()))
    {
        QDomNodeList files = doc.elementsByTagName("entry");
        for (int i = 0; i < files.size(); i++)
            fileList.push_back(SpreadSheet(files.item(i)));
    }

    return fileList;
}

QList<WorkSheet> GoogleDriveAPI::SheetsAPI::getWorkSheets(SpreadSheet file)
{
    QList<WorkSheet> workSheets;
    QNetworkReply* reply = drive->sendRequest(Sheets::GetFileRequest(file));

    QDomDocument doc;
    if (doc.setContent(reply->readAll()))
    {
        QDomNodeList worksheets = doc.elementsByTagName("entry");
        for (int i = 0; i < worksheets.size(); i++)
            workSheets.push_back(WorkSheet(worksheets.item(i)));
    }

    return workSheets;
}
