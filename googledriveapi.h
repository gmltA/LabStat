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

        void test(QString token);
        void insert(QString token);

    private:
        ~GoogleDriveAPI();
        GoogleDriveAPI(QObject *parent = 0) { network = new QNetworkAccessManager(); }
        GoogleDriveAPI(const GoogleDriveAPI&);
        GoogleDriveAPI& operator=(const GoogleDriveAPI&);

        QNetworkAccessManager* network;

    signals:

    public slots:
        void onAPITestFinished();
        void onInsertFinished();
};

#endif // GOOGLEDRIVEAPI_H
