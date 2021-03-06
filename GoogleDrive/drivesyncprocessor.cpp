#include "drivesyncprocessor.h"
#include <QSqlQuery>
#include "../appdatastorage.h"
#include <QDebug>

const QString DriveSyncProcessor::processorTypeName = "Google Drive";

DriveSyncProcessor::DriveSyncProcessor(GoogleDriveAPI* drive, QString fileName, QObject *parent)
    : QObject(parent), ISyncProcessor(DriveSyncProcessor::processorTypeName, "", Origin::OriginOnline), driveService(drive)
{
    sheet = new SpreadSheet();
    sheet->setTitle(fileName);
}

DriveSyncProcessor::~DriveSyncProcessor()
{
    delete sheet;
}

void DriveSyncProcessor::init()
{
    createDbStructure();
    loadTimeTableTagAccordance();
    if (driveService->init())
    {
        auto files = driveService->Sheets.listFiles();
        for (auto file: files)
            if (file.getTitle() == sheet->getTitle())
            {
                delete sheet;
                sheet = new SpreadSheet(file);
                fillSpreadSheet();

                emit initFinished(true);
                return;
            }
    }

    emit initFinished(false);
}

void DriveSyncProcessor::saveData(DataSheet* dataFile)
{
    WorkSheet labs = sheet->getWorkSheet("Лабораторные работы");
    QByteArray workSheetData = driveService->Sheets.getListFeed(labs);
    saveStats(workSheetData, dataFile);
    WorkSheet studentList = sheet->getWorkSheet("Список студентов");
    workSheetData = driveService->Sheets.getListFeed(studentList);
    saveNotes(workSheetData, dataFile);
}

void DriveSyncProcessor::loadData(DataSheet* dataFile)
{
    WorkSheet other = sheet->getWorkSheet("Разное");
    QByteArray workSheetData = driveService->Sheets.getListFeed(other);
    dataFile->setGroupList(parseGroupList(workSheetData));
    dataFile->setTotalLabCount(parseLabWorksCount(workSheetData));

    WorkSheet students = sheet->getWorkSheet("Список студентов");
    workSheetData = driveService->Sheets.getListFeed(students);
    dataFile->setStudentList(parseStudentList(workSheetData));

    WorkSheet labs = sheet->getWorkSheet("Лабораторные работы");
    workSheetData = driveService->Sheets.getListFeed(labs);
    dataFile->setTimeTable(parseTimeTable(workSheetData));
    dataFile->setStatTable(parseStats(workSheetData));
}

void DriveSyncProcessor::syncFile(DataSheet* dataFile, ISyncProcessor::SyncDirection direction)
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

void DriveSyncProcessor::clear(DataSheet* dataFile)
{
    Q_UNUSED(dataFile)
    clearTimeTableTagAccordance();
}

GoogleDriveAPI* DriveSyncProcessor::getDriveService() const
{
    return driveService;
}

void DriveSyncProcessor::setDriveService(GoogleDriveAPI* value)
{
    driveService = value;
}

void DriveSyncProcessor::fillSpreadSheet()
{
    QList<WorkSheet> worksheets = driveService->Sheets.getWorkSheets(*sheet);
    sheet->setWorkSheets(worksheets);
}

void DriveSyncProcessor::createDbStructure()
{
    QSqlQuery query(*(AppDataStorage::getInstance().getDB()));
    //query.exec("DROP TABLE IF EXISTS timetable_tag_accordance;");
    query.exec("CREATE TABLE timetable_tag_accordance("
               "processorId INTEGER NOT NULL,"
               "timeTableId INTEGER NOT NULL,"
               "tagName TEXT(255) NOT NULL,"
               "PRIMARY KEY (processorId, timeTableId)"
               ")");
}

void DriveSyncProcessor::clearTimeTableTagAccordance()
{
    QSqlQuery query(*(AppDataStorage::getInstance().getDB()));
    QString clearQuery = "DELETE FROM timetable_tag_accordance WHERE processorId = %1";
    query.exec(clearQuery.arg(id));
}

void DriveSyncProcessor::saveTimeTableTagAccordance()
{
    clearTimeTableTagAccordance();

    QSqlQuery query(*(AppDataStorage::getInstance().getDB()));
    QString insertQuery = "INSERT INTO timetable_tag_accordance VALUES %1;";
    QString tagAccordanceData = "";
    QString rowTemplate = "(%1, '%2', '%3'),";
    for (auto iter = timeTableTagAccordance.begin(); iter != timeTableTagAccordance.end(); iter++)
        tagAccordanceData += rowTemplate.arg(id).arg(iter.key()).arg(iter.value());

    tagAccordanceData.chop(1);
    insertQuery = insertQuery.arg(tagAccordanceData);

    query.exec(insertQuery);
}

