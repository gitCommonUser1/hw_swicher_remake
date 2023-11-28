#ifndef NDILISTMODELITEM_H
#define NDILISTMODELITEM_H

#include <QObject>
#include <QAbstractListModel>

class NdiItemClass{
    Q_GADGET
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString url READ url WRITE setUrl)
    //if type = 1 , the item is "Other..."
    Q_PROPERTY(int type READ type WRITE setType)
    Q_PROPERTY(int current READ current WRITE setCurrent)
public:
    NdiItemClass(){
        m_name = "";
        m_url = "";
        m_type = 0;
        m_current = 0;
    }

    static bool compareBarData(const NdiItemClass &item1, const NdiItemClass &item2){
        return item1.name() < item2.name();
    }

    Q_INVOKABLE bool operator == (NdiItemClass item){
        if(item.url() == m_url){
            return true;
        }else{
            return false;
        }
    }

    Q_INVOKABLE void operator = (NdiItemClass item){
        setName(item.name());
        setUrl(item.url());
        setType(item.type());
        setCurrent(item.current());
    }

    QString name() const
    {
        return m_name;
    }

    QString url() const
    {
        return m_url;
    }

    int type() const
    {
        return m_type;
    }

    int current() const
    {
        return m_current;
    }

public slots:
    void setName(QString name)
    {
        m_name = name;
    }

    void setUrl(QString url)
    {
        m_url = url;
    }

    void setType(int type)
    {
        m_type = type;
    }

    void setCurrent(int current)
    {
        m_current = current;
    }

private:

    QString m_name;
    QString m_url;
    int m_type;
    int m_current;
};
Q_DECLARE_METATYPE(NdiItemClass)

class NdilistModelItem : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NdilistModelItem(QAbstractListModel *parent = nullptr);

    enum ItemType{
        NdiName = 0,
        NdiUrl,
        NdiType,
        NdiCurrent,
        NdiItem
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    void insertData(int row,NdiItemClass data);
    void removeData(int row);
    void removeAll();
    void setData(int row,NdiItemClass);

private:
    QList<NdiItemClass> m_data;

};

#endif // NDILISTMODELITEM_H
