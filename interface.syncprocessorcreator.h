#ifndef INTERFACE_SYNCPROCESSORCREATOR_H
#define INTERFACE_SYNCPROCESSORCREATOR_H

#include "interface.syncprocessor.h"

class ISyncProcessorCreator
{
    public:
        ISyncProcessorCreator() {}
        virtual ISyncProcessor* createProcessor(QString data) = 0;
        virtual QString getProcessorTitle() = 0;
};

Q_DECLARE_INTERFACE(ISyncProcessorCreator, "ISyncProcessorCreator")

#endif // INTERFACE_SYNCPROCESSORCREATOR_H
