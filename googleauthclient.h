#ifndef GOOGLEAUTHCLIENT_H
#define GOOGLEAUTHCLIENT_H

#include <QObject>

class GoogleAuthClient : public QObject
{
        Q_OBJECT

    public:
        GoogleAuthClient(QObject *parent = 0) : QObject(parent)
        {
            connect(this, &GoogleAuthClient::tokenObtained, this, &GoogleAuthClient::onTokenObtained);
        }

        ~GoogleAuthClient() {}


    signals:
        void tokenObtained(QString);
        void authCompleted(QString);

    public slots:
        void processAuth();

    private slots:
        void onTokenObtained(QString token);
};

#endif // GOOGLEAUTHCLIENT_H
