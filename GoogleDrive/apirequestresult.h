#ifndef GOOGLEAPIREQUESTRESULT_H
#define GOOGLEAPIREQUESTRESULT_H

#include "drivefile.h"

#include <QObject>
#include <QNetworkReply>

class GoogleAPIRequestResult
{
    public:
        GoogleAPIRequestResult() {}
        GoogleAPIRequestResult(const GoogleAPIRequestResult &other)
        {
            Q_UNUSED(other)
        }
        ~GoogleAPIRequestResult() {}

        virtual void handleReply(QNetworkReply* reply) = 0;
};

class UserInfoRequestResult : public GoogleAPIRequestResult
{
    public:
        UserInfoRequestResult() : GoogleAPIRequestResult() {}

        void handleReply(QNetworkReply* reply) override;
};

class InsertFileRequestResult : public GoogleAPIRequestResult
{
    public:
        InsertFileRequestResult(DriveFile* _file) : GoogleAPIRequestResult(), file(_file) {}

        void handleReply(QNetworkReply* reply) override;

        DriveFile* getFile() const;

    private:
        DriveFile* file;
};

Q_DECLARE_METATYPE(GoogleAPIRequestResult*)

#endif // GOOGLEAPIREQUESTRESULT_H
