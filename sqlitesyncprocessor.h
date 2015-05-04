#ifndef SQLITESYNCPROCESSOR_H
#define SQLITESYNCPROCESSOR_H

#include <QObject>
#include "interface.datastore.h"

class SQLiteSyncProcessor : public QObject, public IDataStore
{
        Q_OBJECT
        Q_INTERFACES(IDataStore)

    public:
        SQLiteSyncProcessor(QObject *parent = 0);
        ~SQLiteSyncProcessor();

        void init() override final;
        void syncFile(DataSheet* dataFile) override final;

    private:

    signals:
        void initFinished(bool success) override final;
        void syncDone() override final;

    public slots:
};

#endif // SQLITESYNCPROCESSOR_H
