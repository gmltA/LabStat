#include "appdatastorage.h"
#include "subjecthandler.h"

AppDataStorage::AppDataStorage()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "main");
    db.setDatabaseName("app_data.sqlite");
    db.open();
}

AppDataStorage& AppDataStorage::getInstance()
{
    static AppDataStorage instance;
    return instance;
}

void AppDataStorage::initDBStructure()
{
    QSqlQuery query(db);
    //query.exec("DROP TABLE IF EXISTS subjects;");
    query.exec("CREATE TABLE subjects ("
              "id  INTEGER NOT NULL,"
              "title  TEXT(255) NOT NULL,"
              "PRIMARY KEY (id)"
              ")");

    //query.exec("DROP TABLE IF EXISTS processors;");
    query.exec("CREATE TABLE processors ("
                   "id  INTEGER NOT NULL,"
                   "subjectId  INTEGER NOT NULL,"
                   "processorType  INTEGER NOT NULL,"
                   "data  TEXT(255),"
                   "PRIMARY KEY (id)"
                   ")");
}

void AppDataStorage::loadSubjectsFromDB()
{
    QSqlQuery query(db), processorsQuery(db);

    query.exec("SELECT id, title FROM subjects");
    while (query.next())
    {
        SubjectData* subject = new SubjectData(query.value(0).toInt(), query.value(1).toString());
        processorsQuery.exec(QString("SELECT processorType, data FROM processors WHERE subjectId = %1")
                             .arg(query.value(0).toInt()));

        while (processorsQuery.next())
        {
            subject->attachProcessor(processorsQuery.value(0).toInt(), processorsQuery.value(1).toString());
        }
        SubjectHandler::getInstance()->addSubject(subject);
    }
}

void AppDataStorage::storeSubject(SubjectData* subject)
{
    QSqlQuery query(db);
    QString querySrc = "INSERT INTO subjects VALUES ('%1', '%2')";
    query.exec(querySrc.arg(subject->getId()).arg(subject->getTitle()));
    qDebug() << query.lastError().text();
}
