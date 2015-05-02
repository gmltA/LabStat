#ifndef STUDENT_H
#define STUDENT_H

#include <QAbstractListModel>
#include <QDomDocument>

class Student
{
    public:
        Student(QDomNode studentNode);

        QString getName() const;
        void setName(const QString& value);

        QString getSurname() const;
        void setSurname(const QString& value);

        QString getPatronymic() const;
        void setPatronymic(const QString& value);

        QString getGroup() const;
        void setGroup(const QString& value);

        QString getSubgroup() const;
        void setSubgroup(const QString& value);

        QString getNote() const;
        void setNote(const QString& value);

    private:
        QString name;
        QString surname;
        QString patronymic;

        QString note;

        // todo: convert to int or something
        QString group;
        QString subgroup;
};

class StudentListModel : public QAbstractListModel
{
    Q_OBJECT
    public:
        enum AnimalRoles {
            NameRole = Qt::UserRole + 1,
            NoteRole
        };

        StudentListModel(QObject *parent = 0);

        void addStudent(const Student& animal);

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        QList<Student> students;
};
#endif // STUDENT_H
