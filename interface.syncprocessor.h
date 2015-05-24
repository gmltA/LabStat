#ifndef ISYNCPROCESSOR_H
#define ISYNCPROCESSOR_H

#include "datasheet.h"

#include <QString>
#include <QObject>

class ISyncProcessor
{
    public:
        enum Origin
        {
            OriginOffline,
            OriginOnline,
            OriginAny
        };

        ISyncProcessor(QString _title, QString _data, Origin _origin) : title(_title), data(_data), origin(_origin) {}
        virtual ~ISyncProcessor() {}

        virtual void init() = 0;
        virtual void syncFile(DataSheet* dataFile) = 0;

        Origin getOrigin() const { return origin; }
        void setOrigin(const Origin& value) { origin = value; }

        int getId() const { return id; }
        void setId(int value) { id = value; }

        int getTypeId() const { return typeId; }
        void setTypeId(int value) { typeId = value; }

        QString getData() const { return data; }
        void setData(const QString& value) { data = value; }

        static const QString processorTypeName;

        QString getTitle() const { return title; }

    signals:
        virtual void initFinished(bool success) = 0;
        virtual void syncDone() = 0;

    protected:
        int id;
        int typeId;

        QString title;
        QString data;

        Origin origin;
};

Q_DECLARE_INTERFACE(ISyncProcessor, "ISyncProcessor")

#endif // ISYNCPROCESSOR_H