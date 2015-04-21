#include "drivesyncprocessor.h"

DriveSyncProcessor::DriveSyncProcessor(GoogleDriveAPI* drive, QString fileName, QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOnline, "Google Drive"), driveService(drive)
{
    data = new DataSheet(fileName);
}

DriveSyncProcessor::~DriveSyncProcessor()
{

    data->deleteLater();
}

void DriveSyncProcessor::init()
{
    emit initFinished(driveService->init());
}

void DriveSyncProcessor::syncFile(DataSheet* dataFile)
{

}

GoogleDriveAPI* DriveSyncProcessor::getDriveService() const
{
    return driveService;
}

void DriveSyncProcessor::setDriveService(GoogleDriveAPI* value)
{
    driveService = value;
}
