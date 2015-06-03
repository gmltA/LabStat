#ifndef INTERFACE_AUTHCLIENT
#define INTERFACE_AUTHCLIENT

#include <QObject>

/*!
 * \brief The IAuthClient interface is used by DriveAPI to obtain credentials in different ways.
 */
class IAuthClient
{
    public:
         virtual ~IAuthClient(){}

    signals:
        virtual void tokenObtained(QString) = 0;
        virtual void authCompleted(QString) = 0;

    public slots:
        virtual void processAuth() = 0;
};

Q_DECLARE_INTERFACE(IAuthClient, "IAuthClient")

#endif // INTERFACE_AUTHCLIENT

