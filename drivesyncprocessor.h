#ifndef DRIVESYNCPROCESSOR_H
#define DRIVESYNCPROCESSOR_H

#include <QObject>
#include "interface.datastore.h"
#include "GoogleDrive/driveapi.h"

class DriveSyncProcessor : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)
    public:
        DriveSyncProcessor(GoogleDriveAPI* drive, QObject *parent = 0);
        ~DriveSyncProcessor();

        void init() override final;
        void syncFile(DataSheet *dataFile) override final;

        GoogleDriveAPI* getDriveService() const;
        void setDriveService(GoogleDriveAPI* value);

    private:
        GoogleDriveAPI* driveService;

    signals:
        void syncDone() override final;

    public slots:
};

#endif // DRIVESYNCPROCESSOR_H
