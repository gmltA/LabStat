#ifndef GOOGLEAUTHCLIENT_H
#define GOOGLEAUTHCLIENT_H

#include "interface.authclient.h"
#include <QObject>

class GoogleAuthClient : public QObject, public IAuthClient
{
        Q_OBJECT
        Q_INTERFACES(IAuthClient)

    public:
        GoogleAuthClient(QObject *parent = 0);
        ~GoogleAuthClient() {}


    signals:
        void tokenObtained(QString) override final;
        void authCompleted(QString) override final;

    public slots:
        void processAuth() override final;
};

#endif // GOOGLEAUTHCLIENT_H
