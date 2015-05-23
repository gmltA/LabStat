#include "drivesyncprocessor.h"

DriveSyncProcessor::DriveSyncProcessor(GoogleDriveAPI* drive, QString fileName, QObject *parent)
    : QObject(parent), IDataStore(Origin::OriginOnline, "Google Drive"), driveService(drive)
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

void DriveSyncProcessor::syncFile(DataSheet* dataFile)
{
    WorkSheet other = sheet->getWorkSheet("Разное");
    QByteArray workSheetData = driveService->Sheets.getListFeed(other);
    dataFile->setGroupList(buildGroupList(workSheetData));

    WorkSheet students = sheet->getWorkSheet("Список студентов");
    workSheetData = driveService->Sheets.getListFeed(students);
    dataFile->setStudentList(buildStudentList(workSheetData));

    WorkSheet labs = sheet->getWorkSheet("Лабораторные работы");
    workSheetData = driveService->Sheets.getListFeed(labs);
    dataFile->setTimeTable(buildTimeTable(workSheetData));
    dataFile->setStatTable(buildStats(workSheetData));

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


QList<int> DriveSyncProcessor::buildGroupList(QByteArray rawData)
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

StudentList DriveSyncProcessor::buildStudentList(QByteArray rawData)
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

QList<TimeTableEntry> DriveSyncProcessor::buildTimeTable(QByteArray rawData)
{
    QDomDocument doc;
    QList<TimeTableEntry> timeTable;

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

                timeTable.push_back(TimeTableEntry(entryIndex, QDate::fromString(dateList[dateIndex].text(), "dd.MM.yyyy"),
                                                   QTime::fromString(timeElement.text(), "h:mm"), group, subgroup));
                entryIndex++;
            }
        }
    }
    return timeTable;
}

QList<StatTableEntry> DriveSyncProcessor::buildStats(QByteArray rawData)
{
    QDomDocument doc;
    QList<StatTableEntry> statTable;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        for (int index = 3; index < studentNodes.size(); index++)
        {
            QDomNodeList dataNodes = studentNodes.item(index).childNodes();
            for (int j = 3, entryId = 0, gsxCount = 0; j < dataNodes.size(); j++)
            {
                QString dataTag = dataNodes.item(j).toElement().tagName();
                if (dataTag.contains("gsx"))
                    gsxCount++;

                if (gsxCount >= 3)
                {
                    StatTableEntry entry;
                    entry.id = entryId++;
                    entry.timeTableId = timeTableAccordance.key(dataTag);
                    entry.attended = dataNodes.item(j).toElement().text() == "н" ? false : true;
                    entry.studentId = index - 3;

                    statTable.push_back(entry);
                }
            }
        }
    }
    return statTable;
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
