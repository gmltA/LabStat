#include "subjectdata.h"
#include "synchandler.h"

#include <QDebug>
#include <QtConcurrent>

SyncHandler::SyncHandler(QObject* parent) : QObject(parent)
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
    QtConcurrent::run(processor, &IDataStore::syncFile, dynamic_cast<SubjectData*>(parent())->getDataSheet());
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
    syncProcessors.push_back(processor);
    processor->setId(syncProcessors.indexOf(processor));

    emit processorAddCalled(buildProcessorData(processor));

    connect(dynamic_cast<QObject*>(processor), SIGNAL(initFinished(bool)), this, SLOT(checkProcessorInit(bool)));
    processor->init();
}

void SyncHandler::checkProcessorInit(bool success)
{
    QVariantMap processorData;
    IDataStore* processor = qobject_cast<IDataStore*>(sender());

    if (success)
    {
        //todo: check if we have this processor in collection already
        processorData = buildProcessorData(processor);

        signalMapper->setMapping(dynamic_cast<QObject*>(processor), syncProcessors.indexOf(processor));
    }
    else
        unregisterProcessor(processor);

    processorData["result"] = success;
    emit processorAdded(processorData);
}

void SyncHandler::unregisterProcessor(IDataStore* processor)
{
    syncProcessors.removeOne(processor);
}
