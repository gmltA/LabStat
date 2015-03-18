#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include "interface.datastore.h"
#include <functional>

#include <QNetworkAccessManager>

class GoogleDriveAPI : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)

    public:
        static GoogleDriveAPI& getInstance()
        {
            static GoogleDriveAPI instance;
            return instance;
        }

        QString getToken() const override;
        void setToken(const QString& value) override;

    private:
        ~GoogleDriveAPI() {}
        GoogleDriveAPI(QObject *parent = 0) : QObject(parent), IDataStore(ORIGIN_ONLINE) { network = new QNetworkAccessManager(); }
        GoogleDriveAPI(const GoogleDriveAPI&);
        GoogleDriveAPI& operator=(const GoogleDriveAPI&);

        bool checkAuth(QNetworkReply* reply, std::function<void ()> callback);

        QNetworkAccessManager* network;
        QString token;

    signals:

    public slots:
        void test() override;
        void createFile() override;

        void onAPITestFinished();
        void onInsertFinished();
};

#endif // GOOGLEDRIVEAPI_H
