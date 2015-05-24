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
    QStringList splittedData = data.split("\\");
    QString rootFolder = splittedData[0].isEmpty() ? "root" : splittedData[0];

    GoogleDriveAPI* drive = new GoogleDriveAPI(rootFolder);
    drive->setVerboseOutput(true);

    QObject::connect(drive, SIGNAL(authRequired()), dynamic_cast<QObject*>(authClient), SLOT(processAuth()));
    QObject::connect(dynamic_cast<QObject*>(authClient), SIGNAL(authCompleted(QString)), drive, SLOT(setToken(QString)));

    ISyncProcessor* driveProcessor = new DriveSyncProcessor(drive, splittedData[1]);
    driveProcessor->setData(data);
    return driveProcessor;
}

QString DriveSyncProcessorCreator::getProcessorTitle()
{
    return DriveSyncProcessor::processorTypeName;
}

