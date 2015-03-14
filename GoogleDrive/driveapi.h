#ifndef GOOGLEDRIVEAPI_H
#define GOOGLEDRIVEAPI_H

#include <QNetworkAccessManager>
#include <QObject>

class GoogleDriveAPI : public QObject
{
        Q_OBJECT
    public:
        static GoogleDriveAPI& getInstance()
        {
            static GoogleDriveAPI instance;
            return instance;
        }

        void test();
        void insert();

        QString getToken() const;
        void setToken(const QString& value);

    private:
        ~GoogleDriveAPI();
        GoogleDriveAPI(QObject *parent = 0) : QObject(parent) { network = new QNetworkAccessManager(); }
        GoogleDriveAPI(const GoogleDriveAPI&);
        GoogleDriveAPI& operator=(const GoogleDriveAPI&);

        QNetworkAccessManager* network;
        QString token;

    signals:

    public slots:
        void onAPITestFinished();
        void onInsertFinished();
};

#endif // GOOGLEDRIVEAPI_H
