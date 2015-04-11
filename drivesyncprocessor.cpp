#include "drivesyncprocessor.h"

DriveSyncProcessor::DriveSyncProcessor(GoogleDriveAPI* _drive, QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOnline), drive(_drive)
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
