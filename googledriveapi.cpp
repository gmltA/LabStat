#include "googledriveapi.h"

#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

void GoogleDriveAPI::test(QString token)
{
    QUrl url("https://www.googleapis.com/oauth2/v2/userinfo");
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());

    QNetworkReply* reply = network->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onAPITestFinished()));
}

void GoogleDriveAPI::insert(QString token)
{
    QUrl url("https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart&convert=true");
    QNetworkRequest request;

    QString parentId = "root";
    QString boundary = "ls_delim_boundary";

    QString metadata = QString("{"
                                   "\"title\": \"%1\","
                                   "\"parents\": ["
                                   "{ \"id\": \"%2\"}"
                                   "]"
                                   "}").arg(QString("Test")).arg(parentId);

    request.setUrl(url);
    request.setRawHeader("Content-Type", QString("multipart/related; boundary=\"%1\"").arg(boundary).toLatin1());            //mimeType.name().toLatin1());
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toLatin1());

    QByteArray data = QString("--" + boundary + "\n").toLatin1();
    data += QString("Content-Type: application/json; charset=UTF-8\n\n").toLatin1();
    data += QString(metadata + "\n\n").toLatin1();
    data += QString("--" + boundary + "\n").toLatin1();
    data += QString("Content-Type: text/plain\n\n").toLatin1();
    data += "";
    data += QString("\n--" + boundary + "--").toLatin1();


    request.setRawHeader("Content-Length", QString::number(data.size()).toLatin1());

    QNetworkReply* reply = network->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(onInsertFinished()));
}

GoogleDriveAPI::~GoogleDriveAPI()
{

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
