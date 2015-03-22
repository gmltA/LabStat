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

    private:
        ~GoogleAuthClient() {}
        GoogleAuthClient(QObject *parent = 0) : QObject(parent)
        {
            connect(this, &GoogleAuthClient::tokenObtained, this, &GoogleAuthClient::onTokenObtained);
        }
        GoogleAuthClient(const GoogleAuthClient&) = delete;
        GoogleAuthClient& operator=(const GoogleAuthClient&) = delete;


    signals:
        void authCompleted();
        void tokenObtained(QString);

    public slots:
        void processAuth();

    private slots:
        void onTokenObtained(QString token);
};

#endif // GOOGLEAUTHCLIENT_H
