#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include "../interface.datastore.h"
#include "../interface.authclient.h"
#include "apirequest.h"
#include <functional>

#include <QNetworkAccessManager>

class GoogleDriveAPI : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)

    public:
        GoogleDriveAPI(IAuthClient* _authClient, QObject *parent = 0);
        ~GoogleDriveAPI();

        QString getToken() const override;

        void init() override final;

        void createFolder();
        bool isFolderCreated();

    private:
        bool checkAuth(QNetworkReply* reply);

        QNetworkAccessManager* network;
        QString token;

        IAuthClient* authClient;

    signals:
        void authRequired();
        void authRecovered();

        void workDone();

    public slots:
        void test() override;
        void createFile() override;
        void sendRequest(GoogleAPIRequest* request, QNetworkAccessManager* manager = nullptr);

        void setToken(const QString& value) override;

        void onRequestFinished();
};

#endif // GOOGLEDRIVEAPI_H
