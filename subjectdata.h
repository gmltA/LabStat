#ifndef SUBJECTDATA_H
#define SUBJECTDATA_H

#include "datasheet.h"
#include "synchandler.h"

#include <QObject>


class SubjectData : public QObject
{
        Q_OBJECT
    public:
        SubjectData(int id, QString title, QObject *parent = 0);
        SubjectData(QString title, QObject *parent = 0);
        ~SubjectData();

        DataSheet* getDataSheet() const;
        SyncHandler* getSyncHandler() const;

        void attachProcessor(int processorTypeId, QString additionalData = "LabStat");
        void disconnectAll();

        QString getTitle() const;
        void setTitle(const QString& value);

        int getId() const;
        void setId(int value);

    private:
        int id;
        QString title;

        DataSheet* dataSheet;
        SyncHandler* syncHandler;

    signals:

    public slots:
};

#endif // SUBJECTDATA_H
