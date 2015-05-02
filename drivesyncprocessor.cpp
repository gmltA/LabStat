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
