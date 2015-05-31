#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include "interface.syncprocessor.h"

#include <QObject>
#include <QSignalMapper>
#include <QVariantMap>
#include <QVector>

class SyncHandler : public QObject
{
        Q_OBJECT

    public:
        SyncHandler(QObject* parent);
        ~SyncHandler();

        void registerProcessor(ISyncProcessor* processor);
        void unregisterProcessor(ISyncProcessor* processor);

        void deleteProcessor(int processorId);

        Q_INVOKABLE void sync(int processorIndex);
        void sync(ISyncProcessor::Origin origin = ISyncProcessor::OriginAny);

        QVariantList buildProcessorsData();

    private:
        ISyncProcessor* getProcessorById(int processorId);

        void sync(ISyncProcessor* processor);
        QVariantMap buildProcessorData(ISyncProcessor* processor);

        void deleteProcessor(ISyncProcessor* processor);

        QVector<ISyncProcessor*> syncProcessors;
        QSignalMapper* signalMapper;

    signals:
        void processorAddCalled(QVariantMap processorData);
        void processorAdded(QVariantMap processorData);
        void syncStopped(int processorId);

    private slots:
        void checkProcessorInit(bool success);

};

#endif // SYNCHANDLER_H
