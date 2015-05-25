#ifndef ISYNCPROCESSOR_H
#define ISYNCPROCESSOR_H

#include "datasheet.h"

#include <QString>
#include <QObject>

/*!
 * \brief ISyncProcessor is an interface for every sync processor
 * that can be integrated into application.
 */
class ISyncProcessor
{
    public:
        /*!
         * \brief The Origin enum represents kind of data storage e.g. online or offline
         *
         * Enum elements can be used to sync data with multiple processors simultainously
         * by choosing origin type.
         *
         * \example syncHandler.sync(OriginOffline) will invoke sync operation for all
         * offline processors attached to the current subject.
         */
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

        /*!
         * \brief processorTypeName is used by ISyncProcessorCreator to provide name of processor
         * to SyncProcessorProvider to form list of processors for front-end
         */
        static const QString processorTypeName;

        QString getTitle() const { return title; }

    signals:
        virtual void initFinished(bool success) = 0;
        virtual void syncDone() = 0;

    protected:
        /*!
         * \brief id is a global identicator unique for every processor
         */
        int id;
        /*!
         * \brief typeId is set by derived class
         */
        int typeId;

        QString title;
        QString data;

        Origin origin;
};

Q_DECLARE_INTERFACE(ISyncProcessor, "ISyncProcessor")

#endif // ISYNCPROCESSOR_H
