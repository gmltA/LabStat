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
    emit processorListChanged(currentSubject->getSyncHandler()->buildProcessorsData());
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

void SubjectHandler::attachSQLite(QString rootFolder)
{
    QtConcurrent::run(currentSubject, &SubjectData::attachSQLite, rootFolder);
}

void SubjectHandler::loadGroupData(int group)
{
    QList<Student> students = currentSubject->getDataSheet()->getStudentList();
    if (studentList != nullptr)
        studentList->deleteLater();

    TimeTableModel* timeTable = new TimeTableModel();
    foreach (TimetableEntry timeTableEntry, currentSubject->getDataSheet()->getTimeTable())
    {
        if (group == timeTableEntry.group)
        {
            foreach (Student person, students)
            {
                if (person.getGroup() == group && (person.getSubgroup() == timeTableEntry.subgroup
                        || timeTableEntry.subgroup == 0 || person.getSubgroup() == 0))
                {
                    timeTableEntry.students->addStudent(person);
                }
            }
            timeTable->addEntry(timeTableEntry);
        }
    }
    emit groupDataLoaded(timeTable);
}

SubjectHandler* SubjectHandler::getInstance()
{
    if (!instance)
        instance = new SubjectHandler();

    return instance;
}
