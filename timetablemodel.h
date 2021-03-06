#ifndef TIMETABLEMODEL_H
#define TIMETABLEMODEL_H

#include <QAbstractListModel>
#include "timetableentry.h"

class TimeTableModel : public QAbstractListModel
{
    Q_OBJECT
    public:
        enum TimeTableDataRoles {
            DateRole = Qt::UserRole + 1,
            TimeRole,
            GroupRole,
            StudentsRole
        };

        TimeTableModel(int groupId, QObject *parent = 0);
        ~TimeTableModel();

        void addEntry(TimeTableEntry* entry);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

        Q_INVOKABLE int getGroupId() const;
        Q_INVOKABLE int getClosestEntryIndex();

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        int groupId;
        TimeTable timeTable;
};

#endif // TIMETABLEMODEL_H
