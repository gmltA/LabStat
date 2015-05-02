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

// todo: split code in 2 functions
void DriveSyncProcessor::syncFile(DataSheet* dataFile)
{
    // buildGroupList
    WorkSheet other = sheet->getWorkSheet("Разное");

    QByteArray workSheetData = driveService->Sheets.getListFeed(other);

    QDomDocument* doc = new QDomDocument();
    QStringList groups;
    if (doc->setContent(workSheetData))
    {
        QDomNodeList groupNodes = doc->elementsByTagName("gsx:группы");
        for (int i = 0; i < groupNodes.size(); i++)
            if (!groupNodes.item(i).toElement().text().isEmpty())
                groups.push_back(groupNodes.item(i).toElement().text());

        dataFile->setGroupList(groups);
    }
    delete doc;

    // buildStudentList
    WorkSheet students = sheet->getWorkSheet("Список студентов");

    workSheetData = driveService->Sheets.getListFeed(students);

    doc = new QDomDocument();
    QList<Student> studentList;
    if (doc->setContent(workSheetData))
    {
        QDomNodeList studentNodes = doc->elementsByTagName("entry");
        for (int i = 0; i < studentNodes.size(); i++)
            studentList.push_back(Student(studentNodes.item(i)));

        dataFile->setStudentList(studentList);
    }
    delete doc;

    // buildSchedule
    WorkSheet labs = sheet->getWorkSheet("Лабораторные работы");

    workSheetData = driveService->Sheets.getListFeed(labs);

    doc = new QDomDocument();
    //todo: convert to date list
    QList<QDate> dateList;
    QList<TimetableEntry> timeTable;
    if (doc->setContent(workSheetData))
    {
        // BUG: firstChildElement doesn't work
        QDomNodeList dateNodes = doc->elementsByTagName("entry").at(0).childNodes();
        QDomNodeList timeNodes = doc->elementsByTagName("entry").at(1).childNodes();
        int timeIndex = 0;
        for (int i = 0; i < dateNodes.size(); i++)
        {
            QDomElement dateElement = dateNodes.item(i).toElement();
            if (dateElement.tagName().contains("gsx"))
            {
                dateList.push_back(QDate::fromString(dateElement.text(), "dd.MM.yyyy"));
                if (dateList.count() > 1)
                {
                    while (timeNodes.item(timeIndex).toElement().tagName() != dateElement.tagName() && timeIndex < timeNodes.count())
                    {
                        QDomElement timeElement = timeNodes.item(timeIndex).toElement();
                        if (timeElement.tagName().contains("gsx"))
                            timeTable.append(TimetableEntry(dateList.at(dateList.count() - 2), QTime::fromString(timeElement.text(), "h:mm"), ""));
                        timeIndex++;
                    }
                }
                if (i + 1 == dateNodes.size())
                {
                    while (timeIndex < timeNodes.count())
                    {
                        QDomElement timeElement = timeNodes.item(timeIndex).toElement();
                        if (timeElement.tagName().contains("gsx"))
                            timeTable.append(TimetableEntry(dateList.last(), QTime::fromString(timeElement.text(), "h:mm"), ""));
                        timeIndex++;
                    }
                }
            }
        }

        dataFile->setTimeTable(timeTable);
    }
    delete doc;

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
