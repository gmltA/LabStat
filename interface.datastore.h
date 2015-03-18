#ifndef IDATASTORE_H
#define IDATASTORE_H

#include <QString>
#include <QObject>

enum DataStoreOrigin
{
    ORIGIN_OFFLINE,
    ORIGIN_ONLINE,
    ORIGIN_ANY
};

class IDataStore
{
    public:
        IDataStore(DataStoreOrigin _origin) : origin(_origin) {}
        virtual ~IDataStore() {}

        virtual void createFile() = 0;
        virtual void test() = 0;

        virtual QString getToken() const = 0;
        virtual void setToken(const QString& value) = 0;

        DataStoreOrigin getOrigin() const { return origin; }
        void setOrigin(const DataStoreOrigin& value) { origin = value; }

    private:
        DataStoreOrigin origin;
};

Q_DECLARE_INTERFACE(IDataStore, "IDataStore")

#endif // IDATASTORE_H
