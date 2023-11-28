#ifndef RIGHTMENUMODEL_H
#define RIGHTMENUMODEL_H

#include <QAbstractListModel>
#include <QObject>

class RightMenuData{
    Q_GADGET
public:
    RightMenuData(){
        setIsMin(0);
        setIsMax(0);
    }
    Q_PROPERTY(QString key READ key WRITE setKey)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(int isMax READ isMax WRITE setIsMax)
    Q_PROPERTY(int isMin READ isMin WRITE setIsMin)
    Q_PROPERTY(int special READ special WRITE setSpecial )
    QString key() const
    {
        return m_key;
    }
    QVariant value() const
    {
        return m_value;
    }

    int index() const
    {
        return m_index;
    }

    int isMax() const
    {
        return m_isMax;
    }

    int isMin() const
    {
        return m_isMin;
    }

    int special() const
    {
        return m_special;
    }

public slots:
    void setKey(QString key)
    {
        m_key = key;
    }
    void setValue(QVariant value)
    {
        if (m_value == value)
            return;
        m_value = value;
    }
    void setIndex(int index)
    {
        m_index = index;
    }

    void setIsMax(int isMax)
    {
        m_isMax = isMax;
    }

    void setIsMin(int isMin)
    {
        m_isMin = isMin;
    }

    void setSpecial(int special)
    {
        m_special = special;
    }

private:
    QString m_key;
    QVariant m_value;
    int m_index;
    int m_isMax;
    int m_isMin;
    int m_special;
};
Q_DECLARE_METATYPE(RightMenuData)

class RightMenuModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(RIGHT_SPECIAL)
public:
    //enum
    enum RIGHT_SPECIAL{
        COLOR = 1,
        LOCATION,
        LOAD_PICTURE,
        PATTERN,
        WSTRING,
    };
    RightMenuModel(QObject *parent = nullptr);

    void appendItem(const RightMenuData& item);
    void insertItem(const RightMenuData& item,int index);
    void removeItem(int index);
    void clearItem();

    enum ItemRoles{
        TypeKey = Qt::UserRole + 100,
        TypeValue,
        TypeIndex,
        TypeIsMin,
        TypeIsMax,
        TypeSpecial,
    };

    int rowCount(const QModelIndex & parent = QModelIndex())const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    int length();
protected:
    QHash<int, QByteArray> roleNames() const;
public:
    //对外数据
    QList<RightMenuData>m_items;
    void init_items();
    void change_item(int first,int second,int third);
public slots:

signals:

};


#endif // RIGHTMENUMODEL_H
