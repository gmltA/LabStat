#ifndef SQLITESYNCPROCESSORCREATOR_H
#define SQLITESYNCPROCESSORCREATOR_H

#include "interface.syncprocessorcreator.h"

class SQLiteSyncProcessorCreator : public ISyncProcessorCreator
{
        Q_INTERFACES(ISyncProcessorCreator)

    public:
        SQLiteSyncProcessorCreator() : ISyncProcessorCreator() {}
        ISyncProcessor* createProcessor(QString data);
        QString getProcessorTitle();
};

#endif // SQLITESYNCPROCESSORCREATOR_H
