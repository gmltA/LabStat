#include "sqlitesyncprocessor.h"

SQLiteSyncProcessor::SQLiteSyncProcessor(QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOffline, "SQLite local"),
      db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

SQLiteSyncProcessor::~SQLiteSyncProcessor()
{
}

void SQLiteSyncProcessor::init()
{
    emit initFinished(true);
}

void SQLiteSyncProcessor::syncFile(DataSheet *dataFile)
{
    dataFile->synced(id);
    emit syncDone();
}

