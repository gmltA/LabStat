#ifndef DATASHEET_H
#define DATASHEET_H

#include <QObject>
#include <QStringList>
#include "student.h"

class DataSheet : public QObject
{
        Q_OBJECT
        Q_ENUMS(Subject)

    public:
        enum Subject
        {
            One,
            Two,
            None
        };

        explicit DataSheet(QString _fileName = "", QObject* parent = 0);
        ~DataSheet();

        QString toString() const;

        uint getId() const;
        void setId(const uint& value);

        uint getGroupId() const;
        void setGroupId(const uint& value);

        Subject getSubject() const;
        void setSubject(const Subject& value);

        QStringList getGroupList() const;
        void setGroupList(const QStringList& value);

        QString getFileName() const;
        void setFileName(const QString& value);

        QList<Student> getStudentList() const;
        void setStudentList(const QList<Student>& value);

    private:
        uint id;
        QString fileName;

        uint groupId;
        Subject subject;

        QStringList groups;
        QList<Student> students;

        static const char* subjectString(DataSheet::Subject subject);

    signals:
        void groupListChanged(QStringList groups);
};

#endif // DATASHEET_H
