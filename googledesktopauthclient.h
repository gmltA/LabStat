#ifndef GOOGLEDESKTOPAUTHCLIENT_H
#define GOOGLEDESKTOPAUTHCLIENT_H

#include "interface.authclient.h"
#include <QObject>

class GoogleDesktopAuthClient : public QObject, public IAuthClient
{
        Q_OBJECT
        Q_INTERFACES(IAuthClient)

    public:
        GoogleDesktopAuthClient(QObject *parent = 0);
        ~GoogleDesktopAuthClient();

    private:
        void refreshToken();

    signals:
        void tokenObtained(QString) override final;
        void authCompleted(QString) override final;

    public slots:
        void processAuth() override final;

    private slots:
        void onTokenObtained(QString token) override final;
};

#endif // GOOGLEDESKTOPAUTHCLIENT_H
