#ifndef GOOGLEAUTHCLIENT_H
#define GOOGLEAUTHCLIENT_H

#include "interface.authclient.h"
#include <QObject>

/*!
 * \brief The GoogleAuthClient class interacts with JRE and obtains Google Play auth token to connect to Drive.
 */
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
