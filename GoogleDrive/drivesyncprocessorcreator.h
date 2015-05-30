#ifndef DRIVESYNCPROCESSORCREATOR_H
#define DRIVESYNCPROCESSORCREATOR_H

#include "interface.syncprocessorcreator.h"

class DriveSyncProcessorCreator : public ISyncProcessorCreator
{
        Q_INTERFACES(ISyncProcessorCreator)

    public:
        DriveSyncProcessorCreator() : ISyncProcessorCreator() {}
        ISyncProcessor* createProcessor(QString data);

        QString getProcessorTitle();
};

#endif // DRIVESYNCPROCESSORCREATOR_H
