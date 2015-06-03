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
        deleteProcessor(processor);
    }
}

void SyncHandler::sync(int processorIndex, ISyncProcessor::SyncDirection direction)
{
    sync(getProcessorById(processorIndex), direction);
}

void SyncHandler::sync(ISyncProcessor* processor, ISyncProcessor::SyncDirection direction)
{
    if (!processor)
    {
        qDebug() << "Trying to sync data with NULL processor";
        return;
    }

    connect(dynamic_cast<QObject*>(processor), SIGNAL(syncDone()), signalMapper, SLOT(map()));
    QtConcurrent::run(processor, &ISyncProcessor::syncFile, dynamic_cast<SubjectData*>(parent())->getDataSheet(), direction);
}

QVariantMap SyncHandler::buildProcessorData(ISyncProcessor* processor)
{
    QVariantMap processorData;
    processorData["id"] = syncProcessors.contains(processor) ? processor->getId() : -1;
    processorData["title"] = processor->getTitle();
    processorData["online"] = processor->getOrigin() == ISyncProcessor::OriginOnline ? 1 : 0;

    return processorData;
}

void SyncHandler::deleteProcessor(ISyncProcessor* processor)
{
    SubjectData* subject = dynamic_cast<SubjectData*>(parent());

    processor->clear(subject->getDataSheet());
    AppDataStorage::getInstance().removeProcessor(subject, processor);

    dynamic_cast<QObject*>(processor)->deleteLater();
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

ISyncProcessor*SyncHandler::getProcessorById(int processorId)
{
    ISyncProcessor* result = nullptr;
    foreach (ISyncProcessor* processor, syncProcessors)
    {
        if (processor->getId() == processorId)
            result = processor;
    }
    return result;
}

void SyncHandler::registerProcessor(ISyncProcessor* processor)
{
    if (processor->getId() == PROC_ID_INVALID)
    {
        if (syncProcessors.count() > 0)
            processor->setId(syncProcessors.last()->getId() + 1);
        else
            processor->setId(0);
    }
    syncProcessors.push_back(processor);

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
    {
        unregisterProcessor(processor);
        delete processor;
    }

    processorData["result"] = success;
    emit processorAdded(processorData);
}

void SyncHandler::unregisterProcessor(ISyncProcessor* processor)
{
    syncProcessors.removeOne(processor);
}

void SyncHandler::deleteProcessor(int processorId)
{
    foreach (ISyncProcessor* processor, syncProcessors)
        if (processor->getId() == processorId)
        {
            unregisterProcessor(processor);
            deleteProcessor(processor);
            break;
        }
}
