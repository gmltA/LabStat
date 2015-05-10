#include "GoogleDrive/drivesyncprocessor.h"
#include "SQLite/sqlitesyncprocessor.h"
#include "subjectdata.h"

#include "GoogleDrive/googledesktopauthclient.h"
#include "GoogleDrive/googleauthclient.h"
#include "GoogleDrive/interface.authclient.h"
#include "GoogleDrive/API/driveapi.h"

SubjectData::SubjectData(QString _title, QObject *parent) : QObject(parent), title(_title)
{
    dataSheet = new DataSheet(_title);
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

QString SubjectData::getTitle() const
{
    return title;
}

void SubjectData::setTitle(const QString& value)
{
    title = value;
}
