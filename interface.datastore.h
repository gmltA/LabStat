#ifndef IDATASTORE_H
#define IDATASTORE_H

#include "datasheet.h"

#include <QString>
#include <QObject>

class IDataStore
{
    public:
        enum Origin
        {
            OriginOffline,
            OriginOnline,
            OriginAny
        };

        IDataStore(Origin _origin) : origin(_origin) {}
        virtual ~IDataStore() {}

        virtual void init() = 0;
        virtual void syncFile(DataSheet* dataFile) = 0;

        Origin getOrigin() const { return origin; }
        void setOrigin(const Origin& value) { origin = value; }

    signals:
        virtual void syncDone() = 0;

    private:
        Origin origin;
};

Q_DECLARE_INTERFACE(IDataStore, "IDataStore")

#endif // IDATASTORE_H
