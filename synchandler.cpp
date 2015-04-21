#include "synchandler.h"

#include <QDebug>
#include <QtConcurrent>

SyncHandler::SyncHandler() : QObject()
{
    signalMapper = new QSignalMapper();
    connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(syncStopped(int)));
}

SyncHandler::~SyncHandler()
{
    foreach(IDataStore* processor, syncProcessors)
        dynamic_cast<QObject*>(processor)->deleteLater();
}

void SyncHandler::sync(int processorIndex)
{
    IDataStore* processor = syncProcessors.at(processorIndex);
    sync(processor);
}

void SyncHandler::sync(IDataStore* processor)
{
    if (!processor)
    {
        qDebug() << "Trying to sync data with NULL processor";
        return;
    }

    connect(dynamic_cast<QObject*>(processor), SIGNAL(syncDone()), signalMapper, SLOT(map()));
    QtConcurrent::run(processor, &IDataStore::init);
}

QVariantMap SyncHandler::buildProcessorData(IDataStore* processor)
{
    QVariantMap processorData;
    processorData["id"] = syncProcessors.contains(processor) ? syncProcessors.indexOf(processor) : -1;
    processorData["title"] = processor->getTitle();
    processorData["online"] = processor->getOrigin() == IDataStore::OriginOnline ? 1 : 0;

    return processorData;
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
    emit processorAddCalled(buildProcessorData(processor));

    connect(dynamic_cast<QObject*>(processor), SIGNAL(initFinished(bool)), this, SLOT(checkProcessorInit(bool)));
    processor->init();
}

void SyncHandler::checkProcessorInit(bool success)
{
    QVariantMap processorData;
    if (success)
    {
        IDataStore* processor = qobject_cast<IDataStore*>(sender());

        //todo: check if we have this processor in collection already
        syncProcessors.push_back(processor);
        processorData = buildProcessorData(processor);

        signalMapper->setMapping(dynamic_cast<QObject*>(processor), syncProcessors.indexOf(processor));
    }
    processorData["result"] = success;
    emit processorAdded(processorData);
}

void SyncHandler::unregisterProcessor(IDataStore* processor)
{
    syncProcessors.removeOne(processor);
}
