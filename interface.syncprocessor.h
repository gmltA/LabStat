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

        ISyncProcessor(Origin _origin, QString _title) : origin(_origin), title(_title) {}
        virtual ~ISyncProcessor() {}

        virtual void init() = 0;
        virtual void syncFile(DataSheet* dataFile) = 0;

        Origin getOrigin() const { return origin; }
        void setOrigin(const Origin& value) { origin = value; }

        QString getTitle() const { return title; }

        int getId() const { return id; }
        void setId(int value) { id = value; }

    signals:
        virtual void initFinished(bool success) = 0;
        virtual void syncDone() = 0;

    protected:
        int id;
        Origin origin;
        const QString title;
};

Q_DECLARE_INTERFACE(ISyncProcessor, "ISyncProcessor")

#endif // ISYNCPROCESSOR_H
