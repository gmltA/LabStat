#ifndef INTERFACE_AUTHCLIENT
#define INTERFACE_AUTHCLIENT

#include <QObject>

class IAuthClient
{
    public:
         virtual ~IAuthClient(){}

    signals:
        virtual void tokenObtained(QString) = 0;
        virtual void authCompleted(QString) = 0;

    public slots:
        virtual void processAuth() = 0;

    private slots:
        virtual void onTokenObtained(QString token) = 0;
};

Q_DECLARE_INTERFACE(IAuthClient, "IAuthClient")

#endif // INTERFACE_AUTHCLIENT

