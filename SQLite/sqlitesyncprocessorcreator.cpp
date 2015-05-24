#include "sqlitesyncprocessor.h"
#include "sqlitesyncprocessorcreator.h"

ISyncProcessor* SQLiteSyncProcessorCreator::createProcessor(QString data)
{
    return new SQLiteSyncProcessor(data);
}

QString SQLiteSyncProcessorCreator::getProcessorTitle()
{
    return SQLiteSyncProcessor::processorTypeName;
}
