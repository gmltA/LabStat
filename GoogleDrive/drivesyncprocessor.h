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

        void saveData(DataSheet* dataFile);
        void loadData(DataSheet* dataFile);

    private:
        GoogleDriveAPI* driveService;
        SpreadSheet* sheet;

        void fillSpreadSheet();
        void createDbStructure();
        void saveTimeTableTagAccordance();
        void loadTimeTableTagAccordance();

        QList<int>              parseGroupList(QByteArray rawData);
        StudentList             parseStudentList(QByteArray rawData);
        TimeTable               parseTimeTable(QByteArray rawData);
        StatTable               parseStats(QByteArray rawData);
        int                     parseLabWorksCount(QByteArray rawData);

        void saveStats(QByteArray rawData, DataSheet* dataFile);
        bool saveStatEntry(QDomDocument* row, StatTableEntry* entry);

        QList<QDomElement>      selectDateElementList(QDomNodeList dateNodes);

        QMap<int, QString>      timeTableTagAccordance;

    signals:
        void initFinished(bool success) override final;
        void syncDone() override final;

    public slots:
};

#endif // DRIVESYNCPROCESSOR_H
