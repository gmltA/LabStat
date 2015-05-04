#ifndef DRIVESYNCPROCESSOR_H
#define DRIVESYNCPROCESSOR_H

#include <QObject>
#include "../interface.datastore.h"
#include "API/driveapi.h"

class DriveSyncProcessor : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)

    public:
        DriveSyncProcessor(GoogleDriveAPI* drive, QString fileName, QObject *parent = 0);
        ~DriveSyncProcessor();

        void init() override final;
        void syncFile(DataSheet* dataFile) override final;

        GoogleDriveAPI* getDriveService() const;
        void setDriveService(GoogleDriveAPI* value);

    private:
        GoogleDriveAPI* driveService;
        SpreadSheet* sheet;

        void fillSpreadSheet();

    signals:
        void initFinished(bool success) override final;
        void syncDone() override final;

    public slots:
};

#endif // DRIVESYNCPROCESSOR_H
