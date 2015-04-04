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

        virtual void createFile() = 0;
        virtual void test() = 0;

        virtual QString getToken() const = 0;

        Origin getOrigin() const { return origin; }
        void setOrigin(const Origin& value) { origin = value; }

    public slots:
        virtual void setToken(const QString& value) = 0;

    private:
        Origin origin;
};

Q_DECLARE_INTERFACE(IDataStore, "IDataStore")

#endif // IDATASTORE_H
