#ifndef APPDATASTORAGE_H
#define APPDATASTORAGE_H

#include "subjectdata.h"

#include <QtSql>

/*!
 * \brief The AppDataStorage class is a global object (singleton) that is used to store global application DataSheet
 *
 * It uses isolated SQLite connection to separate DB that stores generic data (subjects, processors per subject).
 * Class also provides pointer to DB object for SyncProcessors to store specific data
 * \see DriveSyncProcessor::saveTimeTableTagAccordance
 *
 * \todo Replace with \c QSettings
 */
class AppDataStorage
{
        Q_DISABLE_COPY(AppDataStorage)
    public:
        static AppDataStorage& getInstance();

        void initDBStructure();
        void loadSubjectsFromDB();

        void storeSubject(SubjectData* subject);
        void storeProcessor(SubjectData* subject, ISyncProcessor* processor);

        void removeSubject(SubjectData* subject);
        void removeProcessor(SubjectData* subject, ISyncProcessor* processor);

        QSqlDatabase* getDB();

    private:
        AppDataStorage();

        QSqlDatabase db;
};

#endif // APPDATASTORAGE_H