void DriveSyncProcessor::loadTimeTableTagAccordance()
{
    QSqlQuery query(*(AppDataStorage::getInstance().getDB()));
    QString queryString = "SELECT timeTableId, tagName FROM timetable_tag_accordance WHERE processorId = %1";
    query.exec(queryString.arg(id));

    timeTableTagAccordance.clear();
    while (query.next())
    {
        timeTableTagAccordance[query.value(0).toInt()] = query.value(1).toString();
    }
}

QList<int> DriveSyncProcessor::parseGroupList(QByteArray rawData)
{
    QDomDocument doc;
    QList<int> groupList;
    if (doc.setContent(rawData))
    {
        QDomNodeList groupNodes = doc.elementsByTagName("gsx:группы");
        for (int i = 0; i < groupNodes.size(); i++)
            if (!groupNodes.item(i).toElement().text().isEmpty())
                groupList.push_back(groupNodes.item(i).toElement().text().toInt());
    }
    return groupList;
}

StudentList DriveSyncProcessor::parseStudentList(QByteArray rawData)
{
    QDomDocument doc;
    StudentList studentList;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        for (int i = 0; i < studentNodes.size(); i++)
            studentList.push_back(new Student(i, studentNodes.item(i)));
    }
    return studentList;
}

TimeTable DriveSyncProcessor::parseTimeTable(QByteArray rawData)
{
    QDomDocument doc;
    TimeTable timeTable;

    if (doc.setContent(rawData))
    {
        QList<QDomElement> dateList = selectDateElementList(doc.elementsByTagName("entry").at(0).childNodes());
        QDomNodeList timeNodes = doc.elementsByTagName("entry").at(1).childNodes();
        QDomNodeList groupNodes = doc.elementsByTagName("entry").at(2).childNodes();

        int dateIndex = 0;
        int entryIndex  = 0;
        for (int i = 0; i < timeNodes.size(); i++)
        {
            QDomElement timeElement = timeNodes.item(i).toElement();
            if (timeElement.tagName().contains("gsx"))
            {
                if (dateList.count() - 1 > dateIndex && dateList[dateIndex + 1].tagName() == timeElement.tagName())
                    ++dateIndex;

                QStringList groupData;
                for (int groupIndex = 0; groupIndex < groupNodes.size(); groupIndex++)
                    if (groupNodes.item(groupIndex).toElement().tagName() == timeElement.tagName())
                    {
                        groupData = groupNodes.item(groupIndex).toElement().text().split("-");
                        break;
                    }

                int group = groupData[0].toInt();
                int subgroup = groupData.size() < 2 ? 0 : groupData[1].toInt();

                timeTableTagAccordance[entryIndex] = timeElement.tagName();

                timeTable.push_back(new TimeTableEntry(entryIndex, QDate::fromString(dateList[dateIndex].text(), "dd.MM.yyyy"),
                                                   QTime::fromString(timeElement.text(), "h:mm"), group, subgroup));
                entryIndex++;
            }
        }
        saveTimeTableTagAccordance();
    }
    return timeTable;
}

StatTable DriveSyncProcessor::parseStats(QByteArray rawData)
{
    QDomDocument doc;
    StatTable statTable;
    if (doc.setContent(rawData))
    {
        int entryId = 0;
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        for (int index = 3; index < studentNodes.size(); index++)
        {
            QDomNodeList dataNodes = studentNodes.item(index).childNodes();
            for (int j = 3, gsxCount = 0; j < dataNodes.size(); j++)
            {
                QString dataTag = dataNodes.item(j).toElement().tagName();
                if (dataTag.contains("gsx"))
                    gsxCount++;

                if (gsxCount >= 3)
                {
                    StatTableEntry* entry = new StatTableEntry(entryId++, timeTableTagAccordance.key(dataTag), index - 3,
                                                                dataNodes.item(j).toElement().text());

                    statTable.push_back(entry);
                }
            }
        }
    }
    return statTable;
}

