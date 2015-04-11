#include "drivesyncprocessor.h"

DriveSyncProcessor::DriveSyncProcessor(GoogleDriveAPI* drive, QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOnline), driveService(drive)
{

}

DriveSyncProcessor::~DriveSyncProcessor()
{

}

void DriveSyncProcessor::init()
{

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
