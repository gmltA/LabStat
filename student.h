#ifndef STUDENT_H
#define STUDENT_H

#include <QDomDocument>
#include <QObject>

class Student : public QObject
{
        Q_OBJECT
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

    signals:
        void dataChanged(Student* student);

    private:
        int id;

        QString name;
        QString surname;
        QString patronymic;

        QString note;

        int group;
        int subgroup;
};

typedef QList<Student*> StudentList;

#endif // STUDENT_H
