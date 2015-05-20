#ifndef STUDENT_H
#define STUDENT_H

#include <QAbstractListModel>
#include <QDomDocument>

struct StatTableEntry
{
    int id;
    int timeTableId;
    int studentId;
    bool attended;
};

class Student
{
    public:
        Student(int _id, QString _surname, QString _name, QString _patronymic = "", QString _note = "");
        Student(int _id, QDomNode studentNode);

        QString getName() const;
        void setName(const QString& value);

        QString getSurname() const;
        void setSurname(const QString& value);

        QString getPatronymic() const;
        void setPatronymic(const QString& value);

        int getGroup() const;
        void setGroup(const int& value);

        int getSubgroup() const;
        void setSubgroup(const int& value);

        QString getNote() const;
        void setNote(const QString& value);

        int getId() const;
        void setId(int value);

    private:
        int id;

        QString name;
        QString surname;
        QString patronymic;

        QString note;

        int group;
        int subgroup;
};

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

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<Student> students;
        QList<StatTableEntry> stats;
};

Q_DECLARE_METATYPE(StudentListModel*)
#endif // STUDENT_H
