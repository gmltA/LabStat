#ifndef SUBJECTHANDLER_H
#define SUBJECTHANDLER_H

#include "subjectdata.h"
#include "student.h"
#include <QObject>

class QQmlEngine;
class QJSEngine;

/*
 * Pointer singleton here because QML requires object to be pointer.
 * Otherwise ASSERT'ation on program exit happens.
 */

class SubjectHandler : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(SubjectHandler)
    public:
        static SubjectHandler* getInstance();

        static void init();

        static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
        {
            Q_UNUSED(engine);
            Q_UNUSED(scriptEngine);

            return SubjectHandler::getInstance();
        }

        Q_INVOKABLE void sync(int processorIndex);
        Q_INVOKABLE void attachDrive(QString rootFolder = "LabStat");
        Q_INVOKABLE void attachSQLite(QString rootFolder = "LabStat");
        Q_INVOKABLE void loadGroupData(int group);

        SubjectData* getCurrentSubject() const;
        Q_INVOKABLE void setCurrentSubject(int id);

        void addSubject(SubjectData* subject);
        Q_INVOKABLE void addSubject(QString subjectTitle);

    private:
        SubjectHandler();
        ~SubjectHandler() {}

        void setCurrentSubject(SubjectData* subject);

        static SubjectHandler* instance;
        QList<SubjectData*> subjects;
        SubjectData* currentSubject;

        StudentListModel* studentList;
        QStringList subjectModel;

    signals:
        void subjectListChanged(QStringList subjectListModel);

        void processorListChanged(QVariantList processors);
        void processorAddCalled(QVariantMap processorData);
        void processorAdded(QVariantMap processorData);
        void syncStopped(int processorId);

        void groupListChanged(QList<int> groups);
        void groupDataLoaded(TimeTableModel* timeTable);

    public slots:
};

#endif // SUBJECTHANDLER_H
