#ifndef IDATASTORE_H
#define IDATASTORE_H

#include <QObject>

class IDataStore
{
    public:
        virtual ~IDataStore() {}

        virtual void createFile() = 0;
        virtual void test() = 0;

        virtual QString getToken() const = 0;
        virtual void setToken(const QString& value) = 0;
};

Q_DECLARE_INTERFACE(IDataStore, "IDataStore")

#endif // IDATASTORE_H
