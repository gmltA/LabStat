#ifndef SQLITESYNCPROCESSOR_H
#define SQLITESYNCPROCESSOR_H

#include <QtSql>
#include <QObject>
#include "interface.syncprocessor.h"

class SQLiteSyncProcessor : public QObject, public ISyncProcessor
{
        Q_OBJECT
        Q_INTERFACES(ISyncProcessor)

    public:
        SQLiteSyncProcessor(QString connectionName, QObject *parent = 0);
        ~SQLiteSyncProcessor();

        void init() override final;
        void syncFile(DataSheet* dataFile) override final;

        void updateStudent(Student* person);

        static const QString processorTypeName;

        void saveData(DataSheet* dataFile);
        void loadData(DataSheet* dataFile);

    private:
        QSqlDatabase db;

        void saveLabCount(DataSheet* dataFile);
        void loadLabCount(DataSheet* dataFile);
        void saveTimeTable(DataSheet* dataFile);
        void loadTimeTable(DataSheet* dataFile);
        void saveStatTable(DataSheet* dataFile);
        void loadStatTable(DataSheet* dataFile);
        void saveStudentList(DataSheet* dataFile);
        void loadStudentList(DataSheet* dataFile);

        void createDbStructure();
        QString serializeStudent(int subjectId, Student* person);
        QString serializeTimeTableEntry(int subjectId, TimeTableEntry* entry);
        QString serializeStatTableEntry(int subjectId, StatTableEntry* entry);
        QString serializeLabStats(QMap<int, bool> labStats);

    signals:
        void initFinished(bool success) override final;
        void syncDone() override final;

    public slots:
};

#endif // SQLITESYNCPROCESSOR_H
