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
        GoogleAPIRequestResult(const GoogleAPIRequestResult &other) : QObject(other.parent())
        {
            Q_UNUSED(other)
        }
        ~GoogleAPIRequestResult() {}

        virtual void handleReply(QNetworkReply* reply) = 0;

        std::function<void ()> getCallback() const;
        void setCallback(const std::function<void ()>& value);

    private:
        // Best thing that came to my mind after 3 days of brainstorm, think about rewriting it later
        std::function<void ()> callback;
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

Q_DECLARE_METATYPE(GoogleAPIRequestResult*)

#endif // GOOGLEAPIREQUESTRESULT_H
