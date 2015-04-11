#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include "apirequest.h"
#include <functional>

class GoogleDriveAPI : public QObject
{
        Q_OBJECT

    public:
        GoogleDriveAPI(QString _rootFolderName, QObject *parent = 0);
        ~GoogleDriveAPI();

        QString getToken() const;

        void init();

        void createFile(DriveFile* file);
        void updateFile(DriveFile* file);
        void getFile(DriveFile* file);

        QVector<DriveFile> listFiles(DriveFile* templateFile);
        QVector<DriveFile> listFiles(QString searchQuery);

        template<class T = GoogleAPIRequestResult*>
        T sendRequest(GoogleAPIRequest* request);

    private:
        bool checkAuth(QNetworkReply* reply);

        QString token;
        DriveFile* appRootDir;

    signals:
        void authRequired();
        void authRecovered();

    public slots:
        void setToken(const QString& value);
};

#endif // GOOGLEDRIVEAPI_H
