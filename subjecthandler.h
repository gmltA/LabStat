#ifndef SUBJECTHANDLER_H
#define SUBJECTHANDLER_H

#include "subjectdata.h"
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

        SubjectData* getCurrentSubject() const;
        void setCurrentSubject(int id);

        void addSubject(SubjectData* subject);

    private:
        SubjectHandler();
        ~SubjectHandler() {}

        void setCurrentSubject(SubjectData* subject);

        static SubjectHandler* instance;
        QList<SubjectData*> subjects;
        SubjectData* currentSubject;

    signals:
        void processorAddCalled(QVariantMap processorData);
        void processorAdded(QVariantMap processorData);
        void syncStopped(int processorId);

    public slots:
};

#endif // SUBJECTHANDLER_H