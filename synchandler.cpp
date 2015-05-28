#include "subjectdata.h"
#include "synchandler.h"
#include "appdatastorage.h"

#include <QDebug>
#include <QtConcurrent>

SyncHandler::SyncHandler(QObject* parent) : QObject(parent)
{
    signalMapper = new QSignalMapper();
    connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(syncStopped(int)));
}

SyncHandler::~SyncHandler()
{
    foreach(ISyncProcessor* processor, syncProcessors)
    {
        AppDataStorage::getInstance().removeProcessor(dynamic_cast<SubjectData*>(parent()), processor);
        dynamic_cast<QObject*>(processor)->deleteLater();
    }
}

void SyncHandler::sync(int processorIndex)
{
    ISyncProcessor* processor = syncProcessors.at(processorIndex);
    sync(processor);
}

void SyncHandler::sync(ISyncProcessor* processor)
{
    if (!processor)
    {
        qDebug() << "Trying to sync data with NULL processor";
        return;
    }

    connect(dynamic_cast<QObject*>(processor), SIGNAL(syncDone()), signalMapper, SLOT(map()));
    QtConcurrent::run(processor, &ISyncProcessor::syncFile, dynamic_cast<SubjectData*>(parent())->getDataSheet());
}

QVariantMap SyncHandler::buildProcessorData(ISyncProcessor* processor)
{
    QVariantMap processorData;
    processorData["id"] = syncProcessors.contains(processor) ? processor->getId() : -1;
    processorData["title"] = processor->getTitle();
    processorData["online"] = processor->getOrigin() == ISyncProcessor::OriginOnline ? 1 : 0;

    return processorData;
}

void SyncHandler::sync(ISyncProcessor::Origin origin)
{
    foreach(ISyncProcessor* processor, syncProcessors)
    {
        if (processor->getOrigin() == origin || origin == ISyncProcessor::OriginAny)
        {
            sync(processor);
        }
    }
}

QVariantList SyncHandler::buildProcessorsData()
{
    QVariantList processorsData;
    foreach (ISyncProcessor* processor, syncProcessors)
    {
        processorsData.append(buildProcessorData(processor));
    }

    return processorsData;
}

void SyncHandler::registerProcessor(ISyncProcessor* processor)
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
    ISyncProcessor* processor = qobject_cast<ISyncProcessor*>(sender());

    if (success)
    {
        //todo: check if we have this processor in collection already
        processorData = buildProcessorData(processor);
        AppDataStorage::getInstance().storeProcessor(dynamic_cast<SubjectData*>(parent()), processor);

        signalMapper->setMapping(dynamic_cast<QObject*>(processor), processor->getId());
    }
    else
        unregisterProcessor(processor);

    processorData["result"] = success;
    emit processorAdded(processorData);
}

void SyncHandler::unregisterProcessor(ISyncProcessor* processor)
{
    syncProcessors.removeOne(processor);
}

void SyncHandler::deleteProcessor(int processorIndex)
{
    foreach (ISyncProcessor* processor, syncProcessors)
        if (processor->getId() == processorIndex)
        {
            unregisterProcessor(processor);
            AppDataStorage::getInstance().removeProcessor(dynamic_cast<SubjectData*>(parent()), processor);
            dynamic_cast<QObject*>(processor)->deleteLater();
            break;
        }
}
