#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include "apirequest.h"
#include <QNetworkReply>

#define VERBOSE(message) \
    if (verboseOutput) \
        qDebug() << message;

class GoogleDriveAPI : public QObject
{
        Q_OBJECT

    public:
        GoogleDriveAPI(QString _rootFolderName, QObject *parent = 0);
        ~GoogleDriveAPI();

        QString getToken() const;

        void init();

        bool createFile(DriveFile* file);
        bool updateFile(DriveFile* file);
        void getFile(DriveFile* file);

        QVector<DriveFile> listFiles(DriveFile* templateFile);
        QVector<DriveFile> listFiles(QString searchQuery);

        QNetworkReply* sendRequest(GoogleAPIRequest request);

        bool getVerboseOutput() const;
        void setVerboseOutput(bool value);

    private:
        bool checkAuth(QNetworkReply* reply);

        QString token;
        DriveFile* appRootDir;

        bool verboseOutput;

    signals:
        void authRequired();
        void authRecovered();

    public slots:
        void setToken(const QString& value);
};

#endif // GOOGLEDRIVEAPI_H
