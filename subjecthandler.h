#ifndef SUBJECTHANDLER_H
#define SUBJECTHANDLER_H

#include "subjectdata.h"
#include "student.h"
#include <QObject>

class QQmlEngine;
class QJSEngine;

/*!
 * \brief SubjectHandler is a connector between front-end and back-end of the application.
 *
 * Singleton object provides selection\manage\interacting functionality for QML.
 * All invoked actions are performed on \c currentSubject object.
 *
 * \note This singleton is pointer-based because QML requires object to be always available.
 * Otherwise ASSERT'ation on program exit happens.
 */
class SubjectHandler : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(SubjectHandler)
    public:
        static SubjectHandler* getInstance();

        /*!
         * \brief init embeds SubjectHandler type as singleton to QML.
         *
         * It registers C++ type as QML module. Functions of singleton can be invoked from QML code
         * without creating explicit object;
         */
        static void init();
        Q_INVOKABLE void initData();

        /*!
         * \brief qmlInstance is a function that provides class instance to QML engine
         * \param engine is unused
         * \param scriptEngine is unused
         * \return instance of SubjectHandler
         */
        static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
        {
            Q_UNUSED(engine);
            Q_UNUSED(scriptEngine);

            return SubjectHandler::getInstance();
        }

        Q_INVOKABLE void sync(int processorIndex);
        Q_INVOKABLE void forcedSync(int processorIndex, int direction);

        Q_INVOKABLE void attachProcessor(int processorTypeId, QString rootFolder = "LabStat");
        Q_INVOKABLE void deleteProcessor(int processorIndex);
        Q_INVOKABLE QStringList getAvailableProcessorTypes();

        Q_INVOKABLE void loadGroupData(int group);

        SubjectData* getCurrentSubject() const;
        Q_INVOKABLE void setCurrentSubject(int id);

        void addSubject(SubjectData* subject);
        Q_INVOKABLE void addSubject(QString subjectTitle);
        Q_INVOKABLE void deleteCurrentSubject();

        void sendInitialList();

    private:
        SubjectHandler();
        ~SubjectHandler() {}

        void setCurrentSubject(SubjectData* subject = nullptr);

        static SubjectHandler* instance;
        QList<SubjectData*> subjects;
        SubjectData* currentSubject;

        QStringList subjectModel;

    signals:
        void subjectListChanged(QStringList subjectListModel);

        void processorListChanged(QVariantList processors);
        void processorAddCalled(QVariantMap processorData);
        void processorAdded(QVariantMap processorData);
        void syncStopped(int processorId);

        void groupListChanged(QList<int> groups);
        void groupDataLoaded(TimeTableModel* timeTable);

        void dataInitialized();
};

#endif // SUBJECTHANDLER_H
