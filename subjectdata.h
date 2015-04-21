#ifndef SUBJECTDATA_H
#define SUBJECTDATA_H

#include "datasheet.h"
#include "synchandler.h"

#include <QObject>

class SubjectData : public QObject
{
        Q_OBJECT
    public:
        explicit SubjectData(QString title, QObject *parent = 0);
        ~SubjectData();

        DataSheet* getDataSheet() const;
        SyncHandler* getSyncHandler() const;

        Q_INVOKABLE void attachDrive(QString rootFolder = "LabStat");
        void disconnectAll();

    private:
        DataSheet* dataSheet;
        SyncHandler* syncHandler;

    signals:

    public slots:
};

#endif // SUBJECTDATA_H
