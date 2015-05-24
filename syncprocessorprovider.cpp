#include "syncprocessorprovider.h"

SyncProcessorProvider& SyncProcessorProvider::getInstance()
{
    static SyncProcessorProvider instance;
    return instance;
}

ISyncProcessor* SyncProcessorProvider::createProcessor(int processorTypeId, QString data)
{
    if (creators.size() == 0 || creators.size() < processorTypeId)
        return nullptr;
    else
    {
        ISyncProcessor* processor = creators.at(processorTypeId)->createProcessor(data);
        processor->setTypeId(processorTypeId);
        return processor;
    }
}

void SyncProcessorProvider::addCreator(ISyncProcessorCreator* creator)
{
    creators.append(creator);
}

QStringList SyncProcessorProvider::getAvailableProcessorTypes()
{
    QStringList processorTypes;
    foreach (ISyncProcessorCreator* creator, creators)
        processorTypes.append(creator->getProcessorTitle());

    return processorTypes;
}
