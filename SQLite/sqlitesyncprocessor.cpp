#include "sqlitesyncprocessor.h"
#include <QDebug>

const QString SQLiteSyncProcessor::processorTypeName = "SQLite storage";

SQLiteSyncProcessor::SQLiteSyncProcessor(QString connectionName, QObject *parent)
    : QObject(parent), ISyncProcessor(SQLiteSyncProcessor::processorTypeName, connectionName, Origin::OriginOffline)
{
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
}

SQLiteSyncProcessor::~SQLiteSyncProcessor()
{
    QString connectionName = db.connectionName();
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
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

void SQLiteSyncProcessor::clearLabCount(DataSheet* dataFile)
{
    QSqlQuery query(db);
    QString clearQuery = "DELETE FROM lab_works_count WHERE subjectId = %1";
    query.exec(clearQuery.arg(dataFile->getId()));
}

void SQLiteSyncProcessor::clearTimeTable(DataSheet* dataFile)
{
    QSqlQuery query(db);
    QString clearQuery = "DELETE FROM timetable_entry WHERE subjectId = %1";
    query.exec(clearQuery.arg(dataFile->getId()));
}

void SQLiteSyncProcessor::clearStatTable(DataSheet* dataFile)
{
    QSqlQuery query(db);
    QString clearQuery = "DELETE FROM stats WHERE subjectId = %1";
    query.exec(clearQuery.arg(dataFile->getId()));
}

void SQLiteSyncProcessor::clearStudentList(DataSheet* dataFile)
{
    QSqlQuery query(db);
    QString clearQuery = "DELETE FROM students WHERE subjectId = %1";
    query.exec(clearQuery.arg(dataFile->getId()));
}

void SQLiteSyncProcessor::saveTimeTable(DataSheet* dataFile)
{
    clearTimeTable(dataFile);

    QSqlQuery query(db);
    QString insertQuery = "INSERT INTO timetable_entry VALUES %1;";
    QString timeTableData = "";
    foreach (TimeTableEntry* entry, dataFile->getTimeTable())
        timeTableData += serializeTimeTableEntry(dataFile->getId(), entry);

    timeTableData.chop(1);
    insertQuery = insertQuery.arg(timeTableData);

    query.exec(insertQuery);
}

void SQLiteSyncProcessor::loadLabCount(DataSheet* dataFile)
{
    QString queryString = "SELECT lab_count FROM lab_works_count WHERE subjectId = %1";
    QSqlQuery query(db);
    query.exec(queryString.arg(dataFile->getId()));
    query.next();
    dataFile->setTotalLabCount(query.value(0).toInt());
}

void SQLiteSyncProcessor::saveLabCount(DataSheet* dataFile)
{
    clearLabCount(dataFile);

    QSqlQuery query(db);
    QString insertQuery = "INSERT INTO lab_works_count VALUES (%1, %2);";
    insertQuery = insertQuery.arg(dataFile->getId()).arg(dataFile->getTotalLabCount());

    query.exec(insertQuery);
}

void SQLiteSyncProcessor::loadTimeTable(DataSheet* dataFile)
{
    QString queryString = "SELECT id, dateTime, groupId, subgroupId FROM timetable_entry WHERE subjectId = %1";
    QSqlQuery query(queryString.arg(dataFile->getId()), db);
    TimeTable timeTable;
    while (query.next())
    {
        timeTable.push_back(new TimeTableEntry(query.value(0).toInt(), QDateTime::fromString(query.value(1).toString()),
                                           query.value(2).toInt(), query.value(3).toInt()));
    }

    if (!timeTable.isEmpty())
        dataFile->setTimeTable(timeTable);
}

void SQLiteSyncProcessor::saveStatTable(DataSheet* dataFile)
{
    clearStatTable(dataFile);

    QSqlQuery query(db);
    QString insertQuery = "INSERT INTO stats VALUES %1;";
    QString statTableData = "";
    foreach (StatTableEntry* entry, dataFile->getStatTable())
        statTableData += serializeStatTableEntry(dataFile->getId(), entry);

    statTableData.chop(1);
    insertQuery = insertQuery.arg(statTableData);

    query.exec(insertQuery);
}

void SQLiteSyncProcessor::loadStatTable(DataSheet* dataFile)
{
    QString queryString = "SELECT id, timeTableId, studentId, attended FROM stats WHERE subjectId = %1";
    QSqlQuery query(queryString.arg(dataFile->getId()), db);
    StatTable stats;
    while (query.next())
    {
        stats.push_back(new StatTableEntry(query.value(0).toInt(), query.value(1).toInt(),
                                            query.value(2).toInt(), query.value(3).toString()));
    }

    if (!stats.isEmpty())
        dataFile->setStatTable(stats);
}

void SQLiteSyncProcessor::saveStudentList(DataSheet* dataFile)
{
    clearStudentList(dataFile);

    QSqlQuery query(db);
    QString insertQuery = "INSERT INTO students VALUES %1;";
    QString studentsData = "";
    foreach (Student* person, dataFile->getStudentList())
    {
        studentsData += serializeStudent(dataFile->getId(), person);
        connect(person, &Student::dataUpdated, this, &SQLiteSyncProcessor::updateStudent);
    }

    studentsData.chop(1);
    insertQuery = insertQuery.arg(studentsData);

    query.exec(insertQuery);
}

void SQLiteSyncProcessor::loadStudentList(DataSheet* dataFile)
{
    QString queryString = "SELECT id, surname, name, patronymic, note, groupId, subgroupId FROM students WHERE subjectId = %1";
    QSqlQuery query(queryString.arg(dataFile->getId()), db);

    StudentList studentList;
    QList<int> groupList;
    while (query.next())
    {
        Student* person = new Student(query.value(0).toInt(), query.value(1).toString(),
                                        query.value(2).toString(), query.value(3).toString(),
                                        query.value(4).toString());
        person->setGroup(query.value(5).toInt());
        person->setSubgroup(query.value(6).toInt());
        connect(person, &Student::dataUpdated, this, &SQLiteSyncProcessor::updateStudent);

        studentList.push_back(person);

        if (!groupList.contains(person->getGroup()))
            groupList.append(person->getGroup());
    }

    if (!groupList.isEmpty())
    {
        std::sort(groupList.begin(), groupList.end());
        dataFile->setGroupList(groupList);
    }

    if (!studentList.isEmpty())
        dataFile->setStudentList(studentList);
}

void SQLiteSyncProcessor::updateStudent(Student* person)
{
    QSqlQuery query(db);
    QString queryString = "REPLACE INTO students VALUES %1;";

    // todo: 0 is a subject id which should be passed from somewhere
    QString studentData = serializeStudent(0, person);
    studentData.chop(1);
    query.exec(queryString.arg(studentData));
}

void SQLiteSyncProcessor::saveData(DataSheet* dataFile)
{
    saveStudentList(dataFile);
    saveTimeTable(dataFile);
    saveStatTable(dataFile);
    saveLabCount(dataFile);
}

void SQLiteSyncProcessor::loadData(DataSheet* dataFile)
{
    loadStudentList(dataFile);
    loadTimeTable(dataFile);
    loadStatTable(dataFile);
    loadLabCount(dataFile);
}

void SQLiteSyncProcessor::syncFile(DataSheet* dataFile, ISyncProcessor::SyncDirection direction)
{
    if (direction == ISyncProcessor::SyncDefault)
        direction = dataFile->getLastSyncTime().isValid() ? ISyncProcessor::SyncWrite : SyncLoad;

    if (direction == ISyncProcessor::SyncWrite)
    {
        saveData(dataFile);
    }
    else if (direction == ISyncProcessor::SyncLoad)
    {
        loadData(dataFile);
    }

    dataFile->synced(id);
    emit syncDone();
}

void SQLiteSyncProcessor::clear(DataSheet* dataFile)
{
    clearLabCount(dataFile);
    clearTimeTable(dataFile);
    clearStatTable(dataFile);
    clearStudentList(dataFile);
}

void SQLiteSyncProcessor::createDbStructure()
{
    QSqlQuery query(db);
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

    //query.exec("DROP TABLE IF EXISTS stats;");
    query.exec("CREATE TABLE stats ("
                   "id  INTEGER NOT NULL,"
                   "subjectId  INTEGER NOT NULL,"
                   "timeTableId  INTEGER NOT NULL,"
                   "studentId  INTEGER NOT NULL,"
                   "attended  INTEGER NOT NULL,"
                   "PRIMARY KEY (subjectId, id))");
    qDebug() << query.lastError().text();

    //query.exec("DROP TABLE IF EXISTS lab_works_count;");
    query.exec("CREATE TABLE lab_works_count ("
                   "subjectId  INTEGER NOT NULL,"
                   "lab_count  INTEGER NOT NULL,"
                   "PRIMARY KEY (subjectId)"
                   ")");
    qDebug() << query.lastError().text();
}

QString SQLiteSyncProcessor::serializeStudent(int subjectId, Student* person)
{
    QString personTpl = "(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8'),";
    return personTpl.arg(subjectId)
            .arg(person->getId())
            .arg(person->getSurname())
            .arg(person->getName())
            .arg(person->getPatronymic())
            .arg(person->getNote())
            .arg(person->getGroup())
            .arg(person->getSubgroup());
}

QString SQLiteSyncProcessor::serializeTimeTableEntry(int subjectId, TimeTableEntry* entry)
{
    QString ttEntryTpl = "(%1, '%2', '%3', '%4', '%5'),";
    return ttEntryTpl.arg(subjectId)
            .arg(entry->id)
            .arg(entry->dateTime.toString())
            .arg(entry->group)
            .arg(entry->subgroup);
}

QString SQLiteSyncProcessor::serializeStatTableEntry(int subjectId, StatTableEntry* entry)
{
    QString stEntryTpl = "(%1, '%2', '%3', '%4', '%5'),";
    return stEntryTpl.arg(entry->id)
            .arg(subjectId)
            .arg(entry->timeTableId)
            .arg(entry->studentId)
            .arg(entry->labStatsToString());
}
