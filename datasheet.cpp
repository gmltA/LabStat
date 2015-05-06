#include "datasheet.h"
#include <QMetaEnum>

DataSheet::DataSheet(QString _fileName, QObject *parent)
    : QObject(parent), id(0), fileName(_fileName)
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

QString DataSheet::toString() const
{
    // NIY
    return "a \t b \t c \nd \t e \t n";
}

QList<int> DataSheet::getGroupList() const
{
    return groups;
}

void DataSheet::setGroupList(const QList<int>& value)
{
    groups = value;
    emit groupListChanged(groups);
}

void DataSheet::buildGroupList(QByteArray rawData)
{
    QDomDocument doc;
    QList<int> groupList;
    if (doc.setContent(rawData))
    {
        QDomNodeList groupNodes = doc.elementsByTagName("gsx:группы");
        for (int i = 0; i < groupNodes.size(); i++)
            if (!groupNodes.item(i).toElement().text().isEmpty())
                groupList.push_back(groupNodes.item(i).toElement().text().toInt());

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
            studentList.push_back(Student(i, studentNodes.item(i)));

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

QDateTime DataSheet::getLastSyncTime() const
{
    return lastSyncTime;
}

int DataSheet::getLastSyncProcessorId() const
{
    return lastSyncProcessorId;
}

void DataSheet::synced(int processorId)
{
    lastSyncProcessorId = processorId;
    lastSyncTime = QDateTime::currentDateTime();
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
    switch (role)
    {
        case DateRole:
            return entry.dateTime.date();
        case TimeRole:
            return entry.dateTime.time();
        case GroupRole:
            return entry.subgroup;
        case StudentsRole:
        {
            QVariant var;
            var.setValue(entry.students);
            return var;
        }
    }
    return QVariant();
}

int TimeTableModel::subGroupIdForIndex(int index)
{
    return timeTable[index].subgroup;
}

QHash<int, QByteArray> TimeTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[TimeRole] = "time";
    roles[GroupRole] = "group";
    roles[StudentsRole] = "students";
    return roles;
}