int DriveSyncProcessor::parseLabWorksCount(QByteArray rawData)
{
    QDomDocument doc;
    if (doc.setContent(rawData))
    {
        QDomNodeList nodes = doc.elementsByTagName("entry").at(0).childNodes();
        for (int i = 0, gsxCount = 0; i < nodes.size(); i++)
        {
            if (nodes.item(i).toElement().tagName().contains("gsx"))
                gsxCount++;

            if (gsxCount == 5)
                return nodes.item(i).toElement().text().toInt();
        }
    }
    return 0;
}

void DriveSyncProcessor::saveNotes(QByteArray rawData, DataSheet* dataFile)
{
    QDomDocument doc;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        StudentList students = dataFile->getStudentList();
        for (int index = 0; index < studentNodes.size(); index++)
        {
            Student* student = students[index];
            if (student->getUpdatedDate() <= dataFile->getLastSyncTime())
                continue;

            QDomNode studentNode = studentNodes.item(index);

            QString selfLink = studentNode.firstChildElement("id").toElement().text();
            QByteArray rowDataRaw = driveService->sendRequest(GoogleAPIRequest(selfLink, "GET"))->readAll();

            QDomDocument row;
            row.setContent(rowDataRaw);
            saveStudentNote(&row, student->getNote());

            QByteArray updatedRowFeed;
            QTextStream stream(&updatedRowFeed);
            row.save(stream, QDomNode::CDATASectionNode);

            QString editUrl = studentNode.toElement().elementsByTagName("link").item(1).toElement().attribute("href");

            driveService->Sheets.editRow(QUrl(editUrl), updatedRowFeed);
        }
    }
}

void DriveSyncProcessor::saveStudentNote(QDomDocument* row, QString note)
{
    const QString notesTag = "gsx:заметки";
    QDomElement newStatElement = row->createElement(notesTag);
    QDomText statText = row->createTextNode(note);
    newStatElement.appendChild(statText);

    QDomNode rootNode = row->elementsByTagName("entry").item(0);
    rootNode.replaceChild(newStatElement, row->elementsByTagName(notesTag).item(0));
}

void DriveSyncProcessor::saveStats(QByteArray rawData, DataSheet* dataFile)
{
    QDomDocument doc;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        StudentList students = dataFile->getStudentList();
        for (int index = 3; index < studentNodes.size(); index++)
        {
            Student* student = students[index - 3];
            if (student->getUpdatedDate() <= dataFile->getLastSyncTime())
                continue;

            QDomNode studentNode = studentNodes.item(index);

            QString selfLink = studentNode.firstChildElement("id").toElement().text();
            QByteArray rowDataRaw = driveService->sendRequest(GoogleAPIRequest(selfLink, "GET"))->readAll();

            QDomDocument row;
            row.setContent(rowDataRaw);
            foreach (StatTableEntry* entry, dataFile->getStatTable())
            {
                if (entry->studentId != student->getId())
                    continue;

                if (!saveStatEntry(&row, entry))
                    continue;
            }

            QByteArray updatedRowFeed;
            QTextStream stream(&updatedRowFeed);
            row.save(stream, QDomNode::CDATASectionNode);

            QString editUrl = studentNode.toElement().elementsByTagName("link").item(1).toElement().attribute("href");

            driveService->Sheets.editRow(QUrl(editUrl), updatedRowFeed);
        }
    }
}

bool DriveSyncProcessor::saveStatEntry(QDomDocument* row, StatTableEntry* entry)
{
    QString timeTableTagName = timeTableTagAccordance[entry->timeTableId];
    bool tagExsists = false;
    if (row->elementsByTagName(timeTableTagName).count() > 0)
    {
        QString storedStats = row->elementsByTagName(timeTableTagName).item(0).toElement().text();
        if (storedStats == entry->labStatsToString())
            return false;

        tagExsists = true;
    }

    QDomElement newStatElement = row->createElement(timeTableTagName);
    QDomText statText = row->createTextNode(entry->labStatsToString());
    newStatElement.appendChild(statText);

    QDomNode rootNode = row->elementsByTagName("entry").item(0);
    if (tagExsists)
        rootNode.replaceChild(newStatElement, row->elementsByTagName(timeTableTagName).item(0));
    else
        rootNode.appendChild(newStatElement);

    return true;
}

QList<QDomElement> DriveSyncProcessor::selectDateElementList(QDomNodeList dateNodes)
{
    QList<QDomElement> dateList;

    for (int i = 0; i < dateNodes.size(); i++)
    {
        QDomElement dateElement = dateNodes.item(i).toElement();
        if (dateElement.tagName().contains("gsx"))
            dateList.push_back(dateElement);
    }

    return dateList;
}
