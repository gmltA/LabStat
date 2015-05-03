#include "datasheet.h"
#include <QMetaEnum>

DataSheet::DataSheet(QString _fileName, QObject *parent)
    : QObject(parent), id(0), fileName(_fileName), groupId(0), subject(Subject::None)
{
}

DataSheet::~DataSheet()
{
}

uint DataSheet::getId() const
{
    return id;
}

void DataSheet::setId(const uint& value)
{
    id = value;
}

DataSheet::Subject DataSheet::getSubject() const
{
    return subject;
}

void DataSheet::setSubject(const Subject& value)
{
    subject = value;
}

QString DataSheet::toString() const
{
    // NIY
    return "a \t b \t c \nd \t e \t n";
}

QStringList DataSheet::getGroupList() const
{
    return groups;
}

void DataSheet::setGroupList(const QStringList& value)
{
    groups = value;
    emit groupListChanged(groups);
}

void DataSheet::buildGroupList(QByteArray rawData)
{
    QDomDocument doc;
    QStringList groupList;
    if (doc.setContent(rawData))
    {
        QDomNodeList groupNodes = doc.elementsByTagName("gsx:группы");
        for (int i = 0; i < groupNodes.size(); i++)
            if (!groupNodes.item(i).toElement().text().isEmpty())
                groupList.push_back(groupNodes.item(i).toElement().text());

        setGroupList(groupList);
    }
}

QString DataSheet::getFileName() const
{
    return fileName;
}

void DataSheet::setFileName(const QString& value)
{
    fileName = value;
}

QList<Student> DataSheet::getStudentList() const
{
    return students;
}

void DataSheet::setStudentList(const QList<Student>& value)
{
    students = value;
}

void DataSheet::buildStudentList(QByteArray rawData)
{
    QDomDocument doc;
    QList<Student> studentList;
    if (doc.setContent(rawData))
    {
        QDomNodeList studentNodes = doc.elementsByTagName("entry");
        for (int i = 0; i < studentNodes.size(); i++)
            studentList.push_back(Student(studentNodes.item(i)));

        setStudentList(studentList);
    }
}

QList<TimetableEntry> DataSheet::getTimeTable() const
{
    return timeTable;
}

void DataSheet::setTimeTable(const QList<TimetableEntry>& value)
{
    timeTable = value;
}

void DataSheet::buildTimeTable(QByteArray rawData)
{
    QDomDocument doc;
    QList<QDomElement> dateList;
    QList<TimetableEntry> timeTable;

    if (doc.setContent(rawData))
    {
        // BUG: firstChildElement doesn't work
        QDomNodeList dateNodes = doc.elementsByTagName("entry").at(0).childNodes();
        QDomNodeList timeNodes = doc.elementsByTagName("entry").at(1).childNodes();
        QDomNodeList groupNodes = doc.elementsByTagName("entry").at(2).childNodes();

        for (int i = 0; i < dateNodes.size(); i++)
        {
            QDomElement dateElement = dateNodes.item(i).toElement();
            if (dateElement.tagName().contains("gsx"))
                dateList.push_back(dateElement);
        }

        int dateIndex = 0;
        int entryIndex  = 0;
        for (int i = 0; i < timeNodes.size(); i++)
        {
            QDomElement timeElement = timeNodes.item(i).toElement();
            if (timeElement.tagName().contains("gsx"))
            {
                if (dateList.count() - 1 > dateIndex && dateList[dateIndex + 1].tagName() == timeElement.tagName())
                    ++dateIndex;

                QStringList groupData;
                for (int groupIndex = 0; groupIndex < groupNodes.size(); groupIndex++)
                    if (groupNodes.item(groupIndex).toElement().tagName() == timeElement.tagName())
                        groupData = groupNodes.item(groupIndex).toElement().text().split("-");

                int group = groupData[0].toInt();
                int subgroup = groupData.size() < 2 ? 0 : groupData[1].toInt();

                timeTable.push_back(TimetableEntry(entryIndex++, QDate::fromString(dateList[dateIndex].text(), "dd.MM.yyyy"),
                                                   QTime::fromString(timeElement.text(), "h:mm"), group, subgroup));
            }
        }

        setTimeTable(timeTable);
    }
}

uint DataSheet::getGroupId() const
{
    return groupId;
}

void DataSheet::setGroupId(const uint& value)
{
    groupId = value;
}

const char* DataSheet::subjectString(DataSheet::Subject subject)
{
    const QMetaObject &mo = DataSheet::staticMetaObject;
    int index = mo.indexOfEnumerator("Subject");
    QMetaEnum metaEnum = mo.enumerator(index);

    return metaEnum.valueToKey(subject);
}

TimeTableModel::TimeTableModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

void TimeTableModel::addEntry(const TimetableEntry& entry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    timeTable << entry;
    endInsertRows();
}

int TimeTableModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return timeTable.count();
}

QVariant TimeTableModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= timeTable.count())
        return QVariant();

    const TimetableEntry& entry = timeTable[index.row()];
    if (role == DateRole)
        return entry.dateTime.date();
    else if (role == TimeRole)
        return entry.dateTime.time();
    else if (role == GroupRole)
        return entry.subroup;
    return QVariant();
}

QHash<int, QByteArray> TimeTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[TimeRole] = "time";
    roles[GroupRole] = "group";
    return roles;
}
