#include "subjecthandler.h"
#include <QtQml>
#include <QtConcurrent>

SubjectHandler* SubjectHandler::instance = nullptr;

SubjectHandler::SubjectHandler() : QObject(), currentSubject(nullptr), studentList(nullptr)
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

void SubjectHandler::attachDrive(QString rootFolder)
{
    QtConcurrent::run(currentSubject, &SubjectData::attachDrive, rootFolder);
}

void SubjectHandler::loadGroupData(QString group)
{
    QList<Student> students = currentSubject->getDataSheet()->getStudentList();
    if (studentList != nullptr)
        studentList->deleteLater();

    studentList = new StudentListModel();
    foreach (Student person, students)
    {
        if (person.getGroup() == group)
            studentList->addStudent(person);
    }
    emit groupDataLoaded(studentList, timeTable);
}

SubjectHandler* SubjectHandler::getInstance()
{
    if (!instance)
        instance = new SubjectHandler();

    return instance;
}

