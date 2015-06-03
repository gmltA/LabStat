#ifndef INTERFACE_DATAITEM
#define INTERFACE_DATAITEM

#include <QObject>

/*!
 * \brief The IDataItem interface should be implemented by every data entry used in specific SyncProcessor
 *
 * \warning This interface was designed to unify interaction between diffeerent sync processors by passing
 * abstract data entry. For now it is useless due to replacement by generic \c DataSheet object.
 */
class IDataItem
{
    public:
        virtual ~IDataItem() {}

        virtual QString getId() const
        {
            return id;
        }
        virtual QString getTitle() const
        {
            return title;
        }

        virtual void setId(const QString &_id)
        {
            id = _id;
        }
        virtual void setTitle(const QString &_title)
        {
            title = _title;
        }

    protected:
        IDataItem() {}
        IDataItem(QString _title) : title(_title) {}
        IDataItem(QString _id, QString _title) : id(_id), title(_title) {}
        QString id;
        QString title;
};

Q_DECLARE_INTERFACE(IDataItem, "IDataItem")

#endif // INTERFACE_DATAITEM

