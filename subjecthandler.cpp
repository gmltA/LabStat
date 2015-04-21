#include "subjecthandler.h"
#include <QDebug>
#include <QtQml>

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
}

void SubjectHandler::addSubject(SubjectData* subject)
{
    subjects.push_back(subject);
    if (!currentSubject)
        setCurrentSubject(subject);
}

void SubjectHandler::init()
{
    qmlRegisterSingletonType<SubjectHandler>("SubjectHandler", 1, 0, "SubjectHandler", &SubjectHandler::qmlInstance);
}

void SubjectHandler::sync(int processorIndex)
{
    currentSubject->getSyncHandler()->sync(processorIndex);
}

SubjectHandler* SubjectHandler::getInstance()
{
    if (!instance)
        instance = new SubjectHandler();

    return instance;
}
