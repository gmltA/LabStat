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
        GoogleDriveAPI(IAuthClient* _authClient, QString _rootFolderName, QObject *parent = 0);
        ~GoogleDriveAPI();

        QString getToken() const override;

        void init() override final;

        void syncFile(DataSheet* dataFile) override final;

        void createFileSync(DriveFile* file);
        void getFileSync(DriveFile* file);
        QVector<DriveFile> listFilesSync(DriveFile* templateFile);
        QVector<DriveFile> listFilesSync(QString searchQuery);

        void updateFileSync(DriveFile* file);

        template<class T = GoogleAPIRequestResult*>
        T sendSyncRequest(GoogleAPIRequest* request);

        void loadFileTable();
        void storeFileTable();
    private:
        bool checkAuth(QNetworkReply* reply);

        QString token;

        QHash<uint,DriveFileInfo> fileTable;

        DriveFile* appRootDir;

        IAuthClient* authClient;

    signals:
        void authRequired();
        void authRecovered();

        void workDone();
        void syncDone() override;

    public slots:
        void test() override;

        void setToken(const QString& value) override;

        void onRequestFinished();
};

#endif // GOOGLEDRIVEAPI_H
