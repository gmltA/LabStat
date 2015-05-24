#include "subjecthandler.h"
#include <QtQml>
#include "appdatastorage.h"
#include <QtConcurrent>

SubjectHandler* SubjectHandler::instance = nullptr;

SubjectHandler::SubjectHandler() : QObject(), currentSubject(nullptr)
{
}

SubjectData* SubjectHandler::getCurrentSubject() const
{
    return currentSubject;
}

void SubjectHandler::setCurrentSubject(int id)
{
    SubjectData* subject = subjects.at(id);
    if (subject)
        setCurrentSubject(subject);
}

void SubjectHandler::setCurrentSubject(SubjectData* subject)
{
    if (currentSubject)
        currentSubject->disconnectAll();

    currentSubject = subject;
    connect(currentSubject->getSyncHandler(), &SyncHandler::processorAddCalled, this, &SubjectHandler::processorAddCalled);
    connect(currentSubject->getSyncHandler(), &SyncHandler::processorAdded, this, &SubjectHandler::processorAdded);
    connect(currentSubject->getSyncHandler(), &SyncHandler::syncStopped, this, &SubjectHandler::syncStopped);

    connect(currentSubject->getDataSheet(), &DataSheet::groupListChanged, this, &SubjectHandler::groupListChanged);

    emit processorListChanged(currentSubject->getSyncHandler()->buildProcessorsData());
    emit groupListChanged(currentSubject->getDataSheet()->getGroupList());
    emit groupDataLoaded(currentSubject->getDataSheet()->getTimeTableModel());
}

void SubjectHandler::addSubject(SubjectData* subject)
{
    subjects.push_back(subject);
    if (!currentSubject)
        setCurrentSubject(subject);

    subjectModel.append(subject->getTitle());

    emit subjectListChanged(subjectModel);
}

void SubjectHandler::addSubject(QString subjectTitle)
{
    SubjectData* subject = new SubjectData(subjectTitle);
    addSubject(subject);
    subject->setId(subjects.indexOf(subject));

    AppDataStorage::getInstance().storeSubject(subject);
}

void SubjectHandler::sendInitialList()
{
    emit subjectListChanged(subjectModel);
}

void SubjectHandler::init()
{
    qmlRegisterSingletonType<SubjectHandler>("SubjectHandler", 1, 0, "SubjectHandler", &SubjectHandler::qmlInstance);
    AppDataStorage::getInstance().initDBStructure();
    AppDataStorage::getInstance().loadSubjectsFromDB();
}

void SubjectHandler::sync(int processorIndex)
{
    currentSubject->getSyncHandler()->sync(processorIndex);
}

void SubjectHandler::attachProcessor(int processorTypeId, QString rootFolder)
{
    QtConcurrent::run(currentSubject, &SubjectData::attachProcessor, processorTypeId, rootFolder);
}

QStringList SubjectHandler::getAvailableProcessors()
{
    QStringList processors;
    processors.append("Google Drive");
    processors.append("SQLite storage");
    return processors;
}

void SubjectHandler::loadGroupData(int group)
{
    emit groupDataLoaded(currentSubject->getDataSheet()->getTimeTableModel(group));
}

SubjectHandler* SubjectHandler::getInstance()
{
    if (!instance)
        instance = new SubjectHandler();

    return instance;
}
