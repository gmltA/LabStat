#include "sqlitesyncprocessor.h"

SQLiteSyncProcessor::SQLiteSyncProcessor(QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOffline, "SQLite local")
{

}

SQLiteSyncProcessor::~SQLiteSyncProcessor()
{
}

void SQLiteSyncProcessor::init()
{
    // todo: check and\or re-init DB structure
    emit initFinished(true);
}

void SQLiteSyncProcessor::syncFile(DataSheet *dataFile)
{
    emit syncDone();
}

