#ifndef APPDATASTORAGE_H
#define APPDATASTORAGE_H

#include "subjectdata.h"

#include <QtSql>

class AppDataStorage
{
        Q_DISABLE_COPY(AppDataStorage)
    public:
        static AppDataStorage& getInstance();

        void initDBStructure();
        void loadSubjectsFromDB();

        void storeSubject(SubjectData* subject);
        void storeProcessor(SubjectData* subject, ISyncProcessor* processor);

    private:
        AppDataStorage();

        QSqlDatabase db;
};

#endif // APPDATASTORAGE_H
