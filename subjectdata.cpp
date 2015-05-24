#include "GoogleDrive/drivesyncprocessor.h"
#include "SQLite/sqlitesyncprocessor.h"
#include "subjectdata.h"
#include "syncprocessorprovider.h"

#include "GoogleDrive/googledesktopauthclient.h"
#include "GoogleDrive/googleauthclient.h"
#include "GoogleDrive/interface.authclient.h"
#include "GoogleDrive/API/driveapi.h"

SubjectData::SubjectData(int _id, QString _title, QObject *parent) : QObject(parent), id(_id), title(_title)
{
    dataSheet = new DataSheet(_title);
    syncHandler = new SyncHandler(this);
}

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

void SubjectData::attachProcessor(int processorTypeId, QString additionalData)
{
    if (additionalData.isEmpty())
        additionalData = title;

    ISyncProcessor* processor = SyncProcessorProvider::getInstance().createProcessor(processorTypeId, additionalData);
    if (!processor)
    {
        qDebug() << "Processor with given typed (" << processorTypeId << ") is not registered";
        return;
    }
    syncHandler->registerProcessor(processor);
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

int SubjectData::getId() const
{
    return id;
}

void SubjectData::setId(int value)
{
    id = value;
}
