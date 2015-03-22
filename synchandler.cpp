#include "synchandler.h"
#include <QDebug>

SyncHandler::SyncHandler(QObject *parent) : QObject(parent)
{

}

void SyncHandler::sync(IDataStore* processor)
{
    if (!processor)
    {
        qDebug() << "Trying to sync data with NULL processor";
        return;
    }
}

void SyncHandler::sync(IDataStore::Origin origin)
{
    foreach(IDataStore* processor, syncProcessors)
    {
        if (processor->getOrigin() == origin || origin == IDataStore::OriginAny)
        {
            sync(processor);
        }
    }
}

void SyncHandler::registerProcessor(IDataStore* processor)
{
    //todo: check if we have this processor in ollection already
    syncProcessors.push_back(processor);
}

void SyncHandler::unregisterProcessor(IDataStore* processor)
{
    syncProcessors.removeOne(processor);
}

SyncHandler::~SyncHandler()
{

}
