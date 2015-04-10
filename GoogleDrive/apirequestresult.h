#ifndef GOOGLEAPIREQUESTRESULT_H
#define GOOGLEAPIREQUESTRESULT_H

#include "drivefile.h"

#include <functional>

#include <QObject>
#include <QNetworkReply>

class GoogleAPIRequestResult : public QObject
{
        Q_OBJECT
    public:
        GoogleAPIRequestResult(QObject* parent = 0) : QObject(parent) {}
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

class UpdateFileRequestResult : public GoogleAPIRequestResult
{
    public:
        UpdateFileRequestResult(DriveFile* _file) : GoogleAPIRequestResult(), file(_file) {}

        void handleReply(QNetworkReply* reply) override;

        DriveFile* getFile() const;

    private:
        DriveFile* file;
};

class GetFileRequestResult : public GoogleAPIRequestResult
{
    public:
        GetFileRequestResult(DriveFile* _file) : GoogleAPIRequestResult(), file(_file) {}

        void handleReply(QNetworkReply* reply) override;

        DriveFile* getFile() const;

    private:
        DriveFile* file;
};

class ListFilesRequestResult : public GoogleAPIRequestResult
{
    public:
        ListFilesRequestResult() : GoogleAPIRequestResult() {}
        ~ListFilesRequestResult();

        void handleReply(QNetworkReply* reply) override;

        QVector<DriveFile> getFileList() const;

    private:
        QVector<DriveFile> fileList;
};

#endif // GOOGLEAPIREQUESTRESULT_H
