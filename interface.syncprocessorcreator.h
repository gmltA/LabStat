#ifndef INTERFACE_SYNCPROCESSORCREATOR_H
#define INTERFACE_SYNCPROCESSORCREATOR_H

#include "interface.syncprocessor.h"

/*!
 * \brief The ISyncProcessorCreator interface provides unified methods to implement plugin system
 *
 * Every new \c ISyncProcessor should provide Creator to its object. Creator has all data needed to register
 * new kind of syncronization and display its title to user.
 */
class ISyncProcessorCreator
{
    public:
        ISyncProcessorCreator() {}
        virtual ISyncProcessor* createProcessor(QString data) = 0;
        virtual QString getProcessorTitle() = 0;
};

Q_DECLARE_INTERFACE(ISyncProcessorCreator, "ISyncProcessorCreator")

#endif // INTERFACE_SYNCPROCESSORCREATOR_H
