#ifndef DRIVESYNCPROVIDER_H
#define DRIVESYNCPROVIDER_H

#include "interface.datastore.h"
#include <QObject>
#include <GoogleDrive/driveapi.h>

class DriveSyncProcessor : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)
    public:
        explicit DriveSyncProcessor(GoogleDriveAPI* _drive, QObject *parent = 0);
        ~DriveSyncProcessor();

        void init() override final;
        void syncFile(DataSheet* dataFile) override final;

        GoogleDriveAPI* getDriveService() const;
        void setDriveService(GoogleDriveAPI* value);

    private:
        GoogleDriveAPI* driveService;

    signals:
        void syncDone() override final;

    public slots:
};

#endif // DRIVESYNCPROVIDER_H
