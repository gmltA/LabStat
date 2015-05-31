#include "subjectdata.h"
#include "syncprocessorprovider.h"
#include "appdatastorage.h"
#include <QDebug>

SubjectData::SubjectData(int _id, QString _title, QObject *parent) : QObject(parent), id(_id), title(_title)
{
    dataSheet = new DataSheet(_title, _id);
    syncHandler = new SyncHandler(this);
}

SubjectData::SubjectData(QString _title, QObject *parent) : QObject(parent), title(_title)
{
    dataSheet = new DataSheet(_title);
    syncHandler = new SyncHandler(this);
}

SubjectData::~SubjectData()
{
    delete syncHandler;
    delete dataSheet;
    AppDataStorage::getInstance().removeSubject(this);
}

DataSheet* SubjectData::getDataSheet() const
{
    return dataSheet;
}

SyncHandler* SubjectData::getSyncHandler() const
{
    return syncHandler;
}

void SubjectData::attachProcessor(int processorId, int processorTypeId, QString additionalData)
{
    if (additionalData.isEmpty())
        additionalData = title;

    ISyncProcessor* processor = SyncProcessorProvider::getInstance().createProcessor(processorTypeId, additionalData);
    if (!processor)
    {
        qDebug() << "Processor with given typeId (" << processorTypeId << ") is not registered";
        return;
    }
    if (processorId != PROC_ID_INVALID)
        processor->setId(processorId);

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
    dataSheet->setId(value);
}
