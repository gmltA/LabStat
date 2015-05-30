#include "drivesyncprocessor.h"

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

void DriveSyncProcessor::syncFile(DataSheet* dataFile)
{
    /*!
     * \todo implement proper algorythm to choose between load and store functions
     */
    if (dataFile->getLastSyncTime().isValid())
    {
        saveData(dataFile);
    }
    else
    {
        loadData(dataFile);
    }

    dataFile->synced(id);
    emit syncDone();
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
        // BUG: firstChildElement doesn't work
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
                        groupData = groupNodes.item(groupIndex).toElement().text().split("-");

                int group = groupData[0].toInt();
                int subgroup = groupData.size() < 2 ? 0 : groupData[1].toInt();

                timeTableAccordance[entryIndex] = timeElement.tagName();

                timeTable.push_back(new TimeTableEntry(entryIndex, QDate::fromString(dateList[dateIndex].text(), "dd.MM.yyyy"),
                                                   QTime::fromString(timeElement.text(), "h:mm"), group, subgroup));
                entryIndex++;
            }
        }
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
                    StatTableEntry* entry = new StatTableEntry(entryId++, timeTableAccordance.key(dataTag), index - 3,
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

void DriveSyncProcessor::saveStats(QByteArray rawData, DataSheet* dataFile)
{
    QDomDocument doc;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        StudentList students = dataFile->getStudentList();
        for (int index = 3; index < studentNodes.size(); index++)
        {
            QDomNode studentNode = studentNodes.item(index);
            Student* student = students[index - 3];

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
    QString timeTableTagName = timeTableAccordance[entry->timeTableId];
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
