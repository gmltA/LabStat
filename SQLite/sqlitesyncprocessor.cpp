#include "sqlitesyncprocessor.h"
#include <QDebug>

SQLiteSyncProcessor::SQLiteSyncProcessor(QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOffline, "SQLite local"),
      db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

SQLiteSyncProcessor::~SQLiteSyncProcessor()
{
}

void SQLiteSyncProcessor::init()
{
    //todo: get DB file name from front-end dialog?
    db.setDatabaseName("db_name.sqlite");

    if (db.open())
    {
        createDbStructure();
        emit initFinished(true);
        return;
    }
    emit initFinished(false);
}

void SQLiteSyncProcessor::saveStudentList(DataSheet* dataFile)
{
    QSqlQuery query;
    query.exec("DELETE FROM students;");

    QString basicQuery = "INSERT INTO students VALUES %1;";
    QString studentsData = "";
    foreach (Student person, dataFile->getStudentList())
        studentsData += serializeStudent(person);

    studentsData.chop(1);
    basicQuery = basicQuery.arg(studentsData);

    query.exec(basicQuery);
}

void SQLiteSyncProcessor::loadStudentList(DataSheet* dataFile)
{
    QSqlQuery query("SELECT id, surname, name, patronymic, note, `group`, subgroup FROM students");
    QList<Student> studentList;
    QList<int> groupList;
    while (query.next())
    {
        Student person(query.value(0).toInt(), query.value(1).toString(),
                        query.value(2).toString(), query.value(3).toString(),
                        query.value(4).toString());
        person.setGroup(query.value(5).toInt());
        person.setSubgroup(query.value(6).toInt());

        studentList.push_back(person);

        if (!groupList.contains(person.getGroup()))
            groupList.append(person.getGroup());
    }

    if (!groupList.isEmpty())
    {
        std::sort(groupList.begin(), groupList.end());
        dataFile->setGroupList(groupList);
    }

    if (!studentList.isEmpty())
        dataFile->setStudentList(studentList);
}

void SQLiteSyncProcessor::syncFile(DataSheet* dataFile)
{
    // no sync was made or data was updated from different source
    if (dataFile->getLastSyncTime().isValid() && dataFile->getLastSyncProcessorId() != id)
    {
        saveStudentList(dataFile);
    }
    else
        loadStudentList(dataFile);

    dataFile->synced(id);
    emit syncDone();
}

void SQLiteSyncProcessor::createDbStructure()
{
    QSqlQuery query;
    query.exec("CREATE TABLE students("
               "id INTEGER NOT NULL,"
               "surname TEXT(255) NOT NULL,"
               "name TEXT(255) NOT NULL,"
               "patronymic TEXT(255),"
               "note TEXT(255),"
               "'group' INTEGER NOT NULL,"
               "subgroup INTEGER,"
               "PRIMARY KEY (id)"
               ")");
    qDebug() << query.lastError().text();
}

QString SQLiteSyncProcessor::serializeStudent(Student person)
{
    QString personTpl = "(%1, '%2', '%3', '%4', '%5', '%6', '%7'),";
    return personTpl.arg(person.getId())
            .arg(person.getSurname())
            .arg(person.getName())
            .arg(person.getPatronymic())
            .arg(person.getNote())
            .arg(person.getGroup())
            .arg(person.getSubgroup());
}
