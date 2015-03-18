#ifndef SYNCHANDLER_H
#define SYNCHANDLER_H

#include "interface.datastore.h"

#include <QObject>
#include <QVector>

class SyncHandler : public QObject
{
        Q_OBJECT
    public:
        static SyncHandler& getInstance()
        {
            static SyncHandler instance;
            return instance;
        }

        void sync(DataStoreOrigin origin = ORIGIN_ANY);

        void registerProcessor(IDataStore* processor);
        void unregisterProcessor(IDataStore* processor);

    private:
        ~SyncHandler();
        SyncHandler(QObject *parent = 0);
        SyncHandler(const SyncHandler&);
        SyncHandler& operator=(const SyncHandler&);

        void sync(IDataStore* processor);

        QVector<IDataStore*> syncProcessors;

    signals:

    public slots:
};

#endif // SYNCHANDLER_H
