#include "googledesktopauthclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>

GoogleDesktopAuthClient::GoogleDesktopAuthClient(QObject *parent) : QObject(parent)
{
    connect(this, &GoogleDesktopAuthClient::tokenObtained, this, &GoogleDesktopAuthClient::authCompleted);
}

GoogleDesktopAuthClient::~GoogleDesktopAuthClient()
{

}

void GoogleDesktopAuthClient::refreshToken()
{
    QUrl url("https://accounts.google.com/o/oauth2/token");
    QNetworkRequest request;

    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QSettings settings;
    QString refresh_token = settings.value("refresh_token").toString();

    QString str = "client_id=301432611724-d6in25u9em3v95m84d2ngpi5j0btbdkp.apps.googleusercontent.com";
    str += "&redirect_uri=urn:ietf:wg:oauth:2.0:oob";
    str += "&client_secret=tSzn_1898tCK5VrfyZWH3Sci";
    str += "&grant_type=refresh_token";
    str += "&refresh_token=" + refresh_token;
    QByteArray params = str.toLatin1();

    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    QNetworkReply* reply = mgr->post(request, params);
    connect(reply, &QNetworkReply::finished, [=](){
        QString json = reply->readAll();
        QJsonDocument sd = QJsonDocument::fromJson(json.toUtf8());
        QJsonObject sett2 = sd.object();
        QString access_token = sett2.value(QString("access_token")).toString();
        QSettings settings1;
        settings1.setValue("access_token", access_token);
        emit tokenObtained(access_token);
    });
}

void GoogleDesktopAuthClient::processAuth()
{
    QSettings settings;
    if (settings.value("refresh_token").toBool())
        refreshToken();
    else
        qDebug() << "No refresh token!";
}

