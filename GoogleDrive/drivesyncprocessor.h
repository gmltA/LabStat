#ifndef DRIVESYNCPROCESSOR_H
#define DRIVESYNCPROCESSOR_H

#include <QObject>
#include "../interface.syncprocessor.h"
#include "API/driveapi.h"

class DriveSyncProcessor : public QObject, public ISyncProcessor
{
        Q_OBJECT
        Q_INTERFACES(ISyncProcessor)

    public:
        DriveSyncProcessor(GoogleDriveAPI* drive, QString fileName, QObject *parent = 0);
        ~DriveSyncProcessor();

        void init() override final;
        void syncFile(DataSheet* dataFile) override final;

        GoogleDriveAPI* getDriveService() const;
        void setDriveService(GoogleDriveAPI* value);

        static const QString processorTypeName;

    private:
        GoogleDriveAPI* driveService;
        SpreadSheet* sheet;

        void fillSpreadSheet();
        QList<int>              buildGroupList(QByteArray rawData);
        StudentList             buildStudentList(QByteArray rawData);
        TimeTable               buildTimeTable(QByteArray rawData);
        StatTable               buildStats(QByteArray rawData);

        QList<QDomElement>      selectDateElementList(QDomNodeList dateNodes);

        QMap<int, QString>      timeTableAccordance;

    signals:
        void initFinished(bool success) override final;
        void syncDone() override final;

    public slots:
};

#endif // DRIVESYNCPROCESSOR_H
