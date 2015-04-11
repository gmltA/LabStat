#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include "interface.datastore.h"

#include <QObject>
#include <QSignalMapper>
#include <QVariantMap>
#include <QVector>

class QQmlEngine;
class QJSEngine;

/*
 * Pointer singleton here because QML requires object to be pointer.
 * Otherwise ASSERT'ation in on program exit happens.
 */

class SyncHandler : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(SyncHandler)

    public:
        static SyncHandler* getInstance();

        void registerProcessor(IDataStore* processor);
        void unregisterProcessor(IDataStore* processor);

        static void init();
        Q_INVOKABLE void sync(int processorIndex);
        void sync(IDataStore::Origin origin = IDataStore::OriginAny);

        static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
        {
            Q_UNUSED(engine);
            Q_UNUSED(scriptEngine);

            return SyncHandler::getInstance();
        }
    private:
        SyncHandler();
        ~SyncHandler() {}

        void sync(IDataStore* processor);
        QVariantMap buildProcessorData(IDataStore* processor);

        static SyncHandler* instance;
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
