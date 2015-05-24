#include "drivesyncprocessor.h"
#include "drivesyncprocessorcreator.h"
#include "googleauthclient.h"
#include "googledesktopauthclient.h"

#include "API/driveapi.h"

ISyncProcessor* DriveSyncProcessorCreator::createProcessor(QString data)
{
    IAuthClient* authClient;
#if defined(Q_OS_ANDROID)
    authClient = new GoogleAuthClient();
#else
    authClient = new GoogleDesktopAuthClient();
#endif
    GoogleDriveAPI* drive = new GoogleDriveAPI(data);
    drive->setVerboseOutput(true);

    QObject::connect(drive, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    QObject::connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), drive, SLOT(setToken(QString)));

    //todo: pass string from somewhere
    return new DriveSyncProcessor(drive, "LSTest1");
}

QString DriveSyncProcessorCreator::getProcessorTitle()
{
    return DriveSyncProcessor::processorTypeName;
}

