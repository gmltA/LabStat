#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include "../interface.datastore.h"
#include "../googleauthclient.h"
#include "apirequest.h"
#include <functional>

#include <QNetworkAccessManager>

class GoogleDriveAPI : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)

    public:
        GoogleDriveAPI(GoogleAuthClient* _authcClient, QObject *parent = 0);
        ~GoogleDriveAPI();

        QString getToken() const override;
        void setToken(const QString& value) override;

    private:
        bool checkAuth(QNetworkReply* reply);

        QNetworkAccessManager* network;
        QString token;

        GoogleAuthClient* authClient;
    signals:
        void authUpdated();

    public slots:
        void test() override;
        void createFile() override;
        void sendRequest(GoogleAPIRequest* request);

        void onRequestFinished();
};

#endif // GOOGLEDRIVEAPI_H
