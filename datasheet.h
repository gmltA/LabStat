#ifndef DATASHEET_H
#define DATASHEET_H

#include <QObject>

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

        explicit DataSheet(QObject *parent = 0);
        ~DataSheet();

        uint getId() const;
        void setId(const uint& value);

        uint getGroupId() const;
        void setGroupId(const uint& value);

        Subject getSubject() const;
        void setSubject(const Subject& value);

        QString getTitle() const;

        QString toString() const;

    private:
        uint id;
        uint groupId;
        Subject subject;

        static const char* subjectString(DataSheet::Subject subject);
};

#endif // DATASHEET_H
