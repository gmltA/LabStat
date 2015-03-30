#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include "interface.datastore.h"

#include <QObject>
#include <QVector>

class SyncHandler : public QObject
{
        Q_OBJECT
    public:
        static SyncHandler& getInstance();

        void registerProcessor(IDataStore* processor);
        void unregisterProcessor(IDataStore* processor);

    private:
        ~SyncHandler();
        SyncHandler(QObject *parent = 0);
        SyncHandler(const SyncHandler&) = delete;
        SyncHandler& operator=(const SyncHandler&) = delete;

        void sync(IDataStore* processor);

        QVector<IDataStore*> syncProcessors;

    signals:

    public slots:
        void sync(IDataStore::Origin origin = IDataStore::OriginAny);
};

#endif // SYNCHANDLER_H
