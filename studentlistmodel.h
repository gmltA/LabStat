#ifndef STUDENTLISTMODEL_H
#define STUDENTLISTMODEL_H

#include <QAbstractListModel>
#include "student.h"
#include "stattableentry.h"

class StudentListModel : public QAbstractListModel
{
        Q_OBJECT
    public:
        enum StudentDataRoles {
            NameRole = Qt::UserRole + 1,
            SurnameRole,
            NoteRole,
            SubGroupRole,
            AttendenceRole,
            LabWorksRole
        };

        StudentListModel(QObject *parent = 0);

        void addStudent(Student* student);
        void addStatEntry(StatTableEntry* entry);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);

        int getTotalLabCount() const;
        void setTotalLabCount(int value);

    signals:
        void statEntryAdded(StatTableEntry* entry);

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        int totalLabCount;
        StudentList students;
        StatTable stats;

        QMap<int, bool> convertListToMap(const QVariant& value);
        StatTableEntry* statEntryForStudent(int studentId) const;
        StatTableEntry* getOrCreateStatEntryForStudent(int studentId);
};

Q_DECLARE_METATYPE(StudentListModel*)

#endif // STUDENTLISTMODEL_H
