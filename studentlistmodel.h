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
            AttendenceRole
        };

        StudentListModel(QObject *parent = 0);

        void addStudent(const Student& student);
        void addStatEntry(const StatTableEntry& entry);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<Student> students;
        QList<StatTableEntry> stats;
};

Q_DECLARE_METATYPE(StudentListModel*)

#endif // STUDENTLISTMODEL_H
