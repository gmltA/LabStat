#ifndef GOOGLEAUTHCLIENT_H
#define GOOGLEAUTHCLIENT_H

#include <QObject>
#include <QAndroidJniEnvironment>

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
        Q_UNUSED(parent)
        connect(this, SIGNAL(tokenObtained(QString)), this, SLOT(apiTest(QString)));
    }


signals:
    void tokenObtained(QString);

public slots:
    void processAuth();

private slots:
    void apiTest(QString token);

private:
};

#endif // GOOGLEAUTHCLIENT_H
