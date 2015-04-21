#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include "interface.datastore.h"

#include <QObject>
#include <QSignalMapper>
#include <QVariantMap>
#include <QVector>

class SyncHandler : public QObject
{
        Q_OBJECT

    public:
        SyncHandler();
        ~SyncHandler();

        void registerProcessor(IDataStore* processor);
        void unregisterProcessor(IDataStore* processor);

        Q_INVOKABLE void sync(int processorIndex);
        void sync(IDataStore::Origin origin = IDataStore::OriginAny);

    private:

        void sync(IDataStore* processor);
        QVariantMap buildProcessorData(IDataStore* processor);

        QVector<IDataStore*> syncProcessors;
        QSignalMapper* signalMapper;

    signals:
        void processorAddCalled(QVariantMap processorData);
        void processorAdded(QVariantMap processorData);
        void syncStopped(int processorId);

    private slots:
        void checkProcessorInit(bool success);

};

#endif // SYNCHANDLER_H
