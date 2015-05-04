#include "drivesyncprocessor.h"
#include "sqlitesyncprocessor.h"
#include "subjectdata.h"

#include "googledesktopauthclient.h"
#include "googleauthclient.h"
#include "interface.authclient.h"
#include "GoogleDrive/driveapi.h"

SubjectData::SubjectData(QString title, QObject *parent) : QObject(parent)
{
    dataSheet = new DataSheet(title);
    syncHandler = new SyncHandler(this);
}

SubjectData::~SubjectData()
{
    dataSheet->deleteLater();
    syncHandler->deleteLater();
}

DataSheet* SubjectData::getDataSheet() const
{
    return dataSheet;
}

SyncHandler* SubjectData::getSyncHandler() const
{
    return syncHandler;
}

void SubjectData::attachDrive(QString rootFolder)
{
    IAuthClient* authClient;
#if defined(Q_OS_ANDROID)
    authClient = new GoogleAuthClient();
#else
    authClient = new GoogleDesktopAuthClient();
#endif
    GoogleDriveAPI* drive = new GoogleDriveAPI(rootFolder);
    drive->setVerboseOutput(true);

    QObject::connect(drive, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    QObject::connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), drive, SLOT(setToken(QString)));

    DriveSyncProcessor* driveProcessor = new DriveSyncProcessor(drive, "LSTest1");

    syncHandler->registerProcessor(driveProcessor);
}

void SubjectData::attachSQLite(QString rootFolder)
{
    SQLiteSyncProcessor* sqliteProcessor = new SQLiteSyncProcessor();
    syncHandler->registerProcessor(sqliteProcessor);
}

void SubjectData::disconnectAll()
{
    syncHandler->disconnect();
    dataSheet->disconnect();
    disconnect();
}
