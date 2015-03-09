#ifndef GOOGLEAUTHCLIENT_H
#define GOOGLEAUTHCLIENT_H

#include <QObject>
#include <QAndroidJniEnvironment>
#include <QNetworkAccessManager>

class GoogleAuthClient : public QObject
{
    Q_OBJECT
public:

    static GoogleAuthClient& getInstance()
    {
        static GoogleAuthClient instance;
        return instance;
    }

    GoogleAuthClient(QObject *parent = 0) {
        connect(this, SIGNAL(tokenObtained(QString)), this, SLOT(apiTest(QString)));
    }


signals:
    void tokenObtained(QString);

public slots:
    void processAuth();

private slots:
    void getEmail(QNetworkReply* reply);
    void apiTest(QString token);

private:
    QNetworkAccessManager* mgr;
};

#endif // GOOGLEAUTHCLIENT_H
