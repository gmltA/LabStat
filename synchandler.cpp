#include "synchandler.h"

#include <QDebug>
#include <QtQml>
#include <QtConcurrent>

SyncHandler* SyncHandler::instance = nullptr;

SyncHandler::SyncHandler() : QObject()
{
    signalMapper = new QSignalMapper();
    connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(syncStopped(int)));
}

void SyncHandler::init()
{
    qmlRegisterSingletonType<SyncHandler>("SyncHandler", 1, 0, "SyncHandler", &SyncHandler::qmlInstance);
}

SyncHandler* SyncHandler::getInstance()
{
    if (!instance)
        instance = new SyncHandler();

    return instance;
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
    //QtConcurrent::run(processor, &IDataStore::init);
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
    connect(dynamic_cast<QObject*>(processor), SIGNAL(initFinished(bool)), this, SLOT(checkProcessorInit(bool)));
    processor->init();
}

void SyncHandler::checkProcessorInit(bool success)
{
    if (success)
    {
        IDataStore* processor = qobject_cast<IDataStore*>(sender());

        //todo: check if we have this processor in collection already
        syncProcessors.push_back(processor);

        signalMapper->setMapping(dynamic_cast<QObject*>(processor), syncProcessors.indexOf(processor));

        QVariantMap processorData;
        processorData["id"] = syncProcessors.indexOf(processor);
        processorData["title"] = processor->getTitle();
        processorData["online"] = processor->getOrigin() == IDataStore::OriginOnline ? 1 : 0;
        emit processorAdded(processorData);
    }
}

void SyncHandler::unregisterProcessor(IDataStore* processor)
{
    syncProcessors.removeOne(processor);
}
