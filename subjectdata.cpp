#include "subjectdata.h"

SubjectData::SubjectData(QString title, QObject *parent) : QObject(parent)
{
    dataSheet = new DataSheet(title);
    syncHandler = new SyncHandler();
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

void SubjectData::disconnectAll()
{
    syncHandler->disconnect();
    dataSheet->disconnect();
    disconnect();
}
