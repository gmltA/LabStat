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
    dataFile->buildGroupList(workSheetData);

    WorkSheet students = sheet->getWorkSheet("Список студентов");
    workSheetData = driveService->Sheets.getListFeed(students);
    dataFile->buildStudentList(workSheetData);

    WorkSheet labs = sheet->getWorkSheet("Лабораторные работы");
    workSheetData = driveService->Sheets.getListFeed(labs);
    dataFile->buildTimeTable(workSheetData);

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
