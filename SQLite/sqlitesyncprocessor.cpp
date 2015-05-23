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

void SQLiteSyncProcessor::saveTimeTable(DataSheet* dataFile)
{
    QSqlQuery query;
    QString cleanQuery = "DELETE FROM timetable_entry WHERE subjectId = %1";
    query.exec(cleanQuery.arg(dataFile->getId()));

    QString basicQuery = "INSERT INTO timetable_entry VALUES %1;";
    QString timeTableData = "";
    foreach (TimeTableEntry entry, dataFile->getTimeTable())
        timeTableData += serializeTimeTableEntry(dataFile->getId(), entry);

    timeTableData.chop(1);
    basicQuery = basicQuery.arg(timeTableData);

    query.exec(basicQuery);
}

void SQLiteSyncProcessor::loadTimeTable(DataSheet* dataFile)
{
    QString queryString = "SELECT id, dateTime, groupId, subgroupId FROM timetable_entry WHERE subjectId = %1";
    QSqlQuery query(queryString.arg(dataFile->getId()));
    QList<TimeTableEntry> timeTable;
    while (query.next())
    {
        timeTable.push_back(TimeTableEntry(query.value(0).toInt(), QDateTime::fromString(query.value(1).toString()),
                                           query.value(2).toInt(), query.value(3).toInt()));
    }

    if (!timeTable.isEmpty())
        dataFile->setTimeTable(timeTable);
}

void SQLiteSyncProcessor::saveStudentList(DataSheet* dataFile)
{
    QSqlQuery query;
    QString cleanQuery = "DELETE FROM students WHERE subjectId = %1";
    query.exec(cleanQuery.arg(dataFile->getId()));

    QString basicQuery = "INSERT INTO students VALUES %1;";
    QString studentsData = "";
    foreach (Student person, dataFile->getStudentList())
        studentsData += serializeStudent(dataFile->getId(), person);

    studentsData.chop(1);
    basicQuery = basicQuery.arg(studentsData);

    query.exec(basicQuery);
}

void SQLiteSyncProcessor::loadStudentList(DataSheet* dataFile)
{
    QString queryString = "SELECT id, surname, name, patronymic, note, groupId, subgroupId FROM students WHERE subjectId = %1";
    QSqlQuery query(queryString.arg(dataFile->getId()));

    StudentList studentList;
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
        saveTimeTable(dataFile);
    }
    else
    {
        loadStudentList(dataFile);
        loadTimeTable(dataFile);
    }

    dataFile->synced(id);
    emit syncDone();
}

void SQLiteSyncProcessor::createDbStructure()
{
    QSqlQuery query;
    //query.exec("DROP TABLE IF EXISTS students;");
    query.exec("CREATE TABLE students("
               "subjectId INTEGER NOT NULL,"
               "id INTEGER NOT NULL,"
               "surname TEXT(255) NOT NULL,"
               "name TEXT(255) NOT NULL,"
               "patronymic TEXT(255),"
               "note TEXT(255),"
               "groupId INTEGER NOT NULL,"
               "subgroupId INTEGER,"
               "PRIMARY KEY (subjectId, id)"
               ")");
    qDebug() << query.lastError().text();

    //query.exec("DROP TABLE IF EXISTS timetable_entry;");
    query.exec("CREATE TABLE timetable_entry ("
                   "subjectId INTEGER NOT NULL,"
                   "id INTEGET NOT NULL,"
                   "dateTime TEXT(255) NOT NULL,"
                   "groupId INTEGER NOT NULL,"
                   "subgroupId INTEGER,"
                   "PRIMARY KEY (subjectId, id)"
                   ")");
    qDebug() << query.lastError().text();
}

QString SQLiteSyncProcessor::serializeStudent(int subjectId, Student person)
{
    QString personTpl = "(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8'),";
    return personTpl.arg(subjectId)
            .arg(person.getId())
            .arg(person.getSurname())
            .arg(person.getName())
            .arg(person.getPatronymic())
            .arg(person.getNote())
            .arg(person.getGroup())
            .arg(person.getSubgroup());
}

QString SQLiteSyncProcessor::serializeTimeTableEntry(int subjectId, TimeTableEntry entry)
{
    QString ttEntryTpl = "(%1, '%2', '%3', '%4', '%5'),";
    return ttEntryTpl.arg(subjectId)
            .arg(entry.id)
            .arg(entry.dateTime.toString())
            .arg(entry.group)
            .arg(entry.subgroup);
}
