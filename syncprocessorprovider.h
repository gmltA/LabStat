#ifndef SYNCPROCESSORPROVIDER_H
#define SYNCPROCESSORPROVIDER_H

#include "interface.syncprocessorcreator.h"
#include <QObject>

class SyncProcessorProvider
{
        Q_DISABLE_COPY(SyncProcessorProvider)
    public:
        static SyncProcessorProvider& getInstance();

        ISyncProcessor* createProcessor(int processorTypeId, QString data);

        void addCreator(ISyncProcessorCreator* creator);

        QStringList getAvailableProcessorTypes();

    private:
        SyncProcessorProvider() {}

        QList<ISyncProcessorCreator*> creators;
};

#endif // SYNCPROCESSORPROVIDER_H
